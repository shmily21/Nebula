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
#include <CommCtrl.h>

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
#define SCROLLBAR_STATE_HOTTRACKED      0x00000002L
#define SCROLLBAR_STATE_PRESSED         0x00000003L
#define SCROLLBAR_STATE_DISABLED        0x00000004L

// 用于向滚动条发出消息
// 窗口句柄，消息，位置，是否是垂直滚动条
#define SendScrollBarMessage(hwnd,cmd,pos,vert) \
    ::SendMessage((HWND)(hwnd),(vert)?WM_VSCROLL:WM_HSCROLL,MAKEWPARAM((cmd),(pos)),0)


#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagScrollBar
{
    HWND        hwnd;                       /* 窗口句柄 */
    UINT        flags;                      /* 滚动条禁用标记 */
    DWORD       style;                      /* 窗口Style */
    SCROLLINFO  Horz;                       /* 水平滚动条 */
    SCROLLINFO  Vert;                       /* 垂直滚动条 */
    WNDPROC     pfnOldProc;                 /* 原窗口处理函数 */
    BOOL        fLeftScrollBar;             /* 滚动条左侧Style */
    BOOL        fPreventStyleChange;        /* 不允许更改滚动条样式 */
    BOOL        fMouseTracking;             /* 鼠标滚动 */
    BOOL        fTracking;                  /* 滚动 */
    BOOL        fTrackVert;                 /* 垂直滚动标记 */
    BOOL        fRichEdit;                  /* 富文本编辑控件 */
    int         nTrackCode;                 /* 点击测试代码 */
    int         nTrackPos;                  /* 点击测试位置 */
    int         nOffsetPoint;               /* 鼠标点击位置偏移量 */
    UINT        nScrollTimerMsg;            /* 消息WPARAM参数 */
    UINT        nLastCode;                  /* 上次点击测试代码 */
    BOOL        fLastVert;                  /* 上次点击测试为垂直滚动 */
    HBITMAP     hBmp;                       /* 关联位图 */
}SCROLLBAR,*LPSCROLLBAR;

/* 用于滚动条计算的数据结构 */
typedef struct tagScrollBarCalc
{
    INT     pixelLeft;
    INT     pixelTop;
    INT     pixelRight;
    INT     pixelBottom;
    union
    {
        INT     pixelUpArrow;
        INT     pixelLeftArrow;
    };
    union
    {
        INT     pixelDownArrow;
        INT     pixelRightArrow;
    };
    union
    {
        INT     pixelThumbTop;
        INT     pixelThumbLeft;
    };
    union
    {
        INT     pixelThumbBottom;
        INT     pixelThumbRight;
    };

}SCROLLBARCALC,*LPSCROLLBARCALC;

LPSCROLLBAR SkinEx_GetScrollBar(HWND hwnd);

BOOL SkinEx_SetScrollBarParams(LPSCROLLINFO psi,SCROLLINFO si,BOOL* pfScroll,INT* plres,BOOL bOldPos);

BOOL SkinEx_IsScrollInfoActive(LPSCROLLINFO lpsi);

BOOL SkinEx_GetSizeBoxRect(LPSCROLLBAR psb,LPRECT lprc);

BOOL SkinEx_GetScrollBarRect(LPSCROLLBAR psb,BOOL fVert,LPRECT lprc);

VOID SkinEx_ScrollBarCalc(LPSCROLLBAR psb,LPSCROLLBARCALC lprc,BOOL fVert);

BOOL SkinEx_GetThumbRect(LPSCROLLBAR psb,LPRECT lprc,BOOL fVert);

BOOL SkinEx_GetGrooveRect(LPSCROLLBAR psb,LPRECT lprc,BOOL fVert);



BOOL SkinEx_DrawGroove(LPSCROLLBAR psb,HDC hDC,LPRECT lprc,BOOL fVert);

BOOL SkinEx_DrawArrow(LPSCROLLBAR psb,HDC hDC,BOOL fVert,INT nArrow,UINT uState);

VOID SkinEx_DrawThumb(LPSCROLLBAR psb,HDC hDC,BOOL fVert);

VOID SkinEx_DrawScrollBar(LPSCROLLBAR psb,HDC hDC,BOOL fVert);

BOOL SkinEx_DrawSizeBox(LPSCROLLBAR psb,HDC hDC);

VOID SkinEx_Track(LPSCROLLBAR psb,BOOL fVert,UINT nHit,POINT pt);

BOOL SkinEx_TrackThumb(LPSCROLLBAR psb,BOOL fVert,POINT pt);

BOOL SkinEx_HotTrack(LPSCROLLBAR psb,INT nHitCode,BOOL fVert,BOOL fMouseDown);

UINT SkinEx_HitTest(LPSCROLLBAR psb,BOOL fVert,POINT pt);

BOOL SkinEx_EnableArrows(LPSCROLLBAR psb,INT nBar,UINT nArrows);

UINT SkinEx_GetDisableFlags(LPSCROLLBAR psb,BOOL fVert);

UINT SkinEx_GetState(LPSCROLLBAR psb,BOOL fVert,UINT nHit);



LRESULT SkinEx_OnStyleChanged(LPSCROLLBAR psb,INT nStyleType,LPSTYLESTRUCT lpStyleStruct);

LRESULT SkinEx_OnNcHitTest(LPSCROLLBAR psb,WPARAM wParam,LPARAM lParam);

LRESULT SkinEx_OnNcPaint(LPSCROLLBAR psb,WAPRAM wParam,LPARAM lParam);

LRESULT SkinEx_OnNcCalcSize(LPSCROLLBAR psb,BOOL bClacValidRect,NCCALCSIZE_PARAMS* lpncsp);

LRESULT SkinEx_OnNcMouseMove(LPSCROLLBAR psb,WPARAM wParam,LPARAM lParam);

LRESULT SkinEx_OnNcLButtonDown(LPSCROLLBAR psb,WPARAM wParam,LPARAM lParam);

LRESULT SkinEx_OnNcMouseLeave(LPSCROLLBAR psb,WPARAM wParam,LPARAM lParam);

LRESULT SkinEx_OnMouseMove(LPSCROLLBAR psb,WPARAM wParam,LPARAM lParam);

LRESULT SkinEx_OnLButtonUp(LPSCROLLBAR psb,WAPRAM wParam,LPARAM lParam);

LRESULT SkinEx_OnTimer(LPSCROLLBAR psb,WPARAM wParam,LPARAM lParam);



LRESULT CALLBACK SkinEx_Proc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

#ifdef __cplusplus
}
#endif


#endif /* WIN32 */

#endif /* __WANGBIN_NEBULA_MODULES_SKINEX_INCLUDE_INTERNAL_H__ */

