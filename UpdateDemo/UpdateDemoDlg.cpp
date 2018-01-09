
// UpdateDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UpdateDemo.h"
#include "UpdateDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CUpdateDemoDlg 对话框



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


// CUpdateDemoDlg 消息处理程序

BOOL CUpdateDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	InitSetting();

	InitStyle();

	OutputLog("检测服务端文件是否有更新","UpdateLog.log");
	CWinThread* pThreadCheckUpdate = ::AfxBeginThread(ThreadCheckUpdate, NULL);

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUpdateDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUpdateDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUpdateDemoDlg::InitSetting()
{
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用
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
	//背景颜色
	m_backbrush.CreateSolidBrush(RGB(212, 242, 255));

	// 更新完成按钮
	m_btn_finish.LoadAltImage(IDB_PNG_FINISH_2, "PNG");
	m_btn_finish.LoadStdImage(IDB_PNG_FINISH_1, "PNG");
	m_btn_finish.EnableToggle(true);

	//更新列
	LONG lStyle;
	lStyle = GetWindowLong(m_listCtrl.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_listCtrl.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_listCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_listCtrl.SetExtendedStyle(dwStyle); //设置扩展风格
	m_listCtrl.InsertColumn(0, "Index", LVCFMT_LEFT, 60);
	m_listCtrl.InsertColumn(1, "NAME", LVCFMT_LEFT, 250);
	m_listCtrl.InsertColumn(2, "TIME", LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(3, "VERSION", LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(4, "UPDATE", LVCFMT_LEFT, 150);

	
}


//点击更新完成按钮
void CUpdateDemoDlg::OnBnClickedButtonFinish()
{
	// TODO:  在此添加控件通知处理程序代码

}


HBRUSH CUpdateDemoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	hbr = (HBRUSH)m_backbrush;
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//检查是否有更新
UINT CUpdateDemoDlg::ThreadCheckUpdate(LPVOID pParam)
{
	//是否有更新标志
	CUpdateDemoDlg * pDlg = ((CUpdateDemoDlg *)(AfxGetApp()->m_pMainWnd));
	CUpdateDemoDlg * pWnd = (CUpdateDemoDlg *)pParam;

	bool bExistUpdate = false;
	vector<FileInfo> vec_CurrenFile;
	map<CString, FileInfo> map_OldFile;

	//获取当前路径下所有文件的信息
	vec_CurrenFile.clear();
	pDlg->FindFile("", "",  ".", vec_CurrenFile);

	//读取config.ini中以前的版本信息
	map_OldFile.clear();
	pDlg->ReadIni(".\\config.ini", map_OldFile);

	//将当前所有文件信息与之前的config.ini进行比较
	for (int i = 0; i < vec_CurrenFile.size(); i++)
	{

		//存在的话  对修改时间，文件大小比较，二者有一个不同，就向上提升版本
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
				pDlg->m_listCtrl.SetItemText(nRow, 4, "更新");
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
			pDlg->m_listCtrl.SetItemText(nRow, 4, "新增");
			bExistUpdate = true;
		}
	}

	if (bExistUpdate)
	{
		pDlg->WriteIni(".\\config.ini", vec_CurrenFile);
	}

	return bExistUpdate;
}

//递归查找
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

		//如果是文件夹
		if (ff.IsDirectory() && !ff.IsDots())
		{
			if (sPrePath != "")
				FindFile(sPrePath + "\\" + name, sPreUrl + name + "/", FilePath, vec_File);
			else
				FindFile(sPrePath + name, name + "/", FilePath, vec_File);
		}

		//不是文件夹，是文件
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
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
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
			
			//map中不存在当前文件，则插入到map中
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
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
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