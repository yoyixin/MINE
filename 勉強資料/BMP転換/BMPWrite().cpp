#include <stdio.h>  
#include "BmpRot.h"  
#include "stdlib.h"  
#include "math.h"  
#include <iostream>  
  
#define PI 3.14159//?ü¦Gè?  
#define LENGTH_NAME_BMP 30//bmp?Ð¶¼IÅå?x  
  
using namespace std;  
  
//?Êè?  
BITMAPFILEHEADER strHead;  
RGBQUAD strPla[256];//256F?FÂ  
BITMAPINFOHEADER strInfo;  
  
//?¦Ê?¶?M§  
void showBmpHead(BITMAPFILEHEADER pBmpHead){  
    cout<<"Ê?¶?:"<<endl;  
    cout<<"¶å¬:"<<pBmpHead.bfSize<<endl;  
    cout<<"Û¯_1:"<<pBmpHead.bfReserved1<<endl;  
    cout<<"Û¯_2:"<<pBmpHead.bfReserved2<<endl;  
    cout<<"??Ê?IÎÚ?:"<<pBmpHead.bfOffBits<<endl<<endl;  
}  
  
void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead){  
    cout<<"Ê?M§?:"<<endl;  
    cout<<"??ÌI?x:"<<pBmpInforHead.biSize<<endl;  
    cout<<"Ê??:"<<pBmpInforHead.biWidth<<endl;  
    cout<<"Ê?:"<<pBmpInforHead.biHeight<<endl;  
    cout<<"biPlanes½Ê:"<<pBmpInforHead.biPlanes<<endl;  
    cout<<"biBitCountÑp?FÊ:"<<pBmpInforHead.biBitCount<<endl;  
    cout<<"??û®:"<<pBmpInforHead.biCompression<<endl;  
    cout<<"biSizeImage??Ê?èpI?:"<<pBmpInforHead.biSizeImage<<endl;  
    cout<<"Xûüª¦:"<<pBmpInforHead.biXPelsPerMeter<<endl;  
    cout<<"Yûüª¦:"<<pBmpInforHead.biYPelsPerMeter<<endl;  
    cout<<"gpI?F:"<<pBmpInforHead.biClrUsed<<endl;  
    cout<<"dv?F:"<<pBmpInforHead.biClrImportant<<endl;  
}  
  
  
int main(){  
    char strFile[LENGTH_NAME_BMP];//bmp¶¼  
    IMAGEDATA *imagedata = NULL;//??ªz¶?´?ÐIfM§Iñ??  
    IMAGEDATA *imagedataRot = NULL;//??ªz¶?ù?@I?ÐIfM§Iñ??  
    int width,height;//?ÐI?xax  
    cout<<"??üv?æI¶¼:"<<endl;  
    cin>>strFile;  
    FILE *fpi,*fpw;  
    fpi=fopen(strFile,"rb");  
    if(fpi != NULL){  
        //æ?æ¶?^  
        WORD bfType;  
        fread(&bfType,1,sizeof(WORD),fpi);  
        if(0x4d42!=bfType)  
        {  
            cout<<"the file is not a bmp file!"<<endl;  
            return NULL;  
        }  
        //?æbmp¶I¶?aM§?  
        fread(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpi);  
        //showBmpHead(strHead);//?¦¶?  
        fread(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpi);  
        //showBmpInforHead(strInfo);//?¦¶M§?  
  
        //?æ?FÂ  
        for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)  
        {  
            fread((char *)&(strPla[nCounti].rgbBlue),1,sizeof(BYTE),fpi);  
            fread((char *)&(strPla[nCounti].rgbGreen),1,sizeof(BYTE),fpi);  
            fread((char *)&(strPla[nCounti].rgbRed),1,sizeof(BYTE),fpi);  
            fread((char *)&(strPla[nCounti].rgbReserved),1,sizeof(BYTE),fpi);  
        }  
  
        width = strInfo.biWidth;  
        height = strInfo.biHeight;  
        //??êsI?K?¥4I®{  
        width = (width * sizeof(IMAGEDATA) + 3) / 4 * 4;  
        imagedata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));  
        //imagedata = (IMAGEDATA*)malloc(width * height);  
        imagedataRot = (IMAGEDATA*)malloc(2 * width * 2 * height * sizeof(IMAGEDATA));  
        //n»´n?ÐIf?  
        for(int i = 0;i < height;++i)  
        {  
            for(int j = 0;j < width;++j)  
            {  
                (*(imagedata + i * width + j)).blue = 0;  
                //(*(imagedata + i * width + j)).green = 0;  
                //(*(imagedata + i *  width + j)).red = 0;  
            }  
        }  
        //n»ù?@?ÐIf?  
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
        //?o?ÐIf  
        fread(imagedata,sizeof(struct tagIMAGEDATA) * width,height,fpi);  
        fclose(fpi);  
    }  
    else  
    {  
        cout<<"file open error!"<<endl;  
        return NULL;  
    }  
  
    //?Ðù??  
    int RotateAngle;//vù?Ipx  
    double angle;//vù?IÊx  
    int midX_pre,midY_pre,midX_aft,midY_aft;//ù???IS_I¿?  
    midX_pre = width / 2;  
    midY_pre = height / 2;  
    midX_aft = width;  
    midY_aft = height;  
    int pre_i,pre_j,after_i,after_j;//ù?O@??If_¿?  
    cout<<"?üvù?Ipxi0x360xCt??ù?jF"<<endl;  
    cin>>RotateAngle;  
    angle = 1.0 * RotateAngle * PI / 180;  
    for(int i = 0;i < 2 * height;++i)  
    {  
        for(int j = 0;j < 2 * width;++j)  
        {  
            after_i = i - midX_aft;//¿???  
            after_j = j - midY_aft;  
            pre_i = (int)(cos((double)angle) * after_i - sin((double)angle) * after_j) + midX_pre;  
            pre_j = (int)(sin((double)angle) * after_i + cos((double)angle) * after_j) + midY_pre;  
            if(pre_i >= 0 && pre_i < height && pre_j >= 0 && pre_j < width)//Ý´?ä?à  
                *(imagedataRot + i * 2 * width + j) = *(imagedata + pre_i * width + pre_j);  
        }  
    }  
  
    //Û¶bmp?Ð  
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
    //Û¶?FÂ  
    for(unsigned int nCounti=0;nCounti<strInfo.biClrUsed;nCounti++)  
    {  
        fwrite(&strPla[nCounti].rgbBlue,1,sizeof(BYTE),fpw);  
        fwrite(&strPla[nCounti].rgbGreen,1,sizeof(BYTE),fpw);  
        fwrite(&strPla[nCounti].rgbRed,1,sizeof(BYTE),fpw);  
        fwrite(&strPla[nCounti].rgbReserved,1,sizeof(BYTE),fpw);  
    }  
    //Û¶f  
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
  
    //?úà¶  
    delete[] imagedata;  
    delete[] imagedataRot;  
}

//Ê?¶?è?;  
//´sïÜ¶?^M§iR°??ÌIà¶??rèC  
//v¥Á¹I?«s\³??æ¶M§j  
typedef struct  tagBITMAPFILEHEADER{  
    //WORD bfType;//¶?^CK?¥0x424DC¦gBMh  
    DWORD bfSize;//¶å¬  
    WORD bfReserved1;//Û¯  
    WORD bfReserved2;//Û¯  
    DWORD bfOffBits;//¸¶???Ê?IÎÚ?  
}BITMAPFILEHEADER;  
  
typedef struct tagBITMAPINFOHEADER{  
    DWORD biSize;//M§?å¬  
    LONG biWidth;//??x  
    LONG biHeight;//?x  
    WORD biPlanes;//Ê½ÊCK??1  
    WORD biBitCount;//?fÊ  
    DWORD  biCompression; //???^  
    DWORD  biSizeImage; //???å¬?  
    LONG  biXPelsPerMeter; //½ª¦  
    LONG  biYPelsPerMeter; //¼ª¦  
    DWORD  biClrUsed; //Ê???pIFÊ  
    DWORD  biClrImportant; //{Ê?dvIFÊ  
}BITMAPINFOHEADER; //Ê?M§?è?  
  
typedef struct tagRGBQUAD{  
    BYTE rgbBlue; //??FI?FªÊ  
    BYTE rgbGreen; //??FI?FªÊ  
    BYTE rgbRed; //??FI?FªÊ  
    BYTE rgbReserved; //Û¯?  
}RGBQUAD;//?FÂè?  
  
//fM§  
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
        cout<<"¶s¶Ý"<<endl;  
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
  
    //?æ?  
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
      
    //¾  
      
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
  
  
    //Ê  
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
