// 10G-APD-TEST.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "10G-APD-TEST.h"
//#include "10G-APD-TESTDlg.h"
#include "Login.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy10GAPDTESTApp

BEGIN_MESSAGE_MAP(CMy10GAPDTESTApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMy10GAPDTESTApp ����

CMy10GAPDTESTApp::CMy10GAPDTESTApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMy10GAPDTESTApp ����

CMy10GAPDTESTApp theApp;


// CMy10GAPDTESTApp ��ʼ��

BOOL CMy10GAPDTESTApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	HANDLE h =CreateMutex(NULL,FALSE,"YouProgramName");
	if( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		MessageBox(NULL,_T("�ó����������У������ظ���!"),_T("��ʾ"),MB_OK); 
		return FALSE;
	}


	////////////////////ע��ؼ�/////////////////////////
	CString strLampPath = _T("D:\\Ctrl\\StatusLamp.ocx"); 
	if ( !IsRegistOcx() )
		RegisterOcx(strLampPath);
	/////////////////////////////////////////////////////


	Login dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}


void CMy10GAPDTESTApp::RegisterOcx( CString strOcxFileName ) 
{ 
	HINSTANCE hLib = LoadLibrary( strOcxFileName.GetBuffer() ); 
	if ( hLib == NULL ) 
	{ 
		//MessageBox(NULL,_T("��������OCX�ļ�!"),_T("��ʾ"),MB_OK); 
		return; 
	} 
	//��ȡע�ắ��DllRegisterServer��ַ
	FARPROC lpDllEntryPoint; 
	lpDllEntryPoint = GetProcAddress( hLib, "DllRegisterServer" ); 
	//����ע�ắ��DllRegisterServer 
	if( lpDllEntryPoint != NULL ) 
	{ 
		if( FAILED((*lpDllEntryPoint)()) ) 
		{ 
			MessageBox(NULL,_T("����DllRegisterServerʧ��,�������win7ϵͳ����һ��ִ��ʱ���Ҽ�ѡ��<�Թ���Ա�������>��ʽ��ִ����������! "),_T("��ʾ"),MB_OK); 
			FreeLibrary(hLib); 
			return; 
		}; 
		MessageBox(NULL,_T("ע��ɹ�! "),_T("��ʾ"),MB_OK);
	} 
	else 
	{
		
		MessageBox(NULL,_T("����DllRegisterServerʧ��,�������win7ϵͳ����һ��ִ��ʱ���Ҽ�ѡ��<�Թ���Ա�������>��ʽ��ִ����������! "),_T("��ʾ"),MB_OK); 
	}
}

void CMy10GAPDTESTApp::UnRegisterOcx( CString strOcxFileName )
{
	HINSTANCE hLib = LoadLibrary(strOcxFileName.GetBuffer()); 
	if (hLib==NULL) 
	{ 
		return; 
	} 
	FARPROC lpDllEntryPoint; 
	lpDllEntryPoint = GetProcAddress( hLib, "DllUnregisterServer"); 
	if( lpDllEntryPoint != NULL ) 
	{ 
		if( ((*lpDllEntryPoint)()) ) 
		{
			FreeLibrary(hLib); 
			return; 
		}
	}
}

BOOL CMy10GAPDTESTApp::IsRegistOcx()
{
	HKEY hKey;
	if ( RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("CLSID\\{45A71C0E-E00C-4D19-991E-F6C085955332}\\InprocServer32"), 0, KEY_READ,&hKey) != ERROR_SUCCESS )
		return false;

	return true;
}