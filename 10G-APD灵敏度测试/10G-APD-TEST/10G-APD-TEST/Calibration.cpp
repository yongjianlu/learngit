// Calibration.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "10G-APD-TEST.h"
#include "Calibration.h"


// Calibration �Ի���

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


// Calibration ��Ϣ�������

void Calibration::OnBnClickedOk()
{
	g_PowerVal = (float)atof(m_PowerVal_edit);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void Calibration::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void Calibration::OnEnChangePowerEdit()
{
	UpdateData(true);
}
