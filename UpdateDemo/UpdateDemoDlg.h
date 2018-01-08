
// UpdateDemoDlg.h : 头文件
//

#pragma once
#include "HeadFileShared\HeadFileShared.h"
#include "GdipButton\GdipButton.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>
#include <map>
using namespace std;


//文件信息结构体
struct FileInfo
{
	//文件编号
	int nIndex;

	//文件名称带路径 例    Update/AutoUpdate.exe
	CString sFileName;

	//文件下载URL     例    URL=http://192.168.1.105:8080/spzpjk/updateclient/AutoUpdate.exe
	CString sFileURL;
	
	//文件最新修改时间 例
	CString sFileChangeTime;

	//文件版本			例   v1.0.1
	CString sFileVersion;
};

// CUpdateDemoDlg 对话框
class CUpdateDemoDlg : public CDialogEx
{
// 构造
public:
	CUpdateDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UPDATEDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CBrush m_backbrush;
	CListCtrl m_listCtrl;
	CGdipButton m_btn_finish;
	CWinThread* pThreadCheckUpdate;

	void InitStyle();

	void FindFile(CString sPrePath, CString sFilePath, vector<FileInfo> &vec_File);

	static UINT ThreadCheckUpdate(LPVOID pParam);
	afx_msg void OnBnClickedButtonFinish();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
};
