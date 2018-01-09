
// UpdateDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UpdateDemo.h"
#include "UpdateDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUpdateDemoDlg �Ի���



CUpdateDemoDlg::CUpdateDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpdateDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUpdateDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_FINISH, m_btn_finish);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CUpdateDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_FINISH, &CUpdateDemoDlg::OnBnClickedButtonFinish)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CUpdateDemoDlg ��Ϣ�������

BOOL CUpdateDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	InitSetting();

	InitStyle();

	OutputLog("��������ļ��Ƿ��и���","UpdateLog.log");
	CWinThread* pThreadCheckUpdate = ::AfxBeginThread(ThreadCheckUpdate, NULL);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CUpdateDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUpdateDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CUpdateDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUpdateDemoDlg::InitSetting()
{
	CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����
	char RetURL[MAX_PATH];
	BOOL bFind = finder.FindFile(".\\updatesetting.ini");
	if (bFind)
	{
		::GetPrivateProfileString("ServerInfo", "url", "1", RetURL, MAX_PATH, ".\\updatesetting.ini");
		m_sServerUrl = RetURL;
	}
	else
	{
		m_sServerUrl = "";
	}
}

void CUpdateDemoDlg::InitStyle()
{
	//������ɫ
	m_backbrush.CreateSolidBrush(RGB(212, 242, 255));

	// ������ɰ�ť
	m_btn_finish.LoadAltImage(IDB_PNG_FINISH_2, "PNG");
	m_btn_finish.LoadStdImage(IDB_PNG_FINISH_1, "PNG");
	m_btn_finish.EnableToggle(true);

	//������
	LONG lStyle;
	lStyle = GetWindowLong(m_listCtrl.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(m_listCtrl.m_hWnd, GWL_STYLE, lStyle);//����style

	DWORD dwStyle = m_listCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	m_listCtrl.SetExtendedStyle(dwStyle); //������չ���
	m_listCtrl.InsertColumn(0, "Index", LVCFMT_LEFT, 60);
	m_listCtrl.InsertColumn(1, "NAME", LVCFMT_LEFT, 250);
	m_listCtrl.InsertColumn(2, "TIME", LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(3, "VERSION", LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(4, "UPDATE", LVCFMT_LEFT, 150);

	
}


//���������ɰ�ť
void CUpdateDemoDlg::OnBnClickedButtonFinish()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

}


HBRUSH CUpdateDemoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	hbr = (HBRUSH)m_backbrush;
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

//����Ƿ��и���
UINT CUpdateDemoDlg::ThreadCheckUpdate(LPVOID pParam)
{
	//�Ƿ��и��±�־
	CUpdateDemoDlg * pDlg = ((CUpdateDemoDlg *)(AfxGetApp()->m_pMainWnd));
	CUpdateDemoDlg * pWnd = (CUpdateDemoDlg *)pParam;

	bool bExistUpdate = false;
	vector<FileInfo> vec_CurrenFile;
	map<CString, FileInfo> map_OldFile;

	//��ȡ��ǰ·���������ļ�����Ϣ
	vec_CurrenFile.clear();
	pDlg->FindFile("", "",  ".", vec_CurrenFile);

	//��ȡconfig.ini����ǰ�İ汾��Ϣ
	map_OldFile.clear();
	pDlg->ReadIni(".\\config.ini", map_OldFile);

	//����ǰ�����ļ���Ϣ��֮ǰ��config.ini���бȽ�
	for (int i = 0; i < vec_CurrenFile.size(); i++)
	{

		//���ڵĻ�  ���޸�ʱ�䣬�ļ���С�Ƚϣ�������һ����ͬ�������������汾
		if (map_OldFile.count(vec_CurrenFile[i].sFileName))
		{
			if (map_OldFile[vec_CurrenFile[i].sFileName].nFileSize != vec_CurrenFile[i].nFileSize || map_OldFile[vec_CurrenFile[i].sFileName].sFileChangeTime != vec_CurrenFile[i].sFileChangeTime)
			{
				CString sPreVersion = map_OldFile[vec_CurrenFile[i].sFileName].sFileVersion;
				CString sNewVersion = "";
				int nIndex = -1;
				nIndex = sPreVersion.ReverseFind('.');
				sNewVersion = sPreVersion.Right(sPreVersion.GetLength() -  nIndex - 1);
				sNewVersion = sPreVersion.Left(nIndex + 1) + ConvertToCString(atoi(sNewVersion) + 1);
				vec_CurrenFile[i].sFileVersion = sNewVersion;

				int nRow = pDlg->m_listCtrl.InsertItem(i, "11");
				pDlg->m_listCtrl.SetItemText(nRow, 0, ConvertToCString(i));
				pDlg->m_listCtrl.SetItemText(nRow, 1, vec_CurrenFile[i].sFileName);
				pDlg->m_listCtrl.SetItemText(nRow, 2, vec_CurrenFile[i].sFileChangeTime);
				pDlg->m_listCtrl.SetItemText(nRow, 3, vec_CurrenFile[i].sFileVersion);
				pDlg->m_listCtrl.SetItemText(nRow, 4, "����");
				bExistUpdate = true;

			}
			else
			{
				vec_CurrenFile[i].sFileVersion = map_OldFile[vec_CurrenFile[i].sFileName].sFileVersion;
				int nRow = pDlg->m_listCtrl.InsertItem(i, "11");
				pDlg->m_listCtrl.SetItemText(nRow, 0, ConvertToCString(i));
				pDlg->m_listCtrl.SetItemText(nRow, 1, vec_CurrenFile[i].sFileName);
				pDlg->m_listCtrl.SetItemText(nRow, 2, vec_CurrenFile[i].sFileChangeTime);
				pDlg->m_listCtrl.SetItemText(nRow, 3, vec_CurrenFile[i].sFileVersion);
				pDlg->m_listCtrl.SetItemText(nRow, 4, "");
				bExistUpdate = true;
			}
		}
		else
		{
			vec_CurrenFile[i].sFileVersion = "v1.0.1";
			int nRow = pDlg->m_listCtrl.InsertItem(i, "11");
			pDlg->m_listCtrl.SetItemText(nRow, 0, ConvertToCString(i));
			pDlg->m_listCtrl.SetItemText(nRow, 1, vec_CurrenFile[i].sFileName);
			pDlg->m_listCtrl.SetItemText(nRow, 2, vec_CurrenFile[i].sFileChangeTime);
			pDlg->m_listCtrl.SetItemText(nRow, 3, "v1.0.1");
			pDlg->m_listCtrl.SetItemText(nRow, 4, "����");
			bExistUpdate = true;
		}
	}

	if (bExistUpdate)
	{
		pDlg->WriteIni(".\\config.ini", vec_CurrenFile);
	}

	return bExistUpdate;
}

//�ݹ����
void CUpdateDemoDlg::FindFile(CString sPrePath, CString sPreUrl, CString sFilePath, vector<FileInfo> &vec_File)
{
	CFile file;
	CFileStatus fileStatus;
	CFileFind ff;
	CString sFindPath = sFilePath + "\\*.*";
	FileInfo fileInfo;

	int nRet = ff.FindFile(sFindPath);
	while (nRet)
	{
		nRet = ff.FindNextFile();
		CString FilePath = ff.GetFilePath();
		CString name = ff.GetFileName();

		//������ļ���
		if (ff.IsDirectory() && !ff.IsDots())
		{
			if (sPrePath != "")
				FindFile(sPrePath + "\\" + name, sPreUrl + name + "/", FilePath, vec_File);
			else
				FindFile(sPrePath + name, name + "/", FilePath, vec_File);
		}

		//�����ļ��У����ļ�
		if (!ff.IsDirectory() && !ff.IsDots())
		{
			if (CFile::GetStatus(FilePath.GetBuffer(), fileStatus))
			{
				if (sPrePath != "")
					fileInfo.sFileName = sPrePath + "\\" + name;
				else
					fileInfo.sFileName = name;

				fileInfo.sFileChangeTime = fileStatus.m_mtime.Format("%Y%m%d %X");
				fileInfo.nFileSize = fileStatus.m_size;
				fileInfo.sFileURL = m_sServerUrl + sPreUrl + name;
				vec_File.push_back(fileInfo);
			}
		}
	}

	ff.Close();
}

void CUpdateDemoDlg::ReadIni(CString sIniPath, map<CString, FileInfo> & map_File)
{
	CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����  
	BOOL bFind = finder.FindFile(sIniPath);
	if (!bFind)
	{

	}
	else
	{
		unsigned int index = 0;
		int  RetIndex;
		char RetFlNm[MAX_PATH];
		char RetURL[MAX_PATH];
		char RetModifyTm[MAX_PATH];
		int RetSize = 0;
		char RetVersion[MAX_PATH];
		char buf[1024 * 100];  //100KB
		FileInfo fileInfo;

		::GetPrivateProfileSectionNamesA(buf, sizeof(buf), sIniPath.GetBuffer());
		for (char *Name = buf; *Name != 0; Name += strlen(Name) + 1)
		{

			RetIndex = ::GetPrivateProfileIntA(Name, "Index", 1, sIniPath.GetBuffer());
			::GetPrivateProfileString(Name, "Name", "1", RetFlNm, MAX_PATH, sIniPath.GetBuffer());
			::GetPrivateProfileString(Name, "URL", "1", RetURL, MAX_PATH, sIniPath.GetBuffer());
			::GetPrivateProfileString(Name, "ModifyTm", "1", RetModifyTm, MAX_PATH, sIniPath.GetBuffer());
			RetSize = ::GetPrivateProfileInt(Name, "Size", 0, sIniPath.GetBuffer());
			::GetPrivateProfileString(Name, "Version", "1", RetVersion, MAX_PATH, sIniPath.GetBuffer());
			
			//map�в����ڵ�ǰ�ļ�������뵽map��
			if (!map_File.count(RetFlNm))
			{
				fileInfo.nIndex = RetIndex;
				fileInfo.sFileName = RetFlNm;
				fileInfo.sFileURL = RetURL;
				fileInfo.sFileChangeTime = RetModifyTm;
				fileInfo.nFileSize = RetSize;
				fileInfo.sFileVersion = RetVersion;
				map_File[RetFlNm] = fileInfo;
			}
		}
	}
}
void CUpdateDemoDlg::WriteIni(CString sIniPath, vector<FileInfo> &vec_File)
{
	CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����  
	BOOL bFind = finder.FindFile(sIniPath);
	if (bFind)
	{
		DeleteFile(sIniPath);
	}

	
	for (int i = 0; i < vec_File.size(); i++)
	{
		CString Name = "FileSection" + ConvertToCString(i);
		::WritePrivateProfileString(Name, "Index", ConvertToCString(i), sIniPath.GetBuffer());
		::WritePrivateProfileString(Name, "Name",  vec_File[i].sFileName, sIniPath.GetBuffer());
		::WritePrivateProfileString(Name, "URL",  vec_File[i].sFileURL, sIniPath.GetBuffer());
		::WritePrivateProfileString(Name, "ModifyTm",  vec_File[i].sFileChangeTime, sIniPath.GetBuffer());
		::WritePrivateProfileString(Name, "Size", ConvertToCString(vec_File[i].nFileSize), sIniPath.GetBuffer());
		::WritePrivateProfileString(Name, "Version", vec_File[i].sFileVersion, sIniPath.GetBuffer());
	}
}