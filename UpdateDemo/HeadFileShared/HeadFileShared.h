#ifndef HEADFILESHARED_H
#define HEADFILESHARED_H
#endif
#include "afxwin.h"

//最新更改时间：2017-09-30 09:53:11


//获取可执行文件的绝对路径
CString GetExecuTableFilePath();

//log 日志输出 如果不指定log日志路径，则在可执行文件路径下，日志文件名称以时间命名 例：20170922.log
void OutputLog(CString sLogText, CString sSavePath = "");

//CSring trans to  Utf8 
char* ConvertCStringToUtf8(const char *zFilename);

//UTF8 trans to GBK
void  ConvertUtf8ToGBK(CString&amp, char *strUtf8);

//nNum trans to CString
CString ConvertToCString(int nNUm);

//fNum trans to CString .6f
CString ConvertToCString(float fNum);

//单例模式 true 创建互斥体成功； false 创建互斥体失败
bool InitSingleton(CString sNameOfMutexObject);
