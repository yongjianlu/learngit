// StatusLampPropPage.cpp : CStatusLampPropPage 属性页类的实现。

#include "stdafx.h"
#include "StatusLamp.h"
#include "StatusLampPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CStatusLampPropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(CStatusLampPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CStatusLampPropPage, "STATUSLAMP.StatusLampPropPage.1",
	0xe941d1de, 0x6bba, 0x447d, 0xa5, 0xa9, 0x31, 0xb1, 0xbe, 0x11, 0x3c, 0x54)



// CStatusLampPropPage::CStatusLampPropPageFactory::UpdateRegistry -
// 添加或移除 CStatusLampPropPage 的系统注册表项

BOOL CStatusLampPropPage::CStatusLampPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_STATUSLAMP_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CStatusLampPropPage::CStatusLampPropPage - 构造函数

CStatusLampPropPage::CStatusLampPropPage() :
	COlePropertyPage(IDD, IDS_STATUSLAMP_PPG_CAPTION)
{
}



// CStatusLampPropPage::DoDataExchange - 在页和属性间移动数据

void CStatusLampPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CStatusLampPropPage 消息处理程序
