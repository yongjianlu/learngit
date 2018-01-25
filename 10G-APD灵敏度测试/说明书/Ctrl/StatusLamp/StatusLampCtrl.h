#pragma once

// StatusLampCtrl.h : CStatusLampCtrl ActiveX 控件类的声明。


// CStatusLampCtrl : 有关实现的信息，请参阅 StatusLampCtrl.cpp。

class CStatusLampCtrl : public COleControl
{
	DECLARE_DYNCREATE(CStatusLampCtrl)

// 构造函数
public:
	CStatusLampCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CStatusLampCtrl();

	DECLARE_OLECREATE_EX(CStatusLampCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CStatusLampCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CStatusLampCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CStatusLampCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {	
		dispidRoundRect = 12,
		dispidTextHeight = 11,
		dispidMsgTextColor = 10,
		dispidTextVisible = 9,
		dispidAlignment = 8,
		dispidV_Alignment = 7,
		dispidBolderVisible = 6,
		dispidBolderColor = 5,
		dispidBkColor = 4,
		dispidStyle = 3,
		dispidMsgText = 2,
		dispidLampStatus = 1
	};

	typedef struct __tagLIGHTINFO
	{
		LPCWSTR		lpcCaption;
		COLORREF	clrBkGround;
		COLORREF	clrFore;
	}LIGHTINFO, *LPLIGHTINFO;

	enum PFStaticStatus
	{			
		PFS_FAIL	=	0,	
		PFS_PASS,
		PFS_MAX,
	};

private:
	CRect		m_rcBounds;
	CFont		m_Font;	

	LIGHTINFO	m_pfList[PFS_MAX];
	DWORD		m_dwAlignment[3];
	DWORD		m_dwVAlignment[3];

protected:
	void OnLampStatusChanged(void);
	VARIANT_BOOL m_bLampStatus;

	void OnMsgTextChanged(void);
	CString m_strMsgText;

	void OnStyleChanged(void);
	SHORT m_nStyle;

	void OnBolderColorChanged(void);
	OLE_COLOR m_clrBolderColor;

	void OnBolderVisibleChanged(void);
	VARIANT_BOOL m_bBolderVisible;

	void OnV_AlignmentChanged(void);
	ULONG m_uVAlignment;

	void OnAlignmentChanged(void);
	ULONG m_uAlignment;

	void OnTextVisibleChanged(void);
	VARIANT_BOOL m_bTextVisible;

	void OnRoundRectChanged(void);
	VARIANT_BOOL m_bRoundRect;

};

