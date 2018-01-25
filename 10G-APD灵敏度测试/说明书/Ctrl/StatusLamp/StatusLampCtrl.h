#pragma once

// StatusLampCtrl.h : CStatusLampCtrl ActiveX �ؼ����������


// CStatusLampCtrl : �й�ʵ�ֵ���Ϣ������� StatusLampCtrl.cpp��

class CStatusLampCtrl : public COleControl
{
	DECLARE_DYNCREATE(CStatusLampCtrl)

// ���캯��
public:
	CStatusLampCtrl();

// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// ʵ��
protected:
	~CStatusLampCtrl();

	DECLARE_OLECREATE_EX(CStatusLampCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(CStatusLampCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CStatusLampCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(CStatusLampCtrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
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

