// StatusLampPropPage.cpp : CStatusLampPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "StatusLamp.h"
#include "StatusLampPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CStatusLampPropPage, COlePropertyPage)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CStatusLampPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CStatusLampPropPage, "STATUSLAMP.StatusLampPropPage.1",
	0xe941d1de, 0x6bba, 0x447d, 0xa5, 0xa9, 0x31, 0xb1, 0xbe, 0x11, 0x3c, 0x54)



// CStatusLampPropPage::CStatusLampPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CStatusLampPropPage ��ϵͳע�����

BOOL CStatusLampPropPage::CStatusLampPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_STATUSLAMP_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CStatusLampPropPage::CStatusLampPropPage - ���캯��

CStatusLampPropPage::CStatusLampPropPage() :
	COlePropertyPage(IDD, IDS_STATUSLAMP_PPG_CAPTION)
{
}



// CStatusLampPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CStatusLampPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CStatusLampPropPage ��Ϣ�������
