/*
***************************************************************************************************
// FileName     :appletst.h
//
// Brief        :appletst
//
// Author       :WangBin
//
// Description  :
//
//
//
// CopyRight    : WangBin.
//                                              <perturbed@sina.com>
//
***************************************************************************************************
*/

#ifndef __WANGBIN_NEBULA_MODULES_GLOBAL_INCLUDE_APPLETST_H__
#define __WANGBIN_NEBULA_MODULES_GLOBAL_INCLUDE_APPLETST_H__

/*
* For Microsoft Compiler In Windows.
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif // if defined(_MSC_VER) && _MSC_VER > 1000

#ifdef WIN32
#ifdef __NEBULA_GLOBAL_USE_SOCKET
#   include <WinSock2.h>
#   include <WS2tcpip.h>
#endif
#include <Windows.h>
#include <WinBase.h>
#endif

/*
* For Applet Runtime Data Struct
*/
typedef struct APP_DATA_ST
{
#ifdef WIN32
    HINSTANCE               hInstance;
    LPSTR                   lpCmdLine;
    INT                     nCmdShow;

    HANDLE                  hFileMapping;
    LPVOID                  pMapViewOfFile;

    STARTUPINFO             ProcessSi;
    PROCESS_INFORMATION     ProcessPi;
#elif defined(__linux__)
    char**                  lpCmdLine;
#endif
    int                     nRestart_Times;             // 子进程重启次数
    int                     WatchThreadSwitcher;        // 监视子进程开关
    int                     SocketThreadSwitcher;       // 网络服务线程开关
}app_data_st;

#endif  /* __WANGBIN_NEBULA_MODULES_GLOBAL_INCLUDE_APPLETST_H__ */