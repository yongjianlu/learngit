// StatusLampCtrl.cpp : CStatusLampCtrl ActiveX �ؼ����ʵ�֡�

#include "stdafx.h"
#include "StatusLamp.h"
#include "StatusLampCtrl.h"
#include "StatusLampPropPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CStatusLampCtrl, COleControl)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CStatusLampCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// ����ӳ��

BEGIN_DISPATCH_MAP(CStatusLampCtrl, COleControl)
	DISP_PROPERTY_NOTIFY_ID(CStatusLampCtrl, "LampStatus", dispidLampStatus, m_bLampStatus, OnLampStatusChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CStatusLampCtrl, "MsgText", dispidMsgText, m_strMsgText, OnMsgTextChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CStatusLampCtrl, "Style", dispidStyle, m_nStyle, OnStyleChanged, VT_I2)
	DISP_PROPERTY_NOTIFY_ID(CStatusLampCtrl, "BolderColor", dispidBolderColor, m_clrBolderColor, OnBolderColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY_ID(CStatusLampCtrl, "BolderVisible", dispidBolderVisible, m_bBolderVisible, OnBolderVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CStatusLampCtrl, "V_Alignment", dispidV_Alignment, m_uVAlignment, OnV_AlignmentChanged, VT_UI4)
	DISP_PROPERTY_NOTIFY_ID(CStatusLampCtrl, "Alignment", dispidAlignment, m_uAlignment, OnAlignmentChanged, VT_UI4)
	DISP_PROPERTY_NOTIFY_ID(CStatusLampCtrl, "TextVisible", dispidTextVisible, m_bTextVisible, OnTextVisibleChanged, VT_BOOL)
	DISP_STOCKPROP_FONT()
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_FORECOLOR()
	DISP_PROPERTY_NOTIFY_ID(CStatusLampCtrl, "RoundRect", dispidRoundRect, m_bRoundRect, OnRoundRectChanged, VT_BOOL)
END_DISPATCH_MAP()



// �¼�ӳ��

BEGIN_EVENT_MAP(CStatusLampCtrl, COleControl)
END_EVENT_MAP()



// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CStatusLampCtrl, 1)
	PROPPAGEID(CStatusLampPropPage::guid)
END_PROPPAGEIDS(CStatusLampCtrl)



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CStatusLampCtrl, "STATUSLAMP.StatusLampCtrl.1",
	0x45a71c0e, 0xe00c, 0x4d19, 0x99, 0x1e, 0xf6, 0xc0, 0x85, 0x95, 0x53, 0x32)



// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(CStatusLampCtrl, _tlid, _wVerMajor, _wVerMinor)



// �ӿ� ID

const IID BASED_CODE IID_DStatusLamp =
		{ 0xCD85728B, 0x92CA, 0x4397, { 0xA2, 0xBF, 0xF7, 0x2, 0x31, 0x9, 0xE9, 0x6C } };
const IID BASED_CODE IID_DStatusLampEvents =
		{ 0x96B7E964, 0xD07D, 0x419E, { 0x94, 0x4, 0xA7, 0xFE, 0x5F, 0xB9, 0x46, 0xBB } };



// �ؼ�������Ϣ

static const DWORD BASED_CODE _dwStatusLampOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CStatusLampCtrl, IDS_STATUSLAMP, _dwStatusLampOleMisc)



// CStatusLampCtrl::CStatusLampCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CStatusLampCtrl ��ϵͳע�����

BOOL CStatusLampCtrl::CStatusLampCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: ��֤���Ŀؼ��Ƿ���ϵ�Ԫģ���̴߳������
	// �йظ�����Ϣ����ο� MFC ����˵�� 64��
	// ������Ŀؼ������ϵ�Ԫģ�͹�����
	// �����޸����´��룬��������������
	// afxRegApartmentThreading ��Ϊ 0��

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_STATUSLAMP,
			IDB_STATUSLAMP,
			afxRegApartmentThreading,
			_dwStatusLampOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CStatusLampCtrl::CStatusLampCtrl - ���캯��

CStatusLampCtrl::CStatusLampCtrl()
{
	InitializeIIDs(&IID_DStatusLamp, &IID_DStatusLampEvents);
	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
	m_pfList[PFS_FAIL].lpcCaption	=	_T("FAIL");
	m_pfList[PFS_FAIL].clrBkGround	=	RGB(255,48,48);
	m_pfList[PFS_FAIL].clrFore		=	RGB(32,32,32);

	m_pfList[PFS_PASS].lpcCaption	=	_T("PASS");
	m_pfList[PFS_PASS].clrBkGround	=	RGB(48,255,48);
	m_pfList[PFS_PASS].clrFore		=	RGB(48,48,255);

	m_dwAlignment[0]	=	DT_LEFT;
	m_dwAlignment[1]	=	DT_CENTER;
	m_dwAlignment[2]	=	DT_RIGHT;

	m_dwVAlignment[0]	=	DT_TOP;	
	m_dwVAlignment[1]	=	DT_VCENTER;
	m_dwVAlignment[2]	=	DT_BOTTOM;

	m_bLampStatus		=	VARIANT_TRUE;
	m_strMsgText		=	_T("��ʾ:");
	m_nStyle			=	1;
	m_clrBolderColor	=	RGB( 192, 192, 192 );
	m_bBolderVisible	=	VARIANT_TRUE;
	m_uVAlignment		=	1;
	m_uAlignment		=	0;
	m_bTextVisible		=	VARIANT_FALSE;
	m_bRoundRect		=	VARIANT_TRUE;
}



// CStatusLampCtrl::~CStatusLampCtrl - ��������

CStatusLampCtrl::~CStatusLampCtrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�
}



// CStatusLampCtrl::OnDraw - ��ͼ����

void CStatusLampCtrl::OnDraw( CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)	return;

// 	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
// 	pdc->Ellipse(rcBounds);
	// TODO: �����Լ��Ļ�ͼ�����滻����Ĵ��롣
	m_rcBounds.SetRect(	rcBounds.left, rcBounds.top, rcBounds.right, rcBounds.bottom );

	COLORREF clrBack = ( m_nStyle == 1)? GetBackColor(): m_pfList[ m_bLampStatus ].clrBkGround;


	//���ư�ť����
	CBrush	brush;
	brush.CreateSolidBrush( clrBack );
	pdc->SelectObject( &brush );

	CPen pen;
	pen.CreatePen( PS_SOLID, 1, m_bBolderVisible? m_clrBolderColor: clrBack );
	pdc->SelectObject( &pen );

	//���߿�
	if( m_bRoundRect)
		pdc->RoundRect( &rcBounds, CPoint( 5, 5 ) );			
	else
		pdc->Rectangle( &rcBounds );
	
	
	//��������
	CFont* pOldFont = SelectStockFont( pdc );
	
	DWORD	style = DT_SINGLELINE | DT_END_ELLIPSIS | m_dwAlignment[m_uAlignment] | m_dwVAlignment[m_uVAlignment];
	if( m_bTextVisible )
	{
		pdc->SetTextColor( ( m_nStyle == 1 )? GetForeColor(): m_pfList[ m_bLampStatus ].clrFore );
		pdc->SetBkMode( TRANSPARENT );
		pdc->DrawText( ( m_nStyle == 1 )? m_strMsgText: m_pfList[ m_bLampStatus ].lpcCaption, &m_rcBounds, style );		
	}

	pdc->SelectObject(pOldFont);
}



// CStatusLampCtrl::DoPropExchange - �־���֧��

void CStatusLampCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
	PX_Short( pPX, _T("LampStatus"), m_bLampStatus );
	PX_String( pPX, _T("MsgText"), m_strMsgText, _T("��ʾ:") );
	PX_Short( pPX, _T("Style"), m_nStyle );
	PX_Color( pPX, _T("BolderColor"), m_clrBolderColor );
	PX_Short( pPX, _T("BolderVisible"), m_bBolderVisible );
	PX_ULong( pPX, _T("V_Alignment"), m_uVAlignment );
	PX_ULong( pPX, _T("Alignment"), m_uAlignment );
	PX_Short( pPX, _T("TextVisible"), m_bTextVisible );
	PX_Short( pPX, _T("RoundRect"), m_bRoundRect );
}



// CStatusLampCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CStatusLampCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��
}

// CStatusLampCtrl ��Ϣ�������

void CStatusLampCtrl::OnLampStatusChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: �ڴ�������Դ���������
	Invalidate();
	SetModifiedFlag();
}

void CStatusLampCtrl::OnMsgTextChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: �ڴ�������Դ���������
	Invalidate();
	SetModifiedFlag();
}

void CStatusLampCtrl::OnStyleChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: �ڴ�������Դ���������
	Invalidate();
	SetModifiedFlag();
}

void CStatusLampCtrl::OnBolderColorChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: �ڴ�������Դ���������
	Invalidate();
	SetModifiedFlag();
}

void CStatusLampCtrl::OnBolderVisibleChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: �ڴ�������Դ���������
	Invalidate();
	SetModifiedFlag();
}

void CStatusLampCtrl::OnV_AlignmentChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: �ڴ�������Դ���������
	Invalidate();
	SetModifiedFlag();
}

void CStatusLampCtrl::OnAlignmentChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: �ڴ�������Դ���������
	Invalidate();
	SetModifiedFlag();
}

void CStatusLampCtrl::OnTextVisibleChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: �ڴ�������Դ���������
	Invalidate();
	SetModifiedFlag();
}

void CStatusLampCtrl::OnRoundRectChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ�������Դ���������
	Invalidate();
	SetModifiedFlag();
}

