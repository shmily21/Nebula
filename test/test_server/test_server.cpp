#include "global.h"
#if defined(NEBULA_FOR_WINDOWS)
#   include <Windows.h>
#   include <WinBase.h>
#elif defined(NEBULA_FOR_LINUX)
#
#else
#   error "No Support Platform."
#endif


#ifndef WIN32

int main(int agrc,char* argv[])
{
    WSADATA     wsaData;
    INT nWsaVersion=::WSAStartup(MAKEWORD(0x02,0x02),&wsaData);
    if(NO_ERROR!=nWsaVersion)
    {
        return EXIT_FAILURE;
    }

    SOCKET sockClient=::socket(AF_INET,SOCK_STREAM,0);
    if(INVALID_SOCKET==sockClient)
    {
        ::WSACleanup();
        return EXIT_FAILURE;
    }

    SOCKADDR_IN     addSrv;
    addSrv.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
    addSrv.sin_family=AF_INET;
    addSrv.sin_port=htons(8902);

    while(SOCKET_ERROR==::connect(sockClient,(SOCKADDR*)&addSrv,sizeof(SOCKADDR)))
    {
        ;
    }
}

#else


#endif