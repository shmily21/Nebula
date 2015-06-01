#include "autosocket.h"

#ifdef NEBULA_FOR_WINDOWS

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <Windows.h>
#include <WinBase.h>

#include <vector>

#pragma comment(lib,"Ws2_32.lib")       // For Socket
#pragma comment(lib,"Kernel32.lib")     // For IO Completion Port (IOCP)

#define DATA_BUFFER_SIZE    1024*2

#define SERVER_TCP_PORT     8902

typedef struct
{
    OVERLAPPED      overlapped;
    WSABUF          databuff;
    char            buffer[DATA_BUFFER_SIZE];
    int             buffer_len;
    int             operation_type;
}PER_IO_OPERATION_DATA,*LPPER_IO_OPERATION_DATA,*LPPER_IO_DATA,PER_IO_DATA;

typedef struct
{
    SOCKET                  socket;
    SOCKADDR_STORAGE        ClientAddr;
}PER_HANDLE_DATA,*LPPER_HANDLE_DATA;

static HINSTANCE __g_hinstance(NULL);
static BOOL     __g_run_server(TRUE);
static std::vector<PER_HANDLE_DATA*> clientGroup;

static HANDLE hMutex(NULL);
DWORD WINAPI ServerWorkThread(LPVOID CompletionPortID);
DWORD WINAPI ServerSendThread(LPVOID lpParam);

static SOCKET skServer(INVALID_SOCKET);
static HANDLE hIOCP(NULL);

UINT WINAPI AcceptThreadProc(LPVOID lpParameter)
{
    sockadd_in sockAddr={0};
    int len=sizeof(sockAddr);
    while(TRUE)
    {
        SOCKET sNew=WSAAccept(skServer,(sockaddr*)&sockAddr,&len,NULL,NULL);
        ::CreateIoCompletionPort((HANDLE)sNew,hIOCP,ULONG_PTR(sNew),0);
        LPPER_IO_OPERATION_DATA PerIoData(NULL);
        PerIoData=(LPPER_IO_OPERATION_DATA)::GlobalAlloc(::GetProcessHeap(),sizeof(PER_IO_OPERATION_DATA));
        ::ZeroMemory(&(PerIoData->overlapped),sizeof(OVERLAPPED));
        PerIoData->databuff.len=1024;
        PerIoData->databuff.buf=PerIoData->buffer;
        PerIoData->operation_type=0;

        DWORD RecvBytes(0);
        DWORD Flags(0);
        WSARecv(PerHandleData->socket,&(PerIoData->databuff),1,&RecvBytes,&Flags,&(PerIoData->overlapped),NULL);
        int Error=WSAGetLastError();
    }
}

UINT WINAPI WorkerThreadProc(LPVOID lpParameter)
{
    DWORD CompleteBytes(0);
    DWORD dwKey(0);
    POVERLAPPED
}



INT APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPreInstance,LPSTR lpCmdLine,INT nCmdShow)
{
    UNREFERENCED_PARAMETER(hPreInstance);
    ::RegisterApplicationRestart(L"applet_server_restarted",0);
    __g_hinstance=hInstance;

    ::WSADATA wsaData;
    INT nWsaDataResult=::WSAStartup(MAKEWORD(0x02,0x02),&wsaData);
    if(NO_ERROR != nWsaDataResult)
    {
        ::OutputDebugString("WSAStartup Error\n");
        return EXIT_FAILURE;
    }

    hMutex=::CreateMutex(NULL,FALSE,NULL);

    HANDLE hCompletionPort=::CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
    if(NULL==hCompletionPort)
    {
        ::OutputDebugString("CreateIoCompletionPort Error\n");
        ::WSACleanup();
        return EXIT_FAILURE;
    }

    SYSTEM_INFO mySysInfo;
    ::GetSystemInfo(&mySysInfo);

    for(DWORD i(0);i<(mySysInfo.dwNumberOfProcessors*2);++i)
    {
        HANDLE ThreadHandle=::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ServerWorkThread,hCompletionPort,0,NULL);
        if(NULL==ThreadHandle)
        {
            ::OutputDebugString("CreateThread Error\n");
            ::WSACleanup();
            return EXIT_FAILURE;
        }
        ::CloseHandle(ThreadHandle);
    }


    SOCKET srvSocket=::WSASocket(AF_INET,SOCKET_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);

    SOCKADDR_IN srvAddr;
    srvAddr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
    srvAddr.sin_family=AF_INET;
    srvAddr.sin_port=htons(SERVER_TCP_PORT);

    INT bindResult=::bind(srvSocket,(SOCKADDR*)&srvAddr,sizeof(SOCKADDR));
    if(SOCKET_ERROR==bindResult)
    {
        ::OutputDebugString("bind Error\n")
        ::WSACleanup();
        return EXIT_FAILURE;
    }

    INT listenResult=::listen(srvSocket,SOMAXCONN);
    if(SOCKET_ERROR==listenResult)
    {
        ::OutputDebugString("listen Error\n");
        ::WSACleanup();
        return EXIT_FAILURE;
    }

    //开始处理IO数据
    HANDLE sendThread=::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ServerSendThread,0,0,NULL);

    while(__g_run_server)
    {
        PER_HANDLE_DATA* PerHandleData(NULL);
        SOCKADDR_IN saRemote;
        INT RemoteLen;
        SOCKET acceptSocket;
        acceptSocket=::accept(srvSocket,(SOCKADDR*)&saRemote,&RemoteLen);
        if(SOCKET_ERROR==acceptSocket)
        {
            ::OutputDebugString("accept Error\n");
            ::WSACleanup();
            return EXIT_FAILURE;
        }

        PerHandleData=(LPPER_HANDLE_DATA)::GlobalAlloc(::GetProcessHeap(),sizeof(PER_HANDLE_DATA));
        PerHandleData->socket=acceptSocket;
        ::CopyMemory(&PerHandleData->ClientAddr,&saRemote,RemoteLen);
        clientGroup.push_back(PerHandleData);

        ::CreateIoCompletionPort((HANDLE)(PerHandleData->socket),hCompletionPort,(DWORD)PerHandleData,0);

        LPPER_IO_OPERATION_DATA PerIoData(NULL);
        PerIoData=(LPPER_IO_OPERATION_DATA)::GlobalAlloc(::GetProcessHeap(),sizeof(PER_IO_OPERATION_DATA));
        ::ZeroMemory(&(PerIoData->overlapped),sizeof(OVERLAPPED));
        PerIoData->databuff.len=1024;
        PerIoData->databuff.buf=PerIoData->buffer;

        DWORD RecvBytes(0);
        DWORD Flags(0);
        ::WSARecv(PerHandleData->socket,&(PerIoData->databuff),1,&RecvBytes,&Flags,&(PerIoData->overlapped),NULL);
    }

    ::WSACleanup();
    return EXIT_SUCCESS;
}

DWORD WINAPI ServerWorkThread(LPVOID lpParameter)
{
    HANDLE CompletionPort=(HANDLE)lpParameter;
    DWORD BytesTransferred(0);
    LPOVERLAPPED lpOverlapped;
    LPPER_HANDLE_DATA PerHandleData(NULL);
    LPPER_IO_DATA PerIoData(NULL);
    DWORD RecvBytes(0);
    DWORD Flags(0);
    BOOL bRet(FALSE);

    while(__g_run_server)
    {
        bRet=::GetQueuedCompletionStatus(CompletionPort,&BytesTransferred,(PULONG_PTR)&PerHandleData,(LPOVERLAPPED*)&lpOverlapped,INFINITE);
        if(0==bRet)
        {
            ::OutputDebugString("GetQueuedCompletionStatus Error\n");
            return EXIT_FAILURE;
        }

        PerIoData=(LPPER_IO_DATA)CONTAINING_RECORD(lpOverlapped,PER_IO_DATA,overlapped);

        if(0==BytesTransferred)
        {
            closesocket(PerHandleData->socket);
            GlobalFree(PerHandleData);
            GlobalFree(PerIoData);
            continue;
        }

        ::WaitForSingleObject(hMutex,INFINITE);
        ::OutputDebugString(PerIoData->databuff.buf);
        ::OutputDebugString("\n");
        ::ReleaseMutex(hMutex);

        ZeroMemory(&(PerIoData->overlapped),sizeof(OVERLAPPED));
        PerIoData->databuff.len=1024;
        PerIoData->databuff.buf=PerIoData->buffer;
        PerIoData->operation_type=0;
        ::WSARecv(PerHandleData->socket,&(PerIoData->databuff),1,&RecvBytes,&Flags,&(PerIoData->overlapped),NULL);
    }
    return EXIT_SUCCESS;
}

DWORD WINAPI ServerSendThread(LPVOID lpParameter)
{
    while(__g_run_server)
    {
        char talk[200];
        gets(talk);
        int len(0);
        for(len=0;talk[len]!='\0';++len)
        {

        }


    }
}

#endif /* NEBULA_FOR_WINDOWS */