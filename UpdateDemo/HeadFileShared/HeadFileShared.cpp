#include "HeadFileShared.h"
#include "stdafx.h"


///**************************

///* ��������GetExecuTableFilePath
///* ���ܣ���ȡ��ִ���ļ�����·�� 
///* ���ߣ�������
///* ���ڣ�2017-3-21 11:16:42

///**************************
CString GetExecuTableFilePath()
{
	TCHAR szModule[MAX_PATH];
	CString m_FilePathOfExe;
	GetModuleFileName(NULL, szModule, MAX_PATH);//�õ������������ȫ·��  
	m_FilePathOfExe.Format("%s", szModule);
	int nIndex = m_FilePathOfExe.ReverseFind('\\');
	m_FilePathOfExe = m_FilePathOfExe.Left(nIndex);
	return m_FilePathOfExe;
}

///**************************

///* ��������OutputLog
///* ���ܣ����log��־  
///* ���ߣ�������
///* ���ڣ�2017-3-21 11:16:42

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
		//�����ļ��Ĵ򿪲���
		CStdioFile outFile(logName, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		CString msLine;


		//��ΪLog�ļ�������Ҫ��ÿ��Log��ʱ�����ʱ���ʽ�����ɶ��壬
		//����ĸ�ʽ�磺2010-June-10 Thursday, 15:58:12
		msLine = tt.Format("[%Y-%B-%d %A, %H:%M:%S] ") + msg;
		msLine += "\n";

		//���ļ�ĩβ�����¼�¼
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

///* ��������ConvertCStringToUtf8
///* ���ܣ���CString ת utf8��ʽ��sqlite3 �����ݿ�ʱ����ʹ��    
///* ���ߣ�������
///* ���ڣ�2017-3-21 11:16:42

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

///* ��������ConvertUtf8ToGBK
///* ���ܣ�utf8 ת GBK  
///* ���ߣ�������
///* ���ڣ�2017-3-21 11:16:42

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

///* ��������ConvertToCString
///* ���ܣ�nNum trans to CString  
///* ���ߣ�������
///* ���ڣ�2017-3-21 11:16:42

///**************************
CString ConvertToCString(int nNUm)
{
	CString sResult = "";
	sResult.Format("%d", nNUm);
	return sResult;
}

///**************************

///* ��������ConvertToCString
///* ���ܣ�fNum trans to CString   
///* ���ߣ�������
///* ���ڣ�2017-3-21 11:16:42

///**************************
CString ConvertToCString(float fNUm)
{
	CString sResult = "";
	sResult.Format("%.6f", fNUm);
	return sResult;
}

///**************************

///* ��������Function
///* ���ܣ�����ģʽ   �½�������
///* ���ߣ�������
///* ���ڣ�2017-09-29 15:07:12

///**************************
bool InitSingleton(CString sNameOfMutexObject)
{
	HANDLE MutexSingleton;
	MutexSingleton = ::CreateMutex(NULL, TRUE, _T("TrackFaceDemo"));

	if (MutexSingleton != NULL)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS) //�����Ѿ������У�ֱ�ӽ���
		{
			CloseHandle(MutexSingleton);
			return false;
		}
	}
	return true;
}
