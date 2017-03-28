// Client.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctime> 

int main(int argc, char* argv[])
{
    srand(time(NULL));  
  
    DWORD wlen = 0;  
    Sleep(1000);//ｵﾈｴ�pipeｵﾄｴｴｽｨｳﾉｹｦ｣｡  
  
    BOOL bRet = WaitNamedPipe(TEXT("\\\\.\\Pipe\\mypipe"), NMPWAIT_WAIT_FOREVER);  
  
    if (!bRet)  
    {  
        printf("connect the namedPipe failed!\n");  
        return 0;  
    }  
  
    HANDLE hPipe=CreateFile(            //ｹﾜｵﾀﾊ�ﾓﾚﾒｻﾖﾖﾌﾘﾊ箏ﾄﾎﾄｼ�  
        TEXT("\\\\.\\Pipe\\mypipe"),    //ｴｴｽｨｵﾄﾎﾄｼ�ﾃ�  
        GENERIC_READ | GENERIC_WRITE,   //ﾎﾄｼ�ﾄ｣ﾊｽ  
        0,                              //ﾊﾇｷ�ｹｲﾏ�  
        NULL,                           //ﾖｸﾏ�ﾒｻｸ�SECURITY_ATTRIBUTESｽ盪ｹｵﾄﾖｸﾕ�  
        OPEN_EXISTING,                  //ｴｴｽｨｲﾎﾊ�  
        FILE_ATTRIBUTE_NORMAL,          //ﾎﾄｼ�ﾊ�ﾐﾔ(ﾒ�ｲﾘ,ﾖｻｶﾁ)NORMALﾎｪﾄｬﾈﾏﾊ�ﾐﾔ  
        NULL);                          //ﾄ｣ｰ蟠ｴｽｨﾎﾄｼ�ｵﾄｾ莖�  
  
    if (INVALID_HANDLE_VALUE == hPipe)  
    {  
        printf("open the exit pipe failed!\n");  
    }  
    else  
    {  
        while(true)  
        {  
            char buf[256] = "";  
            sprintf(buf,"%s%d",buf, rand()%1000);
            if(WriteFile(hPipe,buf,sizeof(buf),&wlen,0)==FALSE) //ﾏ�ｷ�ﾎ�ﾆ�ｷ｢ﾋﾍﾄﾚﾈﾝ  
            {  
                printf("write to pipe failed!\n");  
                break;  
            }  
            else  
            {  
                printf("To Server: data = %s, size = %d\n", buf, wlen);  
                char rbuf[256] = "";  
                DWORD rlen = 0;  
                ReadFile(hPipe, rbuf, sizeof(rbuf), &rlen, 0);  //ｽﾓﾊﾜｷ�ﾎ�ｷ｢ﾋﾍｹ�ﾀｴｵﾄﾄﾚﾈﾝ  
                printf("From Server: data = %s, size = %d\n", rbuf, rlen);  
            }  
            Sleep(1000);  
        }  
        CloseHandle(hPipe);//ｹﾘｱﾕｹﾜｵﾀ  
    }  
  
    //system("PAUSE");  
    return 0;  
}

