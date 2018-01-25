// StatusLamp.cpp : CStatusLampApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "StatusLamp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CStatusLampApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x719C8216, 0xBFF6, 0x4A40, { 0xA6, 0x46, 0x3, 0x18, 0xC1, 0x7E, 0xBD, 0xDE } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CStatusLampApp::InitInstance - DLL 初始化

BOOL CStatusLampApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。
	}

	return bInit;
}



// CStatusLampApp::ExitInstance - DLL 终止

int CStatusLampApp::ExitInstance()
{
	// TODO: 在此添加您自己的模块终止代码。

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
