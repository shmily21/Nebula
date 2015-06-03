#include "internal.h"
#include "scrollbar.h"

/* ����������Բ���ʱ�Ĺؼ��� */
static TCHAR g_szPropScrollBar[]=TEXT("SkinEx_ScrollBar_Prop");

// ������ͼƬ��Դ�ļ��������¹�񻮷֣�ͼƬ��Դ��С 110*110
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
* ��ȡ���������ݽṹ
*/
LPSCROLLBAR SkinEx_GetScrollBar(HWND hWnd)
{
    return (LPSCROLLBAR)GetProp(hWnd,g_szPropScrollBar);
}

BOOL SkinEx_SetScrollBarParams(LPSCROLLBAR psi,SCROLLINFO si,BOOL* pfScroll,INT* plres,BOOL bOldPos)
{
    
}

/*
* ���������Ƿ�Ϊ�״̬
*/
BOOL SkinEx_IsScrollInfoActive(LPSCROLLINFO lpsi)
{
    if(lpsi->nPage>(UINT)lpsi->nMax || lpsi->nMax<=lpsi->nMin || lpsi->nMax==0) return FALSE;
    else return TRUE;
}


/*
* ��ȡ������SizeBox�����С
*/
BOOL SkinEx_GetSizeBoxRect(LPSCROLLBAR psb,LPRECT lprc)
{
    if(!psb || !lprc) return FALSE;
    RECT    rect;
    ::SetRectEmpty(lprc);
    DWORD   dwStyle=::GetWindowLong(psb->hwnd,GWL_STYLE);
    if((dwStyle & WS_HSCROLL) && (dwStyle & WS_VSCROLL))
    {
        ::GetClientRect(psb->hwnd,&rect);                   // �ͻ��˾���
        ::ClientToScreen(psb->hwnd,(LPPOINT)&rect);         // �ͻ��˾��ε����ת��Ϊ��Ļ����
        ::ClientToScreen(psb->hwnd,((LPPOINT)&rect)+1);     // �ͻ��˾��ε����½�ת��Ϊ��Ļ���꣨RECTת��ΪPOINT,+1��ָ���һ���㣩

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
* ��ȡ�������������
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

    if(fVert)   // ����Ǵ�ֱ������
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
* ������������λ�ü���С
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
    }
}

/*
* ��ȡ������Thumb�������
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

BOOL SkinEx_GetGrooveRect(LPSCROLLBAR psb,LPRECT lprc,BOOL fVert);