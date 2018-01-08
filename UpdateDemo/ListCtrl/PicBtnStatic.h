#pragma once
#include "id_header.h"
// CPicBtnStatic
/*
@@ this is create by zhiyuanc 20171213 
@@ This class is intended to display the edit button in the list control
*/
enum SHOW_TYPE{ EDIT_ = 0, DELETE_ };//���һ��״̬��,��ʾ��ͬ��ť��ÿ���һ�������״̬�������һ��״̬

class CPicBtnStatic : public CStatic
{
	DECLARE_DYNAMIC(CPicBtnStatic)
	
public:
	CPicBtnStatic(void* pNotify);
	virtual ~CPicBtnStatic();
	inline void SetShowType(SHOW_TYPE type){ m_type = type; }
	void SetClickCallBackFunc(PCLICKCALLBACK pClickCallBackFunc);
private:
	int getBmpID(SHOW_TYPE& type, bool bNormal = true);//��ȡλͼid��������������������������ڰ�ť�ϵ����
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
private:
	void* m_pNotify;	//
	PCLICKCALLBACK m_pClickCallBackFunc;//����¼��ص�����
	bool m_bMouseOver;//����Ƿ��ھ�̬����
	bool m_bPreMouseOver;//���״̬�Ƿ�ı䣬�ж��Ƿ���Ҫˢ��
	SHOW_TYPE m_type;		//��ť���ͣ��༭����ɾ����Ŀǰֻ�����֣�������չ
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


