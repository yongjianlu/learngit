// 10G-APD-TESTDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "10G-APD-TEST.h"
#include "10G-APD-TESTDlg.h"
#include "enumser.h"
#include "Calibration.h"
#include "ReadFile.h"
#include "fit.h"
#include "ReadTsSpec.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MODEL1 _T("10G-APD")
#define MODEL2 _T("10G-PIN-TIA")

#pragma comment(lib,"AIT-VOA-01.lib")			//��˥����
#pragma comment(lib,"APAT-VBR.lib")				//����VBR������
#pragma comment(lib,"PowerMeter_PM7501.lib")	//�⹦�ʼ�
#pragma comment(lib,"PSS-BERT-1-II.lib")		//������


static CFont * g_pFont1;//	�����С���Ʊ���1�������СΪ80��
static CFont * g_pFont2;//	�����С���Ʊ���2�������СΪ20��
static CFont * g_pFont3;//	�����С���Ʊ���2�������СΪ15��

static COLORREF InitColor = RGB(205,192,112);	//Button��ʼ����ɫ
static COLORREF TextColor = RGB(0,0,128);		//Button������ɫ
static COLORREF PassColor = RGB(144,238,144);	//Button Pass��ɫ
static COLORREF FailColor = RGB(255,000,0);		//Button File��ɫ

#define Color_Red      1		//��ɫ
#define Color_Green    2		//��ɫ
#define Color_BLUE     3		//��ɫ


static CString g_strActiveStep = _T("���");	//��������
static int g_IntActiveStep = 10;				//��ǰ���򣨲�����ı�ʶ��

static int g_ColorLogo = Color_BLUE;	//������ɫ

static BOOL g_DeviceInit  = FALSE;		//�Ƿ�������豸��ʼ������ɺ�ΪTRUE
static BOOL g_Calibration = FALSE;		//�Ƿ�����ɹ�У׼����ɺ�ΪTRUE
static BOOL g_Extractdata = FALSE;		//���������ȡ�Ƿ�ɹ����ɹ�ΪTRUE
static BOOL g_BeginTest   = FALSE;		//�Ƿ��ѿ����������ԣ��ѿ���ΪTRUE

static BOOL g_ThreadExit  = FALSE;		//�����˳��̣߳���Ϊtrue����ʾ�߳��˳�
static BOOL g_deviceTestState = FALSE;	//��ʾ�����Ƿ��ѿ�ʼ���в���

static long g_TimeBegin;				//��¼��ʼ����ʱ��
static long g_TimeEnd;					//

static CString  g_FailType		= _T("");		//����ʧ��ԭ������
static int      g_FailItemCode  = -1;			//ʧ�ܴ���

static CString g_strMaterialCode = _T("");	//���ݲ�Ʒ�ͺŻ�ȡ�������ϱ���
static CString g_strTsFileName   = _T("");	//�������ϱ����ȡ����TS�ļ���

static CString g_strModelType = _T("");	//��һ����ȡ���ʱ��ȡ���Ĳ�Ʒ�ͺ�


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMy10GAPDTESTDlg �Ի���




CMy10GAPDTESTDlg::CMy10GAPDTESTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy10GAPDTESTDlg::IDD, pParent)
	, m_strWorkOrderNo_edit(_T(""))
	, m_strBatchNo_edit(_T(""))
	, m_strPDModel_edit(_T(""))
	, m_strModel_edit(_T(""))
	, m_strSn_edit(_T(""))
	, m_strTestEr_edit(_T(""))
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
	, m_fTanP_edit(0)
	, m_SenTime_edit(0)
	, m_CompenVal_edit(0)
	, m_strVbrVal_edit(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_AitVoa  = new CAITVOA01();			//��˥����
	m_ApatVbr = new CAPATVBR();				//VBR������
	m_PM7501  = new CPowerMeter_PM7501();	//�⹦�ʼ�
	m_Bert    = new PssBert1II();			//������

	m_strTestEr_edit = g_UserAccount;		//�����˺�

	Data = new SaveData();		//��Ҫ��������ݵĽṹ

	m_SenTestBeginPowVal = 0.0f;
	m_PowStep1 = 0.0f;
	m_PowStep2 = 0.0f;
}

void CMy10GAPDTESTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_GridControl(pDX, IDC_GridDS, m_GridDS);
	DDX_GridControl(pDX, IDC_GridFitt, m_GridFitt);
	DDX_GridControl(pDX, IDC_GridTS, m_GridTS);

	DDX_Control(pDX, IDC_Channel_COMBO, m_Channel_Combo);
	DDX_Control(pDX, IDC_Type_COMBO, m_Type_Combo);
	DDX_Control(pDX, IDC_Rate_COMBO, m_Rate_Combo);
	DDX_Control(pDX, IDC_Amplitude_COMBO, m_Amplitude_Combo);
	DDX_Control(pDX, IDC_VopVal_COMBO, m_VopVal_Combo);
	DDX_Control(pDX, IDC_PowerWavelength_COMBO, m_PowerWaveLen_Combo);
	DDX_Control(pDX, IDC_AttWavelength_COMBO, m_AttWaveLen_Combo);
	DDX_Text(pDX, IDC_WorkOrderNo_EDIT, m_strWorkOrderNo_edit);
	DDX_Text(pDX, IDC_BatchNo_EDIT, m_strBatchNo_edit);
	DDX_Text(pDX, IDC_PDModel_EDIT, m_strPDModel_edit);
	DDX_Text(pDX, IDC_Model_EDIT, m_strModel_edit);
	DDX_Text(pDX, IDC_SN_EDIT, m_strSn_edit);
	DDX_Text(pDX, IDC_TestEr_EDIT, m_strTestEr_edit);
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
	DDX_Text(pDX, IDC_TanP_EDIT, m_fTanP_edit);
	DDX_Text(pDX, IDC_SenTime_EDIT, m_SenTime_edit);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
	DDX_Control(pDX, IDC_CHECK1, m_Check1);
	DDX_Text(pDX, IDC_CompensationVal_EDIT, m_CompenVal_edit);
	DDX_Text(pDX, IDC_Vbr_EDIT, m_strVbrVal_edit);
}

BEGIN_MESSAGE_MAP(CMy10GAPDTESTDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_WorkOrderNo_EDIT, &CMy10GAPDTESTDlg::OnEnChangeWorkordernoEdit)
	ON_EN_CHANGE(IDC_BatchNo_EDIT, &CMy10GAPDTESTDlg::OnEnChangeBatchnoEdit)
	ON_EN_CHANGE(IDC_SN_EDIT, &CMy10GAPDTESTDlg::OnEnChangeSnEdit)
	ON_EN_CHANGE(IDC_PowerVal_EDIT, &CMy10GAPDTESTDlg::OnEnChangePowervalEdit)
	ON_BN_CLICKED(IDC_Calibration1_BUTTON, &CMy10GAPDTESTDlg::OnBnClickedCalibration1Button)
	ON_BN_CLICKED(IDC_ExtractData_BUTTON, &CMy10GAPDTESTDlg::OnBnClickedExtractdataButton)
	ON_BN_CLICKED(IDC_DeviceInit_BUTTON, &CMy10GAPDTESTDlg::OnBnClickedDeviceinitButton)
	ON_BN_CLICKED(IDC_BeginTest_BUTTON, &CMy10GAPDTESTDlg::OnBnClickedBegintestButton)
	ON_EN_CHANGE(IDC_PDModel_EDIT, &CMy10GAPDTESTDlg::OnEnChangePdmodelEdit)
	ON_BN_CLICKED(IDC_TESTSPEC_CHECK, &CMy10GAPDTESTDlg::OnBnClickedTestspecCheck)
END_MESSAGE_MAP()


// CMy10GAPDTESTDlg ��Ϣ�������


void CMy10GAPDTESTDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMy10GAPDTESTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMy10GAPDTESTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMy10GAPDTESTDlg::OnOK()
{
	if(g_Calibration == FALSE)
	{
		MessageBox(_T("��У׼δ�ɹ������Ƚ��й�У׼��"), _T("��ʾ��Ϣ"), MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(g_Extractdata == FALSE)
	{
		MessageBox(_T("�������δ��ȡ��������ȡ������ݣ�"), _T("��ʾ��Ϣ"), MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(g_DeviceInit == FALSE)
	{
		MessageBox(_T("�豸δ��ʼ������������豸��ʼ����"), _T("��ʾ��Ϣ"), MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(g_BeginTest == TRUE)
	{
		StopTest();		//ֹͣ��������
		return ;
	}

	BeginTest();		//������������
}

void CMy10GAPDTESTDlg::OnCancel()
{

}

void CMy10GAPDTESTDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	g_ThreadExit = TRUE;	//�߳��˳�
	Sleep(500);

	delete m_AitVoa;
	delete m_ApatVbr;
	delete m_PM7501;
	delete m_Bert;

	m_AitVoa  = NULL;
	m_ApatVbr = NULL;
	m_PM7501  = NULL;
	m_Bert    = NULL;

	g_SqlCon->Close();		//�ر����ݿ�
	delete g_SqlCon;
	delete g_recordSql;
	g_SqlCon	= NULL;
	g_recordSql = NULL;

	g_AccCon->Close();
	delete g_AccCon;
	delete g_recordAcc;
	g_AccCon	= NULL;
	g_recordAcc = NULL;

	EndDialog(IDCANCEL);    //�رմ��� 
	CDialog::OnClose();
}

HBRUSH CMy10GAPDTESTDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if( IDC_Results_STATIC  ==  pWnd->GetDlgCtrlID())//�жϷ�����Ϣ�Ŀռ��Ƿ��Ǹþ�̬�ı���
		{
			pDC->SetBkMode(OPAQUE);					//�����ı�����ģʽΪ͸��

			pDC->SetBkColor(RGB(255,255,205));			//�����ı�����Ϊ��ɫ
			hbr=CreateSolidBrush(RGB(255,255,205));		//�ؼ��ı���ɫΪ��ɫ

			switch (g_ColorLogo)
			{ 
			case Color_Red : 
				pDC->SetTextColor(RGB(255,0,0));			//����������ɫ
				break; 

			case Color_Green : 
				pDC->SetTextColor(RGB(0,255,0));			//����������ɫ
				break;  

			case Color_BLUE : 
				pDC->SetTextColor(RGB(30,144,255));
				break;

			default : 
				break; 
			}
		}
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

BOOL CMy10GAPDTESTDlg::OnInitDialog()
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

	CString Str =  _T(" �����Ȳ��� 2018-01-23  V1.2");
	if(g_IntModelState == 0)
	{
		SetWindowText(MODEL1 + Str);
	}
	else if(g_IntModelState == 1)
	{
		SetWindowText(MODEL2 + Str);
	}

	//g_UserPermissions
	if(!_ttoi(g_EnabledArray[47]))	//û��ʹ����֤��������Ȩ��
	{
		CWnd * pWnd = NULL;
		pWnd = GetDlgItem(IDC_TESTSPEC_CHECK);
		pWnd->EnableWindow(FALSE);
	}

	/////////////////////�豸����//////////////////////////////

	GetComNum();				//��ȡ������ǰ���Դ��ں�
	if(DeviceConnet() == FALSE) //���豸�������ӣ�ʧ�����˳�����
	{
		//exit(0);
	}

	////////////////////���ݿ�����/////////////////////////////

	if(SqlServerConnet() == FALSE)	//�������ݿ����ӳ�ʼ��
	{
		MessageBox(_T("���������ݿ�����ʧ�ܣ������˳���"),_T("������ʾ"),MB_OKCANCEL | MB_ICONQUESTION);
		//exit(0);
	}

	if(AccessConnet() == FALSE)
	{
		MessageBox(_T("�������ݿ�����ʧ�ܣ������˳���"),_T("������ʾ"),MB_OKCANCEL | MB_ICONQUESTION);
		//exit(0);
	}

	ControlsTxetInit();			//�ؼ���ʼ��
	DeviceConfigControls();		//�豸���ÿؼ���ʼ��
	GridInit();					//���ؼ���ʼ��
	ButtonInit();				//Button�ؼ���ʼ��
	GetCalState();				//��У׼״̬���


	AfxBeginThread( ThreadTest, this );			//���������߳�
	AfxBeginThread( ThreadTimeRecord, this );	//����ʱ���߳�

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//�ؼ���ʼ��
void CMy10GAPDTESTDlg::ControlsTxetInit()
{
	g_pFont1 = new CFont;
	g_pFont1->CreateFont(80,								// nHeight
						0,									// nWidth
						0,									// nEscapement
						0,									// nOrientation
						FW_BOLD,							// nWeight
						false,								// bItalic
						FALSE,								// bUnderline
						0,									// cStrikeOut
						ANSI_CHARSET,						// nCharSet
						OUT_DEFAULT_PRECIS,					// nOutPrecision
						CLIP_DEFAULT_PRECIS,				// nClipPrecision
						DEFAULT_QUALITY,					// nQuality
						DEFAULT_PITCH | FF_SWISS,			// nPitchAndFamily
						_T("Arial"));						// lpszFac

	g_pFont2 = new CFont;
	g_pFont2->CreateFont(20,0,0,0,FW_BOLD,false,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_SWISS,
						_T("Arial"));

	g_pFont3 = new CFont;
	g_pFont3->CreateFont(15,0,0,0,FW_BOLD,false,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_SWISS,
						_T("Arial"));

	GetDlgItem(IDC_Results_STATIC)->SetWindowText(_T("��������������"));
	GetDlgItem(IDC_Results_STATIC)->SetFont(g_pFont1,false);

	GetDlgItem(IDC_ExtractData_BUTTON)->SetWindowText(_T("���������ȡ"));
	GetDlgItem(IDC_ExtractData_BUTTON)->SetFont(g_pFont2,false);

	GetDlgItem(IDC_DeviceInit_BUTTON)->SetWindowText(_T("�豸��ʼ��"));
	GetDlgItem(IDC_DeviceInit_BUTTON)->SetFont(g_pFont2,false);

	GetDlgItem(IDC_BeginTest_BUTTON)->SetWindowText(_T("������������"));
	GetDlgItem(IDC_BeginTest_BUTTON)->SetFont(g_pFont2,false);

	GetDlgItem(IDC_Calibration1_BUTTON)->SetWindowText(_T("��ʼ��У׼"));
	GetDlgItem(IDC_Calibration1_BUTTON)->SetFont(g_pFont3,false);

	GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("��ʾ��Ϣ"));
	GetDlgItem(IDC_STATIC1)->SetFont(g_pFont2,false);

	m_fSenGoVal_edit = ReadIniFileNum("Config.ini","TEST","SenPower");	//��ʼֵ
	UpdateData(false);

	m_Check1.SetCheck(1);		//Ĭ��Ϊѡ��״̬

	GetDlgItem(IDC_TestUser_STATIC)->SetWindowTextA(g_UserAccount);		//�û��˺�
	GetDlgItem(IDC_TestErName_STATIC)->SetWindowTextA(g_UserName);		//�û�����

	m_ctrlProgress.SetRange(0,100);	//���ý������ķ�Χ
	m_ctrlProgress.SetStep(10);		//���ý�������ÿһ��������
	m_ctrlProgress.SetPos(0);		//���ý������ĵ�ǰλ��
}


//�豸���ÿؼ���ʼ��
void CMy10GAPDTESTDlg::DeviceConfigControls()
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
	m_Amplitude_Combo.SetCurSel(4);

	//VOPȡֵ��ʽѡ��
	m_VopVal_Combo.InsertString(0,_T("Vbr * 0.9"));
	m_VopVal_Combo.InsertString(1,_T("Vbr - 2"));
	m_VopVal_Combo.InsertString(2,_T("Vbr - 3"));
	m_VopVal_Combo.InsertString(2,_T("Vbr - 4"));
	m_VopVal_Combo.SetCurSel(1);

	//�⹦�ʼƲ���ѡ��
	m_PowerWaveLen_Combo.InsertString(0,_T("850nm"));
	m_PowerWaveLen_Combo.InsertString(1,_T("980nm"));
	m_PowerWaveLen_Combo.InsertString(2,_T("1046nm"));
	m_PowerWaveLen_Combo.InsertString(3,_T("1310nm"));
	m_PowerWaveLen_Combo.InsertString(4,_T("1480nm"));
	m_PowerWaveLen_Combo.InsertString(5,_T("1550nm"));
	m_PowerWaveLen_Combo.InsertString(6,_T("1610nm"));
	m_PowerWaveLen_Combo.SetCurSel(3);

	//��˥��������ѡ��
	m_AttWaveLen_Combo.InsertString(0,_T("1310nm"));
	m_AttWaveLen_Combo.InsertString(1,_T("1550nm"));
	m_AttWaveLen_Combo.SetCurSel(0);
}	

//���ؼ���ʼ��
void CMy10GAPDTESTDlg::GridInit()
{
	//ͳ����Ϣ
	m_Method.GridCtrlInit(m_GridDS,6,2,RGB(255, 236, 139),32,73);//�Ա����г�ʼ��
	m_Method.WriteGridCtrOne(m_GridDS,0,0,_T("ͳ����Ŀ  "),RGB(0, 0, 0));
	m_Method.WriteGridCtrOne(m_GridDS,0,1,_T("ͳ��ֵ"),RGB(0, 0, 0));
	m_Method.WriteGridCtrOne(m_GridDS,1,0,_T("����������"),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridDS,2,0,_T("����Ʒ����"),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridDS,3,0,_T("��Ʒ����  "),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridDS,4,0,_T("��Ʒ��    "),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridDS,5,0,_T("��λUPH   "),RGB(100, 0, 0));

	for(int row = 0; row < 6; row++)
	{
		for(int col = 0; col < 2; col++)
		{
			if(row > 0)
			{
				m_Method.OnCellModel(m_GridDS,row,col,1,RGB(190,190,190));	//��Ԫ��ģʽ������ɫ��
				if(col > 0)
					m_Method.WriteGridCtrOne(m_GridDS,row,col,_T("0"),RGB(255, 0, 0));
			}	
			else
			{
				m_Method.OnCellModel(m_GridDS,row,col,1,RGB(176,226,255));	////��ͷ��Ԫ�� ��ɫ��ģʽ
			}
		}
	}

	//�����Ȳ��� �������
	m_Method.GridCtrlInit(m_GridFitt,10,2,RGB(255, 236, 139),24,72);//�Ա����г�ʼ��
	m_Method.WriteGridCtrOne(m_GridFitt,0,0,_T("�۲��⹦��ֵ"),RGB(0, 0, 0));
	m_Method.WriteGridCtrOne(m_GridFitt,0,1,_T("��Ӧ������"),RGB(0, 0, 0));

	for(int row = 0; row < 10; row++)		//��
	{
		for(int col = 0; col < 2; col++)	//��
		{
			if(row < 6)
			{
				if(row > 0)
				{
					m_Method.OnCellModel(m_GridFitt,row,col,1,RGB(180,205,205));	//��Ԫ��ģʽ������ɫ��
					m_Method.WriteGridCtrOne(m_GridFitt,row,col,_T(""),RGB(0, 0, 0));
				}
				else
				{
					m_Method.OnCellModel(m_GridFitt,row,col,1,RGB(176,226,255));	////��ͷ��Ԫ�� ��ɫ��ģʽ
				}
			}
			else
			{
				m_Method.OnCellModel(m_GridFitt,row,col,1,RGB(240,240,255));	//��Ԫ��ģʽ������ɫ��
				if(col > 0)
				{
					m_Method.WriteGridCtrOne(m_GridFitt,row,col,_T(""),RGB(0, 0, 0));
				}
			}
		}
	}
	m_Method.WriteGridCtrOne(m_GridFitt,7,0,_T("���ϵ��A��"),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridFitt,8,0,_T("���ϵ��B��"),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridFitt,9,0,_T("������ֵ ��"),RGB(100, 0, 0));

	//10G APD ���ղ��Խ�� ����
	m_Method.GridCtrlInit(m_GridTS,10,3,RGB(255, 236, 139),24,65);//�Ա����г�ʼ��
	m_Method.WriteGridCtrOne(m_GridTS,0,0,_T("������Ŀ"),RGB(0, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,0,1,_T("����״̬"),RGB(0, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,0,2,_T("���ֵ "),RGB(0, 0, 0));

	m_Method.WriteGridCtrOne(m_GridTS,2,0,_T("Icc���� "),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,2,1,_T(""),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,2,2,_T(""),RGB(100, 0, 0));

	m_Method.WriteGridCtrOne(m_GridTS,4,0,_T("Id����  "),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,4,1,_T(""),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,4,2,_T(""),RGB(100, 0, 0));

	m_Method.WriteGridCtrOne(m_GridTS,6,0,_T("VBR���� "),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,6,1,_T(""),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,6,2,_T(""),RGB(100, 0, 0));

	m_Method.WriteGridCtrOne(m_GridTS,8,0,_T("SEN���� "),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,8,1,_T(""),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,8,2,_T(""),RGB(100, 0, 0));

	for(int row = 0; row < 10; row++)		//��
	{
		for(int col = 0; col < 3; col++)	//��
		{
			if(row < 1)
			{
				m_Method.OnCellModel(m_GridTS,row,col,1,RGB(176,226,255));	////��ͷ��Ԫ�� ��ɫ��ģʽ
			}
			else
			{
				if(col < 1)
					m_Method.OnCellModel(m_GridTS,row,col,1,RGB(135,206,225));
				else
					m_Method.OnCellModel(m_GridTS,row,col,1,RGB(255, 236, 139));
			}

		}
	}
}

//Button�ؼ���ʼ������Button�ؼ������ػ�
void CMy10GAPDTESTDlg::ButtonInit()
{
	//�����������ȡ����ť�ؼ�
	GetDlgItem(IDC_ExtractData_BUTTON)->ModifyStyle(0,BS_OWNERDRAW,0);	//����ť�޸�ΪBS_OWNERDRAW���,����button�Ĳ����Ի�ģʽ
	m_ButtonExtractData.Attach(IDC_ExtractData_BUTTON,this);			//�󶨿ؼ�IDC_BUTTON1����CMyButton����Ӧ���غ���DrawItem()
	m_ButtonExtractData.SetDownColor(RGB(255,255,224));					//����Button Down�ı���ɫ
	m_ButtonExtractData.SetUpColor(InitColor);							//����Button Up�ı���ɫ
	m_ButtonExtractData.SetTextColor(TextColor);						//Button��������ɫ

	//���豸��ʼ������ť�ؼ�
	GetDlgItem(IDC_DeviceInit_BUTTON)->ModifyStyle(0,BS_OWNERDRAW,0);
	m_ButtonConfigInit.Attach(IDC_DeviceInit_BUTTON,this);
	m_ButtonConfigInit.SetDownColor(RGB(255,255,224));
	m_ButtonConfigInit.SetUpColor(InitColor);
	m_ButtonConfigInit.SetTextColor(TextColor);

	//�������������ԡ���ť�ؼ�
	GetDlgItem(IDC_BeginTest_BUTTON)->ModifyStyle(0,BS_OWNERDRAW,0);
	m_ButtonBeginTest.Attach(IDC_BeginTest_BUTTON,this);
	m_ButtonBeginTest.SetDownColor(RGB(255,255,224));
	m_ButtonBeginTest.SetUpColor(InitColor);
	m_ButtonBeginTest.SetTextColor(TextColor);

	//����У׼1����ť�ؼ�
	GetDlgItem(IDC_Calibration1_BUTTON)->ModifyStyle(0,BS_OWNERDRAW,0);
	m_ButtonCalibration1.Attach(IDC_Calibration1_BUTTON,this);
	m_ButtonCalibration1.SetDownColor(RGB(255,255,224));
	m_ButtonCalibration1.SetUpColor(InitColor);
	m_ButtonCalibration1.SetTextColor(TextColor);

}

//�����ű༭��
void CMy10GAPDTESTDlg::OnEnChangeWorkordernoEdit()
{
	UpdateData(true);
}

//���κű༭��
void CMy10GAPDTESTDlg::OnEnChangeBatchnoEdit()
{
	UpdateData(true);
}

//PD�ͺ� �༭��
void CMy10GAPDTESTDlg::OnEnChangePdmodelEdit()
{
	UpdateData(true);
}

//ϵ�кű༭��
void CMy10GAPDTESTDlg::OnEnChangeSnEdit()
{
	UpdateData(true);
}

//ʵ�ʹ���ֵ �༭��
void CMy10GAPDTESTDlg::OnEnChangePowervalEdit()
{
	UpdateData(true);
}

//��ȡ������ǰ���ô���
void CMy10GAPDTESTDlg::GetComNum()
{
	memset(ComNum, -1, 1024);
	int j = 0;

	CUIntArray ports;
	EnumerateSerialPorts(ports);
	
	for (int i=0; i<ports.GetSize(); i++)
	{		
		int COM = ports.ElementAt(i);

		if(COM != -1)
		{
			ComNum[j + 1] = COM;
			j += 2;
		}
	}
}

//���Ӹ����豸���ɹ�����TRUE��ʧ�ܷ���FALSE
BOOL CMy10GAPDTESTDlg::DeviceConnet()
{
	/*CString ErrStr = _T("");
	ErrStr.Format(_T("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d"),
		ComNum[0],ComNum[1],ComNum[2],ComNum[3],ComNum[4],ComNum[5],ComNum[6],ComNum[7],
		ComNum[8],ComNum[9],ComNum[10],ComNum[11],ComNum[12],ComNum[13]);
	MessageBox(ErrStr);*/
	

	if(m_AitVoa->OpenDevice(ComNum,sizeof(ComNum)/sizeof(int)) == FALSE)
	{
		::MessageBox(NULL, _T("��˥��������ʧ�ܣ������˳�����"), _T("���Ӵ���"), MB_OK);
		return FALSE;
	}

	if(m_PM7501->OpenDevice(ComNum,sizeof(ComNum)/sizeof(int)) == FALSE)
	{
		::MessageBox(NULL, _T("�⹦�ʼ�����ʧ�ܣ������˳�����"), _T("���Ӵ���"), MB_OK);
		return FALSE;
	}

	if(m_ApatVbr->OpenDevice(ComNum,sizeof(ComNum)/sizeof(int)) == FALSE)
	{
		::MessageBox(NULL, _T("VBR����������ʧ�ܣ������˳�����"), _T("���Ӵ���"), MB_OK);
		return FALSE;
	}

	if(m_Bert->OpenDevice(ComNum,sizeof(ComNum)/sizeof(int)) == FALSE)
	{
		::MessageBox(NULL, _T("����������ʧ�ܣ������˳�����"), _T("���Ӵ���"), MB_OK);
		return FALSE;
	}
	return TRUE;
}

//��ȡ��У׼��״̬
void CMy10GAPDTESTDlg::GetCalState()
{
	CString  CalState = ReadIniFileStr("Config.ini","CAL","CalState");
	if(CalState == _T("PASS"))
	{
		m_fTanP_edit = ReadIniFileNum("Config.ini","CAL","TanP");
		CalStatePass();		//У׼�ɹ�
		UpdateData(false);
	}
	
}

//���½��й�У׼
void CMy10GAPDTESTDlg::SetCalState()
{
	WriteIniFileStr("Config.ini","CAL","CalState","FAIL");
	GetDlgItem(IDC_Calibration1_BUTTON)->SetWindowText(_T("��ʼ��У׼"));
	g_Calibration = FALSE;
	m_ButtonCalibration1.SetUpColor(FailColor);
	GetDlgItem(IDC_Calibration1_BUTTON)->Invalidate();
}

//У׼�ɹ�
void CMy10GAPDTESTDlg::CalStatePass()
{
	GetDlgItem(IDC_Calibration1_BUTTON)->SetWindowText(_T("��У׼�ɹ�"));
	g_Calibration = TRUE;
	m_ButtonCalibration1.SetUpColor(PassColor);
	GetDlgItem(IDC_Calibration1_BUTTON)->Invalidate();

	WriteIniFileStr("Config.ini","CAL","CalState","PASS");
	WriteIniFileNum("Config.ini","CAL","TanP",m_fTanP_edit);
}

//У׼ʧ��
void CMy10GAPDTESTDlg::CalStateFail()
{
	GetDlgItem(IDC_Calibration1_BUTTON)->SetWindowText(_T("��У׼ʧ��"));
	g_Calibration = FALSE;
	m_ButtonCalibration1.SetUpColor(FailColor);
	GetDlgItem(IDC_Calibration1_BUTTON)->Invalidate();
}

//��ʼ��У׼
void CMy10GAPDTESTDlg::OnBnClickedCalibration1Button()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(g_DeviceInit == FALSE)
	{
		::MessageBox(NULL,_T("���Ƚ����豸��ʼ����"),_T("��ʾ��Ϣ"),MB_OK);
		return ;
	}

	if(g_Calibration == TRUE)
	{
		INT_PTR nRes = MessageBox(_T("���Ƿ�ȷ�����½���У׼��"), _T("��ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
		if (IDCANCEL == nRes)
		{
			return; 
		}
		SetCalState();
		return ;
	}

	GetDlgItem(IDC_Calibration1_BUTTON)->SetWindowText(_T("����У׼������"));

	if(SetPowerVal(m_fSenGoVal_edit) != 0)
	{
		MessageBox(_T("���ù⹦�ʼƵ� �⹦��ֵʧ�ܣ�"), _T("��ʾ��Ϣ"), MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	float fCalPowerVal = 0.0f;	//��ȡ���Ĺ⹦�ʼƵ�ǰ�Ĺ���ֵ

	if(m_PM7501->GetPower(fCalPowerVal) == FALSE)
	{
		MessageBox(_T("��ȡ�⹦�ʼƵ�ǰ����ֵʧ�ܣ�У׼ʧ�ܣ�"), _T("������Ϣ"), MB_OKCANCEL | MB_ICONQUESTION);
		CalStateFail();
		return ;
	}

	Calibration dlg;
	INT_PTR nResponse = dlg.DoModal();

	if(nResponse != 1)
	{
		MessageBox(_T("У׼ʧ�ܣ�"), _T("������Ϣ"), MB_OKCANCEL | MB_ICONQUESTION);
		CalStateFail();
		return;
	}
	m_fTanP_edit = fCalPowerVal - g_PowerVal;
	UpdateData(false);
	
	CalStatePass();		//У׼�ɹ�
}

//�����������ȡ�� ��ť��Ϣ����
void CMy10GAPDTESTDlg::OnBnClickedExtractdataButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(m_strWorkOrderNo_edit == _T(""))
	{
		MessageBox(_T("�����빤���ţ�"), _T("��ʾ��Ϣ"), MB_OK | MB_ICONEXCLAMATION);
		return ;
	}

	if(m_strBatchNo_edit == _T(""))
	{
		MessageBox(_T("���������κţ�"), _T("��ʾ��Ϣ"), MB_OK | MB_ICONEXCLAMATION);
		return ;
	}

	if(JudgeWorkOrderNo() == FALSE)
	{
		MessageBox(_T("�����Ų���ȷ�����������룡"), _T("��ʾ��Ϣ"), MB_OK | MB_ICONEXCLAMATION);
		return ;
	}

	if(JudgeBatchNo() == FALSE)
	{
		MessageBox(_T("���κŲ���ȷ�����������룡"), _T("��ʾ��Ϣ"), MB_OK | MB_ICONEXCLAMATION);
		return ;
	}

	GetDlgItem(IDC_ExtractData_BUTTON)->SetWindowTextA(_T("���������ȡ�С�����������"));
	//if(GetModelSpec() == FALSE)	//��ȡ���Թ��
	if(FALSE == GetProductTestSpec_TS())
	{
		//MessageBox(_T("��ȡ���Թ��ʧ�ܣ�"), _T("��ʾ��Ϣ"), MB_OK | MB_ICONEXCLAMATION);
		GetDlgItem(IDC_ExtractData_BUTTON)->SetWindowTextA(_T("���������ȡ"));
		return ;
	}

	g_Extractdata = TRUE;
	GetDlgItem(IDC_ExtractData_BUTTON)->SetWindowText(_T("�����ȡ�ɹ�"));
	m_ButtonExtractData.SetUpColor(PassColor);
	GetDlgItem(IDC_ExtractData_BUTTON)->Invalidate();
}

//���豸��ʼ������ť��Ϣ����
void CMy10GAPDTESTDlg::OnBnClickedDeviceinitButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(g_Extractdata == FALSE)
	{
		MessageBox(_T("�������δ��ȡ��������ȡ������ݣ�"), _T("��ʾ��Ϣ"), MB_OKCANCEL | MB_ICONQUESTION);
		//return ;
	}

	GetDlgItem(IDC_DeviceInit_BUTTON)->SetWindowText(_T("���ڳ�ʼ��������"));

	//��ʼ��������
	int IntChannel   = m_Channel_Combo.GetCurSel();		//������ͨ��ѡ��
	int IntType      = m_Type_Combo.GetCurSel();		//����������ѡ��
	int IntRate	     = m_Rate_Combo.GetCurSel();		//����������ѡ��
	int IntAmplitude = m_Amplitude_Combo.GetCurSel();	//�����Ƿ���ѡ��	
	
	if(m_Bert->InitDevice(IntChannel,IntType,IntRate,IntAmplitude) == FALSE)
	{
		MessageBox(_T("�����ǳ�ʼ��ʧ�ܣ���"), _T("��ʾ��Ϣ"), MB_OKCANCEL | MB_ICONQUESTION);
		goto FAILCODE;
	}

	//��ѯ�����ǵ�ǰ����
	int IntQueType      = 0;	//����������ѡ��
	int IntQueRate	    = 0;	//����������ѡ��
	int IntQueAmplitude = 0;	//�����Ƿ���ѡ��

	if(m_Bert->QueryDeviceConfig(IntChannel, IntQueType, IntQueRate, IntQueAmplitude) == FALSE)	//��ѯ����������ʧ��
	{
		MessageBox(_T("��ѯ�����ǳ�ʼ������ʧ�ܣ���"), _T("��ʾ��Ϣ"), MB_OKCANCEL | MB_ICONQUESTION);
		goto FAILCODE;
	}

	if(IntType != IntQueType || IntRate != IntQueRate || IntAmplitude != IntQueAmplitude)	//��ѯ���ĺ������õĲ���ͬ
	{
		MessageBox(_T("�����ǳ�ʼ���뵱ǰ���ò�һ�£���"), _T("��ʾ��Ϣ"), MB_OKCANCEL | MB_ICONQUESTION);
		goto FAILCODE;
	}


	//�⹦�ʳ�ʼ��
	int IntPowerWaveLen = m_PowerWaveLen_Combo.GetCurSel();	//�⹦�ʼƲ���ѡ��
	if(m_PM7501->SetWaveLength(IntPowerWaveLen) == FALSE)
	{
		MessageBox(_T("�⹦�ʼ����ò���ʧ�ܣ���"), _T("��ʾ��Ϣ"), MB_OKCANCEL | MB_ICONQUESTION);
		goto FAILCODE;
	}

	//��˥������ʼ��
	int IntAttWaveLen = m_AttWaveLen_Combo.GetCurSel();	//��˥����
	if(m_AitVoa->SetWaveLength(IntAttWaveLen) == FALSE)
	{
		MessageBox(_T("��˥�������ò���ʧ�ܣ���"), _T("��ʾ��Ϣ"), MB_OKCANCEL | MB_ICONQUESTION);
		goto FAILCODE;
	}

	if(SetPowerVal(m_fSenGoVal_edit) != 0)
	{
		MessageBox(_T("���ù⹦�ʼƵ� �⹦��ֵʧ�ܣ�"), _T("��ʾ��Ϣ"), MB_OKCANCEL | MB_ICONQUESTION);
		goto FAILCODE;
	}

	//VBR�����ǳ�ʼ��

	goto PASSCODE;

FAILCODE:
	g_DeviceInit = FALSE;
	GetDlgItem(IDC_DeviceInit_BUTTON)->SetWindowText(_T("��ʼ��ʧ��"));
	m_ButtonConfigInit.SetUpColor(FailColor);
	GetDlgItem(IDC_DeviceInit_BUTTON)->Invalidate();
	return ;

PASSCODE:
	g_DeviceInit = TRUE;
	GetDlgItem(IDC_DeviceInit_BUTTON)->SetWindowText(_T("��ʼ���ɹ�"));
	m_ButtonConfigInit.SetUpColor(PassColor);
	GetDlgItem(IDC_DeviceInit_BUTTON)->Invalidate();
	return ;
}

//�������������ԡ���ť��Ϣ����
void CMy10GAPDTESTDlg::OnBnClickedBegintestButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(g_Calibration == FALSE)
	{
		MessageBox(_T("��У׼δ�ɹ������Ƚ��й�У׼��"), _T("��ʾ��Ϣ"), MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(g_Extractdata == FALSE)
	{
		MessageBox(_T("�������δ��ȡ��������ȡ������ݣ�"), _T("��ʾ��Ϣ"), MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(g_DeviceInit == FALSE)
	{
		MessageBox(_T("�豸δ��ʼ������������豸��ʼ����"), _T("��ʾ��Ϣ"), MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(g_BeginTest == TRUE)
	{
		StopTest();		//ֹͣ��������
		return ;
	}

	BeginTest();		//������������
}

//ʱ���¼�߳�
UINT CMy10GAPDTESTDlg::ThreadTimeRecord(LPVOID lParam)
{
	CMy10GAPDTESTDlg * obj = ( CMy10GAPDTESTDlg* )lParam;
	CString Timestr = "";

	while(1)
	{
		if(g_ThreadExit == TRUE)
		{
			return 0;
		}

		if(g_deviceTestState == TRUE)
		{
			g_TimeEnd = GetTickCount();	//ȡ�õ�ǰϵͳ����ʱ��(ms)	
			Timestr.Format("%d",(g_TimeEnd - g_TimeBegin)/1000);
			obj->GetDlgItem(IDC_TESTTIME_STATIC)->SetWindowText(Timestr);
		}
		Sleep(1000);
	}

	return 0;
}

//�����߳�
UINT CMy10GAPDTESTDlg::ThreadTest( LPVOID lParam )
{
	CMy10GAPDTESTDlg * obj = ( CMy10GAPDTESTDlg* )lParam;

	while(1)
	{
		if(g_ThreadExit == TRUE)
		{
			return 0;
		}

		if(g_BeginTest == FALSE)				//����Ƿ��ѿ�����������
		{
			Sleep(100);
			continue;
		}

		obj->m_ctrlProgress.SetPos(0);			//���ý������ĵ�ǰλ��

		if(obj->m_strSn_edit == _T(""))			//����Ƿ�������SN��
		{
			obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("������SN��"));
			Sleep(1000);
			continue;
		}

		if(obj->m_strPDModel_edit == _T(""))	//����Ƿ�������PD�ͺ�
		{
			obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("������PD�ͺ�"));
			Sleep(200);
			continue;
		}
		obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T(""));
		
		if(obj->JudgeSN() == FALSE)				//�ж������SN���Ƿ���ȷ
		{
			obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("SN�Ų�����"));
			continue;
		}
		obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T(""));
			
		if(obj->JudgeSNtoWorkOrder() == FALSE)	//�ж������SN���Ƿ����ڵ�ǰ������
		{
			obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("SN�Ų����ڹ�����"));
			continue;
		}
		obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T(""));

		if(obj->JudgeSNtoBatch() == FALSE)		//�ж������SN���Ƿ����ڵ�ǰ���κ�
		{
			obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("SN�Ų��������κ�"));
			continue;
		}
		obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T(""));

		if ( BST_CHECKED == obj->IsDlgButtonChecked(IDC_CHECK1) )	//��תϵͳ�Ƿ��ѹ�ѡ�����ѹ�ѡ���鹤������
		{
			if(obj->JudgeStep(obj->m_strSn_edit) == FALSE)			//�жϸ������Ƿ����ڵ�ǰ���򣨲����
			{
				continue;
			}
		}

		obj->GridInit();	//��ʼ�����

		if(obj->GetTestInfoToWorkOrder() == FALSE)	//��ȡ��ǰ�����Ĳ�����Ϣ,���Ѳ�����������Ʒ�� �ȣ�
		{
			obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("ͳ����Ϣ��ȡʧ�ܣ�"));
			Sleep(500);
			continue;
		}


		obj->BeginDeviceTest();			//������ʼ����
		

		//obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("���ڽ���Icc���ԡ�����"));
		//if(obj->IccTest() == FALSE)		// Icc���� ��ȡIccֵ
		//{
		//	obj->TestResultsDeal();		//���Խ������
		//	continue;
		//}

		obj->m_ctrlProgress.SetPos(20);		//���ý������ĵ�ǰλ��

		obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("���ڽ���Vbr���ԡ�����"));
		if(g_IntModelState == 0)			//10G APD��������
		{
			if(obj->VbrTest() == FALSE)		// VBR���� ��ȡVbrֵ��Idֵ
			{
				obj->TestResultsDeal();		//���Խ������
				continue;
			}
		}
		else								//10G-PIN-TIA ����
		{
			if(obj->ApdVccOutPut(TRUE) == FALSE)//����APD��VCC��ѹ���
			{
				obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("����APD��VCC��ѹ���ʧ��"));
				obj->TestResultsDeal();		//���Խ������
				continue;
			}
		}

		obj->m_ctrlProgress.SetPos(40);	//���ý������ĵ�ǰλ��

		obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("���ڽ���Sen���ԡ�����"));
		if(obj->SenTest() == FALSE)		// Sen����
		{
			obj->TestResultsDeal();		//���Խ������
			continue;
		}

		obj->m_ctrlProgress.SetPos(90);	//���ý������ĵ�ǰλ��

		if(obj->SaveTestDataAccess() == FALSE)		//�������ݱ����ڱ���
		{
			obj->MessageBox(_T("������������ڱ���ʧ�ܣ���"), _T("������ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
			obj->TestFail();
			continue;
		}

		if(obj->SaveTestDataServer() == FALSE)		//�������ݱ����ڷ�����
		{
			obj->MessageBox(_T("������������ڷ�����ʧ�ܣ���"), _T("������ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
			obj->TestFail();
			continue;
		}

		obj->TestPass();					//�������Գɹ�

		obj->m_ctrlProgress.SetPos(100);	//���ý������ĵ�ǰλ��

		Sleep(500);
	}

	return 0;
}

//����Icc���ԣ���ȡIcc��ֵ�����޹������£�
BOOL CMy10GAPDTESTDlg::IccTest()
{
	if(m_AitVoa->SetAttVal(60.0f) == FALSE)	//�������ù�˥������˥��ֵΪ60dbm
	{
		m_Method.WriteGridCtrOne(m_GridTS,2,1,_T("FAIL"),RGB(0, 0, 0));
		m_Method.OnCellModel(m_GridTS,2,1,1,FailColor);	//��Ԫ����ɫ��ģʽ
		m_Method.WriteGridCtrOne(m_GridTS,2,2,_T("���ù�˥������˥��ֵʧ��"),RGB(100, 0, 0));
		TestFail();	
		return FALSE;
	}
	
	float fIccVal = 0.0f;	//����Icc���Ժ�õ���Iccֵ

	DWORD RetuVal = m_ApatVbr->IccTestResult(fIccVal);

	if(RetuVal == 0)
	{
		CString strIccVal = _T("");
		strIccVal.Format("%0.2f",fIccVal);
		m_Method.WriteGridCtrOne(m_GridTS,2,1,_T("PASS"),RGB(0, 0, 0));
		m_Method.OnCellModel(m_GridTS,2,1,1,PassColor);					//��Ԫ����ɫ��ģʽ
		m_Method.WriteGridCtrOne(m_GridTS,2,2,strIccVal,RGB(100, 0, 0));

		if(fIccVal < m_fIccMin_edit)						//��ȡ���� Icc С����С����ֵ
		{
			m_Method.OnCellModel(m_GridTS,3,1,1,FailColor);	//��Ԫ����ɫ��ģʽ
			g_FailType     = _T("IccС");	
			g_FailItemCode = 114;
			TestFail();										//��������ʧ��
			return FALSE;
		}
		if(fIccVal > m_fIccMax_edit)						//��ȡ���� Icc �����������ֵ
		{
			m_Method.OnCellModel(m_GridTS,3,2,1,FailColor);	//��Ԫ����ɫ��ģʽ
			g_FailType     = _T("Icc��");	
			g_FailItemCode = 115;
			TestFail();										//��������ʧ��
			return FALSE;
		}
		
		Data->m_IccVal = fIccVal;
		return TRUE;
	}
	else
	{
		m_Method.WriteGridCtrOne(m_GridTS,2,1,_T("FAIL"),RGB(0, 0, 0));
		m_Method.OnCellModel(m_GridTS,2,1,1,FailColor);	//��Ԫ����ɫ��ģʽ

		if(RetuVal == 1)
		{
			m_Method.WriteGridCtrOne(m_GridTS,2,2,_T("ѡ���5����ģʽʧ��"),RGB(100, 0, 0));
		}
		else if(RetuVal == 2)
		{
			m_Method.WriteGridCtrOne(m_GridTS,2,2,_T("����VAPD��ѹ���ʧ��"),RGB(100, 0, 0));
		}
		else if(RetuVal == 3)
		{
			m_Method.WriteGridCtrOne(m_GridTS,2,2,_T("����VCC��ѹ���ʧ��"),RGB(100, 0, 0));
		}
		else if(RetuVal == 4)
		{
			m_Method.WriteGridCtrOne(m_GridTS,2,2,_T("��ȡIccֵ��Vcc����ֵʧ��"),RGB(100, 0, 0));
		}
		else if(RetuVal == 5)
		{
			m_Method.WriteGridCtrOne(m_GridTS,2,2,_T("�ر�VAPD��ѹ���ʧ��"),RGB(100, 0, 0));
		}
		else
		{
			m_Method.WriteGridCtrOne(m_GridTS,2,2,_T("�ر�VCC��ѹ���ʧ��"),RGB(100, 0, 0));
		}
		
		TestFail();		//��������ʧ��
		return FALSE;
	}
}

//����VBR���ԣ���ȡ Id ֵ�� Vbr ֵ��Icc ֵ
BOOL CMy10GAPDTESTDlg::VbrTest()
{
	
	if(m_AitVoa->SetAttVal(60.0f) == FALSE)	//�������ù�˥������˥��ֵΪ60dbm
	{
		m_Method.WriteGridCtrOne(m_GridTS,2,1,_T("FAIL"),RGB(0, 0, 0));
		m_Method.OnCellModel(m_GridTS,2,1,1,FailColor);	//��Ԫ����ɫ��ģʽ
		m_Method.WriteGridCtrOne(m_GridTS,2,2,_T("���ù�˥������˥��ֵʧ��"),RGB(100, 0, 0));
		TestFail();	
		return FALSE;
	}

	float fVbrVal = 0.0f;	//VBR ���Գɹ���õ��� Vbr ֵ
	float fIdVal  = 0.0f;	//VBR ���Գɹ���õ��� Id ֵ
	float fIccVal = 0.0f;	//����Icc���Ժ�õ���Iccֵ

	DWORD RetuVal = m_ApatVbr->VbrTestResult(fVbrVal,fIdVal,fIccVal);	//Vbr����

	if(RetuVal != 0)
	{
		m_Method.WriteGridCtrOne(m_GridTS,2,1,_T("FAIL"),RGB(0, 0, 0));
		m_Method.OnCellModel(m_GridTS,2,1,1,FailColor);	//��Ԫ����ɫ��ģʽ

		m_Method.WriteGridCtrOne(m_GridTS,4,1,_T("FAIL"),RGB(0, 0, 0));
		m_Method.OnCellModel(m_GridTS,4,1,1,FailColor);	//��Ԫ����ɫ��ģʽ

		m_Method.WriteGridCtrOne(m_GridTS,6,1,_T("FAIL"),RGB(0, 0, 0));
		m_Method.OnCellModel(m_GridTS,6,1,1,FailColor);	//��Ԫ����ɫ��ģʽ

		if(RetuVal == 1)
		{
			m_Method.WriteGridCtrOne(m_GridTS,4,2,_T("ѡ���Vbrģʽʧ��"),RGB(0, 0, 0));
		}
		else if(RetuVal == 2)
		{
			m_Method.WriteGridCtrOne(m_GridTS,4,2,_T("����VAPD��ѹ���ʧ"),RGB(0, 0, 0));
		}
		else if(RetuVal == 3)
		{
			m_Method.WriteGridCtrOne(m_GridTS,4,2,_T("����VCC��ѹ���ʧ��"),RGB(0, 0, 0));
		}
		else if(RetuVal == 4)
		{
			m_Method.WriteGridCtrOne(m_GridTS,4,2,_T("����������VBR���Խ���ʧ��"),RGB(0, 0, 0));
		}
		else if(RetuVal == 5)
		{
			m_Method.WriteGridCtrOne(m_GridTS,4,2,_T("���� VBR ����ʧ��"),RGB(0, 0, 0));
		}
		else if(RetuVal == 6)
		{
			m_Method.WriteGridCtrOne(m_GridTS,4,2,_T("��ȡVbrֵ��Vbr��ѹֵ��ʧ��"),RGB(0, 0, 0));
		}
		else
		{
			m_Method.WriteGridCtrOne(m_GridTS,4,2,_T("��ȡIdֵ��VAPD����ֵ��ʧ��"),RGB(0, 0, 0));
		}
		return FALSE;
	}

	CString strVal = _T("");

	strVal.Format("%0.2f",fIccVal);
	m_Method.WriteGridCtrOne(m_GridTS,2,1,_T("PASS"),RGB(0, 0, 0));
	m_Method.OnCellModel(m_GridTS,2,1,1,PassColor);					//��Ԫ����ɫ��ģʽ
	m_Method.WriteGridCtrOne(m_GridTS,2,2,strVal,RGB(100, 0, 0));

	strVal.Format("%0.2f",fIdVal);
	m_Method.WriteGridCtrOne(m_GridTS,4,1,_T("PASS"),RGB(0, 0, 0));
	m_Method.OnCellModel(m_GridTS,4,1,1,PassColor);					//��Ԫ����ɫ��ģʽ
	m_Method.WriteGridCtrOne(m_GridTS,4,2,strVal,RGB(100, 0, 0));

	strVal.Format("%0.2f",fVbrVal);
	m_Method.WriteGridCtrOne(m_GridTS,6,1,_T("PASS"),RGB(0, 0, 0));
	m_Method.OnCellModel(m_GridTS,6,1,1,PassColor);					//��Ԫ����ɫ��ģʽ
	m_Method.WriteGridCtrOne(m_GridTS,6,2,strVal,RGB(100, 0, 0));


	if(fIccVal < m_fIccMin_edit)						//��ȡ���� Icc С����С����ֵ
	{
		m_Method.OnCellModel(m_GridTS,3,1,1,FailColor);	//��Ԫ����ɫ��ģʽ
		g_FailType     = _T("IccС");	
		g_FailItemCode = 114;
		return FALSE;
	}
	if(fIccVal > m_fIccMax_edit)						//��ȡ���� Icc �����������ֵ
	{
		m_Method.OnCellModel(m_GridTS,3,2,1,FailColor);	//��Ԫ����ɫ��ģʽ
		g_FailType     = _T("Icc��");	
		g_FailItemCode = 115;
		return FALSE;
	}

	if(fIdVal < m_fIdMin_edit)	//��ȡ���� Id С����С����ֵ
	{
		m_Method.OnCellModel(m_GridTS,5,1,1,FailColor);		//��Ԫ����ɫ��ģʽ
		g_FailType     = _T("IdС");	
		g_FailItemCode = 116;
		return FALSE;
	}
	if(fIdVal > m_fIdMax_edit)	//��ȡ���� ID �����������ֵ
	{
		m_Method.OnCellModel(m_GridTS,5,2,1,FailColor);		//��Ԫ����ɫ��ģʽ
		g_FailType     = _T("Id��");	
		g_FailItemCode = 117;
		return FALSE;
	}

	if(fVbrVal < m_fVbrMin_edit)	//��ȡ���� Vbr С����С����ֵ
	{
		m_Method.OnCellModel(m_GridTS,7,1,1,FailColor);		//��Ԫ����ɫ��ģʽ
		g_FailType     = _T("VBRƫС");	
		g_FailItemCode = 112;
		return FALSE;
	}
	if(fVbrVal > m_fVbrMax_edit)	//��ȡ���� Vbr �����������ֵ
	{
		m_Method.OnCellModel(m_GridTS,7,2,1,FailColor);		//��Ԫ����ɫ��ģʽ
		g_FailType     = _T("VBRƫ��");	
		g_FailItemCode = 113;
		return FALSE;
	}

	Data->m_IccVal = fIccVal;	//Icc
	Data->m_IdVal	= fIdVal;	//Idֵ
	Data->m_VbrVal	= fVbrVal;	//Vbrֵ
	Data->m_VbrTestPass = 1;	//Vbr���Խ�������Գɹ���

	return TRUE;
}

//�����Ȳ��ԣ��ɹ����� TRUE�� ʧ�ܷ��� FALSE
BOOL CMy10GAPDTESTDlg::SenTest()
{
	float x_Data[10] = {0};		//�۲��x��
	float y_Data[10] = {0};		//�۲��y��
	float Coefficient[10] = {0};//��ϼ���ĵ���ϵ�����ֱ��� a��b��c���ȵ�

	CString strTemp = _T("");

	if(ObservationPoints(x_Data, y_Data, 5) == FALSE)//���� 5 ���۲�� 
	{
		return FALSE;
	}

	m_ctrlProgress.SetPos(70);			

	if(FittTest(x_Data, y_Data, Coefficient, 1) != 0)//�Թ۲�������ϼ��� 
	{
		return FALSE;
	}

	m_ctrlProgress.SetPos(80);		

	strTemp.Format("%f",Coefficient[0]);
	m_Method.WriteGridCtrOne(m_GridFitt, 7, 1, strTemp, RGB(100, 0, 0));//����ϵ��a�������

	strTemp.Format("%f",Coefficient[1]);
	m_Method.WriteGridCtrOne(m_GridFitt, 8, 1, strTemp, RGB(100, 0, 0));//����ϵ��b�������

	float fSenVal = 0.0f;	//��ʽ�����õ��������� 
	float fBerVal = 0.0f;	//ѡ��������ʾ���ʽת����õ���ֵ

	fBerVal = (float)log10(-log10(atof(m_strVbrVal_edit)));
	//int IntBer = m_VbrVal_Combo.GetCurSel();	//��ȡѡ��������ʹ��
	//if(IntBer == 0)	//ѡȡ��������Ϊ10E-3
	//{
	//	fBerVal = (float)log10(-log10(1.0E-3));
	//}
	//else if(IntBer == 1)	//ѡȡ��������Ϊ10E-10
	//{
	//	fBerVal = (float)log10(-log10(1.0E-10));
	//}
	//else
	//{
	//	fBerVal = (float)log10(-log10(1.0E-12));
	//}

	fSenVal = ((fBerVal - Coefficient[1]) / Coefficient[0]) + m_CompenVal_edit;	//�Ӳ���ֵ

	Data->m_SenVal = fSenVal;	//������ֵ

	strTemp.Format("%f",fSenVal);
	m_Method.WriteGridCtrOne(m_GridFitt, 9, 1, strTemp, RGB(100, 0, 0));

	m_Method.WriteGridCtrOne(m_GridTS,8,1,_T("PASS"),RGB(0, 0, 0));
	m_Method.OnCellModel(m_GridTS,8,1,1,PassColor);	//��Ԫ����ɫ��ģʽ
	m_Method.WriteGridCtrOne(m_GridTS,8,2,strTemp,RGB(0, 0, 0));

	if(fSenVal < m_fSenMin_edit)	//������С����С����ֵ
	{
		m_Method.OnCellModel(m_GridTS,9,1,1,FailColor);		//��Ԫ����ɫ��ģʽ
		g_FailType     = _T("SENС");	
		g_FailItemCode = 107;
		return FALSE;
	}

	if(fSenVal > m_fSenMax_edit)	//�����ȴ����������ֵ
	{
		m_Method.OnCellModel(m_GridTS,9,2,1,FailColor);		//��Ԫ����ɫ��ģʽ
		g_FailType     = _T("SEN��");	
		g_FailItemCode = 108;
		return FALSE;
	}

	Data->m_SenTestPass = 1;		//�����Ȳ��Խ�������Գɹ���

	return TRUE;
}

//ͨ�����ڹ�˥������˥��ֵ�������ù⹦�ʼƵ� �⹦��ֵ(�����Ľ��չ� = ����ֵ - TanP)
DWORD CMy10GAPDTESTDlg::SetPowerVal(float PowerVal)
{
	float fPowerValBef = 0.0f;	//�ڵ��ڹ�˥����ǰ ��ȡ���Ĺ⹦��ֵ
	float fAttValBef   = 0.0f;	//�ڵ��ڹ�˥����ǰ ��ȡ���Ĺ�˥��ֵֵ
	int IntWaveLen = 0;			//��ǰ��˥�����Ĳ���
	float AttValSetp = 0.0f;	//˥�����仯�Ĳ���

	for(int i = 0; i < 10; i++)
	{
		if(m_PM7501->GetPower(fPowerValBef) == FALSE)	//��ȡ�⹦��ֵ
		{
			return 1;
		}
		
		if(m_AitVoa->GetWaveLengthAtt(IntWaveLen,fAttValBef) == FALSE)	//��ȡ��˥��ֵ
		{
			return 2;
		}

		if(fPowerValBef < PowerVal + 0.15 && fPowerValBef > PowerVal - 0.15)
		{
			break;
		}

		if(fabs(PowerVal - fPowerValBef) > 20)		//��ǰ�⹦��ֵ��Ŀ��⹦��ֵ��ֵ����20ʱ����Ϊ20
		{
			AttValSetp = 20.0f;
		}
		else if(fabs(PowerVal - fPowerValBef) > 10)	//��ǰ�⹦��ֵ��Ŀ��⹦��ֵ��ֵ����10ʱ����Ϊ10
		{
			AttValSetp = 10.0f;
		}
		else if(fabs(PowerVal - fPowerValBef) > 5)	//��ǰ�⹦��ֵ��Ŀ��⹦��ֵ��ֵ����5ʱ����Ϊ5
		{
			AttValSetp = 5.0f;
		}
		else if(fabs(PowerVal - fPowerValBef) > 1)	//��ǰ�⹦��ֵ��Ŀ��⹦��ֵ��ֵ����1ʱ����Ϊ1
		{
			AttValSetp = 1.0f;
		}
		else if(fabs(PowerVal - fPowerValBef) > 0.5)	//����Ϊ0.5
		{
			AttValSetp = 0.50f;
		}
		else if(fabs(PowerVal - fPowerValBef) > 0.2)	//����Ϊ0.2
		{
			AttValSetp = 0.20f;
		}
		else
		{
			AttValSetp = 0.10f;
		}
		
		if(fPowerValBef < PowerVal) //��ǰ�⹦��ֵС��Ŀ��⹦��ֵ�����С˥��ֵ
		{
			if(m_AitVoa->SetAttVal(fAttValBef - AttValSetp) == FALSE)
			{
				return 3;
			}
		}

		if(fPowerValBef > PowerVal) //��ǰ�⹦��ֵ����Ŀ��⹦��ֵ��������˥��ֵ
		{
			if(m_AitVoa->SetAttVal(fAttValBef + AttValSetp) == FALSE)
			{
				return 3;
			}
		}
		Sleep(200);
	}

	return 0;
}

//��ȡ��ǰ�⹦���µ�ͬ��ʧ��״̬�������������ɹ����� TRUE 
BOOL CMy10GAPDTESTDlg::GetBitErrState(int Channel, int & Sync, INT64 & BitErrNum)
{
	double dBer			 = 0.0;	//��ѯ�ɹ���õ���������

	if(m_Bert->BeginBitErrTest(Channel) == FALSE)	//�����������
	{
		MessageBox(_T("�����������ʧ�ܣ�"));
		return FALSE;	
	}
	Sleep(1000);	//�������ʱ��
	if(m_Bert->QueryBitErrState(Channel, Sync, BitErrNum, dBer) == FALSE)//��ѯ��������
	{
		MessageBox(_T("��ѯ��������ʧ�ܣ�"));
		return FALSE;
	}
	
	if(m_Bert->StopBitErrTest(Channel) == FALSE)		//ֹͣ������
	{
		MessageBox(_T("ֹͣ������ʧ�ܣ�"));
		return FALSE;
	}
	return TRUE;
}

//��ȡ��ǰ��Ʒ�����Ȳ��Թ⹦����ʼֵ,�ɹ����� TRUE
BOOL CMy10GAPDTESTDlg::GetSenTestBeginPowVal(float & fBeginPowVal)
{
	int IntChannel   = m_Channel_Combo.GetCurSel();		//������ͨ��ѡ��
	int IntSync          = 0;	//��ѯ�ɹ���õ���ͬ����ʧ��״̬��Ϊ0ʱ��ʾͬ����1��ʾʧ��,2��ʾ������
	INT64 Int64BitErrNum = 0;	//��ѯ�ɹ���õ���������

	if(SetPowerVal(m_fSenGoVal_edit + m_fTanP_edit) != 0)// �����������չ⹦�ʵ�Ŀ��ֵ
	{
		MessageBox(_T("�����������չ⹦�ʵ�Ŀ��ֵʧ�ܣ�"));
		return FALSE;
	}
	if(FALSE == GetBitErrState(IntChannel, IntSync,Int64BitErrNum))
	{
		return FALSE;
	}

	if(IntSync == 1)				//ʧ��״̬
	{
		for(int i = 1; i < 6; i++)
		{
			if(SetPowerVal(m_fSenGoVal_edit + i * m_PowStep1 + m_fTanP_edit) != 0)// �����������չ⹦�ʵ�Ŀ��ֵ
			{
				MessageBox(_T("�����������չ⹦�ʵ�Ŀ��ֵʧ��1��"));
				return FALSE;
			}
			if(FALSE == GetBitErrState(IntChannel, IntSync,Int64BitErrNum))
			{
				return FALSE;
			}

			if(IntSync != 1 && Int64BitErrNum > 0)
			{
				fBeginPowVal = m_fSenGoVal_edit + i * m_PowStep1;
				return TRUE;	
			}
		}
		return FALSE;//�Ҳ�����ʼ��
	}
	else if(Int64BitErrNum == 0)	//��ʧ��״̬��������Ϊ0
	{
		int i = 1;
		while(1)
		{
			if(SetPowerVal(m_fSenGoVal_edit - i * m_PowStep2 + m_fTanP_edit) != 0)// �����������չ⹦�ʵ�Ŀ��ֵ
			{
				MessageBox(_T("�����������չ⹦�ʵ�Ŀ��ֵʧ��2��"));
				return FALSE;
			}
			if(FALSE == GetBitErrState(IntChannel, IntSync,Int64BitErrNum))
			{
				return FALSE;
			}

			if(IntSync == 1)	//ʧ��
			{
				return FALSE;
			}

			if(Int64BitErrNum > 0)	//���������
			{
				fBeginPowVal = m_fSenGoVal_edit - i * m_PowStep1;
				return TRUE;
			}
		}
	}

	fBeginPowVal = m_fSenGoVal_edit;
	return TRUE;
}

//��ȡ�۲��,�ɹ�����TRUE��ʧ�ܷ���FALSE
BOOL CMy10GAPDTESTDlg::ObservationPoints(float x_Data[], float y_Data[], int IntNum)
{
	int IntChannel   = m_Channel_Combo.GetCurSel();		//������ͨ��ѡ��

	//�����ҵ� 5 ���۲��
	int IntSync          = 0;	//��ѯ�ɹ���õ���ͬ����ʧ��״̬��Ϊ0ʱ��ʾͬ����1��ʾʧ��,2��ʾ������
	INT64 Int64BitErrNum = 0;	//��ѯ�ɹ���õ���������
	double dBer			 = 0.0;	//��ѯ�ɹ���õ���������

	if(FALSE == GetSenTestBeginPowVal(m_SenTestBeginPowVal))//���ҵ�ǰ��Ʒ�����Ȳ��Թ⹦����ʼֵ
	{
		m_Method.WriteGridCtrOne(m_GridTS,8,1,_T("FAIL"),RGB(0, 0, 0));
		m_Method.OnCellModel(m_GridTS,8,1,1,FailColor);	//��Ԫ����ɫ��ģʽ
		m_Method.WriteGridCtrOne(m_GridTS,8,2,_T("��ȡ�����Ȳ�����ʼ�⹦��ֵʧ��"),RGB(0, 0, 0));
		return FALSE;
	}

	for(int i = 0; i < IntNum; i++)
	{
		if(SetPowerVal(m_SenTestBeginPowVal - m_fSenStep_edit * i + m_fTanP_edit) != 0)// �����������չ⹦�ʵ�Ŀ��ֵ
		{
			m_Method.WriteGridCtrOne(m_GridTS,8,1,_T("FAIL"),RGB(0, 0, 0));
			m_Method.OnCellModel(m_GridTS,8,1,1,FailColor);	//��Ԫ����ɫ��ģʽ
			m_Method.WriteGridCtrOne(m_GridTS,8,2,_T("�����������չ⹦�ʵ�Ŀ��ֵʧ��"),RGB(0, 0, 0));
			return FALSE;
		}

		Sleep(100);

		float fPowerVal = 0.0f;	//���ں��ȡ���Ĺ⹦��ֵ
		if(m_PM7501->GetPower(fPowerVal) == FALSE)	//��ȡ�⹦��ֵ
		{
			m_Method.WriteGridCtrOne(m_GridTS,8,1,_T("FAIL"),RGB(0, 0, 0));
			m_Method.OnCellModel(m_GridTS,8,1,1,FailColor);	//��Ԫ����ɫ��ģʽ
			m_Method.WriteGridCtrOne(m_GridTS,8,2,_T("��ȡ�⹦��ֵʧ��"),RGB(0, 0, 0));
			return FALSE;
		}

		CString strTemp = _T("");
		strTemp.Format(_T("%0.2f"),fPowerVal - m_fTanP_edit);
		m_Method.WriteGridCtrOne(m_GridFitt, i + 1,0, strTemp, RGB(100, 0, 0));
		x_Data[i] = fPowerVal - m_fTanP_edit;

		if(m_Bert->BeginBitErrTest(IntChannel) == FALSE)	//�����������
		{
			m_Method.WriteGridCtrOne(m_GridTS,8,1,_T("FAIL"),RGB(0, 0, 0));
			m_Method.OnCellModel(m_GridTS,8,1,1,FailColor);	//��Ԫ����ɫ��ģʽ
			m_Method.WriteGridCtrOne(m_GridTS,8,2,_T("�����������ʧ��"),RGB(0, 0, 0));
			return FALSE;	
		}

		Sleep(m_SenTime_edit * 1000);	//�������ʱ��

		if(m_Bert->QueryBitErrState(IntChannel, IntSync, Int64BitErrNum, dBer) == FALSE)//��ѯ��������
		{
			m_Method.WriteGridCtrOne(m_GridTS,8,1,_T("FAIL"),RGB(0, 0, 0));
			m_Method.OnCellModel(m_GridTS,8,1,1,FailColor);	//��Ԫ����ɫ��ģʽ
			m_Method.WriteGridCtrOne(m_GridTS,8,2,_T("��ѯ��������ʧ��"),RGB(0, 0, 0));
			return FALSE;
		}

		if(IntSync == 1)		//ʧ��״̬������Ϊ0
		{
			m_Method.WriteGridCtrOne(m_GridTS,8,1,_T("FAIL"),RGB(0, 0, 0));
			m_Method.OnCellModel(m_GridTS,8,1,1,FailColor);	
			m_Method.WriteGridCtrOne(m_GridTS,8,2,_T("�����Ǵ���ʧ��״̬"),RGB(0, 0, 0));
			return FALSE;
		}
		
		if(m_Bert->StopBitErrTest(IntChannel) == FALSE)		//ֹͣ������
		{
			m_Method.WriteGridCtrOne(m_GridTS,8,1,_T("FAIL"),RGB(0, 0, 0));
			m_Method.OnCellModel(m_GridTS,8,1,1,FailColor);	//��Ԫ����ɫ��ģʽ
			m_Method.WriteGridCtrOne(m_GridTS,8,2,_T("ֹͣ������ʧ��"),RGB(0, 0, 0));
			return FALSE;
		}
		
		strTemp.Format(_T("%0.8lf"),dBer);
		m_Method.WriteGridCtrOne(m_GridFitt, i + 1,1, strTemp, RGB(100, 0, 0));

		y_Data[i] = (float)log10(-log10(dBer));
	}

	return TRUE;
}

//���ݹ۲�����ݣ�����ֱ����ϼ���
DWORD CMy10GAPDTESTDlg::FittTest(float x_Data[], float y_Data[], float Coefficient[], int Poly_n)
{
	std::vector<double> x_data, y_data; 
	x_data.resize(5);  
	y_data.resize(5);

	for(int i = 0; i < 5; i++)
	{
		x_data[i] = x_Data[i];
		y_data[i] = y_Data[i];
	}

	float C[2] = {0};
	czy::Fit fit;  

    fit.polyfit(x_data, y_data, Poly_n, true);  
    for (int i=0; i< (int)fit.getFactorSize(); i++)  
	{
		C[i] = (float)fit.getFactor(i);
	}

	Coefficient[0] = C[1];	//ϵ�� a
	Coefficient[1] = C[0];	//ϵ�� b

	return 0;
}

//�жϹ������Ƿ���ڣ������򷵻�TRUE
BOOL CMy10GAPDTESTDlg::JudgeWorkOrderNo()
{
	CString strCmd = _T("");	//Sql����ַ���
	strCmd.Format(_T("select count(*) as Number from Manufacture_info where Manufacture_ID = '%s'"),
				  m_strWorkOrderNo_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//
	{
		MessageBox(_T("���ݿ⹤���Ų�ѯʧ�ܣ���"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	CString strNumber = g_SqlCon->Result("Number");

	int IntNumber = _ttoi(strNumber);

	if(IntNumber == 0)
		return FALSE;

	return TRUE;
}

//�ж����κ��Ƿ����(ȷ�Ϲ����Ŵ��ں�)�������򷵻�TRUE
BOOL CMy10GAPDTESTDlg::JudgeBatchNo()
{
	CString strCmd = _T("");	//Sql����ַ���
	strCmd.Format(_T("select count(*) as Number from dbo.testdata where Manufacture_ID = '%s' AND Batch_ID = %.0f"),
				  m_strWorkOrderNo_edit,atof(m_strBatchNo_edit));
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//
	{
		MessageBox(_T("���ݿ����κŲ�ѯʧ�ܣ���"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	CString strNumber = g_SqlCon->Result("Number");

	int IntNumber = _ttoi(strNumber);

	if(IntNumber == 0)
		return FALSE;

	return TRUE;
}

//�ж������SN���Ƿ���ȷ�������򷵻�TRUE
BOOL CMy10GAPDTESTDlg::JudgeSN()
{
	CString strCmd = _T("");	//Sql����ַ���
	strCmd.Format(_T("select count(*) as Number from dbo.testdata where Serial_No = '%s'"),m_strSn_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//
	{
		//MessageBox(_T("���ݿ����κŲ�ѯʧ�ܣ���"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	CString strNumber = g_SqlCon->Result("Number");

	int IntNumber = _ttoi(strNumber);

	if(IntNumber == 0)
	{
		MessageBox(_T("SN�Ų����ڣ����������룡��"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
		StopTest();		//ֹͣ����
		return FALSE;
	}

	return TRUE;
}

//�ж�SN���Ƿ����ڸù�����(ȷ�Ϲ����Ŵ��ں�)�������򷵻�TRUE
BOOL CMy10GAPDTESTDlg::JudgeSNtoWorkOrder()
{
	CString strCmd = _T("");	//Sql����ַ���
	strCmd.Format(_T("select count(*) as Number from dbo.testdata where Manufacture_ID = '%s' AND Serial_No = '%s'"),
		m_strWorkOrderNo_edit,m_strSn_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//��
	{
		//MessageBox(_T("���ݿ����κŲ�ѯʧ�ܣ���"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	CString strNumber = g_SqlCon->Result("Number");

	int IntNumber = _ttoi(strNumber);

	if(IntNumber == 0)
	{
		MessageBox(_T("��SN�Ų����ڵ�ǰ�����ţ����������빤���Ż� SN �ţ���"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
		StopTest();		//ֹͣ����

		g_Extractdata = FALSE;
		GetDlgItem(IDC_ExtractData_BUTTON)->SetWindowText(_T("���������ȡ"));
		m_ButtonExtractData.SetUpColor(FailColor);
		GetDlgItem(IDC_ExtractData_BUTTON)->Invalidate();
		return FALSE;
	}

	return TRUE;
}

//�ж�SN���Ƿ����ڸ����κ�(ȷ�Ϲ����š����κŴ��ں�)�������򷵻�TRUE
BOOL CMy10GAPDTESTDlg::JudgeSNtoBatch()
{
	CString strCmd = _T("");	//Sql����ַ���
	strCmd.Format(_T("select count(*) as Number from dbo.testdata where Manufacture_ID = '%s' AND Batch_ID = %.0f\
		AND Serial_No = '%s'"),m_strWorkOrderNo_edit,atof(m_strBatchNo_edit),m_strSn_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//
	{
		//MessageBox(_T("���ݿ����κŲ�ѯʧ�ܣ���"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	CString strNumber = g_SqlCon->Result("Number");

	int IntNumber = _ttoi(strNumber);

	if(IntNumber == 0)
	{
		MessageBox(_T("��SN�Ų����ڵ�ǰ���κţ��������������κţ���"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
		StopTest();		//ֹͣ����
		g_Extractdata = FALSE;
		GetDlgItem(IDC_ExtractData_BUTTON)->SetWindowText(_T("���������ȡ"));
		m_ButtonExtractData.SetUpColor(FailColor);
		GetDlgItem(IDC_ExtractData_BUTTON)->Invalidate();
		return FALSE;
	}

	return TRUE;
}

//��ȡ���� ����Ĺ����������κ��µ� SN ��,�ɹ����� TRUE
BOOL CMy10GAPDTESTDlg::GetBatchToSN(CString Serial_No[])
{
	CString strCmd = _T("");	//Sql����ַ���
	strCmd.Format(_T("select Serial_No from dbo.testdata where Manufacture_ID = '%s' AND Batch_ID = '%0.f'"),
		m_strWorkOrderNo_edit,atof(m_strBatchNo_edit));
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//
	{
		//MessageBox(_T("���ݿ����κŲ�ѯʧ�ܣ���"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	int i = 0;
	while( !g_SqlCon->Result.IsEOF() )
	{
		Serial_No[i++] = g_SqlCon->Result("Serial_No");

		g_SqlCon->Result.MoveToNext();
	}

	return TRUE;
}

//��ȡ���������ͺŵĲ��Թ�� �ɹ����� TRUE
BOOL CMy10GAPDTESTDlg::GetModelSpec()
{
	CString strCmd = _T("");	//Sql����ַ���
	strCmd.Format(_T("select model from Manufacture_info where Manufacture_ID = '%s'"),
		m_strWorkOrderNo_edit);			
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//��ȡ�ͺ�
	{
		MessageBox(_T("���ݿ��ͺŲ�ѯʧ�ܣ���"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}
	m_strModel_edit = g_SqlCon->Result("model");	//��ȡ�ͺ�
	UpdateData(false);

////////////////////////////////////////��ȡ���//////////////////////////////////////////////////////////

	strCmd.Format(_T("select Icc_Min,Icc_Max,Id_Min,Id_Max,Vbr_Min,Vbr_Max,SEN_Min,SEN_Max,Wavelength,Rx_inputWL,\
		SEN_Time,SEN_BER,SEN_DATA_RATE,SEN_Data_Pattern,SEN_Work_MODE,SEN_VOP,SEN_PM_STEP,Siganl_Range from \
		dbo.testsetup where model = '%s'"),m_strModel_edit);			
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//��ȡ���
	{
		MessageBox(_T("���ݿ���������ѯʧ�ܣ���"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	CString strTemp = _T(""); 

	m_fIccMin_edit = (float)(g_SqlCon->Result.Convertor("Icc_Min").ToDouble());
	m_fIccMax_edit = (float)(g_SqlCon->Result.Convertor("Icc_Max").ToDouble());

	m_fIdMin_edit = (float)(g_SqlCon->Result.Convertor("Id_Min").ToDouble());
	m_fIdMax_edit = (float)(g_SqlCon->Result.Convertor("Id_Max").ToDouble());

	m_fVbrMin_edit = (float)(g_SqlCon->Result.Convertor("Vbr_Min").ToDouble());
	m_fVbrMax_edit = (float)(g_SqlCon->Result.Convertor("Vbr_Max").ToDouble());

	m_fSenMin_edit = (float)(g_SqlCon->Result.Convertor("SEN_Min").ToDouble());
	m_fSenMax_edit = (float)(g_SqlCon->Result.Convertor("SEN_Max").ToDouble());

	m_SenTime_edit = g_SqlCon->Result.Convertor("SEN_Time").ToInteger();

	int IntWaveLen = g_SqlCon->Result.Convertor("Rx_inputWL").ToInteger();	//���ղ���
	if(IntWaveLen == 1310)
	{
		m_AttWaveLen_Combo.SetCurSel(0);		//��˥����
		m_PowerWaveLen_Combo.SetCurSel(3);		//�⹦�ʼ�
	}
	else if(IntWaveLen == 1550)
	{
		m_AttWaveLen_Combo.SetCurSel(1);
		m_PowerWaveLen_Combo.SetCurSel(5);
	}
	else
	{
		MessageBox(_T("�������ô��󣬽�Ĭ��ѡ�� 1310 �Ĳ�������"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
	}

	int IntRange = 0;
	IntRange = (int)g_SqlCon->Result.Convertor("Siganl_Range").ToDouble();	//�����Ƿ���ѡ��
	m_Amplitude_Combo.SetCurSel(IntRange);

	m_strVbrVal_edit = g_SqlCon->Result("SEN_BER");		//������ѡ��
	UpdateData(FALSE);

	strTemp = g_SqlCon->Result("SEN_DATA_RATE");	//����ѡ��
	int IntRate = _ttoi(strTemp);

	if(IntRate < 8)
	{
		IntRate = 8;
		MessageBox(_T("�������ô��󣬽�Ĭ��ѡ�� 9.95G �����ʣ���"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
	}
	m_Rate_Combo.SetCurSel(IntRate - 8);

	strTemp = g_SqlCon->Result("SEN_Data_Pattern");	//����ѡ��
	int IntType = _ttoi(strTemp);

	if(IntType == 0)
	{
		m_Type_Combo.SetCurSel(0);
	}
	else if(IntType == 2)
	{
		m_Type_Combo.SetCurSel(1);
	}
	else
	{
		MessageBox(_T("�������ô��󣬽�Ĭ��ѡ�� PRBS7 �����ͣ���"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
	}

	strTemp = g_SqlCon->Result("SEN_Work_MODE");	//����ģʽѡ��(�⡢��ͨ��)
	int IntMode = _ttoi(strTemp);
	if(IntMode == 1)		//��ͨ��
	{
		m_Channel_Combo.SetCurSel(1);
	}
	else if(IntMode == 2)	//��ͨ��
	{
		m_Channel_Combo.SetCurSel(0);
	}
	
	strTemp = g_SqlCon->Result("SEN_VOP");	//VOPȡֵ��ʽ
	int IntVop = _ttoi(strTemp);

	if(IntVop == 0 || IntVop == 3)
	{
		MessageBox(_T("VOPȡֵ��ʽ���ô��󣬽�Ĭ��ѡ�� Vbr-2����"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
	}
	m_VopVal_Combo.SetCurSel(IntVop);

	strTemp = g_SqlCon->Result("SEN_PM_STEP");	//����
	m_fSenStep_edit = (float)atof(strTemp);
	m_fSenGoVal_edit = ReadIniFileNum("Config.ini","TEST","SenPower");			//��ʼֵ
	m_CompenVal_edit = ReadIniFileNum("Config.ini","TEST","SenCompenVal");		//�����Ȳ���ֵ
	m_PowStep1		 = ReadIniFileNum("Config.ini","TEST","PowStep1");
	m_PowStep2		 = ReadIniFileNum("Config.ini","TEST","PowStep2");

	UpdateData(false);

	return TRUE;
}

//��ȡ��ǰ���빤���Ĳ�Ʒ�Ĳ��Թ�񣨴�TS�ļ��л�ȡ�����ɹ����� TRUE
BOOL CMy10GAPDTESTDlg::GetProductTestSpec_TS()
{
	///////////////////��ȡ�ͺ�
	CString strCmd = _T("");	//Sql����ַ���
	strCmd.Format(_T("select model from Manufacture_info where Manufacture_ID = '%s'"),
		m_strWorkOrderNo_edit);			
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	
	{
		MessageBox(_T("���ݿ��ͺŲ�ѯʧ�ܣ���"),_T("����"),MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	m_strModel_edit = g_SqlCon->Result("model");
	UpdateData(false);

	//////////////////����TS�ļ�
	if (!LoadTsFileFun(m_strModel_edit,g_strMaterialCode,g_strTsFileName,((CButton*)(GetDlgItem(IDC_TESTSPEC_CHECK)))->GetCheck()))
	{
		return FALSE;
	}

	////////////////��ȡTS�ļ��Ĳ��Թ��
	if(m_strModel_edit != g_strModelType)
	{
		if(FALSE == ReadTsFileTestSpec())
		{
			return FALSE;
		}
		g_strModelType = m_strModel_edit;
	}

	return TRUE;
}

//��ȡ����ָ��TS�ļ��еĲ�Ʒ���Թ��
BOOL CMy10GAPDTESTDlg::ReadTsFileTestSpec()
{
	CString strSheetName = _T("Product Spec");
	ReadTsSpec * CUexcel = new ReadTsSpec();

	CString strPathName = GetSysRunPath();
	strPathName.Format(_T("%s\\TsFile\\%s"),strPathName,g_strTsFileName);
	if( CUexcel->m_CUexcel->OpenXlsFile( strPathName,FALSE) == -1 )
		return FALSE;
	CUexcel->m_CUexcel->GetSheet(strSheetName);

	CString strTemp1,strTemp2;
	BOOL bEnabled[100];

	CUexcel->ReadTestSpec(_T("Icc"),_T("RX����"),_T("25"),_T("3.3"),strTemp1,strTemp2,bEnabled[0]);
	m_fIccMin_edit = (float)(atof(strTemp1));
	m_fIccMax_edit = (float)(atof(strTemp2));

	CUexcel->ReadTestSpec(_T("������(Id)"),_T("RX����"),_T("25"),_T("3.3"),strTemp1,strTemp2,bEnabled[1]);
	m_fIdMin_edit = (float)(atof(strTemp1));
	m_fIdMax_edit = (float)(atof(strTemp2));

	CUexcel->ReadTestSpec(_T("������ѹ(VBR)"),_T("RX����"),_T("25"),_T("3.3"),strTemp1,strTemp2,bEnabled[2]);
	m_fVbrMin_edit = (float)(atof(strTemp1));
	m_fVbrMax_edit = (float)(atof(strTemp2));

	CUexcel->ReadTestSpec(_T("������"),_T("RX����"),_T("25"),_T("3.3"),strTemp1,strTemp2,bEnabled[3]);
	m_fSenMin_edit = (float)(atof(strTemp1));
	m_fSenMax_edit = (float)(atof(strTemp2));

	int nOrd = -1;
	CString SpecVal,Specdescribe;
	CUexcel->ReadBaseSpec(_T("SEN����ʱ��"),nOrd,SpecVal,Specdescribe);
	m_SenTime_edit = _ttoi(SpecVal);

	CUexcel->ReadBaseSpec(_T("���ղ���-����"),nOrd,SpecVal,Specdescribe);
	if(_ttoi(SpecVal) == 1310)
	{
		m_AttWaveLen_Combo.SetCurSel(0);		//��˥����
		m_PowerWaveLen_Combo.SetCurSel(3);		//�⹦�ʼ�
	}
	else if(_ttoi(SpecVal) == 1550)
	{
		m_AttWaveLen_Combo.SetCurSel(1);
		m_PowerWaveLen_Combo.SetCurSel(5);
	}
	else
	{
		MessageBox(_T("�������ô��󣬽�Ĭ��ѡ�� 1310 �Ĳ�������"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
	}

	CUexcel->ReadBaseSpec(_T("ָ��������"),nOrd,m_strVbrVal_edit,Specdescribe);	//������ѡ��
	CUexcel->ReadBaseSpec(_T("��������-����"),nOrd,SpecVal,Specdescribe);		//����ѡ��
	if(nOrd < 8)
	{
		nOrd = 8;
		MessageBox(_T("�������ô��󣬽�Ĭ��ѡ�� 9.95G �����ʣ���"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
	}
	m_Rate_Combo.SetCurSel(nOrd - 8);

	CUexcel->ReadBaseSpec(_T("��������-����"),nOrd,SpecVal,Specdescribe);//����ѡ��
	if(nOrd == 0)
	{
		m_Type_Combo.SetCurSel(0);
	}
	else if(nOrd == 2)
	{
		m_Type_Combo.SetCurSel(1);
	}
	else
	{
		MessageBox(_T("�������ô��󣬽�Ĭ��ѡ�� PRBS7 �����ͣ���"),_T("��ʾ��Ϣ"),MB_OKCANCEL | MB_ICONQUESTION);
	}

	CUexcel->ReadBaseSpec(_T("�����ǹ���ģʽ"),nOrd,SpecVal,Specdescribe);//����ģʽѡ��(�⡢��ͨ��)
	if(nOrd == 1)		//��ͨ��
	{
		m_Channel_Combo.SetCurSel(1);
	}
	else if(nOrd == 2)	//��ͨ��
	{
		m_Channel_Combo.SetCurSel(0);
	}
	
	CUexcel->ReadBaseSpec(_T("SEN_Vop��ʽ"),nOrd,SpecVal,Specdescribe);//VOPȡֵ��ʽ
	m_VopVal_Combo.SetCurSel(nOrd);

	CUexcel->ReadBaseSpec(_T("���ʼ����ò���"),nOrd,SpecVal,Specdescribe);//����
	m_fSenStep_edit = (float)atof(SpecVal);

	CUexcel->m_CUexcel->CloseExcel();
	delete CUexcel;
	CUexcel = NULL;

	int IntRange = (int)(ReadIniFileNum("Config.ini","TEST","SiganlRange"));	//�����Ƿ���ѡ��	
	m_Amplitude_Combo.SetCurSel(IntRange);

	m_fSenGoVal_edit = ReadIniFileNum("Config.ini","TEST","SenPower");			//��ʼֵ
	m_CompenVal_edit = ReadIniFileNum("Config.ini","TEST","SenCompenVal");		//�����Ȳ���ֵ
	m_PowStep1		 = ReadIniFileNum("Config.ini","TEST","PowStep1");
	m_PowStep2		 = ReadIniFileNum("Config.ini","TEST","PowStep2");

	UpdateData(false);

	return TRUE;
}

//��ȡ����Ĺ����Ĳ�����Ϣ�����Ѳ�����������Ʒ�� �ȣ�
BOOL CMy10GAPDTESTDlg::GetTestInfoToWorkOrder()
{
	int IntTestNumber	= 0;	//���Ե�������
	int IntTestFailNum	= 0;	//����Ʒ����
	int IntTestPassNum	= 0;	//��Ʒ����
	float fTestPass		= 0.0f;	//��Ʒ��

	//��ȡ��Ʒ����
	CString strCmd = _T("");	//Sql����ַ���
	strCmd.Format(_T("select count(*) as Number from dbo.testdata where  Sen_Pass_fail = '1' AND Manufacture_ID = '%s'"),
		m_strWorkOrderNo_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//
	{
		return FALSE;
	}
	CString strNumber = g_SqlCon->Result("Number");
	IntTestPassNum	  = _ttoi(strNumber);

	//��ȡ����Ʒ����
	strCmd.Format(_T("select count(*) as Number from dbo.testdata where  Sen_Pass_fail = '0' AND Manufacture_ID = '%s'"),
		m_strWorkOrderNo_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	
	{
		return FALSE;
	}
	strNumber = g_SqlCon->Result("Number");
	IntTestFailNum	  = _ttoi(strNumber);

	//�����Ѳ�����������Ʒ��
	IntTestNumber = IntTestPassNum + IntTestFailNum;
	if(IntTestNumber == 0)
	{
		fTestPass = 0.0f;
	}
	else
	{
		fTestPass = (float)(IntTestPassNum/IntTestNumber);
	}

	//������д������
	CString strTemp = _T("");
	strTemp.Format("%d",IntTestNumber);
	m_Method.WriteGridCtrOne(m_GridDS,1,1,strTemp,RGB(100, 0, 0));	//����������

	strTemp.Format("%d",IntTestFailNum);
	m_Method.WriteGridCtrOne(m_GridDS,2,1,strTemp,RGB(100, 0, 0));	//����Ʒ����

	strTemp.Format("%d",IntTestPassNum);
	m_Method.WriteGridCtrOne(m_GridDS,3,1,strTemp,RGB(100, 0, 0));	//��Ʒ����

	strTemp.Format("%0.2f",fTestPass * 100);
	m_Method.WriteGridCtrOne(m_GridDS,4,1,strTemp,RGB(100, 0, 0));	//��Ʒ��
	return TRUE;
}

//������������
void CMy10GAPDTESTDlg::BeginTest()
{
	g_BeginTest = TRUE;
	GetDlgItem(IDC_BeginTest_BUTTON)->SetWindowText(_T("�ѿ�����������"));
	m_ButtonBeginTest.SetUpColor(PassColor);
	GetDlgItem(IDC_BeginTest_BUTTON)->Invalidate();
	SetDebugParaCtrlStatus(false);
}

//ֹͣ��������
void CMy10GAPDTESTDlg::StopTest()
{
	g_BeginTest = FALSE;
	GetDlgItem(IDC_BeginTest_BUTTON)->SetWindowText(_T("��ֹͣ��������"));
	m_ButtonBeginTest.SetUpColor(FailColor);
	GetDlgItem(IDC_BeginTest_BUTTON)->Invalidate();
	SetDebugParaCtrlStatus(true);
}

//���������ݿ����ӳ�ʼ��
BOOL CMy10GAPDTESTDlg::SqlServerConnet()
{
	g_SqlCon	= new ADO_Connection();	//sql server���ݿ�
	g_recordSql = new _RecordsetPtr;

	CString strServerIP = ReadIniFileStr("Config.ini","SQL","ServerIP");	//������IP��ַ
	CString strAccount  = ReadIniFileStr("Config.ini","SQL","Account");		//�˺�
	CString strPassWord = ReadIniFileStr("Config.ini","SQL","PassWord");	//����
	CString strDataBase = ReadIniFileStr("Config.ini","SQL","DataBase");	//���ݿ���

	/*********************************�Լ��ܵ�������н���*********************************************/
	/*
	 *������ܷ�����
	 *���ܵ�KeyֵΪ ����APAT��ÿ���ַ���Assic��ֵ���
	 *���ܷ�����
	 *	�������ÿ���ַ���Assicֵ ���� Keyֵ���� ��,���ָ������������롰123�����ܺ��Ϊ "14406,14700,14994"
	 */

	CString strPassWordTemp[10];						//���ڱ���ָ���δ���ܵ����ַ���
	SubCString(strPassWord,strPassWordTemp, _T(","));	//��ȡ���ַ���
	int i = 0;
	int Key = 'A' + 'P' + 'A' + 'T';					//���ܵ�Keyֵ
	char strTemp[1024] = {0};							//���ڱ�����ܺ���ַ�
	while(strPassWordTemp[i] != _T(""))
	{
		strTemp[i++] = _ttoi(strPassWordTemp[i])/Key;
	}
	strPassWord.Format(_T("%s"),strTemp);

	/**************************************************************************************************/

	if( false == g_SqlCon->OpenConnection(strServerIP,strAccount,strPassWord,strDataBase,1433) )
	{
		return FALSE;;
	}

	//��ȡ�����ʶ����
	CString strCmd = _T("");
	strCmd.Format(_T("SELECT Station_Code FROM  sys_info WHERE info_Item ='%s'"),g_strActiveStep);
	if( false == g_SqlCon->Query(strCmd, g_recordSql) )	
	{
		MessageBox(_T("��ȡ��ǰ�����ʶ����ʧ�ܣ�"), _T("������ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}
	g_IntActiveStep = g_SqlCon->Result.Convertor("Station_Code").ToInteger();


	return TRUE;
}

//����������ݵ�����������
BOOL CMy10GAPDTESTDlg::SaveTestDataServer()
{
	Data->m_PDNo		= m_strPDModel_edit;	//PD�ͺ�
	Data->m_TestEr		= m_strTestEr_edit;		//������Ա
	Data->m_WorkOrderNo = m_strWorkOrderNo_edit;//������
	Data->m_SerialNo	= m_strSn_edit;			//ϵ�к�

	SYSTEMTIME st;
	GetLocalTime(&st);
	Data->m_TestTime.Format("%4d-%2d-%2d %2d:%2d:%2d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);


	CString strCmd = _T("");	//Sql����ַ���
	strCmd.Format(_T("Update dbo.testdata  set PD_Type = '%s',Vbr_Pass_Fail = %d,Sen_Pass_fail = %d,Icc = %0.2f,\
		Id = %f,Vbr = %0.2f,SEN = %0.2f,Sen_Time = '%s',SEN_TestMan = '%s' where Manufacture_ID = '%s' AND \
		Serial_No = '%s'"),
		Data->m_PDNo,Data->m_VbrTestPass,Data->m_SenTestPass,Data->m_IccVal,Data->m_IdVal,Data->m_VbrVal,
		Data->m_SenVal,Data->m_TestTime,Data->m_TestEr,Data->m_WorkOrderNo,Data->m_SerialNo);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//
	{
		return FALSE;
	}
	return TRUE;
}

//�������ݿ����ӳ�ʼ��
BOOL CMy10GAPDTESTDlg::AccessConnet()
{
	g_AccCon	= new ADO_Connection();	//Access���ݿ�
	g_recordAcc = new _RecordsetPtr;

	CString FilePath = GetFilePath("SaveData.accdb");	//���ݿ��ļ�·��
	CString StrConnect = "Provider = Microsoft.ACE.OLEDB.12.0;Data Source="  + FilePath;//Access2007

	if(false == g_AccCon->OpenConnection( StrConnect ))
	{
		return FALSE;
	}
	return TRUE;
}

//������������ڱ��ص�Access���ݿ���
BOOL CMy10GAPDTESTDlg::SaveTestDataAccess()
{
	CString strCmd = _T("");	//Sql����ַ���
	strCmd.Format(_T("select * from TestData where SerialNo = '%s'"),m_strSn_edit);
	
	if( false == g_AccCon->Query( strCmd, g_recordAcc  ) )	//
	{
		return FALSE;
	}

	int IntNumber = 0;
	while(!g_AccCon->Result.IsEOF())
	{
		IntNumber++;
		g_AccCon->Result.MoveToNext();
	}


	if(IntNumber > 0)	//�Ѿ��и� SN �ŵĲ�Ʒ���ݣ��޸ļ���
	{
		Data->m_TestEr		= m_strTestEr_edit;		//������Ա
		Data->m_WorkOrderNo = m_strWorkOrderNo_edit;//������
		Data->m_SerialNo	= m_strSn_edit;			//ϵ�к�

		SYSTEMTIME st;
		GetLocalTime(&st);
		Data->m_TestTime.Format("%4d-%2d-%2d %2d:%2d:%2d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

		strCmd.Format(_T("Update TestData set Vbr_Pass_Fail = %d,Sen_Pass_fail = %d,Icc = %f,\
			IdVal = %f,Vbr = %f,SEN = %f,Sen_Time = '%s',SEN_TestMan = '%s' where SerialNo = '%s' "),
			Data->m_VbrTestPass,Data->m_SenTestPass,Data->m_IccVal,Data->m_IdVal,Data->m_VbrVal,
			Data->m_SenVal,Data->m_TestTime,Data->m_TestEr,Data->m_SerialNo);

		if( false == g_AccCon->Query( strCmd, g_recordAcc  ) )	
		{
			return FALSE;
		}
	}
	else					//�� SN �ŵĲ�Ʒ��δ���Թ�����Ҫ���
	{
		Data->m_Model		= m_strModel_edit;		//�ͺ�
		Data->m_WorkOrderNo = m_strWorkOrderNo_edit;//������
		Data->m_BatchNo		= m_strBatchNo_edit;	//���κ�
		Data->m_PDNo		= m_strPDModel_edit;	//PD�ͺ�
		Data->m_TestEr		= m_strTestEr_edit;		//������Ա
		Data->m_SerialNo	= m_strSn_edit;			//ϵ�к�
		SYSTEMTIME st;
		GetLocalTime(&st);
		Data->m_TestTime.Format("%4d-%2d-%2d %2d:%2d:%2d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

		strCmd.Format("INSERT INTO TestData (ModelNo,WorkOrderNo,BatchNo,PDType,SerialNo,Vbr_Pass_Fail,Sen_Pass_fail,Icc,\
			IdVal,Vbr,Sen,Sen_Time,SEN_TestMan) VALUES ('%s','%s','%s','%s','%s',%d,%d,%f,%f,%f,%f,'%s','%s')",
		Data->m_Model,Data->m_WorkOrderNo,Data->m_BatchNo,Data->m_PDNo,Data->m_SerialNo,Data->m_VbrTestPass,
		Data->m_SenTestPass,Data->m_IccVal,Data->m_IdVal,Data->m_VbrVal,Data->m_SenVal,Data->m_TestTime,Data->m_TestEr);

		if( false == g_AccCon->Query( strCmd, g_recordAcc ) )
		{	
			return FALSE;
		}
	}		
	return TRUE;
}

//����ʧ�ܣ�����ʧ����ʾ
void CMy10GAPDTESTDlg::TestFail()
{
	if(ApdVccOutPut(FALSE) == FALSE)//�ر�APD��VCC��ѹ���
	{
		MessageBox(_T("�豸�������ִ������ֶ��ر� VBR ��ѹ��������򽫻���������"), _T("������ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
	}

	GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("����ʧ�ܣ�"));
	g_ColorLogo = Color_Red;
	GetDlgItem(IDC_Results_STATIC)->SetWindowText(_T("FAIL"));
	GetDlgItem(IDC_Results_STATIC)->Invalidate();
	g_deviceTestState = FALSE;
	StopTest();
}

//���Գɹ��������ɹ���ʾ
void CMy10GAPDTESTDlg::TestPass()
{
	if(ApdVccOutPut(FALSE) == FALSE)//�ر�APD��VCC��ѹ���
	{
		MessageBox(_T("�豸�������ִ������ֶ��ر� VBR ��ѹ��������򽫻���������"), _T("������ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
	}

	GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("���Գɹ������������"));
	g_ColorLogo = Color_Green;
	GetDlgItem(IDC_Results_STATIC)->SetWindowText(_T("PASS"));
	GetDlgItem(IDC_Results_STATIC)->Invalidate();
	m_strSn_edit = _T("");
	UpdateData(false);
	StopTest();
}

//������ʼ����
void CMy10GAPDTESTDlg::BeginDeviceTest()
{
	Data->SaveDataInit();				//��ʼ����Ҫ���������
	GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("���ڲ��ԣ����Ժ򡤡���"));
	g_ColorLogo = Color_BLUE;
	GetDlgItem(IDC_Results_STATIC)->SetWindowText(_T("������������"));
	GetDlgItem(IDC_Results_STATIC)->Invalidate();
	g_deviceTestState = TRUE;
	g_TimeBegin = GetTickCount();		//ȡ�õ�ǰϵͳ����ʱ��(ms)
	m_ctrlProgress.SetPos(10);			//���ý������ĵ�ǰλ��

	g_FailType     = _T("");	
	g_FailItemCode = -1;
}

//����ĳ���ؼ��Ŀɲ���״̬
void CMy10GAPDTESTDlg::SetControlsOptional(int ID, bool State)
{
	CWnd * pWnd = NULL;
	pWnd = GetDlgItem(ID);
	pWnd->EnableWindow(State);
}

//�Զ����Կ�ѡ��ؼ�״̬����
void CMy10GAPDTESTDlg::SetDebugParaCtrlStatus(bool setVal)
{
	CWnd * pWnd = NULL; 

	SetControlsOptional(IDC_CHECK1, setVal);
	SetControlsOptional(IDC_ExtractData_BUTTON, setVal);
	SetControlsOptional(IDC_DeviceInit_BUTTON, setVal);
	SetControlsOptional(IDC_Calibration1_BUTTON, setVal);
}

//����/�ر� APD��VCC��ѹ���
BOOL CMy10GAPDTESTDlg::ApdVccOutPut(BOOL State)
{
	if(m_ApatVbr->VoltageOutputEnable(0, State) == FALSE)	//����VAPD��ѹ���
	{
		return FALSE;
	}

	if(m_ApatVbr->VoltageOutputEnable(2, State) == FALSE)	//����VCC��ѹ���
	{
		return FALSE;
	}

	if(State == FALSE)
	{
		if(m_ApatVbr->SetDeviceWorkMode(5) == FALSE)			//ѡ���5����ģʽ
		{
			return FALSE;
		}
	}

	return TRUE;
}

//���й����жϣ����ϵ�ǰ���򷵻�TRUE�����򷵻�FASLE
BOOL CMy10GAPDTESTDlg::JudgeStep(CString strSN)
{
	CString strCmd = _T("");	//Sql����ַ���
	strCmd.Format(_T("select Active_Step,Next_Step from dbo.testdata  where Serial_No = '%s'"),
				  strSN);

	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	
	{
		TestFail();
		GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("���ݿ��ȡ�����������"));
		return FALSE;
	}
	int IntActiveStep = g_SqlCon->Result("Active_Step");	//��ǰ����
	int IntNextStep   = g_SqlCon->Result("Next_Step");		//��һ����

	strCmd.Format(_T("select Pstep from pstepset where dutType = '%s' AND PIndex = %d"),m_strModel_edit,IntActiveStep);
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	
	{
		TestFail();
		return FALSE;
	}
	CString strActiveStep = g_SqlCon->Result("Pstep");	//��ǰ����

	if(strActiveStep != g_strActiveStep)
	{
		TestFail();
		GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("�����������ڵ�ǰ��������"));
		return FALSE;
	}
	
	return TRUE;
}

//���뷵��
BOOL CMy10GAPDTESTDlg::GotoRepair()
{
	INT_PTR nRes = MessageBox(_T("�ò�Ʒ�Ƿ�ת�뷵�ޣ�"), _T("��ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
	if (IDCANCEL == nRes)	//��������
	{
		return FALSE;
	}

	//��ѯ���޴���
	CString strCmd = _T("");	//Sql����ַ���
	strCmd.Format("select Repair from testdata where Manufacture_ID='%s' and Batch_ID= %.0f and Serial_No='%s' ",
		m_strWorkOrderNo_edit,atof(m_strBatchNo_edit),m_strSn_edit);

	if( false == g_SqlCon->Query(strCmd, g_recordSql) )	
	{
		MessageBox(_T("���������뷵��ʧ��1��"), _T("������ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}
	float IntRepair = (float)g_SqlCon->Result.Convertor("Repair").ToInteger();	//���޴���

	//���뷵��	
	strCmd.Format(_T("select count(*) as Number from Repair_info where Serial_No = '%s'"),m_strSn_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//
	{
		MessageBox(_T("�������ݲ�ѯʧ�ܣ���"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	int IntNumber = g_SqlCon->Result.Convertor("Number").ToInteger();

	if(IntNumber == 0)	//δ���������
	{
		strCmd.Format( "INSERT INTO Repair_info (Serial_No,Model_Number,Rep_Index, Old_Manufacture_ID, Old_Batch, \
			Exp_Team,Exp_Operator, Exp_time, Fail_Item_Code,Fail_Station_Code,Fail_type) VALUES ('%s','%s',%f,'%s',\
			%f,'%s','%s','%s',%d,%d,'%s')",
			m_strSn_edit,m_strModel_edit,IntRepair +1,m_strWorkOrderNo_edit,atof(m_strBatchNo_edit),_T("���"),
			m_strTestEr_edit,Data->m_TestTime,g_FailItemCode,6,g_FailType);
			
	}
	else				//�Ѵ��ڷ���
	{
		strCmd.Format( "Update Repair_info set Model_Number = '%s',Rep_Index = %f,Old_Manufacture_ID = '%s',\
			Old_Batch = %f,Exp_Team = '%s',Exp_Operator = '%s',Exp_time = '%s',\
			Fail_Item_Code = %d,Fail_Station_Code = %d,Fail_type = '%s' where Serial_No = '%s'",
			m_strModel_edit,IntRepair +1,m_strWorkOrderNo_edit,atof(m_strBatchNo_edit),_T("���"),
			m_strTestEr_edit,Data->m_TestTime,g_FailItemCode,6,g_FailType,m_strSn_edit);
	}

	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	
	{
		MessageBox(_T("���������뷵��ʧ��2��"), _T("������ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	strCmd.Format("UPDATE testdata SET  Active_Step=0,Next_Step=0,Repair=%f where Serial_No='%s' ",IntRepair +1,m_strSn_edit);

	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	
	{
		MessageBox(_T("���������뷵��ʧ��3��"), _T("������ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	strCmd.Format("SELECT FailCode_Num FROM Count_Product_Fail_Num WHERE Manufacture_ID='%s' AND Batch_ID= %d AND Fail_Code= %d\
		AND Fail_Station_Code=%d ",m_strWorkOrderNo_edit,(int)atof(m_strBatchNo_edit),g_FailItemCode,6);

	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	
	{
		MessageBox(_T("���������뷵��ʧ��4��"), _T("������ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	int IntTotalNum = 0;
	if(!g_SqlCon->Result.IsEOF())
	{
		IntTotalNum = g_SqlCon->Result.Convertor("FailCode_Num").ToInteger();
		IntTotalNum += 1;

		strCmd.Format("UPDATE Count_Product_Fail_Num SET FailCode_Num= %d WHERE Manufacture_ID='%s' AND Batch_ID= %d \
			AND Fail_Code='%d' and Fail_Station_Code=%d ",
			IntTotalNum,m_strWorkOrderNo_edit,_ttoi(m_strBatchNo_edit),g_FailItemCode,6);
	}
	else
	{
		IntTotalNum = 1;
		strCmd.Format("INSERT INTO Count_Product_Fail_Num (Model_Type,Manufacture_ID,Batch_ID,Fail_Station_Code,\
			Fail_Code,FailCode_Num) values ('%s','%s',%d,%d,%d,%d) ",
			m_strModel_edit,m_strWorkOrderNo_edit,_ttoi(m_strBatchNo_edit),6,g_FailItemCode,IntTotalNum);
	}
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	
	{
		MessageBox(_T("���������뷵��ʧ��5��"), _T("������ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	return TRUE;
}

//���Խ������
void CMy10GAPDTESTDlg::TestResultsDeal()
{
	if(g_FailItemCode == -1 && g_FailType == _T(""))
	{
		MessageBox(_T("�豸�������ִ��������豸�����²��ԣ�"), _T("������ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
		TestFail();
		return ;
	}

	if(SaveTestDataAccess() == FALSE)		//�������ݱ����ڱ���
	{
		MessageBox(_T("������������ڱ���ʧ�ܣ���"), _T("������ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
		TestFail();
		return ;
	}

	if(SaveTestDataServer() == FALSE)		//�������ݱ����ڷ�����
	{
		MessageBox(_T("������������ڷ�����ʧ�ܣ���"), _T("������ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
		TestFail();
		return ;
	}

	if(GotoRepair() == TRUE)	//���޴���
	{
		MessageBox(_T("���������뷵�޳ɹ���"), _T("��ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
	}
	TestFail();
	return ;
}

void CMy10GAPDTESTDlg::OnBnClickedTestspecCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_strModelType = _T("2efretyret```");
}
