// SpecConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "10G-APD-TEST.h"
#include "SpecConfig.h"
#include "Login.h"



// SpecConfig �Ի���

IMPLEMENT_DYNAMIC(SpecConfig, CDialog)

SpecConfig::SpecConfig(CWnd* pParent /*=NULL*/)
	: CDialog(SpecConfig::IDD, pParent)
	, m_fIccMax_edit(0)
	, m_fIccMin_edit(0)
	, m_fVbrMax_edit(0)
	, m_fVbrMin_edit(0)
	, m_fSenGoVal_edit(0)
	, m_fSenStep_edit(0)
	, m_fSenMax_edit(0)
	, m_fSenMin_edit(0)
	, m_fIdMax_edit(0)
	, m_fIdMin_edit(0)
{

}

SpecConfig::~SpecConfig()
{
}

void SpecConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_Channel_COMBO, m_Channel_Combo);
	DDX_Control(pDX, IDC_Type_COMBO, m_Type_Combo);
	DDX_Control(pDX, IDC_Rate_COMBO, m_Rate_Combo);
	DDX_Control(pDX, IDC_Amplitude_COMBO, m_Amplitude_Combo);
	DDX_Control(pDX, IDC_VopVal_COMBO, m_VopVal_Combo);
	DDX_Control(pDX, IDC_PowerWavelength_COMBO, m_PowerWaveLen_Combo);
	DDX_Control(pDX, IDC_AttWavelength_COMBO, m_AttWaveLen_Combo);

	DDX_Text(pDX, IDC_IccMax_EDIT, m_fIccMax_edit);
	DDX_Text(pDX, IDC_IccMin_EDIT, m_fIccMin_edit);
	DDX_Text(pDX, IDC_VbrMax_EDIT, m_fVbrMax_edit);
	DDX_Text(pDX, IDC_VbrMin_EDIT, m_fVbrMin_edit);
	DDX_Text(pDX, IDC_SenGo_EDIT, m_fSenGoVal_edit);
	DDX_Text(pDX, IDC_SenStep_EDIT, m_fSenStep_edit);
	DDX_Text(pDX, IDC_SenMax_EDIT, m_fSenMax_edit);
	DDX_Text(pDX, IDC_SenMin_EDIT, m_fSenMin_edit);
	DDX_Text(pDX, IDC_IdMax_EDIT, m_fIdMax_edit);
	DDX_Text(pDX, IDC_IdMin_EDIT, m_fIdMin_edit);
}


BEGIN_MESSAGE_MAP(SpecConfig, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_NextHigLeve_BUTTON, &SpecConfig::OnBnClickedNexthigleveButton)
	ON_EN_CHANGE(IDC_IccMax_EDIT, &SpecConfig::OnEnChangeIccmaxEdit)
	ON_EN_CHANGE(IDC_IccMin_EDIT, &SpecConfig::OnEnChangeIccminEdit)
	ON_EN_CHANGE(IDC_VbrMax_EDIT, &SpecConfig::OnEnChangeVbrmaxEdit)
	ON_EN_CHANGE(IDC_VbrMin_EDIT, &SpecConfig::OnEnChangeVbrminEdit)
	ON_EN_CHANGE(IDC_SenGo_EDIT, &SpecConfig::OnEnChangeSengoEdit)
	ON_EN_CHANGE(IDC_IdMax_EDIT, &SpecConfig::OnEnChangeIdmaxEdit)
	ON_EN_CHANGE(IDC_IdMin_EDIT, &SpecConfig::OnEnChangeIdminEdit)
	ON_EN_CHANGE(IDC_SenMax_EDIT, &SpecConfig::OnEnChangeSenmaxEdit)
	ON_EN_CHANGE(IDC_SenMin_EDIT, &SpecConfig::OnEnChangeSenminEdit)
	ON_EN_CHANGE(IDC_SenStep_EDIT, &SpecConfig::OnEnChangeSenstepEdit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_IccMax_SPIN, &SpecConfig::OnDeltaposIccmaxSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_IccMin_SPIN, &SpecConfig::OnDeltaposIccminSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_VbrMax_SPIN, &SpecConfig::OnDeltaposVbrmaxSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_VbrMin_SPIN, &SpecConfig::OnDeltaposVbrminSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SenGo_SPIN, &SpecConfig::OnDeltaposSengoSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_IdMax_SPIN, &SpecConfig::OnDeltaposIdmaxSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_IdMin_SPIN, &SpecConfig::OnDeltaposIdminSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SenMax_SPIN, &SpecConfig::OnDeltaposSenmaxSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SenMin_SPIN, &SpecConfig::OnDeltaposSenminSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SenStep_SPIN, &SpecConfig::OnDeltaposSenstepSpin)
END_MESSAGE_MAP()


// SpecConfig ��Ϣ�������
void SpecConfig::OnOK()
{
	
}

void SpecConfig::OnCancel()
{

}
void SpecConfig::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	EndDialog(IDCANCEL);		//�رմ��� 
	CDialog::OnClose();
}


BOOL SpecConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	DeviceConfigControls();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//�豸���ÿؼ���ʼ��
void SpecConfig::DeviceConfigControls()
{
	//������ͨ��ѡ��
	m_Channel_Combo.InsertString(0,_T("��ͨ��"));
	m_Channel_Combo.InsertString(1,_T("��ͨ��"));
	m_Channel_Combo.SetCurSel(0);

	//����������ѡ��
	m_Type_Combo.InsertString(0,_T("PRBS7"));
	m_Type_Combo.InsertString(1,_T("PRBS31"));
	m_Type_Combo.SetCurSel(0);

	//����������ѡ��
	m_Rate_Combo.InsertString(0,_T("9.95Gbps"));
	m_Rate_Combo.InsertString(1,_T("10.31Gbps"));
	m_Rate_Combo.InsertString(2,_T("10.52Gbps"));
	m_Rate_Combo.InsertString(3,_T("10.70Gbps"));
	m_Rate_Combo.InsertString(4,_T("11.09Gbps"));
	m_Rate_Combo.InsertString(5,_T("11.32Gbps"));
	m_Rate_Combo.InsertString(6,_T("10Gbps"));
	m_Rate_Combo.SetCurSel(0);

	//�����Ƿ���ѡ��
	m_Amplitude_Combo.InsertString(0,_T("100"));
	m_Amplitude_Combo.InsertString(1,_T("200"));
	m_Amplitude_Combo.InsertString(2,_T("300"));
	m_Amplitude_Combo.InsertString(3,_T("400"));
	m_Amplitude_Combo.InsertString(4,_T("500"));
	m_Amplitude_Combo.InsertString(5,_T("600"));
	m_Amplitude_Combo.InsertString(6,_T("700"));
	m_Amplitude_Combo.InsertString(7,_T("800"));
	m_Amplitude_Combo.SetCurSel(0);

	//VOPȡֵ��ʽѡ��
	m_VopVal_Combo.InsertString(0,_T("Vbr - 1"));
	m_VopVal_Combo.InsertString(1,_T("Vbr - 2"));
	m_VopVal_Combo.InsertString(2,_T("Vbr - 3"));
	m_VopVal_Combo.SetCurSel(1);


	//�⹦�ʼƲ���ѡ��
	m_PowerWaveLen_Combo.InsertString(0,_T("850nm"));
	m_PowerWaveLen_Combo.InsertString(1,_T("980nm"));
	m_PowerWaveLen_Combo.InsertString(2,_T("1310nm"));
	m_PowerWaveLen_Combo.InsertString(3,_T("1480nm"));
	m_PowerWaveLen_Combo.InsertString(4,_T("1550nm"));
	m_PowerWaveLen_Combo.InsertString(5,_T("1610nm"));
	m_PowerWaveLen_Combo.SetCurSel(2);

	//��˥��������ѡ��
	m_AttWaveLen_Combo.InsertString(0,_T("1310nm"));
	m_AttWaveLen_Combo.InsertString(1,_T("1550nm"));
	m_AttWaveLen_Combo.SetCurSel(0);
}

void SpecConfig::OnEnChangeIccmaxEdit()
{
	UpdateData(true);
}

void SpecConfig::OnEnChangeIccminEdit()
{
	UpdateData(true);
}

void SpecConfig::OnEnChangeVbrmaxEdit()
{
	UpdateData(true);
}

void SpecConfig::OnEnChangeVbrminEdit()
{
	UpdateData(true);
}

void SpecConfig::OnEnChangeSengoEdit()
{
	UpdateData(true);
}

void SpecConfig::OnEnChangeIdmaxEdit()
{
	UpdateData(true);
}

void SpecConfig::OnEnChangeIdminEdit()
{
	UpdateData(true);
}

void SpecConfig::OnEnChangeSenmaxEdit()
{
	UpdateData(true);
}

void SpecConfig::OnEnChangeSenminEdit()
{
	UpdateData(true);
}

void SpecConfig::OnEnChangeSenstepEdit()
{
	UpdateData(true);
}

//Spin�ؼ��Ĺ�������
void SpecConfig::SpinOperation(LPNMUPDOWN pNMUpDownTemp,float & fTemp)
{
	 if(pNMUpDownTemp->iDelta == 1)        // �����ֵΪ1 , ˵�������Spin�����¼�ͷ
    {
		fTemp -= 0.1f;
    }
    else if(pNMUpDownTemp->iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ
    {
		fTemp += 0.1f; 
    }
}

void SpecConfig::OnDeltaposIccmaxSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	SpinOperation(pNMUpDown,m_fIccMax_edit);
	UpdateData(false);

	*pResult = 0;
}

void SpecConfig::OnDeltaposIccminSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	SpinOperation(pNMUpDown,m_fIccMin_edit);
	UpdateData(false);

	*pResult = 0;
}

void SpecConfig::OnDeltaposVbrmaxSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	SpinOperation(pNMUpDown,m_fVbrMax_edit);
	UpdateData(false);

	*pResult = 0;
}

void SpecConfig::OnDeltaposVbrminSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	SpinOperation(pNMUpDown,m_fVbrMin_edit);
	UpdateData(false);

	*pResult = 0;
}

void SpecConfig::OnDeltaposSengoSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	SpinOperation(pNMUpDown,m_fSenGoVal_edit);
	UpdateData(false);

	*pResult = 0;
}

void SpecConfig::OnDeltaposSenstepSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	SpinOperation(pNMUpDown,m_fSenStep_edit);
	UpdateData(false);

	*pResult = 0;
}

void SpecConfig::OnDeltaposIdmaxSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	SpinOperation(pNMUpDown,m_fIdMax_edit);
	UpdateData(false);

	*pResult = 0;
}

void SpecConfig::OnDeltaposIdminSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	SpinOperation(pNMUpDown,m_fIdMin_edit);
	UpdateData(false);

	*pResult = 0;
}

void SpecConfig::OnDeltaposSenmaxSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	SpinOperation(pNMUpDown,m_fSenMax_edit);
	UpdateData(false);

	*pResult = 0;
}

void SpecConfig::OnDeltaposSenminSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	SpinOperation(pNMUpDown,m_fSenMin_edit);
	UpdateData(false);

	*pResult = 0;
}

//������һ��
void SpecConfig::OnBnClickedNexthigleveButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnClose();
	Login dlg;
	INT_PTR nResponse = dlg.DoModal();
}