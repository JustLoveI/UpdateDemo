#include "HeadFileShared.h"
#include "stdafx.h"


///**************************

///* 函数名：GetExecuTableFilePath
///* 功能：获取可执行文件绝对路径 
///* 作者：方文林
///* 日期：2017-3-21 11:16:42

///**************************
CString GetExecuTableFilePath()
{
	TCHAR szModule[MAX_PATH];
	CString m_FilePathOfExe;
	GetModuleFileName(NULL, szModule, MAX_PATH);//得到本程序自身的全路径  
	m_FilePathOfExe.Format("%s", szModule);
	int nIndex = m_FilePathOfExe.ReverseFind('\\');
	m_FilePathOfExe = m_FilePathOfExe.Left(nIndex);
	return m_FilePathOfExe;
}

///**************************

///* 函数名：OutputLog
///* 功能：输出log日志  
///* 作者：方文林
///* 日期：2017-3-21 11:16:42

///**************************
void OutputLog(CString msg, CString sFilePath)
{
	try
	{
		CTime tt = CTime::GetCurrentTime();
		CString logName;
		if (sFilePath == "")
			logName = GetExecuTableFilePath() + "\\" + tt.Format("%Y%m%d") + ".log";
		else
			logName = sFilePath;
		//设置文件的打开参数
		CStdioFile outFile(logName, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		CString msLine;


		//作为Log文件，经常要给每条Log打时间戳，时间格式可自由定义，
		//这里的格式如：2010-June-10 Thursday, 15:58:12
		msLine = tt.Format("[%Y-%B-%d %A, %H:%M:%S] ") + msg;
		msLine += "\n";

		//在文件末尾插入新纪录
		outFile.SeekToEnd();
		outFile.WriteString(msLine);
		outFile.Close();
	}
	catch (CFileException *fx)
	{
		fx->Delete();
	}
}

///**************************

///* 函数名：ConvertCStringToUtf8
///* 功能：将CString 转 utf8格式，sqlite3 打开数据库时可以使用    
///* 作者：方文林
///* 日期：2017-3-21 11:16:42

///**************************
char* ConvertCStringToUtf8(const char *zFilename)
{
	int nByte;
	WCHAR *zMbcsFilename;
	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
	nByte = MultiByteToWideChar(codepage, 0, zFilename, -1, NULL, 0)*sizeof(WCHAR);
	zMbcsFilename = (WCHAR *)malloc(nByte*sizeof(zMbcsFilename[0]));
	if (zMbcsFilename == 0)
	{
		return 0;
	}
	nByte = MultiByteToWideChar(codepage, 0, zFilename, -1, zMbcsFilename, nByte);
	if (nByte == 0)
	{
		free(zMbcsFilename);
		zMbcsFilename = 0;
		return NULL;
	}
	else
	{
		int nByte; char *zFilename;
		nByte = WideCharToMultiByte(CP_UTF8, 0, zMbcsFilename, -1, 0, 0, 0, 0);
		zFilename = (char *)malloc(nByte);
		if (zFilename == 0)
		{
			return 0;
		}
		nByte = WideCharToMultiByte(CP_UTF8, 0, zMbcsFilename, -1, zFilename, nByte, 0, 0);
		if (nByte == 0)
		{
			free(zFilename);
			zFilename = 0;
		}
		return zFilename;
	}
}

///**************************

///* 函数名：ConvertUtf8ToGBK
///* 功能：utf8 转 GBK  
///* 作者：方文林
///* 日期：2017-3-21 11:16:42

///**************************
void  ConvertUtf8ToGBK(CString&amp, char *strUtf8)
{

	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
	amp = szGBK;
	delete[] szGBK;
	delete[] wszGBK;

}

///**************************

///* 函数名：ConvertToCString
///* 功能：nNum trans to CString  
///* 作者：方文林
///* 日期：2017-3-21 11:16:42

///**************************
CString ConvertToCString(int nNUm)
{
	CString sResult = "";
	sResult.Format("%d", nNUm);
	return sResult;
}

///**************************

///* 函数名：ConvertToCString
///* 功能：fNum trans to CString   
///* 作者：方文林
///* 日期：2017-3-21 11:16:42

///**************************
CString ConvertToCString(float fNUm)
{
	CString sResult = "";
	sResult.Format("%.6f", fNUm);
	return sResult;
}

///**************************

///* 函数名：Function
///* 功能：单例模式   新建互斥量
///* 作者：方文林
///* 日期：2017-09-29 15:07:12

///**************************
bool InitSingleton(CString sNameOfMutexObject)
{
	HANDLE MutexSingleton;
	MutexSingleton = ::CreateMutex(NULL, TRUE, _T("TrackFaceDemo"));

	if (MutexSingleton != NULL)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS) //程序已经在运行，直接结束
		{
			CloseHandle(MutexSingleton);
			return false;
		}
	}
	return true;
}
