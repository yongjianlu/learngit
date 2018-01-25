#pragma once
#include "_Font.h"

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CStatuslampctrl 包装类

class CStatuslampctrl : public CWnd
{
protected:
	DECLARE_DYNCREATE(CStatuslampctrl)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x45A71C0E, 0xE00C, 0x4D19, { 0x99, 0x1E, 0xF6, 0xC0, 0x85, 0x95, 0x53, 0x32 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 属性
public:


// 操作
public:

// _DStatusLamp

// Functions
//


// Properties
//

BOOL GetLampStatus()
{
	BOOL result;
	GetProperty(0x1, VT_BOOL, (void*)&result);
	return result;
}
void SetLampStatus(BOOL propVal)
{
	SetProperty(0x1, VT_BOOL, propVal);
}
CString GetMsgText()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}
void SetMsgText(CString propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}
short GetStyle()
{
	short result;
	GetProperty(0x3, VT_I2, (void*)&result);
	return result;
}
void SetStyle(short propVal)
{
	SetProperty(0x3, VT_I2, propVal);
}
unsigned long GetBkColor()
{
	unsigned long result;
	GetProperty(0x4, VT_UI4, (void*)&result);
	return result;
}
void SetBkColor(unsigned long propVal)
{
	SetProperty(0x4, VT_UI4, propVal);
}
unsigned long GetBolderColor()
{
	unsigned long result;
	GetProperty(0x5, VT_UI4, (void*)&result);
	return result;
}
void SetBolderColor(unsigned long propVal)
{
	SetProperty(0x5, VT_UI4, propVal);
}
BOOL GetBolderVisible()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}
void SetBolderVisible(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}
unsigned long GetV_Alignment()
{
	unsigned long result;
	GetProperty(0x7, VT_UI4, (void*)&result);
	return result;
}
void SetV_Alignment(unsigned long propVal)
{
	SetProperty(0x7, VT_UI4, propVal);
}
unsigned long GetAlignment()
{
	unsigned long result;
	GetProperty(0x8, VT_UI4, (void*)&result);
	return result;
}
void SetAlignment(unsigned long propVal)
{
	SetProperty(0x8, VT_UI4, propVal);
}
BOOL GetTextVisible()
{
	BOOL result;
	GetProperty(0x9, VT_BOOL, (void*)&result);
	return result;
}
void SetTextVisible(BOOL propVal)
{
	SetProperty(0x9, VT_BOOL, propVal);
}
unsigned long GetMsgTextColor()
{
	unsigned long result;
	GetProperty(0xa, VT_UI4, (void*)&result);
	return result;
}
void SetMsgTextColor(unsigned long propVal)
{
	SetProperty(0xa, VT_UI4, propVal);
}
short GetTextHeight()
{
	short result;
	GetProperty(0xb, VT_I2, (void*)&result);
	return result;
}
void SetTextHeight(short propVal)
{
	SetProperty(0xb, VT_I2, propVal);
}
COleFont GetFont()
{
	LPDISPATCH result;
	GetProperty(DISPID_FONT, VT_DISPATCH, (void*)&result);
	return COleFont(result);
}
void SetFont(LPDISPATCH propVal)
{
	SetProperty(DISPID_FONT, VT_DISPATCH, propVal);
}
unsigned long GetBackColor()
{
	unsigned long result;
	GetProperty(DISPID_BACKCOLOR, VT_UI4, (void*)&result);
	return result;
}
void SetBackColor(unsigned long propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_UI4, propVal);
}
unsigned long GetForeColor()
{
	unsigned long result;
	GetProperty(DISPID_FORECOLOR, VT_UI4, (void*)&result);
	return result;
}
void SetForeColor(unsigned long propVal)
{
	SetProperty(DISPID_FORECOLOR, VT_UI4, propVal);
}
BOOL GetRoundRect()
{
	BOOL result;
	GetProperty(0xc, VT_BOOL, (void*)&result);
	return result;
}
void SetRoundRect(BOOL propVal)
{
	SetProperty(0xc, VT_BOOL, propVal);
}


};
