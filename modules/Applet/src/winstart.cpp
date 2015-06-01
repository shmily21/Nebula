#include "start.h"

#include <cstdio>

#ifdef NEBULA_FOR_WINDOWS


#include <Windows.h>
#include <WinBase.h>

DWORD __watch_process_in_time(LPVOID config)
{
    DEG_MESSAGE("0x00000003","__watch_process_in_time");
    __g_applet_data.WatchThreadSwitcher=1;
    while(__g_applet_data.WatchThreadSwitcher)
    {
        STARTUPINFO start_info={sizeof(STARTUPINFO)};
        PROCESS_INFORMATION process_info;
        start_info.dwFlags=STARTF_USESHOWWINDOW;
        start_info.wShowWindow=SW_SHOW;
        LPTSTR lpCmdLine=FILE_MAPPING_NAME;
        DEG_MESSAGE("0x00000004","__watch_process_in_time");
        BOOL ret=::CreateProcess("c:\\Windows\\system32\\notepad.exe",lpCmdLine,NULL,NULL,TRUE,0,NULL,NULL,&start_info,&process_info);
        if(ret)
        {
            __g_applet_data.ProcessSi=start_info;
            __g_applet_data.ProcessPi=process_info;
            if(!WaitForSingleObject(process_info.hProcess,INFINITE))
            {
                DWORD _exit_code(0);
                if(::GetExitCodeProcess(process_info.hProcess,&_exit_code))
                {
                    if(EXIT_SUCCESS==_exit_code)
                    {
                        __g_applet_data.WatchThreadSwitcher=0;
                    }
                    else
                    {
                        DEG_MESSAGE("0x00000005","GetExitCodeProcess");
                    }
                }
                else
                {
                    DEG_MESSAGE("0x00000006","GetExitCodeProcess");
                }
                ::OutputDebugStringA("Notepad is Close\n");
            }
        }
        else
        {
            DEG_MESSAGE("0x00000005","CreateProcess");
            ::Sleep(1000);
        }
    }
    return 0;
}

DWORD __server_process(LPVOID config)
{

    return 0;
}

INT APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPreInstance,LPSTR lpCmdLine,INT nCmdShow)
{
    UNREFERENCED_PARAMETER(hPreInstance);
    // For Application Restart Operate After Runing 61 seconds ....
    ::RegisterApplicationRestart(L"applet_restarted",0);

    __g_applet_data.hInstance   =hInstance;
    __g_applet_data.lpCmdLine   =lpCmdLine;
    __g_applet_data.nCmdShow    =nCmdShow;
    __g_applet_data.hFileMapping=::CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0x00,sizeof(nebula_data_st),FILE_MAPPING_NAME);
    if(!__g_applet_data.hFileMapping)
    {
        DEG_MESSAGE("0x00000001","CreateFileMapping");
        ::MessageBox(NULL,ERROR_CARETE_FILE_MAPPING_STRING,ERROR_MESSAGE_TITLE,MB_OK);
        return EXIT_FAILURE;
    }
    __g_applet_data.pMapViewOfFile=::MapViewOfFile(__g_applet_data.hFileMapping,FILE_MAP_ALL_ACCESS,0,0,0);
    if(!__g_applet_data.pMapViewOfFile)
    {
        DEG_MESSAGE("0x00000002","MapViewOfFile");
        ::MessageBox(NULL,ERROR_MAP_VIEW_OF_FILE_STRING,ERROR_MESSAGE_TITLE,MB_OK);
        ::CloseHandle(__g_applet_data.hFileMapping);
        return EXIT_FAILURE;
    }
    __g_applet_data.nRestart_Times=0;
    __g_applet_data.WatchThreadSwitcher=1;

    HANDLE hThread=::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)__watch_process_in_time,NULL,0,NULL);
    if(!::WaitForSingleObject(hThread,INFINITE))
    {
        ;
    }
    if(__g_applet_data.pMapViewOfFile) ::UnmapViewOfFile(__g_applet_data.pMapViewOfFile);
    if(__g_applet_data.hFileMapping) ::CloseHandle(__g_applet_data.hFileMapping);
    return EXIT_SUCCESS;
}

#endif /* NEBULA_FOR_WINDOWS */