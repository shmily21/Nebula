/*
***************************************************************************************************
// FileName     :Applet.h
//
// Brief        :Applet
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

#ifndef __WANGBIN_NEBULA_MODULES_APPLET_INCLUDE_START_H__
#define __WANGBIN_NEBULA_MODULES_APPLET_INCLUDE_START_H__

/*
* For Microsoft Compiler In Windows.
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif // if defined(_MSC_VER) && _MSC_VER > 1000

/*
* globa header
*/
#include "global.h"

extern app_data_st __g_applet_data;

#ifdef __linux__
    int main(int argc,char* argv[]);
#elif defined(WIN32)

    #include <Windows.h>
    #include <WinBase.h>


    INT APIENTRY WinMain(HINSTANCE hInstance,           // Current Instance
                        HINSTANCE hPreInstance,         // Pre Hinstance,Discard this,DON'T CARE.
                        LPSTR lpCmdLine,                // Command Line
                        INT nCmdShow                    // Command Show
                        );
#endif


#endif __WANGBIN_NEBULA_MODULES_APPLET_INCLUDE_START_H__