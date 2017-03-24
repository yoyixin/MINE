// (c) COPYRIGHT OMRON Corporation 2008 All Rights Reserved.
//
// FILE NAME     : bmp_trans.cpp
//
//┌───┬────┬─────────────────────┬──────┐
//│ 履歴 │  DATE  │              NOTES                       │    SIGN    │
//├───┼────┼─────────────────────┼──────┤
//│      │17.3.14 │新規作成                                  │HYRON楊     │
//├───┼────┼─────────────────────┼──────┤
//│      │        │                                          │            │
//└───┴────┴─────────────────────┴──────┘
#include "bmp_trans.h"

//DEFINE_LOG_TRACE("bmp_trans.log")
DEFINE_LOG

namespace bmptrans {

  #define PI 3.14159
  //ファイルヘッダ
  typedef struct tagBITMAPFILEHEADER_H{
      WORD bfType;           //識別文字（BMが必須）  
      DWORD bfSize;          //ファイルサイズ
      WORD bfReserved1;      //予備
      WORD bfReserved2;      //予備  
      DWORD bfOffBits;       //データ部までのオフセット
  }BITMAPFILEHEADER_H;

  //情報ヘッダ
  typedef struct tagBITMAPINFOHEADER_H{  
  //  DWORD biSize;          //BMP情報ヘッダのサイズ 最初チェックのため、定義なし
      LONG biWidth;          //BMPの幅
      LONG biHeight;         //BMPの高さ
      WORD biPlanes;         //プレーン数（１が必須）  
      WORD biBitCount;       //1ピクセルのデータ数  
      DWORD  biCompression;  //圧縮 biCompression (無圧縮ならば 0) 
      DWORD  biSizeImage;    //画像のサイズ
      LONG  biXPelsPerMeter; //横方向解像度
      LONG  biYPelsPerMeter; //縦方向解像度
      DWORD  biClrUsed;      //パレット数
      DWORD  biClrImportant; //パレット中の重要な色  
  }BITMAPINFOHEADER_H;

  //パレット情報
  typedef struct tagRGBQUAD_H{  
      BYTE rgbBlue;          //ブルーの分量
      BYTE rgbGreen;         //グリーンの分量
      BYTE rgbRed;           //レッドの分量
      BYTE rgbReserved;      //予備
  }RGBQUAD_H;

  //BMP情報
  typedef struct tagIMAGEDATA_H
  {
      BYTE blue;  
      BYTE green;  
      BYTE red;  
  }IMAGEDATA_H;

  bool MakeYokoBmp(const char* fromFileName, const char* toFileName, int rotateAngle)
  {
    OMRONSOFT_TRACE("変更元ファイル：　"<< fromFileName);
    OMRONSOFT_TRACE("変更先ファイル：　"<< toFileName);
    OMRONSOFT_TRACE("角度：　"<< rotateAngle);
    FILE *fpi,*fpw;
    int width,height;                  //BMPの幅、高さ
    BITMAPFILEHEADER_H strHead;        //ファイルヘッダ
    BITMAPINFOHEADER_H strInfo;        //情報ヘッダ
    RGBQUAD_H strPla[256];             //パレット
    IMAGEDATA_H *imagedata = NULL;     //回り前画素情報
    IMAGEDATA_H *imagedataRot = NULL;  //回り後画素情報

    fpi=fopen(fromFileName,"rb");
    if(!fpi) return false;

    //BMP情報をチェック(BMか？)
    WORD bfType;  
    fread(&bfType,1,sizeof(WORD),fpi);  
    if(0x4d42!=bfType)  
    {
      return false;  
    }
OMRONSOFT_TRACE("…ファイルヘッダを読み込み中…");
    //ファイルヘッダを読み込み
    fread(&strHead,1,sizeof(tagBITMAPFILEHEADER_H),fpi);
OMRONSOFT_TRACE("…情報ヘッダを読み込み中…");
    //情報ヘッダを読み込み
    fread(&strInfo,1,sizeof(tagBITMAPINFOHEADER_H),fpi);
OMRONSOFT_TRACE("…パレット情報を読み込み中…");
    //パレット情報を読み込み
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
OMRONSOFT_TRACE("幅：　" << width);
OMRONSOFT_TRACE("高さ：　" << height);
    //画像の幅は、４の倍数になること
    width = (width * sizeof(IMAGEDATA_H) + 3) / 4 * 4;  
    imagedata = (IMAGEDATA_H*)malloc(width * height * sizeof(IMAGEDATA_H));
    //imagedata = (IMAGEDATA_H*)malloc(width * height);
    imagedataRot = (IMAGEDATA_H*)malloc(width * height * sizeof(IMAGEDATA_H));
OMRONSOFT_TRACE("OLD画素情報初期化");
    //回り前の画素情報を初期化する
    for(int i = 0;i < height;++i)  
    {  
      for(int j = 0;j < width;++j)  
      {  
        (*(imagedata + i * width + j)).blue = 0;  
        //(*(imagedata + i * width + j)).green = 0;  
        //(*(imagedata + i *  width + j)).red = 0;  
      }
    }
OMRONSOFT_TRACE("NEW画素情報初期化");
    //回り後の画素情報を初期化する
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
    //BMPデータを読み込み
    fread(imagedata,sizeof(struct tagIMAGEDATA_H) * width,height,fpi);  
    fclose(fpi);
OMRONSOFT_TRACE("NEW幅：　" << width);
OMRONSOFT_TRACE("NEW高さ：　" << height);
    //BMP回る処理
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
OMRONSOFT_TRACE("新BMP画素座標：　(" << i << "," << j << ")");
OMRONSOFT_TRACE("転換元BMP画素座標：　(" << pre_i << "," << pre_j << ")");
        }
      }
    }
    
    //BMPを保存する
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
    //パレット情報
    {
      for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)  
      {  
          fwrite(&strPla[nCounti].rgbBlue,1,sizeof(BYTE),fpw);  
          fwrite(&strPla[nCounti].rgbGreen,1,sizeof(BYTE),fpw);  
          fwrite(&strPla[nCounti].rgbRed,1,sizeof(BYTE),fpw);  
          fwrite(&strPla[nCounti].rgbReserved,1,sizeof(BYTE),fpw);  
      }
    }
    //画素情報保存
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
  
    //メモリ解放
    delete[] imagedata;  
    delete[] imagedataRot;  
    
    return true;
  }

}
