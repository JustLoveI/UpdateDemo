
// UpdateDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "HeadFileShared\HeadFileShared.h"
#include "GdipButton\GdipButton.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>
#include <map>
using namespace std;


//�ļ���Ϣ�ṹ��
struct FileInfo
{
	//�ļ����
	int nIndex;

	//�ļ����ƴ�·�� ��    Update/AutoUpdate.exe
	CString sFileName;

	//�ļ�����URL     ��    URL=http://192.168.1.105:8080/spzpjk/updateclient/AutoUpdate.exe
	CString sFileURL;
	
	//�ļ������޸�ʱ�� ��
	CString sFileChangeTime;

	//�ļ��汾			��   v1.0.1
	CString sFileVersion;
};

// CUpdateDemoDlg �Ի���
class CUpdateDemoDlg : public CDialogEx
{
// ����
public:
	CUpdateDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UPDATEDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;


	// ���ɵ���Ϣӳ�亯��
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
