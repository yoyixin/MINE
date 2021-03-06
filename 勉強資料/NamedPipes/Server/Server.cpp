// Server.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>  
//#include <ctime>  

int main(int argc, char* argv[])
{
    //srand(time(NULL));  
  
    char buf[256] = "";  
    DWORD rlen = 0;  
    HANDLE hPipe = CreateNamedPipe(  
        TEXT("\\\\.\\Pipe\\mypipe"),                        //ｹﾜｵﾀﾃ�  
        PIPE_ACCESS_DUPLEX,                                 //ｹﾜｵﾀﾀ獎ﾍ   
        PIPE_TYPE_MESSAGE|PIPE_READMODE_MESSAGE|PIPE_WAIT,  //ｹﾜｵﾀｲﾎﾊ�  
        PIPE_UNLIMITED_INSTANCES,                           //ｹﾜｵﾀﾄﾜｴｴｽｨｵﾄﾗ�ｴ�ﾊｵﾀ�ﾊ�ﾁｿ  
        0,                                                  //ﾊ莎�ｻｺｳ衂�ｳ､ｶﾈ 0ｱ�ﾊｾﾄｬﾈﾏ  
        0,                                                  //ﾊ菠�ｻｺｳ衂�ｳ､ｶﾈ 0ｱ�ﾊｾﾄｬﾈﾏ  
        NMPWAIT_WAIT_FOREVER,                               //ｳｬﾊｱﾊｱｼ�  
        NULL);                                                  //ﾖｸｶｨﾒｻｸ�SECURITY_ATTRIBUTESｽ盪ｹ,ｻ�ﾕﾟｴｫｵﾝﾁ聊ｵ.  
    if (INVALID_HANDLE_VALUE == hPipe)  
    {  
        printf("Create Pipe Error(%d)\n",GetLastError());  
    }  
    else  
    {  
        printf("Waiting For Client Connection...\n");  
        if(ConnectNamedPipe(hPipe, NULL)==NULL) //ﾗ霾�ｵﾈｴ�ｿﾍｻｧｶﾋﾁｬｽﾓ｡｣  
        {  
            printf("Connection failed!\n");  
        }  
        else  
        {  
            printf("Connection Success!\n");  
        }  
  
        while (true)  
        {  
            if(ReadFile(hPipe,buf,256,&rlen,NULL)==FALSE) //ｽﾓﾊﾜｿﾍｻｧｶﾋｷ｢ﾋﾍｹ�ﾀｴｵﾄﾄﾚﾈﾝ  
            {             
                printf("Read Data From Pipe Failed!\n");  
                break;  
            }  
            else  
            {  
                printf("From Client: data = %s, size = %d\n", buf, rlen);  
                char wbuf[256] = "";  
                sprintf(wbuf, "%s%d", wbuf);  
                DWORD wlen = 0;  
                WriteFile(hPipe, wbuf, sizeof(wbuf), &wlen, 0); //ﾏ�ｿﾍｻｧｶﾋｷ｢ﾋﾍﾄﾚﾈﾝ  
                printf("To Client: data = %s, size = %d\n", wbuf, wlen);  
                Sleep(1000);  
            }  
        }  
        CloseHandle(hPipe);//ｹﾘｱﾕｹﾜｵﾀ  
    }  
  
    //system("PAUSE");  
    return 0;  
}