// Calibration.cpp : 实现文件
//

#include "stdafx.h"
#include "10G-APD-TEST.h"
#include "Calibration.h"


// Calibration 对话框

IMPLEMENT_DYNAMIC(Calibration, CDialog)

Calibration::Calibration(CWnd* pParent /*=NULL*/)
	: CDialog(Calibration::IDD, pParent)
	, m_PowerVal_edit(_T(""))
{

}

Calibration::~Calibration()
{
}

void Calibration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Power_EDIT, m_PowerVal_edit);
}


BEGIN_MESSAGE_MAP(Calibration, CDialog)
	ON_BN_CLICKED(IDOK, &Calibration::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Calibration::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_Power_EDIT, &Calibration::OnEnChangePowerEdit)
END_MESSAGE_MAP()


// Calibration 消息处理程序

void Calibration::OnBnClickedOk()
{
	g_PowerVal = (float)atof(m_PowerVal_edit);
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void Calibration::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void Calibration::OnEnChangePowerEdit()
{
	UpdateData(true);
}
