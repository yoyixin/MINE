// (c) COPYRIGHT OMRON Corporation 2008 All Rights Reserved.
//
// FILE NAME     : bmp_trans.cpp
//
//������������������������������������������������������������������������������
//�� ���� ��  DATE  ��              NOTES                       ��    SIGN    ��
//������������������������������������������������������������������������������
//��      ��17.3.14 ���V�K�쐬                                  ��HYRON�k     ��
//������������������������������������������������������������������������������
//��      ��        ��                                          ��            ��
//������������������������������������������������������������������������������
#include "bmp_trans.h"

//DEFINE_LOG_TRACE("bmp_trans.log")
DEFINE_LOG

namespace bmptrans {

  #define PI 3.14159
  //�t�@�C���w�b�_
  typedef struct tagBITMAPFILEHEADER_H{
      WORD bfType;           //���ʕ����iBM���K�{�j  
      DWORD bfSize;          //�t�@�C���T�C�Y
      WORD bfReserved1;      //�\��
      WORD bfReserved2;      //�\��  
      DWORD bfOffBits;       //�f�[�^���܂ł̃I�t�Z�b�g
  }BITMAPFILEHEADER_H;

  //���w�b�_
  typedef struct tagBITMAPINFOHEADER_H{  
  //  DWORD biSize;          //BMP���w�b�_�̃T�C�Y �ŏ��`�F�b�N�̂��߁A��`�Ȃ�
      LONG biWidth;          //BMP�̕�
      LONG biHeight;         //BMP�̍���
      WORD biPlanes;         //�v���[�����i�P���K�{�j  
      WORD biBitCount;       //1�s�N�Z���̃f�[�^��  
      DWORD  biCompression;  //���k biCompression (�����k�Ȃ�� 0) 
      DWORD  biSizeImage;    //�摜�̃T�C�Y
      LONG  biXPelsPerMeter; //�������𑜓x
      LONG  biYPelsPerMeter; //�c�����𑜓x
      DWORD  biClrUsed;      //�p���b�g��
      DWORD  biClrImportant; //�p���b�g���̏d�v�ȐF  
  }BITMAPINFOHEADER_H;

  //�p���b�g���
  typedef struct tagRGBQUAD_H{  
      BYTE rgbBlue;          //�u���[�̕���
      BYTE rgbGreen;         //�O���[���̕���
      BYTE rgbRed;           //���b�h�̕���
      BYTE rgbReserved;      //�\��
  }RGBQUAD_H;

  //BMP���
  typedef struct tagIMAGEDATA_H
  {
      BYTE blue;  
      BYTE green;  
      BYTE red;  
  }IMAGEDATA_H;

  bool MakeYokoBmp(const char* fromFileName, const char* toFileName, int rotateAngle)
  {
    OMRONSOFT_TRACE("�ύX���t�@�C���F�@"<< fromFileName);
    OMRONSOFT_TRACE("�ύX��t�@�C���F�@"<< toFileName);
    OMRONSOFT_TRACE("�p�x�F�@"<< rotateAngle);
    FILE *fpi,*fpw;
    int width,height;                  //BMP�̕��A����
    BITMAPFILEHEADER_H strHead;        //�t�@�C���w�b�_
    BITMAPINFOHEADER_H strInfo;        //���w�b�_
    RGBQUAD_H strPla[256];             //�p���b�g
    IMAGEDATA_H *imagedata = NULL;     //���O��f���
    IMAGEDATA_H *imagedataRot = NULL;  //�����f���

    fpi=fopen(fromFileName,"rb");
    if(!fpi) return false;

    //BMP�����`�F�b�N(BM���H)
    WORD bfType;  
    fread(&bfType,1,sizeof(WORD),fpi);  
    if(0x4d42!=bfType)  
    {
      return false;  
    }
OMRONSOFT_TRACE("�c�t�@�C���w�b�_��ǂݍ��ݒ��c");
    //�t�@�C���w�b�_��ǂݍ���
    fread(&strHead,1,sizeof(tagBITMAPFILEHEADER_H),fpi);
OMRONSOFT_TRACE("�c���w�b�_��ǂݍ��ݒ��c");
    //���w�b�_��ǂݍ���
    fread(&strInfo,1,sizeof(tagBITMAPINFOHEADER_H),fpi);
OMRONSOFT_TRACE("�c�p���b�g����ǂݍ��ݒ��c");
    //�p���b�g����ǂݍ���
    {
      for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)  
      {  
        fread((char *)&(strPla[nCounti].rgbBlue),1,sizeof(BYTE),fpi);  
        fread((char *)&(strPla[nCounti].rgbGreen),1,sizeof(BYTE),fpi);  
        fread((char *)&(strPla[nCounti].rgbRed),1,sizeof(BYTE),fpi);  
        fread((char *)&(strPla[nCounti].rgbReserved),1,sizeof(BYTE),fpi);  
      }
    }
    width = strInfo.biWidth;
    height = strInfo.biHeight;
OMRONSOFT_TRACE("���F�@" << width);
OMRONSOFT_TRACE("�����F�@" << height);
    //�摜�̕��́A�S�̔{���ɂȂ邱��
    width = (width * sizeof(IMAGEDATA_H) + 3) / 4 * 4;  
    imagedata = (IMAGEDATA_H*)malloc(width * height * sizeof(IMAGEDATA_H));
    //imagedata = (IMAGEDATA_H*)malloc(width * height);
    imagedataRot = (IMAGEDATA_H*)malloc(width * height * sizeof(IMAGEDATA_H));
OMRONSOFT_TRACE("OLD��f��񏉊���");
    //���O�̉�f��������������
    for(int i = 0;i < height;++i)  
    {  
      for(int j = 0;j < width;++j)  
      {  
        (*(imagedata + i * width + j)).blue = 0;  
        //(*(imagedata + i * width + j)).green = 0;  
        //(*(imagedata + i *  width + j)).red = 0;  
      }
    }
OMRONSOFT_TRACE("NEW��f��񏉊���");
    //����̉�f��������������
    {
      for(int i = 0;i < height;++i)  
      {  
        for(int j = 0;j < width;++j)  
        {  
          (*(imagedataRot + i * width + j)).blue = 0;  
          //(*(imagedataRot + i * 2 * width + j)).green = 0;  
          //(*(imagedataRot + i * 2 * width + j)).red = 0;  
        }
      }
    }
    //fseek(fpi,54,SEEK_SET);  
    //BMP�f�[�^��ǂݍ���
    fread(imagedata,sizeof(struct tagIMAGEDATA_H) * width,height,fpi);  
    fclose(fpi);
OMRONSOFT_TRACE("NEW���F�@" << width);
OMRONSOFT_TRACE("NEW�����F�@" << height);
    //BMP��鏈��
    double angle;
    int midX_pre,midY_pre,midX_aft,midY_aft;
    midX_pre = width / 2;
    midY_pre = height / 2;
    midX_aft = width / 2;
    midY_aft = height / 2;
    int pre_i,pre_j,after_i,after_j;
    angle = 1.0 * rotateAngle * PI / 180;
    {
      for(int i = 0;i < height;++i)  
      {  
        for(int j = 0;j < width;++j)  
        {  
          after_i = i - midX_aft;
          after_j = j - midY_aft;  
          pre_i = (int)(cos((double)angle) * after_i - sin((double)angle) * after_j) + midX_pre;  
          pre_j = (int)(sin((double)angle) * after_i + cos((double)angle) * after_j) + midY_pre;  
          if(pre_i >= 0 && pre_i < height && pre_j >= 0 && pre_j < width)
              *(imagedataRot + i * width + j) = *(imagedata + pre_i * width + pre_j);
OMRONSOFT_TRACE("�VBMP��f���W�F�@(" << i << "," << j << ")");
OMRONSOFT_TRACE("�]����BMP��f���W�F�@(" << pre_i << "," << pre_j << ")");
        }
      }
    }
    
    //BMP��ۑ�����
    if((fpw=fopen(toFileName,"wb"))==NULL)  
    {  
        return false;  
    }  
    WORD bfType_w=0x4d42;  
    fwrite(&bfType_w,1,sizeof(WORD),fpw);  
    //fpw +=2;  
    fwrite(&strHead,1,sizeof(tagBITMAPFILEHEADER_H),fpw);  
    strInfo.biWidth = width / sizeof(IMAGEDATA_H);  
    strInfo.biHeight = height;  
    fwrite(&strInfo,1,sizeof(tagBITMAPINFOHEADER_H),fpw);  
    //�p���b�g���
    {
      for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)  
      {  
          fwrite(&strPla[nCounti].rgbBlue,1,sizeof(BYTE),fpw);  
          fwrite(&strPla[nCounti].rgbGreen,1,sizeof(BYTE),fpw);  
          fwrite(&strPla[nCounti].rgbRed,1,sizeof(BYTE),fpw);  
          fwrite(&strPla[nCounti].rgbReserved,1,sizeof(BYTE),fpw);  
      }
    }
    //��f���ۑ�
    {
      for(int i =0;i < height;++i)  
      {  
        for(int j = 0;j < width;++j)  
        {  
          fwrite( &((*(imagedataRot + i * width + j)).blue),1,sizeof(tagIMAGEDATA_H),fpw);
          //fwrite( &((*(imagedataRot + i * 2 * width + j)).green),1,sizeof(BYTE),fpw);
          //fwrite( &((*(imagedataRot + i * 2 * width + j)).red),1,sizeof(BYTE),fpw);
        }  
      }
    }
    fclose(fpw);  
  
    //���������
    delete[] imagedata;  
    delete[] imagedataRot;  
    
    return true;
  }

}
