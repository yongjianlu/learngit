// 10G-APD-TEST.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy10GAPDTESTApp:
// �йش����ʵ�֣������ 10G-APD-TEST.cpp
//

class CMy10GAPDTESTApp : public CWinApp
{
public:
	CMy10GAPDTESTApp();

	BOOL IsRegistOcx();
	void UnRegisterOcx( CString strOcxFileName );
	void RegisterOcx( CString strOcxFileName );

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy10GAPDTESTApp theApp;