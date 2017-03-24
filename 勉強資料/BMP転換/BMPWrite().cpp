#include <stdio.h>  
#include "BmpRot.h"  
#include "stdlib.h"  
#include "math.h"  
#include <iostream>  
  
#define PI 3.14159//?�����G��?  
#define LENGTH_NAME_BMP 30//bmp?�Е������I�ő�?�x  
  
using namespace std;  
  
//?�ʒ�?  
BITMAPFILEHEADER strHead;  
RGBQUAD strPla[256];//256�F?�F��  
BITMAPINFOHEADER strInfo;  
  
//?����?����?�M��  
void showBmpHead(BITMAPFILEHEADER pBmpHead){  
    cout<<"��?����?:"<<endl;  
    cout<<"�����召:"<<pBmpHead.bfSize<<endl;  
    cout<<"�ۗ���_1:"<<pBmpHead.bfReserved1<<endl;  
    cout<<"�ۗ���_2:"<<pBmpHead.bfReserved2<<endl;  
    cout<<"??��?�����I�Έڎ�?��:"<<pBmpHead.bfOffBits<<endl<<endl;  
}  
  
void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead){  
    cout<<"��?�M��?:"<<endl;  
    cout<<"??�̓I?�x:"<<pBmpInforHead.biSize<<endl;  
    cout<<"��??:"<<pBmpInforHead.biWidth<<endl;  
    cout<<"��?��:"<<pBmpInforHead.biHeight<<endl;  
    cout<<"biPlanes���ʐ�:"<<pBmpInforHead.biPlanes<<endl;  
    cout<<"biBitCount�їp?�F�ʐ�:"<<pBmpInforHead.biBitCount<<endl;  
    cout<<"??����:"<<pBmpInforHead.biCompression<<endl;  
    cout<<"biSizeImage??��?������p�I��?��:"<<pBmpInforHead.biSizeImage<<endl;  
    cout<<"X����������:"<<pBmpInforHead.biXPelsPerMeter<<endl;  
    cout<<"Y����������:"<<pBmpInforHead.biYPelsPerMeter<<endl;  
    cout<<"�g�p�I?�F��:"<<pBmpInforHead.biClrUsed<<endl;  
    cout<<"�d�v?�F��:"<<pBmpInforHead.biClrImportant<<endl;  
}  
  
  
int main(){  
    char strFile[LENGTH_NAME_BMP];//bmp������  
    IMAGEDATA *imagedata = NULL;//??���z��?��?�ГI���f�M���I��?��?  
    IMAGEDATA *imagedataRot = NULL;//??���z��?��?�@�I?�ГI���f�M���I��?��?  
    int width,height;//?�ГI?�x�a���x  
    cout<<"??�����v?��I������:"<<endl;  
    cin>>strFile;  
    FILE *fpi,*fpw;  
    fpi=fopen(strFile,"rb");  
    if(fpi != NULL){  
        //��?�敶��?�^  
        WORD bfType;  
        fread(&bfType,1,sizeof(WORD),fpi);  
        if(0x4d42!=bfType)  
        {  
            cout<<"the file is not a bmp file!"<<endl;  
            return NULL;  
        }  
        //?��bmp�����I����?�a�M��?  
        fread(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpi);  
        //showBmpHead(strHead);//?������?  
        fread(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpi);  
        //showBmpInforHead(strInfo);//?�������M��?  
  
        //?��?�F��  
        for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)  
        {  
            fread((char *)&(strPla[nCounti].rgbBlue),1,sizeof(BYTE),fpi);  
            fread((char *)&(strPla[nCounti].rgbGreen),1,sizeof(BYTE),fpi);  
            fread((char *)&(strPla[nCounti].rgbRed),1,sizeof(BYTE),fpi);  
            fread((char *)&(strPla[nCounti].rgbReserved),1,sizeof(BYTE),fpi);  
        }  
  
        width = strInfo.biWidth;  
        height = strInfo.biHeight;  
        //?��?��s�I��?���K?��4�I�����{  
        width = (width * sizeof(IMAGEDATA) + 3) / 4 * 4;  
        imagedata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));  
        //imagedata = (IMAGEDATA*)malloc(width * height);  
        imagedataRot = (IMAGEDATA*)malloc(2 * width * 2 * height * sizeof(IMAGEDATA));  
        //���n�����n?�ГI���f��?  
        for(int i = 0;i < height;++i)  
        {  
            for(int j = 0;j < width;++j)  
            {  
                (*(imagedata + i * width + j)).blue = 0;  
                //(*(imagedata + i * width + j)).green = 0;  
                //(*(imagedata + i *  width + j)).red = 0;  
            }  
        }  
        //���n����?�@?�ГI���f��?  
        for(int i = 0;i < 2 * height;++i)  
        {  
            for(int j = 0;j < 2 * width;++j)  
            {  
                (*(imagedataRot + i * 2 * width + j)).blue = 0;  
                //(*(imagedataRot + i * 2 * width + j)).green = 0;  
                //(*(imagedataRot + i * 2 * width + j)).red = 0;  
            }  
        }  
        //fseek(fpi,54,SEEK_SET);  
        //?�o?�ГI���f����  
        fread(imagedata,sizeof(struct tagIMAGEDATA) * width,height,fpi);  
        fclose(fpi);  
    }  
    else  
    {  
        cout<<"file open error!"<<endl;  
        return NULL;  
    }  
  
    //?�А�??��  
    int RotateAngle;//�v��?�I�p�x��  
    double angle;//�v��?�I�ʓx��  
    int midX_pre,midY_pre,midX_aft,midY_aft;//��?��??�I���S�_�I��?  
    midX_pre = width / 2;  
    midY_pre = height / 2;  
    midX_aft = width;  
    midY_aft = height;  
    int pre_i,pre_j,after_i,after_j;//��?�O�@??�I���f�_��?  
    cout<<"?���v��?�I�p�x�i0�x��360�x�C�t??��?�j�F"<<endl;  
    cin>>RotateAngle;  
    angle = 1.0 * RotateAngle * PI / 180;  
    for(int i = 0;i < 2 * height;++i)  
    {  
        for(int j = 0;j < 2 * width;++j)  
        {  
            after_i = i - midX_aft;//��???  
            after_j = j - midY_aft;  
            pre_i = (int)(cos((double)angle) * after_i - sin((double)angle) * after_j) + midX_pre;  
            pre_j = (int)(sin((double)angle) * after_i + cos((double)angle) * after_j) + midY_pre;  
            if(pre_i >= 0 && pre_i < height && pre_j >= 0 && pre_j < width)//�݌�?�?��  
                *(imagedataRot + i * 2 * width + j) = *(imagedata + pre_i * width + pre_j);  
        }  
    }  
  
    //�ۑ�bmp?��  
    if((fpw=fopen("b.bmp","wb"))==NULL)  
    {  
        cout<<"create the bmp file error!"<<endl;  
        return NULL;  
    }  
    WORD bfType_w=0x4d42;  
    fwrite(&bfType_w,1,sizeof(WORD),fpw);  
    //fpw +=2;  
    fwrite(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpw);  
    strInfo.biWidth = 2 * width;  
    strInfo.biHeight = 2 * height;  
    fwrite(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpw);  
    //�ۑ�?�F����  
    for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)  
    {  
        fwrite(&strPla[nCounti].rgbBlue,1,sizeof(BYTE),fpw);  
        fwrite(&strPla[nCounti].rgbGreen,1,sizeof(BYTE),fpw);  
        fwrite(&strPla[nCounti].rgbRed,1,sizeof(BYTE),fpw);  
        fwrite(&strPla[nCounti].rgbReserved,1,sizeof(BYTE),fpw);  
    }  
    //�ۑ����f����  
    for(int i =0;i < 2 * height;++i)  
    {  
        for(int j = 0;j < 2 * width;++j)  
        {  
            fwrite( &((*(imagedataRot + i * 2 * width + j)).blue),1,sizeof(BYTE),fpw);  
            //fwrite( &((*(imagedataRot + i * 2 * width + j)).green),1,sizeof(BYTE),fpw);  
            //fwrite( &((*(imagedataRot + i * 2 * width + j)).red),1,sizeof(BYTE),fpw);  
        }  
    }  
    fclose(fpw);  
  
    //?������  
    delete[] imagedata;  
    delete[] imagedataRot;  
}

//��?����?��?;  
//�����s��ܕ���?�^�M���i�R��??�̓I����??�r��C  
//�v�������I?���s�\��??�敶���M���j  
typedef struct  tagBITMAPFILEHEADER{  
    //WORD bfType;//����?�^�C�K?��0x424D�C�������gBM�h  
    DWORD bfSize;//�����召  
    WORD bfReserved1;//�ۗ���  
    WORD bfReserved2;//�ۗ���  
    DWORD bfOffBits;//������?��??��?�����I�Έڎ�?��  
}BITMAPFILEHEADER;  
  
typedef struct tagBITMAPINFOHEADER{  
    DWORD biSize;//�M��?�召  
    LONG biWidth;//?��?�x  
    LONG biHeight;//?�����x  
    WORD biPlanes;//�ʕ��ʐ��C�K??1  
    WORD biBitCount;//?���f�ʐ�  
    DWORD  biCompression; //???�^  
    DWORD  biSizeImage; //???���召��?��  
    LONG  biXPelsPerMeter; //����������  
    LONG  biYPelsPerMeter; //����������  
    DWORD  biClrUsed; //��???�p���I�F�ʐ�  
    DWORD  biClrImportant; //�{��?���d�v�I�F�ʐ�  
}BITMAPINFOHEADER; //��?�M��?��?  
  
typedef struct tagRGBQUAD{  
    BYTE rgbBlue; //??�F�I?�F����  
    BYTE rgbGreen; //??�F�I?�F����  
    BYTE rgbRed; //??�F�I?�F����  
    BYTE rgbReserved; //�ۗ�?  
}RGBQUAD;//?�F��?  
  
//���f�M��  
typedef struct tagIMAGEDATA  
{  
    BYTE blue;  
    //BYTE green;  
    //BYTE red;  
}IMAGEDATA;





#include<iostream>  
#include <Windows.h>  
  
using namespace std;  
  
  
void main()  
{  
      
    FILE* stream=fopen("D:\\3.bmp","rb");  
    if(stream==NULL)  
    {  
        cout<<"�����s����"<<endl;  
        return;  
    }  
      
    int sizeFileHeader=sizeof(BITMAPFILEHEADER);  
    int sizeInfoHeader=sizeof(BITMAPINFOHEADER);  
      
    BITMAPFILEHEADER* bitmapFileHeader=new BITMAPFILEHEADER[sizeFileHeader+1];  
      
    BITMAPINFOHEADER* bitmapInfoHeader=new BITMAPINFOHEADER[sizeInfoHeader+1];  
      
    memset(bitmapFileHeader,0,sizeFileHeader+1);  
    memset(bitmapInfoHeader,0,sizeInfoHeader+1);  
    fread(bitmapFileHeader,sizeof(char),sizeFileHeader,stream);  
    fseek(stream,sizeFileHeader,0);  
    fread(bitmapInfoHeader,sizeof(char),sizeInfoHeader,stream);  
    int srcImageLineByteCount=(((bitmapInfoHeader->biWidth*24)+31)/32)*4;  
    fseek(stream,sizeFileHeader+sizeInfoHeader,0);  
  
    //?��?������  
    int count=srcImageLineByteCount-bitmapInfoHeader->biWidth*(bitmapInfoHeader->biBitCount/8);  
    BYTE* tempData=new BYTE[count];  
    BYTE*** oldImageData=new BYTE**[bitmapInfoHeader->biHeight];  
    for(int i=0;i<bitmapInfoHeader->biHeight;i++)  
    {  
        oldImageData[i]=new BYTE*[bitmapInfoHeader->biWidth];  
        for (int j=0;j<bitmapInfoHeader->biWidth;j++)  
        {  
            oldImageData[i][j]=new BYTE[3];  
            for(int k=0;k<3;k++)  
            {  
                fread(&oldImageData[i][j][k],sizeof(BYTE),1,stream);  
            }  
        }  
          
        for(int m=0;m<count;m++)  
        {  
            fread(&tempData[m],sizeof(BYTE),1,stream);  
        }  
    }  
      
  
    fclose(stream);  
      
    //��������  
      
    BYTE*** newImageData=new BYTE**[bitmapInfoHeader->biHeight];  
      
    for(int i=0;i<bitmapInfoHeader->biHeight;i++)  
    {  
        newImageData[i]=new BYTE*[bitmapInfoHeader->biWidth];  
        for (int j=0;j<bitmapInfoHeader->biWidth;j++)  
        {  
            newImageData[i][j]=new BYTE[3];  
            for (int k=0;k<3;k++)  
            {  
                newImageData[i][j][k]=oldImageData[bitmapInfoHeader->biHeight-i-1][bitmapInfoHeader->biWidth-j-1][k];  
            }  
        }  
          
    }  
  
  
    //�ʐ���  
    FILE* fileWrite=fopen("D:\\7.bmp","a+");  
    fwrite(bitmapFileHeader,sizeof(char),sizeof(BITMAPFILEHEADER),fileWrite);  
    fwrite(bitmapInfoHeader,sizeof(char),sizeof(BITMAPINFOHEADER),fileWrite);  
    for(int i=0;i<bitmapInfoHeader->biHeight;i++)  
    {  
        for(int j=0;j<bitmapInfoHeader->biWidth;j++)  
        {  
            for(int k=0;k<3;k++)  
            {  
                fwrite(&newImageData[i][j][k],sizeof(BYTE),1,fileWrite);  
            }  
              
        }  
        for (int s=0;s<count;s++)  
        {  
            fwrite(&tempData[s],sizeof(BYTE),1,fileWrite);  
        }  
  
    }  
    fclose(fileWrite);  
    delete newImageData;  
    delete oldImageData;  
  
    cout<<"success"<<endl;  
}  
