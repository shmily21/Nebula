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

#ifndef __WANGBIN_NEBULA_MODULES_SKINEX_INCLUDE_INTERNAL_H__
#define __WANGBIN_NEBULA_MODULES_SKINEX_INCLUDE_INTERNAL_H__

/*
* For Microsoft Compiler In Windows.
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif // if defined(_MSC_VER) && _MSC_VER > 1000

#include "global.h"

#ifdef WIN32
#include <Windows.h>
#include <WinBase.h>
#include <Windowsx.h>
#include <Commctrl.h>

// 滚动条最小 Thumb 尺寸
#define SCROLLBAR_MINTHUMB_SIZE         10

// 滚动条 计时器 编号
#define SCROLLBAR_TIMER_DELAY           65533
#define SCROLLBAR_TIMER_SCROLL          65534

// 滚动条计时器启动延迟时间
#define SCROLLBAR_SCROLL_DELAY          300
#define SCROLLBAR_SCROLL_INTERVAL       40

// 点击测试代码
#define HITTEST_SCROLLBAR_NONE          0x00000000L
#define HITTEST_SCROLLBAR_LINEUP        0x00000001L
#define HITTEST_SCROLLBAR_LINEDOWN      0x00000002L
#define HITTEST_SCROLLBAR_THUMB         0x00000003L
#define HITTEST_SCROLLBAR_PAGEUP        0x00000004L
#define HITTEST_SCROLLBAR_PAGEDOWN      0x00000005L
#define HITTEST_SCROLLBAR_SIZEBOX       0x00000006L

// 滚动条状态 代码
#define SCROLLBAR_STATE_DEFAULT         0x00000000L
#define SCROLLBAR_STATE_NORMAL          0x00000001L
#define SCROLLBAR_STATE_HOTTARACKED     0x00000002L
#define SCROLLBAR_STATE_PRESSED         0x00000003L
#define SCROLLBAR_STATE_DISABLED        0x00000004L

// 用于向滚动条发出消息
// 窗口句柄，消息，位置，是否是垂直滚动条
#define SendScrollBarMessage(hwnd,cmd,pos,vert) \
    ::SendMessage((HWND)(hwnd),(vert)?WM_VSCROLL:WM_HSCROLL,MAKEWPARAM((cmd),(pos)),0)


typedef struct tagScrollBar
{
    HWND        hwnd;                       /* 窗口句柄 */
    UINT        flags;                      /* 滚动条禁用标记 */
    DWORD       style;                      /* 窗口Style */
    SCROLLINFO  Horz;                       /* 水平滚动条 */
    SCROLLINFO  Vert;                       /* 垂直滚动条 */
    WINPROC     pfnOldProc;                 /* 原窗口处理函数 */
    BOOL        fLeftScrollBar;             /* 滚动条左侧Style */
    BOOL        fPreventStyleChange;        /* 不允许更改滚动条样式 */
    BOOL        fMouseTracking;             /*  */
    BOOL        fTracking;                  /*  */
    BOOL        fTrackVert;                 /*  */
    BOOL        fRichEdit;                  /*  */
	int         nTrackCode;                 /*  */
	int         nTrackPos;                  /*  */
	int         nOffsetPoint;               /*  */
	UINT        nScrollTimerMsg;            /*  */
	UINT        nLastCode;                  /*  */
	BOOL        fLastVert;                  /*  */
	HBITMAP     hBmp;                       /*  */
}

#endif /* WIN32 */

#endif /* __WANGBIN_NEBULA_MODULES_SKINEX_INCLUDE_INTERNAL_H__ */