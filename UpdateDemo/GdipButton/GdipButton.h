//Download by http://www.NewXing.com
//
// GdipButton.h : Version 1.0 - see article at CodeProject.com
//
// Author:  Darren Sessions
//          
//
// Description:
//     GdipButton is a CButton derived control that uses GDI+ 
//     to support alternate image formats
//
// History
//     Version 1.0 - 2008 June 10
//     - Initial public release
//
// License:
//     This software is released under the Code Project Open License (CPOL),
//     which may be found here:  http://www.codeproject.com/info/eula.aspx
//     You are free to use this software in any way you like, except that you 
//     may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this 
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <gdiplus.h>
using namespace Gdiplus;
// GdipButton.h : header file
//
class CGdiPlusBitmapResource;
/////////////////////////////////////////////////////////////////////////////
// CGdipButton window
//#define GDI_BUTTON_
//#ifdef GDI_BUTTON_
//#define __GDI_BUTTON_ __declspec(dllexport)
//#else if
//#define __GDI_BUTTON_ __declspec(dllimport)
//#endif
class  CGdipButton : public CButton
{
public:

	CGdipButton();
	virtual ~CGdipButton();

	// image types
	enum	{
				STD_TYPE	= 0,
				ALT_TYPE,
				DIS_TYPE
			};

	// sets the image type
	void SetImage(int type);

	BOOL LoadAltImage(UINT id, LPCTSTR pType);
	BOOL LoadStdImage(UINT id, LPCTSTR pType);
	void SetStdDC();
	void SetAltDC();
	// if false, disables the press state and uses grayscale image if it exists
	void EnableButton(BOOL bEnable = TRUE) { m_bIsDisabled = !bEnable; }

	// in toggle mode each press toggles between std and alt images
	void EnableToggle(BOOL bEnable = TRUE);

	// return the enable/disable state
	BOOL IsDisabled(void) {return (m_bIsDisabled == TRUE); }

	void SetBkGnd(CDC* pDC);

	void SetToolTipText(CString spText, BOOL bActivate = TRUE);
	void SetToolTipText(UINT nId, BOOL bActivate = TRUE);
	void SetHorizontal(bool ImagesAreLaidOutHorizontally = FALSE);
	void DeleteToolTip();
	//add by zhiyuanc 2017-05-03 设置按钮不被选中
	void SetSelected(bool bSelected);
	//add by zhiyuanc 20170630 是否需要设置圆角
	void enableRgn(bool use_rgn = false){ m_isRgn = use_rgn; }
	void setRgnRate(int x, int y){ m_rgn_x = x; m_rgn_y = y; }
	//add by zhiyuanc 20170703 是否需要拉伸显示，图片随按钮大小拉伸
//	void enableSetBilt(bool use_setbilt = false){ m_setbilt = use_setbilt; }

	//add by zhiyaunc 20170707 设置添加文字
	void setText(CString str_text, int show_x, int show_y);
	
protected:

	void PaintBk(CDC* pDC);
	void PaintBtn(CDC* pDC);

	BOOL	m_bHaveAltImage;
	BOOL	m_bHaveBitmaps;
	BOOL	m_bMultiLoad;

	BOOL	m_bIsDisabled;
	BOOL	m_bIsToggle;
	BOOL	m_bIsHovering;
	BOOL	m_bIsTracking;

	int		m_nCurType;
	BOOL	m_bStdDC;
	CGdiPlusBitmapResource* m_pAltImage;
	CGdiPlusBitmapResource* m_pStdImage;

	CString			m_tooltext;
	CToolTipCtrl*	m_pToolTip;
	bool m_isRgn;		//是否设置圆角
	int m_rgn_x;
	int m_rgn_y;
	bool m_setbilt;
	CRect m_rgn_rect;
	int m_std_bmp_width;
	int m_std_bmp_height;
	int m_alt_bmp_width;
	int m_alt_bmp_height;
	void	InitToolTip();

	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//设置圆角
	void setRgn();

	//{{AFX_MSG(CGdipButton)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam) ;
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

	CDC		m_dcBk;			// button background
	
	CDC		m_dcStd;		// standard button
	CDC		m_dcStdP;		// standard button pressed
	CDC		m_dcStdH;		// standard button hot

	CDC		m_dcAlt;		// alternate button
	CDC		m_dcAltP;		// alternate button pressed
	CDC		m_dcAltH;		// alternate button hot

	CDC*	m_dcStd_bk;		// standard button
	CDC*	m_dcStdP_bk;		// standard button pressed
	CDC*	m_dcStdH_bk;		// standard button hot

	CDC*	m_dcAlt_bk;		// alternate button
	CDC*	m_dcAltP_bk;		// alternate button pressed
	CDC*	m_dcAltH_bk;		// alternate button hot

	CDC		m_dcGS;			// grayscale button (does not have a hot or pressed state)

	CDC*	m_pCurBtn;		// current pointer to one of the above
	//add by zhiyuanc 20170707 
	CString m_str_text;
	int m_show_text_x;
	int m_show_text_y;
};
