/*
***************************************************************************************************
// FileName     :
//
// Brief        :
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

#ifndef __WANGBIN_NEBULA_MODULES_GLOBAL_INCLUDE_NEBULAERROR_H__
#define __WANGBIN_NEBULA_MODULES_GLOBAL_INCLUDE_NEBULAERROR_H__

/*
* For Microsoft Compiler In Windows.
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif // if defined(_MSC_VER) && _MSC_VER > 1000

#if defined(WIN32) && defined(_MSC_VER)
#include <Windows.h>
#include <WinBase.h>
    #if defined(_DEBUG)
        #define DEG_MESSAGE(CODE,MSG) \
            char _szDebugInfo[128]; \
            ::ZeroMemory(_szDebugInfo,sizeof(_szDebugInfo)); \
            _snprintf(_szDebugInfo,sizeof(_szDebugInfo),"ThreadId=[0x%x] Code=[%s] Report=[%s] \tGetLastError=[%lld]\n", \
                    static_cast<long long int>(::GetCurrentThreadId()), \
                    (const char*)CODE, \
                    (const char*)MSG, \
                    static_cast<long long int>(::GetLastError())); \
            ::OutputDebugString(_szDebugInfo)
    #endif /* _DEBUG */
#endif /* defined(WIN32) && defined(_MSC_VER) */


#define ERROR_MESSAGE_TITLE                 "Applet"

#define ERROR_CREATE_FILE_MAPPING           0x01
#define ERROR_CREATE_FILE_MAPPING_CODE      "0x00000001"
#define ERROR_CARETE_FILE_MAPPING_STRING    "Cannot Strat Application,Error Code = 0x00000001"

#define ERROR_MAP_VIEW_OF_FILE              0x02
#define ERROR_MAP_VIEW_OF_FILE_CODE         "0x00000002"
#define ERROR_MAP_VIEW_OF_FILE_STRING       "Cannot Start Application,Error Code = 0x00000002"

#endif /* __WANGBIN_NEBULA_MODULES_GLOBAL_INCLUDE_NEBULAERROR_H__ */