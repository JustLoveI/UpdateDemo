#pragma once
#include "id_header.h"
// CPicBtnStatic
/*
@@ this is create by zhiyuanc 20171213 
@@ This class is intended to display the edit button in the list control
*/
enum SHOW_TYPE{ EDIT_ = 0, DELETE_ };//添加一个状态机,显示不同按钮，每添加一类可以在状态机中添加一种状态

class CPicBtnStatic : public CStatic
{
	DECLARE_DYNAMIC(CPicBtnStatic)
	
public:
	CPicBtnStatic(void* pNotify);
	virtual ~CPicBtnStatic();
	inline void SetShowType(SHOW_TYPE type){ m_type = type; }
	void SetClickCallBackFunc(PCLICKCALLBACK pClickCallBackFunc);
private:
	int getBmpID(SHOW_TYPE& type, bool bNormal = true);//获取位图id，分两种情况，正常情况和鼠标在按钮上的情况
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
private:
	void* m_pNotify;	//
	PCLICKCALLBACK m_pClickCallBackFunc;//点击事件回调函数
	bool m_bMouseOver;//鼠标是否在静态框上
	bool m_bPreMouseOver;//鼠标状态是否改变，判断是否需要刷新
	SHOW_TYPE m_type;		//按钮类型，编辑还是删除，目前只有两种，可以扩展
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
//	afx_msg void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct);
//	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};


