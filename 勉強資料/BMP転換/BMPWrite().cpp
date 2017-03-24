#include <stdio.h>  
#include "BmpRot.h"  
#include "stdlib.h"  
#include "math.h"  
#include <iostream>  
  
#define PI 3.14159//?周率宏定?  
#define LENGTH_NAME_BMP 30//bmp?片文件名的最大?度  
  
using namespace std;  
  
//?量定?  
BITMAPFILEHEADER strHead;  
RGBQUAD strPla[256];//256色?色板  
BITMAPINFOHEADER strInfo;  
  
//?示位?文件?信息  
void showBmpHead(BITMAPFILEHEADER pBmpHead){  
    cout<<"位?文件?:"<<endl;  
    cout<<"文件大小:"<<pBmpHead.bfSize<<endl;  
    cout<<"保留字_1:"<<pBmpHead.bfReserved1<<endl;  
    cout<<"保留字_2:"<<pBmpHead.bfReserved2<<endl;  
    cout<<"??位?数据的偏移字?数:"<<pBmpHead.bfOffBits<<endl<<endl;  
}  
  
void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead){  
    cout<<"位?信息?:"<<endl;  
    cout<<"??体的?度:"<<pBmpInforHead.biSize<<endl;  
    cout<<"位??:"<<pBmpInforHead.biWidth<<endl;  
    cout<<"位?高:"<<pBmpInforHead.biHeight<<endl;  
    cout<<"biPlanes平面数:"<<pBmpInforHead.biPlanes<<endl;  
    cout<<"biBitCount采用?色位数:"<<pBmpInforHead.biBitCount<<endl;  
    cout<<"??方式:"<<pBmpInforHead.biCompression<<endl;  
    cout<<"biSizeImage??位?数据占用的字?数:"<<pBmpInforHead.biSizeImage<<endl;  
    cout<<"X方向分辨率:"<<pBmpInforHead.biXPelsPerMeter<<endl;  
    cout<<"Y方向分辨率:"<<pBmpInforHead.biYPelsPerMeter<<endl;  
    cout<<"使用的?色数:"<<pBmpInforHead.biClrUsed<<endl;  
    cout<<"重要?色数:"<<pBmpInforHead.biClrImportant<<endl;  
}  
  
  
int main(){  
    char strFile[LENGTH_NAME_BMP];//bmp文件名  
    IMAGEDATA *imagedata = NULL;//??分配存?原?片的像素信息的二?数?  
    IMAGEDATA *imagedataRot = NULL;//??分配存?旋?后的?片的像素信息的二?数?  
    int width,height;//?片的?度和高度  
    cout<<"??入所要?取的文件名:"<<endl;  
    cin>>strFile;  
    FILE *fpi,*fpw;  
    fpi=fopen(strFile,"rb");  
    if(fpi != NULL){  
        //先?取文件?型  
        WORD bfType;  
        fread(&bfType,1,sizeof(WORD),fpi);  
        if(0x4d42!=bfType)  
        {  
            cout<<"the file is not a bmp file!"<<endl;  
            return NULL;  
        }  
        //?取bmp文件的文件?和信息?  
        fread(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpi);  
        //showBmpHead(strHead);//?示文件?  
        fread(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpi);  
        //showBmpInforHead(strInfo);//?示文件信息?  
  
        //?取?色板  
        for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)  
        {  
            fread((char *)&(strPla[nCounti].rgbBlue),1,sizeof(BYTE),fpi);  
            fread((char *)&(strPla[nCounti].rgbGreen),1,sizeof(BYTE),fpi);  
            fread((char *)&(strPla[nCounti].rgbRed),1,sizeof(BYTE),fpi);  
            fread((char *)&(strPla[nCounti].rgbReserved),1,sizeof(BYTE),fpi);  
        }  
  
        width = strInfo.biWidth;  
        height = strInfo.biHeight;  
        //?像?一行的字?数必?是4的整数倍  
        width = (width * sizeof(IMAGEDATA) + 3) / 4 * 4;  
        imagedata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));  
        //imagedata = (IMAGEDATA*)malloc(width * height);  
        imagedataRot = (IMAGEDATA*)malloc(2 * width * 2 * height * sizeof(IMAGEDATA));  
        //初始化原始?片的像素数?  
        for(int i = 0;i < height;++i)  
        {  
            for(int j = 0;j < width;++j)  
            {  
                (*(imagedata + i * width + j)).blue = 0;  
                //(*(imagedata + i * width + j)).green = 0;  
                //(*(imagedata + i *  width + j)).red = 0;  
            }  
        }  
        //初始化旋?后?片的像素数?  
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
        //?出?片的像素数据  
        fread(imagedata,sizeof(struct tagIMAGEDATA) * width,height,fpi);  
        fclose(fpi);  
    }  
    else  
    {  
        cout<<"file open error!"<<endl;  
        return NULL;  
    }  
  
    //?片旋??理  
    int RotateAngle;//要旋?的角度数  
    double angle;//要旋?的弧度数  
    int midX_pre,midY_pre,midX_aft,midY_aft;//旋?所??的中心点的坐?  
    midX_pre = width / 2;  
    midY_pre = height / 2;  
    midX_aft = width;  
    midY_aft = height;  
    int pre_i,pre_j,after_i,after_j;//旋?前后??的像素点坐?  
    cout<<"?入要旋?的角度（0度到360度，逆??旋?）："<<endl;  
    cin>>RotateAngle;  
    angle = 1.0 * RotateAngle * PI / 180;  
    for(int i = 0;i < 2 * height;++i)  
    {  
        for(int j = 0;j < 2 * width;++j)  
        {  
            after_i = i - midX_aft;//坐???  
            after_j = j - midY_aft;  
            pre_i = (int)(cos((double)angle) * after_i - sin((double)angle) * after_j) + midX_pre;  
            pre_j = (int)(sin((double)angle) * after_i + cos((double)angle) * after_j) + midY_pre;  
            if(pre_i >= 0 && pre_i < height && pre_j >= 0 && pre_j < width)//在原?范?内  
                *(imagedataRot + i * 2 * width + j) = *(imagedata + pre_i * width + pre_j);  
        }  
    }  
  
    //保存bmp?片  
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
    //保存?色板数据  
    for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)  
    {  
        fwrite(&strPla[nCounti].rgbBlue,1,sizeof(BYTE),fpw);  
        fwrite(&strPla[nCounti].rgbGreen,1,sizeof(BYTE),fpw);  
        fwrite(&strPla[nCounti].rgbRed,1,sizeof(BYTE),fpw);  
        fwrite(&strPla[nCounti].rgbReserved,1,sizeof(BYTE),fpw);  
    }  
    //保存像素数据  
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
  
    //?放内存  
    delete[] imagedata;  
    delete[] imagedataRot;  
}

//位?文件?定?;  
//其中不包含文件?型信息（由于??体的内存??决定，  
//要是加了的?将不能正??取文件信息）  
typedef struct  tagBITMAPFILEHEADER{  
    //WORD bfType;//文件?型，必?是0x424D，即字符“BM”  
    DWORD bfSize;//文件大小  
    WORD bfReserved1;//保留字  
    WORD bfReserved2;//保留字  
    DWORD bfOffBits;//从文件?到??位?数据的偏移字?数  
}BITMAPFILEHEADER;  
  
typedef struct tagBITMAPINFOHEADER{  
    DWORD biSize;//信息?大小  
    LONG biWidth;//?像?度  
    LONG biHeight;//?像高度  
    WORD biPlanes;//位平面数，必??1  
    WORD biBitCount;//?像素位数  
    DWORD  biCompression; //???型  
    DWORD  biSizeImage; //???像大小字?数  
    LONG  biXPelsPerMeter; //水平分辨率  
    LONG  biYPelsPerMeter; //垂直分辨率  
    DWORD  biClrUsed; //位???用到的色彩数  
    DWORD  biClrImportant; //本位?中重要的色彩数  
}BITMAPINFOHEADER; //位?信息?定?  
  
typedef struct tagRGBQUAD{  
    BYTE rgbBlue; //??色的?色分量  
    BYTE rgbGreen; //??色的?色分量  
    BYTE rgbRed; //??色的?色分量  
    BYTE rgbReserved; //保留?  
}RGBQUAD;//?色板定?  
  
//像素信息  
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
        cout<<"文件不存在"<<endl;  
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
  
    //?取?像数据  
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
      
    //得到数据  
      
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
  
  
    //写数据  
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
