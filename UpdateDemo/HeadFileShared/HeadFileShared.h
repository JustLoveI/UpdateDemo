#ifndef HEADFILESHARED_H
#define HEADFILESHARED_H
#endif
#include "afxwin.h"

//���¸���ʱ�䣺2017-09-30 09:53:11


//��ȡ��ִ���ļ��ľ���·��
CString GetExecuTableFilePath();

//log ��־��� �����ָ��log��־·�������ڿ�ִ���ļ�·���£���־�ļ�������ʱ������ ����20170922.log
void OutputLog(CString sLogText, CString sSavePath = "");

//CSring trans to  Utf8 
char* ConvertCStringToUtf8(const char *zFilename);

//UTF8 trans to GBK
void  ConvertUtf8ToGBK(CString&amp, char *strUtf8);

//nNum trans to CString
CString ConvertToCString(int nNUm);

//fNum trans to CString .6f
CString ConvertToCString(float fNum);

//����ģʽ true ����������ɹ��� false ����������ʧ��
bool InitSingleton(CString sNameOfMutexObject);
