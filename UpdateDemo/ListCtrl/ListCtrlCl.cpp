// ListCtrlCl.cpp : ʵ���ļ�
//

#include "stdafx.h"
//#include "DemoList.h"
#include "ListCtrlCl.h"
//#include "BlackNmWnd.h"

struct stColor
{
	int nRow;
	int nCol;
	COLORREF rgb;
};
// CListCtrlCl

IMPLEMENT_DYNAMIC(  CListCtrlCl, CListCtrl )

CListCtrlCl::CListCtrlCl()
: m_nRowHeight(0)
, m_fontHeight(12)
, m_fontWith(0)
{
	m_color = RGB(0,0,0);
	m_pSpecialInfo = NULL;

}

CListCtrlCl::~CListCtrlCl()
{
}


BEGIN_MESSAGE_MAP( CListCtrlCl, CListCtrl )
	ON_WM_MEASUREITEM()
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT( NM_DBLCLK,OnRDClick )
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT( LVN_KEYDOWN, OnKeyDownNM )
END_MESSAGE_MAP()



// CListCtrlCl ��Ϣ�������



void CListCtrlCl::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	ModifyStyle( 0,LVS_OWNERDRAWFIXED );
	CListCtrl::PreSubclassWindow();
	CHeaderCtrl *pHeader = GetHeaderCtrl();
	m_Header.SubclassWindow(pHeader->GetSafeHwnd());

}

void CListCtrlCl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  ������Ĵ����Ի���ָ����
	TCHAR lpBuffer[256];

	LV_ITEM lvi;

	lvi.mask = LVIF_TEXT | LVIF_PARAM ;
	lvi.iItem = lpDrawItemStruct->itemID ; 
	lvi.iSubItem = 0;
	lvi.pszText = lpBuffer ;
	lvi.cchTextMax = sizeof(lpBuffer);
	VERIFY(GetItem(&lvi));

	LV_COLUMN lvc, lvcprev ;
	::ZeroMemory( &lvc, sizeof(lvc) );
	::ZeroMemory( &lvcprev, sizeof(lvcprev) );
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;

	CDC* pDC;
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rtClient;
	GetClientRect(&rtClient);
	for ( int nCol=0; GetColumn(nCol, &lvc); nCol++)
	{
		if ( nCol > 0 ) 
		{
			// Get Previous Column Width in order to move the next display item
			GetColumn(nCol-1, &lvcprev) ;
			lpDrawItemStruct->rcItem.left += lvcprev.cx ;
			lpDrawItemStruct->rcItem.right += lpDrawItemStruct->rcItem.left; 
		}

		CRect rcItem;   
		if (!GetSubItemRect(lpDrawItemStruct->itemID,nCol,LVIR_LABEL,rcItem))   
			continue;   

		::ZeroMemory( &lvi, sizeof(lvi) );
		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = nCol;
		lvi.pszText = lpBuffer;
		lvi.cchTextMax = sizeof(lpBuffer);
		VERIFY( GetItem( &lvi ) );
		CRect rcTemp;
		rcTemp = rcItem;

		if ( nCol==0 )
		{
			rcTemp.left -=2;
		}

		if ( lpDrawItemStruct->itemState & ODS_SELECTED )
		{
			pDC->FillSolidRect( &rcTemp, GetSysColor(COLOR_HIGHLIGHT)) ;
			pDC->SetTextColor( GetSysColor(COLOR_HIGHLIGHTTEXT)) ;
		}
		else
		{
			COLORREF color;
			color = GetBkColor();
			pDC->FillSolidRect(rcTemp,color);

			if (FindColColor(nCol,color))
			{
				pDC->FillSolidRect(rcTemp,color);
			}
			if (FindItemColor(nCol,lpDrawItemStruct->itemID,color))
			{
				pDC->FillSolidRect(rcTemp,color);
			}
			
			//pDC->SetTextColor(m_color);
		}

		pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));

		UINT   uFormat    = DT_CENTER ;
		if (m_Header.m_Format[nCol]=='0')
		{
			uFormat = DT_LEFT;
		}
		else if ( m_Header.m_Format[nCol]=='1')
		{
			uFormat = DT_CENTER;
		}
		else if ( m_Header.m_Format[nCol]=='2')
		{
			uFormat = DT_RIGHT;
		}
		TEXTMETRIC metric;
		pDC->GetTextMetrics(&metric);
		int ofst;
		ofst = rcItem.Height() - metric.tmHeight;
		rcItem.OffsetRect(0,ofst/2);
		pDC->SetTextColor(m_color);
		COLORREF color;
		if (FindColTextColor(nCol,color))
		{
			pDC->SetTextColor(color);
		}
		if ( FindItemTextColor(nCol,lpDrawItemStruct->itemID,color))
		{
			pDC->SetTextColor(color);
		}
		CFont nFont ,* nOldFont; 
		nFont.CreateFont(m_fontHeight,m_fontWith,0,0,0,FALSE,FALSE,0,0,0,0,0,0,_TEXT("����"));//�������� 
		nOldFont = pDC->SelectObject(&nFont);
		DrawText(lpDrawItemStruct->hDC, lpBuffer, strlen(lpBuffer), 
			&rcItem, uFormat) ;

		pDC->SelectStockObject(SYSTEM_FONT) ;
	}

}

void CListCtrlCl::OnMeasureItem( int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
void CListCtrlCl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{


	if ( m_nRowHeight>0 )
	{
		lpMeasureItemStruct->itemHeight = m_nRowHeight;
	}


}
int CListCtrlCl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat /* = LVCFMT_LEFT */, int nWidth /* = -1 */, int nSubItem /* = -1 */)
{
	m_Header.m_HChar.Add(lpszColumnHeading);
	if (nFormat==LVCFMT_LEFT)
	{
		m_Header.m_Format = m_Header.m_Format + "0";
	}
	else if (nFormat==LVCFMT_CENTER)
	{
		m_Header.m_Format = m_Header.m_Format + "1";
	}
	else if (nFormat==LVCFMT_RIGHT)
	{
		m_Header.m_Format = m_Header.m_Format + "2";
	}
	else
	{
		m_Header.m_Format = m_Header.m_Format + "1";
	}
	return CListCtrl::InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,nSubItem);


}
// Gradient - ����ϵ�������屳����,���ý�����Ϊ0
void CListCtrlCl::SetHeaderBKColor(int R, int G, int B, int Gradient) //���ñ�ͷ����ɫ
{
	m_Header.m_R = R;
	m_Header.m_G = G;
	m_Header.m_B = B;
	m_Header.m_Gradient = Gradient;
}

// ���ñ�ͷ�߶�
void CListCtrlCl::SetHeaderHeight(float Height) //���ñ�ͷ�߶�
{
	m_Header.m_Height = Height;
}
bool CListCtrlCl::FindColColor(int col,COLORREF &color) //��������ɫ
{
	int flag = 0;
	for (POSITION pos = m_ptrListCol.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ptrListCol.GetNext(pos);
		if (pColor->nCol==col)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;


}
bool CListCtrlCl::FindItemColor(int col,int row,COLORREF &color) //������ɫ
{
	int flag = 0;
	for (POSITION pos = m_ptrListItem.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ptrListItem.GetNext(pos);
		if (pColor->nCol==col&&pColor->nRow==row)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;


}
void CListCtrlCl::SetColColor(int col,COLORREF color) //��������ɫ
{
	stColor *pColor  = new stColor;
	pColor->nCol = col;
	pColor->rgb = color;
	m_ptrListCol.AddTail(pColor);
}
void CListCtrlCl::SetItemColor(int col,int row,COLORREF color) //���ø�����ɫ
{
	stColor *pColor  = new stColor;
	pColor->nCol = col;
	pColor->nRow = row;
	pColor->rgb = color;
	m_ptrListItem.AddTail(pColor);
}
void CListCtrlCl::SetRowHeigt(int nHeight) //�����и�
{
	m_nRowHeight = nHeight;

	CRect rcWin;
	GetWindowRect(&rcWin);
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);


}
void CListCtrlCl::SetHeaderFontHW(int nHeight,int nWith) //����ͷ�������͸�
{
	m_Header.m_fontHeight = nHeight;
	m_Header.m_fontWith = nWith;
}
void CListCtrlCl::SetHeaderTextColor(COLORREF color) //����ͷ��������ɫ
{
	m_Header.m_color = color;
}
BOOL CListCtrlCl::SetTextColor(COLORREF cr)  //����������ɫ
{
	m_color = cr;
	return TRUE;
}
void CListCtrlCl::SetFontHW(int nHeight,int nWith) //��������ߺͿ�
{
	m_fontHeight = nHeight;
	m_fontWith = nWith;
}

void CListCtrlCl::SetColTextColor(int col,COLORREF color)
{
	stColor *pColor = new stColor;
	pColor->nCol = col;
	pColor->rgb = color;
	m_colTextColor.AddTail(pColor);
}


bool CListCtrlCl::FindColTextColor(int col,COLORREF &color)
{
	int flag = 0;
	for (POSITION pos = m_colTextColor.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_colTextColor.GetNext(pos);
		if (pColor->nCol==col)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}
bool CListCtrlCl::FindItemTextColor(int col,int row,COLORREF &color)
{
	int flag = 0;
	for (POSITION pos = m_ItemTextColor.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ItemTextColor.GetNext(pos);
		if (pColor->nCol==col&&pColor->nRow==row)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}
void CListCtrlCl::SetItemTextColor(int col,int row,COLORREF color)
{
	stColor *pColor = new stColor;
	pColor->nCol = col;
	pColor->nRow = row;
	pColor->rgb = color;
	m_ItemTextColor.AddTail(pColor);

}

void CListCtrlCl::OnRButtonDown( UINT nFlags, CPoint point)
{

	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (((CBlackNmWnd*)GetParent())->m_bIsSelectedItem )
	{

		//CMenu menu, *pSubMenu; //����Ҫ�õ�CMenu����
		//CPoint point;
		//menu.LoadMenu( IDR_BLACKMENU ); //װ���Զ�����Ҽ��˵� 
		//pSubMenu = menu.GetSubMenu( 0 ); //��ȡ��һ�������˵�
		//openBitmap.LoadBitmap( IDB_BITMAP4 );
		////closeBitmap.LoadBitmapA(IDB_BITMAP3);
		//pSubMenu->SetMenuItemBitmaps( 0, MF_BYPOSITION, &openBitmap, &openBitmap );
		//pSubMenu->SetMenuItemBitmaps( 1, MF_BYPOSITION, &openBitmap, &openBitmap );
		//pSubMenu->SetMenuItemBitmaps( 2, MF_BYPOSITION, &openBitmap, &openBitmap );
		//GetCursorPos( &point ); //��ȡ��ǰ���λ��
		////note 
		//pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);

	}
	
	CListCtrl::OnRButtonDown(nFlags, point);

}


afx_msg void CListCtrlCl::OnRDClick( NMHDR* pNMHDR, LRESULT* pResult )
{


	NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
	int nItem = pNMListView->iItem;
	int s = this->GetSelectionMark();
	//CString mm;
	//mm.Format("%d", s );
	//MessageBox( mm );
	if ( s >= 0 || ( nItem >= 0 && nItem< this->GetItemCount( ) )  )//�ж�˫��λ���Ƿ��������ݵ��б�������
	{

		//.............
		if ( m_pSpecialInfo == NULL  )
		{
			m_pSpecialInfo = new CSpecialInfo;
			m_pSpecialInfo->m_bIsAddMode = false;
			m_pSpecialInfo->m_Nm = this->GetItemText( nItem, 0 );
			m_pSpecialInfo->m_ID = this->GetItemText( nItem, 1 );
			m_pSpecialInfo->m_Sex = this->GetItemText( nItem, 2 );
			m_pSpecialInfo->m_Birth= this->GetItemText( nItem, 3 );
			m_pSpecialInfo->m_AddPerson = this->GetItemText( nItem, 4 );
			m_pSpecialInfo->m_AddDate = this->GetItemText( nItem, 5 );
			m_pSpecialInfo->Create(IDD_BLACKNM_INFO, this);
			m_pSpecialInfo->SetCtrlValue();
			m_pSpecialInfo->ShowWindow( true );

		}

	}

	*pResult = 0;


}

afx_msg void CListCtrlCl:: OnKeyDownNM(NMHDR *pNmHdr, LRESULT *pResult)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//if (nChar == 0x10 )
		//MessageBox("j");
	NMKEY * key = (NMKEY*)pNmHdr;
	
	/*CString mm;
	mm.Format("%d",key->nVKey  );
	MessageBox( mm );*/
	//CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	*pResult = 0;
}




BOOL CListCtrlCl::PreTranslateMessage( MSG* pMsg )
{
	// TODO:  �ڴ����ר�ô����/����û���
	//if (((GetStyle() & LVS_EDITLABELS) == 0) || (pMsg->message != WM_KEYDOWN)
	//	|| (pMsg->wParam != VK_RETURN))
	//	return CListCtrl::PreTranslateMessage(pMsg);

	//// Send the message on to the control 
	//DispatchMessage(pMsg);
	//return TRUE;
	////
	return CListCtrl::PreTranslateMessage(pMsg);
}
