
// IpcClientDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CIpcClientDemoApp:
// �йش����ʵ�֣������ IpcClientDemo.cpp
//

class CIpcClientDemoApp : public CWinApp
{
public:
	CIpcClientDemoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CIpcClientDemoApp theApp;