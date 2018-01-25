// 10G-APD-TEST.cpp : 定义应用程序的类行为。
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


// CMy10GAPDTESTApp 构造

CMy10GAPDTESTApp::CMy10GAPDTESTApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMy10GAPDTESTApp 对象

CMy10GAPDTESTApp theApp;


// CMy10GAPDTESTApp 初始化

BOOL CMy10GAPDTESTApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	HANDLE h =CreateMutex(NULL,FALSE,"YouProgramName");
	if( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		MessageBox(NULL,_T("该程序正在运行，不能重复打开!"),_T("提示"),MB_OK); 
		return FALSE;
	}


	////////////////////注册控件/////////////////////////
	CString strLampPath = _T("D:\\Ctrl\\StatusLamp.ocx"); 
	if ( !IsRegistOcx() )
		RegisterOcx(strLampPath);
	/////////////////////////////////////////////////////


	Login dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}


void CMy10GAPDTESTApp::RegisterOcx( CString strOcxFileName ) 
{ 
	HINSTANCE hLib = LoadLibrary( strOcxFileName.GetBuffer() ); 
	if ( hLib == NULL ) 
	{ 
		//MessageBox(NULL,_T("不能载入OCX文件!"),_T("提示"),MB_OK); 
		return; 
	} 
	//获取注册函数DllRegisterServer地址
	FARPROC lpDllEntryPoint; 
	lpDllEntryPoint = GetProcAddress( hLib, "DllRegisterServer" ); 
	//调用注册函数DllRegisterServer 
	if( lpDllEntryPoint != NULL ) 
	{ 
		if( FAILED((*lpDllEntryPoint)()) ) 
		{ 
			MessageBox(NULL,_T("调用DllRegisterServer失败,如果您是win7系统，第一次执行时请右键选择<以管理员身份运行>方式来执行启动程序! "),_T("提示"),MB_OK); 
			FreeLibrary(hLib); 
			return; 
		}; 
		MessageBox(NULL,_T("注册成功! "),_T("提示"),MB_OK);
	} 
	else 
	{
		
		MessageBox(NULL,_T("调用DllRegisterServer失败,如果您是win7系统，第一次执行时请右键选择<以管理员身份运行>方式来执行启动程序! "),_T("提示"),MB_OK); 
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