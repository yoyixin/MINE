// Client.cpp : 僐儞僜乕儖 傾僾儕働乕僔儑儞梡偺僄儞僩儕 億僀儞僩偺掕媊
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
    Sleep(1000);//等待pipe的创建成功！  
  
    BOOL bRet = WaitNamedPipe(TEXT("\\\\.\\Pipe\\mypipe"), NMPWAIT_WAIT_FOREVER);  
  
    if (!bRet)  
    {  
        printf("connect the namedPipe failed!\n");  
        return 0;  
    }  
  
    HANDLE hPipe=CreateFile(            //管道属于一种特殊的文件  
        TEXT("\\\\.\\Pipe\\mypipe"),    //创建的文件名  
        GENERIC_READ | GENERIC_WRITE,   //文件模式  
        0,                              //是否共享  
        NULL,                           //指向一个SECURITY_ATTRIBUTES结构的指针  
        OPEN_EXISTING,                  //创建参数  
        FILE_ATTRIBUTE_NORMAL,          //文件属性(隐藏,只读)NORMAL为默认属性  
        NULL);                          //模板创建文件的句柄  
  
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
            if(WriteFile(hPipe,buf,sizeof(buf),&wlen,0)==FALSE) //向服务器发送内容  
            {  
                printf("write to pipe failed!\n");  
                break;  
            }  
            else  
            {  
                printf("To Server: data = %s, size = %d\n", buf, wlen);  
                char rbuf[256] = "";  
                DWORD rlen = 0;  
                ReadFile(hPipe, rbuf, sizeof(rbuf), &rlen, 0);  //接受服务发送过来的内容  
                printf("From Server: data = %s, size = %d\n", rbuf, rlen);  
            }  
            Sleep(1000);  
        }  
        CloseHandle(hPipe);//关闭管道  
    }  
  
    //system("PAUSE");  
    return 0;  
}

