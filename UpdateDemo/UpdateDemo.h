
// UpdateDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUpdateDemoApp: 
// �йش����ʵ�֣������ UpdateDemo.cpp
//

class CUpdateDemoApp : public CWinApp
{
public:
	CUpdateDemoApp();
	ULONG_PTR m_gdiplusToken;
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUpdateDemoApp theApp;