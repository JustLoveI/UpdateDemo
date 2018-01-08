// PicBtnStatic.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PicBtnStatic.h"
#include "id_header.h"

// CPicBtnStatic

IMPLEMENT_DYNAMIC(CPicBtnStatic, CStatic)

CPicBtnStatic::CPicBtnStatic(void* pNotify)
{
	m_bMouseOver = false;
	m_bPreMouseOver = false;
	m_pNotify = pNotify;
	m_pClickCallBackFunc = NULL;
}

CPicBtnStatic::~CPicBtnStatic()
{
}


BEGIN_MESSAGE_MAP(CPicBtnStatic, CStatic)
//	ON_WM_DELETEITEM()
//	ON_WM_DRAWITEM()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



// CPicBtnStatic ��Ϣ�������




void CPicBtnStatic::PreSubclassWindow()
{
	// TODO:  �ڴ����ר�ô����/����û���

	DWORD dwStyle = GetStyle();
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_OWNERDRAW);
	CStatic::PreSubclassWindow();
}

void CPicBtnStatic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rect;//��̬��Χ
	GetClientRect(&rect);
	CDC* pDC;
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CBitmap tickBmp;
	BITMAP bmp;

	tickBmp.LoadBitmapA(getBmpID(m_type, !m_bMouseOver));

	tickBmp.GetBitmap(&bmp);
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp = memDC.SelectObject(&tickBmp);
	pDC->BitBlt(rect.left, rect.top, bmp.bmWidth, bmp.bmHeight, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
}

void CPicBtnStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bPreMouseOver = m_bMouseOver;
	m_bMouseOver = true;
	if (m_bPreMouseOver != m_bMouseOver)
	{
		m_bPreMouseOver = m_bMouseOver;
		Invalidate();
	}
	TRACKMOUSEEVENT   tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE;
	_TrackMouseEvent(&tme);
	CStatic::OnMouseMove(nFlags, point);
}


void CPicBtnStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_pClickCallBackFunc != NULL)
	{
		m_pClickCallBackFunc(m_pNotify, GetDlgCtrlID());
	}
	CStatic::OnLButtonDown(nFlags, point);
}


void CPicBtnStatic::OnMouseLeave()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bPreMouseOver = m_bMouseOver;
	m_bMouseOver = false;
	if (m_bPreMouseOver != m_bMouseOver)
	{
		m_bPreMouseOver = m_bMouseOver;
		Invalidate();
	}
	CStatic::OnMouseLeave();
}


int CPicBtnStatic::getBmpID(SHOW_TYPE& type, bool bNormal)
{
	switch (type)
	{
	case EDIT_:
		if (bNormal)
		{
			return IDB_EDIT_NORMAL;
		}
		else
		{
			return IDB_EDIT_MOUSE_OVER;
		}
		break;
	case DELETE_:
		if (bNormal)
		{
			return IDB_DELETE_NORMAL;
		}
		else
		{
			return IDB_DELETE_MOUSE_OVER;
		}
		break;
	default:
		break;
	}
	return 0;
}

void CPicBtnStatic::SetClickCallBackFunc(PCLICKCALLBACK pClickCallBackFunc)
{
	m_pClickCallBackFunc = pClickCallBackFunc;
}