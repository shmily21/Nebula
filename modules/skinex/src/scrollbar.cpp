#include "internal.h"
#include "scrollbar.h"

#include <cmath>

/* 标记用于属性查找时的关键字 */
static TCHAR g_szPropScrollBar[]=TEXT("SkinEx_ScrollBar_Prop");

// 滚动条图片资源文件按照以下规格划分，图片资源大小 110*110
static POINT ptArray[6][6]=
{
    { {0,  0}, {19,  0}, {38,  0}, {57,  0}, {76,  0}, {95,  0} },
    { {0, 19}, {19, 19}, {38, 19}, {57, 19}, {76, 19}, {95, 19} },
    { {0, 38}, {19, 38}, {38, 38}, {57, 38}, {76, 38}, {95, 38} },
    { {0, 57}, {19, 57}, {38, 57}, {57, 57}, {76, 57}, {95, 57} },
    { {0, 76}, {19, 76}, {38, 76}, {57, 76}, {76, 76}, {95, 76} },
    { {0, 95}, {19, 95}, {38, 95}, {57, 95}, {76, 95}, {95, 95} }
};

/*
* 获取滚动条数据结构
*/
LPSCROLLBAR SkinEx_GetScrollBar(HWND hWnd)
{
    return (LPSCROLLBAR)GetProp(hWnd,g_szPropScrollBar);
}

BOOL SkinEx_SetScrollBarParams(LPSCROLLINFO psi,SCROLLINFO si,BOOL* pfScroll,INT* plres,BOOL bOldPos)
{
    if(!psi || !pfScroll || !plres) return FALSE;
    BOOL fChanegd(FALSE);
    if(bOldPos) *plres=psi->nPos;

    if(si.fMask & SIF_RANGE)
    {
        if(si.nMax<si.nMin) si.nMax=si.nMin;

        if(si.nMax != psi->nMax || si.nMin != psi->nMin )
        {
            psi->nMax=si.nMax;
            psi->nMin=si.nMin;

            if(!(si.fMask & SIF_PAGE))
            {
                si.fMask|=SIF_PAGE;
                si.nPage=psi->nPage;
            }

            if(!(si.fMask & SIF_POS))
            {
                si.fMask|=SIF_POS;
                si.nPos=psi->nPos;
            }
            fChanegd=TRUE;
        }
    }

    if(si.fMask & SIF_PAGE)
    {
        UINT nMaxPage=std::abs(psi->nMax-psi->nMin)+1;
        if(si.nPage > nMaxPage) si.nPage=nMaxPage;

        if(psi->nPage != si.nPage )
        {
            psi->nPage=si.nPage;

            if(!(si.fMask & SIF_POS))
            {
                si.fMask|=SIF_POS;
                si.nPos=psi->nPos;
            }

            fChanegd=TRUE;
        }
    }

    if(si.fMask & SIF_POS)
    {
        INT nMaxPos=psi->nMax-((psi->nPage)?psi->nPage-1:0);
        if(si.nPos<psi->nMin) si.nPos=psi->nMin;
        else if(si.nPos>nMaxPos) si.nPos=nMaxPos;

        if(psi->nPos != si.nPos )
        {
            psi->nPos=si.nPos;
            fChanegd=TRUE;
        }
    }

    if(si.fMask & SIF_TRACKPOS)
    {
        if(psi->nTrackPos != si.nTrackPos )
        {
            psi->nTrackPos=si.nTrackPos;
            fChanegd=TRUE;
        }
    }

    if(!bOldPos)
        *plres=psi->nPos;

    if(si.fMask & SIF_RANGE)
    {
        if(*pfScroll=(psi->nMin !=psi->nMax ))
            *pfScroll=((INT)psi->nPage<=(psi->nMax-psi->nMin));
    }
    else if(si.fMask & SIF_PAGE)
    {
        *pfScroll=((INT)psi->nPage<=(psi->nMax-psi->nMin));
    }
    return fChanegd;
}

/*
* 检查滚动条是否为活动状态
*/
BOOL SkinEx_IsScrollInfoActive(LPSCROLLINFO lpsi)
{
    if(lpsi->nPage>(UINT)lpsi->nMax || lpsi->nMax<=lpsi->nMin || lpsi->nMax==0) return FALSE;
    else return TRUE;
}


/*
* 获取滚动条SizeBox区域大小
*/
BOOL SkinEx_GetSizeBoxRect(LPSCROLLBAR psb,LPRECT lprc)
{
    if(!psb || !lprc) return FALSE;
    RECT    rect;
    ::SetRectEmpty(lprc);
    DWORD   dwStyle=::GetWindowLong(psb->hwnd,GWL_STYLE);
    if((dwStyle & WS_HSCROLL) && (dwStyle & WS_VSCROLL))
    {
        ::GetClientRect(psb->hwnd,&rect);
        ::ClientToScreen(psb->hwnd,(LPPOINT)&rect);
        ::ClientToScreen(psb->hwnd,((LPPOINT)&rect)+1);

        if(psb->fLeftScrollBar)
        {
            lprc->left=rect.left-::GetSystemMetrics(SM_CXVSCROLL);
            lprc->right=rect.left;
        }
        else
        {
            lprc->left=rect.right;
            lprc->right=rect.right+::GetSystemMetrics(SM_CXVSCROLL);
        }

        lprc->top=rect.bottom;
        lprc->bottom=rect.bottom+::GetSystemMetrics(SM_CYHSCROLL);
        return TRUE;
    }
    return FALSE;
}

/*
* 获取滚动条区域矩形
*/
BOOL SkinEx_GetScrollBarRect(LPSCROLLBAR psb,BOOL fVert,LPRECT lprc)
{
    if(!psb || !lprc) return FALSE;
    RECT rect;
    ::SetRectEmpty(&rect);
    ::GetClientRect(psb->hwnd,&rect);
    ::ClientToScreen(psb->hwnd,(LPPOINT)&rect);
    ::ClientToScreen(psb->hwnd,((LPPOINT)&rect)+1);

    DWORD dwStyle=::GetWindowLong(psb->hwnd,GWL_STYLE);

    if(fVert)   // 如果是垂直滚动条
    {
        if(psb->fLeftScrollBar)
        {
            lprc->right=lprc->left=rect.left;
            if(dwStyle & WS_VSCROLL)
            {
                lprc->left-=::GetSystemMetrics(SM_CXVSCROLL);
            }
        }
        else
        {
            lprc->right=lprc->left=rect.right;
            if(dwStyle & WS_VSCROLL)
            {
                lprc->right+=::GetSystemMetrics(SM_CXVSCROLL);
            }
        }
        lprc->top=rect.top;
        lprc->bottom=rect.bottom;
    }
    else
    {
        lprc->top=lprc->bottom=rect.bottom;
        if(dwStyle & WS_HSCROLL)
        {
            lprc->bottom+=::GetSystemMetrics(SM_CYHSCROLL);
        }
        lprc->left=rect.left;
        lprc->right=rect.right;
    }
    return TRUE;
}

/*
* 计算滚动条项的位置及大小
*/
VOID SkinEx_ScrollBarCalc(LPSCROLLBAR psb,LPSCROLLBARCALC lpcalc,BOOL fVert)
{
    RECT            rcBar;
    INT             nRange;
    INT             nWorkingsize;
    INT             nArrowsize;
    INT             nThumbsize;
    INT             nStart;
    INT             nThumbpos;
    SCROLLINFO*     psi;

    SkinEx_GetScrollBarRect(psb,fVert,&rcBar);

    lpcalc->pixelLeft   =rcBar.left;
    lpcalc->pixelTop    =rcBar.top;
    lpcalc->pixelRight  =rcBar.right;
    lpcalc->pixelBottom =rcBar.bottom;

    if(fVert)
    {
        psi=&psb->Vert;
        nArrowsize=::GetSystemMetrics(SM_CYVSCROLL);
        nWorkingsize=(rcBar.bottom-rcBar.top)-nArrowsize*2;
        nStart=rcBar.top+nArrowsize;

        lpcalc->pixelUpArrow=rcBar.top+nArrowsize;
        lpcalc->pixelDownArrow=rcBar.bottom-nArrowsize;
    }
    else
    {
        psi=&psb->Horz;
        nArrowsize=::GetSystemMetrics(SM_CXHSCROLL);
        nWorkingsize=(rcBar.right-rcBar.left)-nArrowsize*2;
        nStart=rcBar.left+nArrowsize;

        lpcalc->pixelUpArrow=rcBar.left+nArrowsize;
        lpcalc->pixelDownArrow=rcBar.right-nArrowsize;
    }

    nRange=(psi->nMax-psi->nMin)+1;
    if(nRange>0 && SkinEx_IsScrollInfoActive(psi))
    {
        nThumbsize=::MulDiv(psi->nPage,nWorkingsize,nRange);
        if(nThumbsize<SCROLLBAR_MINTHUMB_SIZE)
            nThumbsize=SCROLLBAR_MINTHUMB_SIZE;
        INT pagesize=(psi->nPage>1)?psi->nPage:1;
        nThumbpos=MulDiv(psi->nPos-psi->nMin,nWorkingsize-nThumbsize,nRange-pagesize);
        if(nThumbpos<0) nThumbpos=0;

        if(nThumbpos>=nWorkingsize-nThumbsize)
            nThumbpos=nWorkingsize-nThumbsize;

        nThumbpos+=nStart;
        lpcalc->pixelThumbTop=nThumbpos;
        lpcalc->pixelThumbBottom=nThumbpos+nThumbsize;
    }
    else
    {
        lpcalc->pixelThumbTop=0;
        lpcalc->pixelThumbBottom=0;
    }

}

/*
* 获取滚动条Thumb区域矩形
*/
BOOL SkinEx_GetThumbRect(LPSCROLLBAR psb,LPRECT lprc,BOOL fVert)
{
    if(!psb || !lprc) return FALSE;
    SCROLLBARCALC sbc;
    SkinEx_ScrollBarCalc(psb,&sbc,fVert);
    RECT rect;
    ::GetWindowRect(psb->hwnd,&rect);

    if(fVert)
    {
        SetRect(lprc,sbc.pixelLeft,sbc.pixelThumbTop,sbc.pixelRight,sbc.pixelBottom);
    }
    else
    {
        SetRect(lprc,sbc.pixelLeft,sbc.pixelTop,sbc.pixelThumbRight,sbc.pixelBottom);
    }
    ::OffsetRect(lprc,-rect.left,-rect.top);
    return TRUE;
}

BOOL SkinEx_GetGrooveRect(LPSCROLLBAR psb,LPRECT lprc,BOOL fVert)
{
    if(!psb || !lprc) return FALSE;
    SCROLLBARCALC   sbc;
    RECT            rect;
    SkinEx_ScrollBarCalc(psb,&sbc,fVert);
    ::GetWindowRect(psb->hwnd,&rect);
    if(fVert)
    {
        ::SetRect(lprc,sbc.pixelLeft,sbc.pixelUpArrow,sbc.pixelRight,sbc.pixelDownArrow);
    }
    else
    {
        ::SetRect(lprc,sbc.pixelUpArrow,sbc.pixelTop,sbc.pixelDownArrow,sbc.pixelBottom);
    }

    ::OffsetRect(lprc,-rect.left,-rect.top);
    return TRUE;
}

/************************************************************\
*                                                           *
*       Draw This ScrollBar                                 *
*                                                           *
\************************************************************/

BOOL SkinEx_DrawGroove(LPSCROLLBAR psb,HDC hDC,LPRECT lprc,BOOL fVert)
{
    if(!hDC || !lprc || IsRectEmpty(lprc)) return FALSE;

    HDC hbmpDC=::CreateCompatibleDC(hDC);
    HBITMAP hOldBmp=(HBITMAP)::SelectObject(hbmpDC,psb->hBmp);

    POINT       pt;

    if(fVert)
    {
        pt=ptArray[0][4];
    }
    else
    {
        pt=ptArray[3][4];
    }

    INT nMode=::SetStretchBltMode(hDC,HALFTONE);
    ::StretchBlt(hDC,lprc->left,lprc->top,lprc->right-lprc->left,lprc->bottom-lprc->top,hbmpDC,pt.x,pt.y,16,16,SRCCOPY);
    ::SetStretchBltMode(hDC,nMode);
    ::SelectObject(hbmpDC,hOldBmp);
    ::DeleteDC(hbmpDC);
    return TRUE;
}

BOOL SkinEx_DrawArrow(LPSCROLLBAR psb,HDC hDC,BOOL fVert,INT nArrow,UINT uState)
{
    RECT                rect;
    RECT                rc;
    SCROLLBARCALC       sbc;
    HDC                 hMemDC;
    HBITMAP             hOldBmp;
    INT                 x;
    INT                 y;
    POINT               pt;

    ::GetWindowRect(psb->hwnd,&rect);
    SkinEx_ScrollBarCalc(psb,&sbc,fVert);

    if(0==uState)
        uState=SkinEx_GetState(psb,fVert,nArrow);
}

VOID SkinEx_DrawThumb(LPSCROLLBAR psb,HDC hDC,BOOL fVert)
{
    SCROLLBARCALC           sbc;
    RECT                    rc;
    RECT                    rect;
    INT                     cx;
    INT                     cy;
    POINT                   pt;
    POINT                   pt1;

    UINT uState=SkinEx_GetState(psb,fVert,HITTEST_SCROLLBAR_THUMB);
    SkinEx_ScrollBarCalc(psb,&sbc,fVert);
    ::GetWindowRect(psb->hwnd,&rect);

    if(sbc.pixelTop>=sbc.pixelBottom || sbc.pixelLeft>=sbc.pixelRight) return ;

    if((sbc.pixelDownArrow - sbc.pixelUpArrow) < (sbc.pixelThumbBottom-sbc.pixelThumbTop) || SCROLLBAR_STATE_DISABLED==uSate )
    {
        SkinEx_GetGrooveRect(psb,&rc,fVert);
        SkinEx_DrawGroove(psb,hDC,&rc,fVert);
        return ;
    }

    if(sbc.pixelUpArrow<sbc.pixelThumbTop)
    {
        if(fVert)
        {
            ::SetRect(&rc,sbc.pixelLeft,sbc.pixelUpArrow,sbc.pixelRight,sbc.pixelThumbTop);
        }
        else
        {
            ::SetRect(&rc,sbc.pixelUpArrow,sbc.pixelTop,sbc.pixelThumbTop,sbc.pixelBottom);
        }
        ::OffsetRect(&rc,-rect.left,-rect.top);
        SkinEx_DrawGroove(psb,hDC,&rc,fVert);
    }

    if(sbc.pixelThumbBottom<sbc.pixelDownArrow)
    {
        if(fVert)
        {
            ::SetRect(&rc,sbc.pixelLeft,sbc.pixelThumbBottom,sbc.pixelRight,sbc.pixelDownArrow);
        }
        else
        {
            ::SetRect(&rc,sbc.pixelThumbBottom,sbc.pixelTop,sbc.pixelDownArrow,sbc.pixelBottom);
        }
        ::OffsetRect(&rc,-rect.left,-rect.top);
        SkinEx_DrawGroove(psb,hDC,&rc,fVert);
    }

    HDC hSrcDC=::CreateCompatibleDC(hDC);
    HBITMAP hOldBmp=(HBITMAP)::SelectObject(hSrcDC,psb->hBmp);

    SkinEx_GetThumbRect(psb,&rc,fVert);
    
    cx=rc.right-rc.left;
    cy=rc.bottom-rc.top;
    RECT rcMemDC;
    ::SetRectEmpty(&rcMemDC);
    ::SetRect(&rcMemDC,0,0,rc.right-rc.left,rc.bottom-rc.top);

    HDC memDC=::CreateCompatibleDC(hDC);
    HBITMAP hMemBitmap=::CreateCompatibleBitmap(hDC,cx,cy);
    HBITMAP hOldBitmap=(HBITMAP)::SelectObject(memDC,hMemBitmap);
    ::SetBkColor(memDC,0xffffff);
    ::ExtTextOut(memDC,0x00,0x00,ETO_OPAQUE,&rcMemDC,NULL,0x00,NULL);

    switch(uState)
    {
    case SCROLLBAR_STATE_NORMAL:
        {
            pt  = fVert ? ptArray[0][2] : ptArray[3][2];
            pt1 = fVert ? ptArray[0][3] : ptArray[3][3];
        }break;
    case SCROLLBAR_STATE_HOTTRACKED:
        {
            pt  = fVert ? ptArray[1][2] : ptArray[4][2];
            pt1 = fVert ? ptArray[1][3] : ptArray[4][3];
        }break;
    case SCROLLBAR_STATE_PRESSED:
        {
            pt  = fVert ? ptArray[2][2] : ptArray[5][2];
            pt1 = fVert ? ptArray[2][3] : ptArray[5][3];
        }break;
    case SCROLLBAR_STATE_DISABLED:
        {
        }break;
    DEFAULT_UNREACHABLE;
    }

    if(fVert)
    {
        for(INT i(4);i<cy-4;i+=8)
        {
            ::BitBlt(memDC,0,i,cx,8,hSrcDC,pt.x,pt.y+4,SRCCOPY);
        }

        ::BitBlt(memDC,0,0,cx,4,hSrcDC,pt.x,pt.y,SRCCOPY);
        ::BitBlt(memDC,0,cy-4,cx,4,hSrcDC,pt.x,(pt.y+16)-4,SRCCOPY);

        if(cy>16+8)
        {
            INT y=(cy-16)/2;
            ::BitBlt(memDC,0,y,cx,16,hSrcDC,pt1.x,pt1.y,SRCCOPY);
        }
    }
    else
    {
        for(INT i(4);i<cx-4;i+=8)
        {
            ::BitBlt(memDC,i,0,8,cy,hSrcDC,pt.x+4,pt.y,SRCCOPY);
        }

        ::BitBlt(memDC,0,0,4,cy,hSrcDC,pt.x,pt.y,SRCCOPY);
        ::BitBlt(memDC,cx-4,0,4,cy,hSrcDC,(pt.x+16)-4,pt.y,SRCCOPY);

        if(cx>16+8)
        {
            INT x=(cx-16)/2;
            ::BitBlt(memDC,x,0,16,cy,hSrcDC,pt1.x,pt1.y,SRCCOPY);
        }
    }

    ::BitBlt(hDC,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,memDC,0,0,SRCCOPY);

    ::SelectObject(memDC,hOldBitmap);
    ::DeleteDC(memDC);

    ::SelectObject(hSrcDC,hOldBmp);
    ::DeleteDC(hSrcDC);

}

VOID SkinEx_DrawScrollBar(LPSCROLLBAR psb,HDC hDC,BOOL fVert)
{
    RECT rcGroove;
    SkinEx_GetGrooveRect(psb,&rcGroove,fVert);
    SkinEx_DrawGroove(psb,hDC,&rcGroove,fVert);
    
    SkinEx_DrawArrow(psb,hDC,fVert,HITTEST_SCROLLBAR_LINEUP,0);
    SkinEx_DrawArrow(psb,hDC,fVert,HITTEST_SCROLLBAR_LINEDOWN,0);

    if(fVert)
    {
        if(SkinEx_IsScrollInfoActive(&psb->Vert))
            SkinEx_DrawThumb(psb,hDC,fVert);
    }
    else
    {
        if(SkinEx_IsScrollInfoActive(&psb->Horz))
            SkinEx_DrawThumb(psb,hDC,fVert);
    }
}



BOOL SkinEx_DrawSizeBox(LPSCROLLBAR psb,HDC hDC)
{
    if(!psb || !hDC) return FALSE;
    RECT            rect;
    RECT            rc;
    ::GetWindowRect(psb->hwnd,&rect);
    SkinEx_GetSizeBoxRect(psb,&rc);
    ::OffsetRect(&rc,-rect.left,-rect.top);

    HDC hMemDC=::CreateCompatibleDC(NULL);
    HBITMAP hOldBmp=(HBITMAP)::SelectObject(hMemDC,psb->hBmp);
    
    ::BitBlt(hDC,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,hMemDC,38,57,SRCCOPY);

    ::SelectObject(hMemDC,hOldBmp);
    ::DeleteDC(hMemDC);
    return TRUE;
}


VOID SkinEx_Track(LPSCROLLBAR psb,BOOL fVert,UINT nHit,POINT pt)
{
    UINT            disFlags;
    LPSCROLLINFO    psi;
    WORD            wSBCode;

    psi = fVert ? &psb->Vert : &psb->Horz;
    disFlags = SkinEx_GetDisableFlags(psb, fVert);

    switch( nHit )
    {
    case HITTEST_SCROLLBAR_THUMB:
        {
            SCROLLBARCALC sbclc;
            SkinEx_ScrollBarCalc(psb, &sbclc, fVert);
            psi->nTrackPos    = psi->nPos;
            psb->nOffsetPoint = (fVert ? pt.y : pt.x) - sbclc.pixelThumbTop;
        }break;
    case HITTEST_SCROLLBAR_LINEUP:
        {
            wSBCode = SB_LINEUP;
            psb->nScrollTimerMsg = MAKELONG(fVert ? WM_VSCROLL : WM_HSCROLL, wSBCode);
            SendScrollBarMessage(psb->hwnd, wSBCode, 0, fVert);
            SetTimer(psb->hwnd, SCROLLBAR_TIMER_DELAY, SCROLLBAR_SCROLL_DELAY, NULL);
        }break;
    case HITTEST_SCROLLBAR_LINEDOWN:
        {
            wSBCode = SB_LINEDOWN;
            psb->nScrollTimerMsg = MAKELONG(fVert ? WM_VSCROLL : WM_HSCROLL, wSBCode);
            SendScrollBarMessage(psb->hwnd, wSBCode, 0, fVert);
            SetTimer(psb->hwnd, SCROLLBAR_TIMER_DELAY, SCROLLBAR_SCROLL_DELAY, NULL);
        }break;
    case HITTEST_SCROLLBAR_PAGEDOWN:
        {
            wSBCode = SB_PAGEDOWN;
            psb->nScrollTimerMsg = MAKELONG(fVert ? WM_VSCROLL : WM_HSCROLL, wSBCode);
            SendScrollBarMessage(psb->hwnd, wSBCode, 0, fVert);
            SetTimer(psb->hwnd, SCROLLBAR_TIMER_DELAY, SCROLLBAR_SCROLL_DELAY, NULL);
        }break;
    case HITTEST_SCROLLBAR_PAGEUP:
        {
            wSBCode = SB_PAGEUP;
            psb->nScrollTimerMsg = MAKELONG(fVert ? WM_VSCROLL : WM_HSCROLL, wSBCode);
            SendScrollBarMessage(psb->hwnd, wSBCode, 0, fVert);
            SetTimer(psb->hwnd, SCROLLBAR_TIMER_DELAY, SCROLLBAR_SCROLL_DELAY, NULL);
        }break;
    DEFAULT_UNREACHABLE;
    }

    psb->nTrackCode  = nHit;
    psb->fTrackVert  = fVert;
    psb->fTracking   = TRUE;

    SkinEx_HotTrack(psb, nHit, fVert, TRUE);
    ::SetCapture(psb->hwnd);

}

BOOL SkinEx_TrackThumb(LPSCROLLBAR psb,BOOL fVert,POINT pt)
{

}

BOOL SkinEx_HotTrack(LPSCROLLBAR psb,INT nHitCode,BOOL fVert,BOOL fMouseDown)
{

}

UINT SkinEx_HitTest(LPSCROLLBAR psb,BOOL fVert,POINT pt)
{

}

BOOL SkinEx_EnableArrows(LPSCROLLBAR psb,INT nBar,UINT nArrows)
{

}

UINT SkinEx_GetDisableFlags(LPSCROLLBAR psb,BOOL fVert)
{

}

UINT SkinEx_GetState(LPSCROLLBAR psb,BOOL fVert,UINT nHit)
{

}



LRESULT SkinEx_OnStyleChanged(LPSCROLLBAR psb,INT nStyleType,LPSTYLESTRUCT lpStyleStruct)
{

}

LRESULT SkinEx_OnNcHitTest(LPSCROLLBAR psb,WPARAM wParam,LPARAM lParam)
{

}

LRESULT SkinEx_OnNcPaint(LPSCROLLBAR psb,WPARAM wParam,LPARAM lParam)
{

}

LRESULT SkinEx_OnNcCalcSize(LPSCROLLBAR psb,BOOL bClacValidRect,NCCALCSIZE_PARAMS* lpncsp)
{

}

LRESULT SkinEx_OnNcMouseMove(LPSCROLLBAR psb,WPARAM wParam,LPARAM lParam)
{

}

LRESULT SkinEx_OnNcLButtonDown(LPSCROLLBAR psb,WPARAM wParam,LPARAM lParam)
{

}

LRESULT SkinEx_OnNcMouseLeave(LPSCROLLBAR psb,WPARAM wParam,LPARAM lParam)
{

}

LRESULT SkinEx_OnMouseMove(LPSCROLLBAR psb,WPARAM wParam,LPARAM lParam)
{

}

LRESULT SkinEx_OnLButtonUp(LPSCROLLBAR psb,WPARAM wParam,LPARAM lParam)
{

}

LRESULT SkinEx_OnTimer(LPSCROLLBAR psb,WPARAM wParam,LPARAM lParam)
{

}


LRESULT CALLBACK SkinEx_Proc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{

}