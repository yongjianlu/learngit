// 10G-APD-TESTDlg.cpp : 实现文件
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

#pragma comment(lib,"AIT-VOA-01.lib")			//光衰减器
#pragma comment(lib,"APAT-VBR.lib")				//自制VBR测试仪
#pragma comment(lib,"PowerMeter_PM7501.lib")	//光功率计
#pragma comment(lib,"PSS-BERT-1-II.lib")		//误码仪


static CFont * g_pFont1;//	字体大小控制变量1（字体大小为80）
static CFont * g_pFont2;//	字体大小控制变量2（字体大小为20）
static CFont * g_pFont3;//	字体大小控制变量2（字体大小为15）

static COLORREF InitColor = RGB(205,192,112);	//Button初始化颜色
static COLORREF TextColor = RGB(0,0,128);		//Button字体颜色
static COLORREF PassColor = RGB(144,238,144);	//Button Pass颜色
static COLORREF FailColor = RGB(255,000,0);		//Button File颜色

#define Color_Red      1		//红色
#define Color_Green    2		//绿色
#define Color_BLUE     3		//蓝色


static CString g_strActiveStep = _T("测包");	//工序名称
static int g_IntActiveStep = 10;				//当前工序（测包）的标识号

static int g_ColorLogo = Color_BLUE;	//字体颜色

static BOOL g_DeviceInit  = FALSE;		//是否已完成设备初始化，完成后为TRUE
static BOOL g_Calibration = FALSE;		//是否已完成光校准，完成后为TRUE
static BOOL g_Extractdata = FALSE;		//规格数据提取是否成功，成功为TRUE
static BOOL g_BeginTest   = FALSE;		//是否已开启批量测试，已开启为TRUE

static BOOL g_ThreadExit  = FALSE;		//用于退出线程，当为true死表示线程退出
static BOOL g_deviceTestState = FALSE;	//表示器件是否已开始进行测试

static long g_TimeBegin;				//记录开始调测时间
static long g_TimeEnd;					//

static CString  g_FailType		= _T("");		//测试失败原因类型
static int      g_FailItemCode  = -1;			//失败代码

static CString g_strMaterialCode = _T("");	//根据产品型号获取到的物料编码
static CString g_strTsFileName   = _T("");	//根据物料编码获取到的TS文件名

static CString g_strModelType = _T("");	//上一次提取规格时读取到的产品型号


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMy10GAPDTESTDlg 对话框




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

	m_AitVoa  = new CAITVOA01();			//光衰减器
	m_ApatVbr = new CAPATVBR();				//VBR测试仪
	m_PM7501  = new CPowerMeter_PM7501();	//光功率计
	m_Bert    = new PssBert1II();			//误码仪

	m_strTestEr_edit = g_UserAccount;		//测试账号

	Data = new SaveData();		//将要保存的数据的结构

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


// CMy10GAPDTESTDlg 消息处理程序


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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy10GAPDTESTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy10GAPDTESTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMy10GAPDTESTDlg::OnOK()
{
	if(g_Calibration == FALSE)
	{
		MessageBox(_T("光校准未成功，请先进行光校准！"), _T("提示信息"), MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(g_Extractdata == FALSE)
	{
		MessageBox(_T("规格数据未提取，请先提取规格数据！"), _T("提示信息"), MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(g_DeviceInit == FALSE)
	{
		MessageBox(_T("设备未初始化，请先完成设备初始化！"), _T("提示信息"), MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(g_BeginTest == TRUE)
	{
		StopTest();		//停止批量测试
		return ;
	}

	BeginTest();		//开启批量测试
}

void CMy10GAPDTESTDlg::OnCancel()
{

}

void CMy10GAPDTESTDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	g_ThreadExit = TRUE;	//线程退出
	Sleep(500);

	delete m_AitVoa;
	delete m_ApatVbr;
	delete m_PM7501;
	delete m_Bert;

	m_AitVoa  = NULL;
	m_ApatVbr = NULL;
	m_PM7501  = NULL;
	m_Bert    = NULL;

	g_SqlCon->Close();		//关闭数据库
	delete g_SqlCon;
	delete g_recordSql;
	g_SqlCon	= NULL;
	g_recordSql = NULL;

	g_AccCon->Close();
	delete g_AccCon;
	delete g_recordAcc;
	g_AccCon	= NULL;
	g_recordAcc = NULL;

	EndDialog(IDCANCEL);    //关闭窗口 
	CDialog::OnClose();
}

HBRUSH CMy10GAPDTESTDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if( IDC_Results_STATIC  ==  pWnd->GetDlgCtrlID())//判断发出消息的空间是否是该静态文本框
		{
			pDC->SetBkMode(OPAQUE);					//设置文本背景模式为透明

			pDC->SetBkColor(RGB(255,255,205));			//设置文本背景为黄色
			hbr=CreateSolidBrush(RGB(255,255,205));		//控件的背景色为黄色

			switch (g_ColorLogo)
			{ 
			case Color_Red : 
				pDC->SetTextColor(RGB(255,0,0));			//设置字体颜色
				break; 

			case Color_Green : 
				pDC->SetTextColor(RGB(0,255,0));			//设置字体颜色
				break;  

			case Color_BLUE : 
				pDC->SetTextColor(RGB(30,144,255));
				break;

			default : 
				break; 
			}
		}
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

BOOL CMy10GAPDTESTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CString Str =  _T(" 灵敏度测试 2018-01-23  V1.2");
	if(g_IntModelState == 0)
	{
		SetWindowText(MODEL1 + Str);
	}
	else if(g_IntModelState == 1)
	{
		SetWindowText(MODEL2 + Str);
	}

	//g_UserPermissions
	if(!_ttoi(g_EnabledArray[47]))	//没有使用验证服务器的权限
	{
		CWnd * pWnd = NULL;
		pWnd = GetDlgItem(IDC_TESTSPEC_CHECK);
		pWnd->EnableWindow(FALSE);
	}

	/////////////////////设备连接//////////////////////////////

	GetComNum();				//获取主机当前可以串口号
	if(DeviceConnet() == FALSE) //对设备进行连接，失败则退出程序
	{
		//exit(0);
	}

	////////////////////数据库连接/////////////////////////////

	if(SqlServerConnet() == FALSE)	//服务数据库连接初始化
	{
		MessageBox(_T("服务器数据库连接失败，程序退出！"),_T("错误提示"),MB_OKCANCEL | MB_ICONQUESTION);
		//exit(0);
	}

	if(AccessConnet() == FALSE)
	{
		MessageBox(_T("本地数据库连接失败，程序退出！"),_T("错误提示"),MB_OKCANCEL | MB_ICONQUESTION);
		//exit(0);
	}

	ControlsTxetInit();			//控件初始化
	DeviceConfigControls();		//设备配置控件初始化
	GridInit();					//表格控件初始化
	ButtonInit();				//Button控件初始化
	GetCalState();				//光校准状态检测


	AfxBeginThread( ThreadTest, this );			//启动测试线程
	AfxBeginThread( ThreadTimeRecord, this );	//启动时间线程

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//控件初始化
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

	GetDlgItem(IDC_Results_STATIC)->SetWindowText(_T("·······"));
	GetDlgItem(IDC_Results_STATIC)->SetFont(g_pFont1,false);

	GetDlgItem(IDC_ExtractData_BUTTON)->SetWindowText(_T("规格数据提取"));
	GetDlgItem(IDC_ExtractData_BUTTON)->SetFont(g_pFont2,false);

	GetDlgItem(IDC_DeviceInit_BUTTON)->SetWindowText(_T("设备初始化"));
	GetDlgItem(IDC_DeviceInit_BUTTON)->SetFont(g_pFont2,false);

	GetDlgItem(IDC_BeginTest_BUTTON)->SetWindowText(_T("开启批量测试"));
	GetDlgItem(IDC_BeginTest_BUTTON)->SetFont(g_pFont2,false);

	GetDlgItem(IDC_Calibration1_BUTTON)->SetWindowText(_T("开始光校准"));
	GetDlgItem(IDC_Calibration1_BUTTON)->SetFont(g_pFont3,false);

	GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("提示信息"));
	GetDlgItem(IDC_STATIC1)->SetFont(g_pFont2,false);

	m_fSenGoVal_edit = ReadIniFileNum("Config.ini","TEST","SenPower");	//起始值
	UpdateData(false);

	m_Check1.SetCheck(1);		//默认为选中状态

	GetDlgItem(IDC_TestUser_STATIC)->SetWindowTextA(g_UserAccount);		//用户账号
	GetDlgItem(IDC_TestErName_STATIC)->SetWindowTextA(g_UserName);		//用户姓名

	m_ctrlProgress.SetRange(0,100);	//设置进度条的范围
	m_ctrlProgress.SetStep(10);		//设置进度条的每一步的增量
	m_ctrlProgress.SetPos(0);		//设置进度条的当前位置
}


//设备配置控件初始化
void CMy10GAPDTESTDlg::DeviceConfigControls()
{
	//误码仪通道选择
	m_Channel_Combo.InsertString(0,_T("电通道"));
	m_Channel_Combo.InsertString(1,_T("光通道"));
	m_Channel_Combo.SetCurSel(0);

	//误码仪码型选择
	m_Type_Combo.InsertString(0,_T("PRBS7"));
	m_Type_Combo.InsertString(1,_T("PRBS31"));
	m_Type_Combo.SetCurSel(0);

	//误码仪速率选择
	m_Rate_Combo.InsertString(0,_T("9.95Gbps"));
	m_Rate_Combo.InsertString(1,_T("10.31Gbps"));
	m_Rate_Combo.InsertString(2,_T("10.52Gbps"));
	m_Rate_Combo.InsertString(3,_T("10.70Gbps"));
	m_Rate_Combo.InsertString(4,_T("11.09Gbps"));
	m_Rate_Combo.InsertString(5,_T("11.32Gbps"));
	m_Rate_Combo.InsertString(6,_T("10Gbps"));
	m_Rate_Combo.SetCurSel(0);

	//误码仪幅度选择
	m_Amplitude_Combo.InsertString(0,_T("100"));
	m_Amplitude_Combo.InsertString(1,_T("200"));
	m_Amplitude_Combo.InsertString(2,_T("300"));
	m_Amplitude_Combo.InsertString(3,_T("400"));
	m_Amplitude_Combo.InsertString(4,_T("500"));
	m_Amplitude_Combo.InsertString(5,_T("600"));
	m_Amplitude_Combo.InsertString(6,_T("700"));
	m_Amplitude_Combo.InsertString(7,_T("800"));
	m_Amplitude_Combo.SetCurSel(4);

	//VOP取值方式选择
	m_VopVal_Combo.InsertString(0,_T("Vbr * 0.9"));
	m_VopVal_Combo.InsertString(1,_T("Vbr - 2"));
	m_VopVal_Combo.InsertString(2,_T("Vbr - 3"));
	m_VopVal_Combo.InsertString(2,_T("Vbr - 4"));
	m_VopVal_Combo.SetCurSel(1);

	//光功率计波长选择
	m_PowerWaveLen_Combo.InsertString(0,_T("850nm"));
	m_PowerWaveLen_Combo.InsertString(1,_T("980nm"));
	m_PowerWaveLen_Combo.InsertString(2,_T("1046nm"));
	m_PowerWaveLen_Combo.InsertString(3,_T("1310nm"));
	m_PowerWaveLen_Combo.InsertString(4,_T("1480nm"));
	m_PowerWaveLen_Combo.InsertString(5,_T("1550nm"));
	m_PowerWaveLen_Combo.InsertString(6,_T("1610nm"));
	m_PowerWaveLen_Combo.SetCurSel(3);

	//光衰减器波长选择
	m_AttWaveLen_Combo.InsertString(0,_T("1310nm"));
	m_AttWaveLen_Combo.InsertString(1,_T("1550nm"));
	m_AttWaveLen_Combo.SetCurSel(0);
}	

//表格控件初始化
void CMy10GAPDTESTDlg::GridInit()
{
	//统计信息
	m_Method.GridCtrlInit(m_GridDS,6,2,RGB(255, 236, 139),32,73);//对表格进行初始化
	m_Method.WriteGridCtrOne(m_GridDS,0,0,_T("统计项目  "),RGB(0, 0, 0));
	m_Method.WriteGridCtrOne(m_GridDS,0,1,_T("统计值"),RGB(0, 0, 0));
	m_Method.WriteGridCtrOne(m_GridDS,1,0,_T("测试总数量"),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridDS,2,0,_T("不良品数量"),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridDS,3,0,_T("良品数量  "),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridDS,4,0,_T("良品率    "),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridDS,5,0,_T("工位UPH   "),RGB(100, 0, 0));

	for(int row = 0; row < 6; row++)
	{
		for(int col = 0; col < 2; col++)
		{
			if(row > 0)
			{
				m_Method.OnCellModel(m_GridDS,row,col,1,RGB(190,190,190));	//单元格模式（与颜色）
				if(col > 0)
					m_Method.WriteGridCtrOne(m_GridDS,row,col,_T("0"),RGB(255, 0, 0));
			}	
			else
			{
				m_Method.OnCellModel(m_GridDS,row,col,1,RGB(176,226,255));	////表头单元格 颜色与模式
			}
		}
	}

	//灵敏度测试 拟合数据
	m_Method.GridCtrlInit(m_GridFitt,10,2,RGB(255, 236, 139),24,72);//对表格进行初始化
	m_Method.WriteGridCtrOne(m_GridFitt,0,0,_T("观察点光功率值"),RGB(0, 0, 0));
	m_Method.WriteGridCtrOne(m_GridFitt,0,1,_T("对应误码率"),RGB(0, 0, 0));

	for(int row = 0; row < 10; row++)		//行
	{
		for(int col = 0; col < 2; col++)	//列
		{
			if(row < 6)
			{
				if(row > 0)
				{
					m_Method.OnCellModel(m_GridFitt,row,col,1,RGB(180,205,205));	//单元格模式（与颜色）
					m_Method.WriteGridCtrOne(m_GridFitt,row,col,_T(""),RGB(0, 0, 0));
				}
				else
				{
					m_Method.OnCellModel(m_GridFitt,row,col,1,RGB(176,226,255));	////表头单元格 颜色与模式
				}
			}
			else
			{
				m_Method.OnCellModel(m_GridFitt,row,col,1,RGB(240,240,255));	//单元格模式（与颜色）
				if(col > 0)
				{
					m_Method.WriteGridCtrOne(m_GridFitt,row,col,_T(""),RGB(0, 0, 0));
				}
			}
		}
	}
	m_Method.WriteGridCtrOne(m_GridFitt,7,0,_T("拟合系数A："),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridFitt,8,0,_T("拟合系数B："),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridFitt,9,0,_T("灵敏度值 ："),RGB(100, 0, 0));

	//10G APD 最终测试结果 数据
	m_Method.GridCtrlInit(m_GridTS,10,3,RGB(255, 236, 139),24,65);//对表格进行初始化
	m_Method.WriteGridCtrOne(m_GridTS,0,0,_T("测试项目"),RGB(0, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,0,1,_T("测试状态"),RGB(0, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,0,2,_T("结果值 "),RGB(0, 0, 0));

	m_Method.WriteGridCtrOne(m_GridTS,2,0,_T("Icc测试 "),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,2,1,_T(""),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,2,2,_T(""),RGB(100, 0, 0));

	m_Method.WriteGridCtrOne(m_GridTS,4,0,_T("Id测试  "),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,4,1,_T(""),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,4,2,_T(""),RGB(100, 0, 0));

	m_Method.WriteGridCtrOne(m_GridTS,6,0,_T("VBR测试 "),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,6,1,_T(""),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,6,2,_T(""),RGB(100, 0, 0));

	m_Method.WriteGridCtrOne(m_GridTS,8,0,_T("SEN测试 "),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,8,1,_T(""),RGB(100, 0, 0));
	m_Method.WriteGridCtrOne(m_GridTS,8,2,_T(""),RGB(100, 0, 0));

	for(int row = 0; row < 10; row++)		//行
	{
		for(int col = 0; col < 3; col++)	//列
		{
			if(row < 1)
			{
				m_Method.OnCellModel(m_GridTS,row,col,1,RGB(176,226,255));	////表头单元格 颜色与模式
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

//Button控件初始化，对Button控件进行重绘
void CMy10GAPDTESTDlg::ButtonInit()
{
	//“规格数据提取”按钮控件
	GetDlgItem(IDC_ExtractData_BUTTON)->ModifyStyle(0,BS_OWNERDRAW,0);	//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
	m_ButtonExtractData.Attach(IDC_ExtractData_BUTTON,this);			//绑定控件IDC_BUTTON1与类CMyButton，响应重载函数DrawItem()
	m_ButtonExtractData.SetDownColor(RGB(255,255,224));					//设置Button Down的背景色
	m_ButtonExtractData.SetUpColor(InitColor);							//设置Button Up的背景色
	m_ButtonExtractData.SetTextColor(TextColor);						//Button的字体颜色

	//“设备初始化”按钮控件
	GetDlgItem(IDC_DeviceInit_BUTTON)->ModifyStyle(0,BS_OWNERDRAW,0);
	m_ButtonConfigInit.Attach(IDC_DeviceInit_BUTTON,this);
	m_ButtonConfigInit.SetDownColor(RGB(255,255,224));
	m_ButtonConfigInit.SetUpColor(InitColor);
	m_ButtonConfigInit.SetTextColor(TextColor);

	//“开启批量测试”按钮控件
	GetDlgItem(IDC_BeginTest_BUTTON)->ModifyStyle(0,BS_OWNERDRAW,0);
	m_ButtonBeginTest.Attach(IDC_BeginTest_BUTTON,this);
	m_ButtonBeginTest.SetDownColor(RGB(255,255,224));
	m_ButtonBeginTest.SetUpColor(InitColor);
	m_ButtonBeginTest.SetTextColor(TextColor);

	//“光校准1”按钮控件
	GetDlgItem(IDC_Calibration1_BUTTON)->ModifyStyle(0,BS_OWNERDRAW,0);
	m_ButtonCalibration1.Attach(IDC_Calibration1_BUTTON,this);
	m_ButtonCalibration1.SetDownColor(RGB(255,255,224));
	m_ButtonCalibration1.SetUpColor(InitColor);
	m_ButtonCalibration1.SetTextColor(TextColor);

}

//工单号编辑框
void CMy10GAPDTESTDlg::OnEnChangeWorkordernoEdit()
{
	UpdateData(true);
}

//批次号编辑框
void CMy10GAPDTESTDlg::OnEnChangeBatchnoEdit()
{
	UpdateData(true);
}

//PD型号 编辑框
void CMy10GAPDTESTDlg::OnEnChangePdmodelEdit()
{
	UpdateData(true);
}

//系列号编辑框
void CMy10GAPDTESTDlg::OnEnChangeSnEdit()
{
	UpdateData(true);
}

//实际功率值 编辑框
void CMy10GAPDTESTDlg::OnEnChangePowervalEdit()
{
	UpdateData(true);
}

//获取主机当前可用串口
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

//连接各个设备，成功返回TRUE，失败返回FALSE
BOOL CMy10GAPDTESTDlg::DeviceConnet()
{
	/*CString ErrStr = _T("");
	ErrStr.Format(_T("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d"),
		ComNum[0],ComNum[1],ComNum[2],ComNum[3],ComNum[4],ComNum[5],ComNum[6],ComNum[7],
		ComNum[8],ComNum[9],ComNum[10],ComNum[11],ComNum[12],ComNum[13]);
	MessageBox(ErrStr);*/
	

	if(m_AitVoa->OpenDevice(ComNum,sizeof(ComNum)/sizeof(int)) == FALSE)
	{
		::MessageBox(NULL, _T("光衰减器连接失败，程序退出！！"), _T("连接错误"), MB_OK);
		return FALSE;
	}

	if(m_PM7501->OpenDevice(ComNum,sizeof(ComNum)/sizeof(int)) == FALSE)
	{
		::MessageBox(NULL, _T("光功率计连接失败，程序退出！！"), _T("连接错误"), MB_OK);
		return FALSE;
	}

	if(m_ApatVbr->OpenDevice(ComNum,sizeof(ComNum)/sizeof(int)) == FALSE)
	{
		::MessageBox(NULL, _T("VBR测试仪连接失败，程序退出！！"), _T("连接错误"), MB_OK);
		return FALSE;
	}

	if(m_Bert->OpenDevice(ComNum,sizeof(ComNum)/sizeof(int)) == FALSE)
	{
		::MessageBox(NULL, _T("误码仪连接失败，程序退出！！"), _T("连接错误"), MB_OK);
		return FALSE;
	}
	return TRUE;
}

//获取光校准的状态
void CMy10GAPDTESTDlg::GetCalState()
{
	CString  CalState = ReadIniFileStr("Config.ini","CAL","CalState");
	if(CalState == _T("PASS"))
	{
		m_fTanP_edit = ReadIniFileNum("Config.ini","CAL","TanP");
		CalStatePass();		//校准成功
		UpdateData(false);
	}
	
}

//重新进行光校准
void CMy10GAPDTESTDlg::SetCalState()
{
	WriteIniFileStr("Config.ini","CAL","CalState","FAIL");
	GetDlgItem(IDC_Calibration1_BUTTON)->SetWindowText(_T("开始光校准"));
	g_Calibration = FALSE;
	m_ButtonCalibration1.SetUpColor(FailColor);
	GetDlgItem(IDC_Calibration1_BUTTON)->Invalidate();
}

//校准成功
void CMy10GAPDTESTDlg::CalStatePass()
{
	GetDlgItem(IDC_Calibration1_BUTTON)->SetWindowText(_T("光校准成功"));
	g_Calibration = TRUE;
	m_ButtonCalibration1.SetUpColor(PassColor);
	GetDlgItem(IDC_Calibration1_BUTTON)->Invalidate();

	WriteIniFileStr("Config.ini","CAL","CalState","PASS");
	WriteIniFileNum("Config.ini","CAL","TanP",m_fTanP_edit);
}

//校准失败
void CMy10GAPDTESTDlg::CalStateFail()
{
	GetDlgItem(IDC_Calibration1_BUTTON)->SetWindowText(_T("光校准失败"));
	g_Calibration = FALSE;
	m_ButtonCalibration1.SetUpColor(FailColor);
	GetDlgItem(IDC_Calibration1_BUTTON)->Invalidate();
}

//开始光校准
void CMy10GAPDTESTDlg::OnBnClickedCalibration1Button()
{
	// TODO: 在此添加控件通知处理程序代码
	if(g_DeviceInit == FALSE)
	{
		::MessageBox(NULL,_T("请先进行设备初始化！"),_T("提示信息"),MB_OK);
		return ;
	}

	if(g_Calibration == TRUE)
	{
		INT_PTR nRes = MessageBox(_T("您是否确定重新进行校准？"), _T("提示"), MB_OKCANCEL | MB_ICONQUESTION);
		if (IDCANCEL == nRes)
		{
			return; 
		}
		SetCalState();
		return ;
	}

	GetDlgItem(IDC_Calibration1_BUTTON)->SetWindowText(_T("正在校准···"));

	if(SetPowerVal(m_fSenGoVal_edit) != 0)
	{
		MessageBox(_T("设置光功率计的 光功率值失败！"), _T("提示信息"), MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	float fCalPowerVal = 0.0f;	//读取到的光功率计当前的功率值

	if(m_PM7501->GetPower(fCalPowerVal) == FALSE)
	{
		MessageBox(_T("读取光功率计当前功率值失败！校准失败！"), _T("错误信息"), MB_OKCANCEL | MB_ICONQUESTION);
		CalStateFail();
		return ;
	}

	Calibration dlg;
	INT_PTR nResponse = dlg.DoModal();

	if(nResponse != 1)
	{
		MessageBox(_T("校准失败！"), _T("错误信息"), MB_OKCANCEL | MB_ICONQUESTION);
		CalStateFail();
		return;
	}
	m_fTanP_edit = fCalPowerVal - g_PowerVal;
	UpdateData(false);
	
	CalStatePass();		//校准成功
}

//“规格数据提取” 按钮消息函数
void CMy10GAPDTESTDlg::OnBnClickedExtractdataButton()
{
	// TODO: 在此添加控件通知处理程序代码

	if(m_strWorkOrderNo_edit == _T(""))
	{
		MessageBox(_T("请输入工单号！"), _T("提示信息"), MB_OK | MB_ICONEXCLAMATION);
		return ;
	}

	if(m_strBatchNo_edit == _T(""))
	{
		MessageBox(_T("请输入批次号！"), _T("提示信息"), MB_OK | MB_ICONEXCLAMATION);
		return ;
	}

	if(JudgeWorkOrderNo() == FALSE)
	{
		MessageBox(_T("工单号不正确，请重新输入！"), _T("提示信息"), MB_OK | MB_ICONEXCLAMATION);
		return ;
	}

	if(JudgeBatchNo() == FALSE)
	{
		MessageBox(_T("批次号不正确，请重新输入！"), _T("提示信息"), MB_OK | MB_ICONEXCLAMATION);
		return ;
	}

	GetDlgItem(IDC_ExtractData_BUTTON)->SetWindowTextA(_T("规格数据提取中······"));
	//if(GetModelSpec() == FALSE)	//获取测试规格
	if(FALSE == GetProductTestSpec_TS())
	{
		//MessageBox(_T("获取测试规格失败！"), _T("提示信息"), MB_OK | MB_ICONEXCLAMATION);
		GetDlgItem(IDC_ExtractData_BUTTON)->SetWindowTextA(_T("规格数据提取"));
		return ;
	}

	g_Extractdata = TRUE;
	GetDlgItem(IDC_ExtractData_BUTTON)->SetWindowText(_T("规格提取成功"));
	m_ButtonExtractData.SetUpColor(PassColor);
	GetDlgItem(IDC_ExtractData_BUTTON)->Invalidate();
}

//“设备初始化”按钮消息函数
void CMy10GAPDTESTDlg::OnBnClickedDeviceinitButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if(g_Extractdata == FALSE)
	{
		MessageBox(_T("规格数据未提取，请先提取规格数据！"), _T("提示信息"), MB_OKCANCEL | MB_ICONQUESTION);
		//return ;
	}

	GetDlgItem(IDC_DeviceInit_BUTTON)->SetWindowText(_T("正在初始化···"));

	//初始化误码仪
	int IntChannel   = m_Channel_Combo.GetCurSel();		//误码仪通道选择
	int IntType      = m_Type_Combo.GetCurSel();		//误码仪码型选择
	int IntRate	     = m_Rate_Combo.GetCurSel();		//误码仪速率选择
	int IntAmplitude = m_Amplitude_Combo.GetCurSel();	//误码仪幅度选择	
	
	if(m_Bert->InitDevice(IntChannel,IntType,IntRate,IntAmplitude) == FALSE)
	{
		MessageBox(_T("误码仪初始化失败！！"), _T("提示信息"), MB_OKCANCEL | MB_ICONQUESTION);
		goto FAILCODE;
	}

	//查询误码仪当前配置
	int IntQueType      = 0;	//误码仪码型选择
	int IntQueRate	    = 0;	//误码仪速率选择
	int IntQueAmplitude = 0;	//误码仪幅度选择

	if(m_Bert->QueryDeviceConfig(IntChannel, IntQueType, IntQueRate, IntQueAmplitude) == FALSE)	//查询误码仪配置失败
	{
		MessageBox(_T("查询误码仪初始化配置失败！！"), _T("提示信息"), MB_OKCANCEL | MB_ICONQUESTION);
		goto FAILCODE;
	}

	if(IntType != IntQueType || IntRate != IntQueRate || IntAmplitude != IntQueAmplitude)	//查询到的和所配置的不相同
	{
		MessageBox(_T("误码仪初始化与当前配置不一致！！"), _T("提示信息"), MB_OKCANCEL | MB_ICONQUESTION);
		goto FAILCODE;
	}


	//光功率初始化
	int IntPowerWaveLen = m_PowerWaveLen_Combo.GetCurSel();	//光功率计波长选择
	if(m_PM7501->SetWaveLength(IntPowerWaveLen) == FALSE)
	{
		MessageBox(_T("光功率计设置波长失败！！"), _T("提示信息"), MB_OKCANCEL | MB_ICONQUESTION);
		goto FAILCODE;
	}

	//光衰减器初始化
	int IntAttWaveLen = m_AttWaveLen_Combo.GetCurSel();	//光衰减器
	if(m_AitVoa->SetWaveLength(IntAttWaveLen) == FALSE)
	{
		MessageBox(_T("光衰减器设置波长失败！！"), _T("提示信息"), MB_OKCANCEL | MB_ICONQUESTION);
		goto FAILCODE;
	}

	if(SetPowerVal(m_fSenGoVal_edit) != 0)
	{
		MessageBox(_T("设置光功率计的 光功率值失败！"), _T("提示信息"), MB_OKCANCEL | MB_ICONQUESTION);
		goto FAILCODE;
	}

	//VBR测试仪初始化

	goto PASSCODE;

FAILCODE:
	g_DeviceInit = FALSE;
	GetDlgItem(IDC_DeviceInit_BUTTON)->SetWindowText(_T("初始化失败"));
	m_ButtonConfigInit.SetUpColor(FailColor);
	GetDlgItem(IDC_DeviceInit_BUTTON)->Invalidate();
	return ;

PASSCODE:
	g_DeviceInit = TRUE;
	GetDlgItem(IDC_DeviceInit_BUTTON)->SetWindowText(_T("初始化成功"));
	m_ButtonConfigInit.SetUpColor(PassColor);
	GetDlgItem(IDC_DeviceInit_BUTTON)->Invalidate();
	return ;
}

//“开启批量测试”按钮消息函数
void CMy10GAPDTESTDlg::OnBnClickedBegintestButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if(g_Calibration == FALSE)
	{
		MessageBox(_T("光校准未成功，请先进行光校准！"), _T("提示信息"), MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(g_Extractdata == FALSE)
	{
		MessageBox(_T("规格数据未提取，请先提取规格数据！"), _T("提示信息"), MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(g_DeviceInit == FALSE)
	{
		MessageBox(_T("设备未初始化，请先完成设备初始化！"), _T("提示信息"), MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(g_BeginTest == TRUE)
	{
		StopTest();		//停止批量测试
		return ;
	}

	BeginTest();		//开启批量测试
}

//时间记录线程
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
			g_TimeEnd = GetTickCount();	//取得当前系统运行时间(ms)	
			Timestr.Format("%d",(g_TimeEnd - g_TimeBegin)/1000);
			obj->GetDlgItem(IDC_TESTTIME_STATIC)->SetWindowText(Timestr);
		}
		Sleep(1000);
	}

	return 0;
}

//调测线程
UINT CMy10GAPDTESTDlg::ThreadTest( LPVOID lParam )
{
	CMy10GAPDTESTDlg * obj = ( CMy10GAPDTESTDlg* )lParam;

	while(1)
	{
		if(g_ThreadExit == TRUE)
		{
			return 0;
		}

		if(g_BeginTest == FALSE)				//检测是否已开启批量测试
		{
			Sleep(100);
			continue;
		}

		obj->m_ctrlProgress.SetPos(0);			//设置进度条的当前位置

		if(obj->m_strSn_edit == _T(""))			//检测是否已输入SN号
		{
			obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("请输入SN号"));
			Sleep(1000);
			continue;
		}

		if(obj->m_strPDModel_edit == _T(""))	//检测是否已输入PD型号
		{
			obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("请输入PD型号"));
			Sleep(200);
			continue;
		}
		obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T(""));
		
		if(obj->JudgeSN() == FALSE)				//判断输入的SN号是否正确
		{
			obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("SN号不存在"));
			continue;
		}
		obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T(""));
			
		if(obj->JudgeSNtoWorkOrder() == FALSE)	//判断输入的SN号是否属于当前工单号
		{
			obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("SN号不属于工单号"));
			continue;
		}
		obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T(""));

		if(obj->JudgeSNtoBatch() == FALSE)		//判断输入的SN号是否属于当前批次号
		{
			obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("SN号不属于批次号"));
			continue;
		}
		obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T(""));

		if ( BST_CHECKED == obj->IsDlgButtonChecked(IDC_CHECK1) )	//流转系统是否已勾选，若已勾选则检查工序流程
		{
			if(obj->JudgeStep(obj->m_strSn_edit) == FALSE)			//判断该器件是否属于当前工序（测包）
			{
				continue;
			}
		}

		obj->GridInit();	//初始化表格

		if(obj->GetTestInfoToWorkOrder() == FALSE)	//获取当前工单的测试信息,（已测试数量、良品率 等）
		{
			obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("统计信息获取失败！"));
			Sleep(500);
			continue;
		}


		obj->BeginDeviceTest();			//器件开始测试
		

		//obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("正在进行Icc测试···"));
		//if(obj->IccTest() == FALSE)		// Icc测试 读取Icc值
		//{
		//	obj->TestResultsDeal();		//测试结果处理
		//	continue;
		//}

		obj->m_ctrlProgress.SetPos(20);		//设置进度条的当前位置

		obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("正在进行Vbr测试···"));
		if(g_IntModelState == 0)			//10G APD器件测试
		{
			if(obj->VbrTest() == FALSE)		// VBR测试 读取Vbr值和Id值
			{
				obj->TestResultsDeal();		//测试结果处理
				continue;
			}
		}
		else								//10G-PIN-TIA 测试
		{
			if(obj->ApdVccOutPut(TRUE) == FALSE)//开启APD、VCC电压输出
			{
				obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("开启APD、VCC电压输出失败"));
				obj->TestResultsDeal();		//测试结果处理
				continue;
			}
		}

		obj->m_ctrlProgress.SetPos(40);	//设置进度条的当前位置

		obj->GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("正在进行Sen测试···"));
		if(obj->SenTest() == FALSE)		// Sen测试
		{
			obj->TestResultsDeal();		//测试结果处理
			continue;
		}

		obj->m_ctrlProgress.SetPos(90);	//设置进度条的当前位置

		if(obj->SaveTestDataAccess() == FALSE)		//测试数据保存在本地
		{
			obj->MessageBox(_T("保存测试数据在本地失败！！"), _T("错误提示"), MB_OKCANCEL | MB_ICONQUESTION);
			obj->TestFail();
			continue;
		}

		if(obj->SaveTestDataServer() == FALSE)		//测试数据保存在服务器
		{
			obj->MessageBox(_T("保存测试数据在服务器失败！！"), _T("错误提示"), MB_OKCANCEL | MB_ICONQUESTION);
			obj->TestFail();
			continue;
		}

		obj->TestPass();					//器件测试成功

		obj->m_ctrlProgress.SetPos(100);	//设置进度条的当前位置

		Sleep(500);
	}

	return 0;
}

//进行Icc测试，读取Icc的值。（无光条件下）
BOOL CMy10GAPDTESTDlg::IccTest()
{
	if(m_AitVoa->SetAttVal(60.0f) == FALSE)	//首先设置光衰减器的衰减值为60dbm
	{
		m_Method.WriteGridCtrOne(m_GridTS,2,1,_T("FAIL"),RGB(0, 0, 0));
		m_Method.OnCellModel(m_GridTS,2,1,1,FailColor);	//单元格颜色与模式
		m_Method.WriteGridCtrOne(m_GridTS,2,2,_T("设置光衰减器的衰减值失败"),RGB(100, 0, 0));
		TestFail();	
		return FALSE;
	}
	
	float fIccVal = 0.0f;	//进行Icc测试后得到的Icc值

	DWORD RetuVal = m_ApatVbr->IccTestResult(fIccVal);

	if(RetuVal == 0)
	{
		CString strIccVal = _T("");
		strIccVal.Format("%0.2f",fIccVal);
		m_Method.WriteGridCtrOne(m_GridTS,2,1,_T("PASS"),RGB(0, 0, 0));
		m_Method.OnCellModel(m_GridTS,2,1,1,PassColor);					//单元格颜色与模式
		m_Method.WriteGridCtrOne(m_GridTS,2,2,strIccVal,RGB(100, 0, 0));

		if(fIccVal < m_fIccMin_edit)						//读取到的 Icc 小于最小门限值
		{
			m_Method.OnCellModel(m_GridTS,3,1,1,FailColor);	//单元格颜色与模式
			g_FailType     = _T("Icc小");	
			g_FailItemCode = 114;
			TestFail();										//器件测试失败
			return FALSE;
		}
		if(fIccVal > m_fIccMax_edit)						//读取到的 Icc 大于最大门限值
		{
			m_Method.OnCellModel(m_GridTS,3,2,1,FailColor);	//单元格颜色与模式
			g_FailType     = _T("Icc大");	
			g_FailItemCode = 115;
			TestFail();										//器件测试失败
			return FALSE;
		}
		
		Data->m_IccVal = fIccVal;
		return TRUE;
	}
	else
	{
		m_Method.WriteGridCtrOne(m_GridTS,2,1,_T("FAIL"),RGB(0, 0, 0));
		m_Method.OnCellModel(m_GridTS,2,1,1,FailColor);	//单元格颜色与模式

		if(RetuVal == 1)
		{
			m_Method.WriteGridCtrOne(m_GridTS,2,2,_T("选择第5工作模式失败"),RGB(100, 0, 0));
		}
		else if(RetuVal == 2)
		{
			m_Method.WriteGridCtrOne(m_GridTS,2,2,_T("开启VAPD电压输出失败"),RGB(100, 0, 0));
		}
		else if(RetuVal == 3)
		{
			m_Method.WriteGridCtrOne(m_GridTS,2,2,_T("开启VCC电压输出失败"),RGB(100, 0, 0));
		}
		else if(RetuVal == 4)
		{
			m_Method.WriteGridCtrOne(m_GridTS,2,2,_T("读取Icc值（Vcc电流值失败"),RGB(100, 0, 0));
		}
		else if(RetuVal == 5)
		{
			m_Method.WriteGridCtrOne(m_GridTS,2,2,_T("关闭VAPD电压输出失败"),RGB(100, 0, 0));
		}
		else
		{
			m_Method.WriteGridCtrOne(m_GridTS,2,2,_T("关闭VCC电压输出失败"),RGB(100, 0, 0));
		}
		
		TestFail();		//器件测试失败
		return FALSE;
	}
}

//进行VBR测试，读取 Id 值、 Vbr 值、Icc 值
BOOL CMy10GAPDTESTDlg::VbrTest()
{
	
	if(m_AitVoa->SetAttVal(60.0f) == FALSE)	//首先设置光衰减器的衰减值为60dbm
	{
		m_Method.WriteGridCtrOne(m_GridTS,2,1,_T("FAIL"),RGB(0, 0, 0));
		m_Method.OnCellModel(m_GridTS,2,1,1,FailColor);	//单元格颜色与模式
		m_Method.WriteGridCtrOne(m_GridTS,2,2,_T("设置光衰减器的衰减值失败"),RGB(100, 0, 0));
		TestFail();	
		return FALSE;
	}

	float fVbrVal = 0.0f;	//VBR 测试成功后得到的 Vbr 值
	float fIdVal  = 0.0f;	//VBR 测试成功后得到的 Id 值
	float fIccVal = 0.0f;	//进行Icc测试后得到的Icc值

	DWORD RetuVal = m_ApatVbr->VbrTestResult(fVbrVal,fIdVal,fIccVal);	//Vbr测试

	if(RetuVal != 0)
	{
		m_Method.WriteGridCtrOne(m_GridTS,2,1,_T("FAIL"),RGB(0, 0, 0));
		m_Method.OnCellModel(m_GridTS,2,1,1,FailColor);	//单元格颜色与模式

		m_Method.WriteGridCtrOne(m_GridTS,4,1,_T("FAIL"),RGB(0, 0, 0));
		m_Method.OnCellModel(m_GridTS,4,1,1,FailColor);	//单元格颜色与模式

		m_Method.WriteGridCtrOne(m_GridTS,6,1,_T("FAIL"),RGB(0, 0, 0));
		m_Method.OnCellModel(m_GridTS,6,1,1,FailColor);	//单元格颜色与模式

		if(RetuVal == 1)
		{
			m_Method.WriteGridCtrOne(m_GridTS,4,2,_T("选择第Vbr模式失败"),RGB(0, 0, 0));
		}
		else if(RetuVal == 2)
		{
			m_Method.WriteGridCtrOne(m_GridTS,4,2,_T("开启VAPD电压输出失"),RGB(0, 0, 0));
		}
		else if(RetuVal == 3)
		{
			m_Method.WriteGridCtrOne(m_GridTS,4,2,_T("开启VCC电压输出失败"),RGB(0, 0, 0));
		}
		else if(RetuVal == 4)
		{
			m_Method.WriteGridCtrOne(m_GridTS,4,2,_T("设置面板进入VBR测试界面失败"),RGB(0, 0, 0));
		}
		else if(RetuVal == 5)
		{
			m_Method.WriteGridCtrOne(m_GridTS,4,2,_T("开启 VBR 测试失败"),RGB(0, 0, 0));
		}
		else if(RetuVal == 6)
		{
			m_Method.WriteGridCtrOne(m_GridTS,4,2,_T("读取Vbr值（Vbr电压值）失败"),RGB(0, 0, 0));
		}
		else
		{
			m_Method.WriteGridCtrOne(m_GridTS,4,2,_T("读取Id值（VAPD电流值）失败"),RGB(0, 0, 0));
		}
		return FALSE;
	}

	CString strVal = _T("");

	strVal.Format("%0.2f",fIccVal);
	m_Method.WriteGridCtrOne(m_GridTS,2,1,_T("PASS"),RGB(0, 0, 0));
	m_Method.OnCellModel(m_GridTS,2,1,1,PassColor);					//单元格颜色与模式
	m_Method.WriteGridCtrOne(m_GridTS,2,2,strVal,RGB(100, 0, 0));

	strVal.Format("%0.2f",fIdVal);
	m_Method.WriteGridCtrOne(m_GridTS,4,1,_T("PASS"),RGB(0, 0, 0));
	m_Method.OnCellModel(m_GridTS,4,1,1,PassColor);					//单元格颜色与模式
	m_Method.WriteGridCtrOne(m_GridTS,4,2,strVal,RGB(100, 0, 0));

	strVal.Format("%0.2f",fVbrVal);
	m_Method.WriteGridCtrOne(m_GridTS,6,1,_T("PASS"),RGB(0, 0, 0));
	m_Method.OnCellModel(m_GridTS,6,1,1,PassColor);					//单元格颜色与模式
	m_Method.WriteGridCtrOne(m_GridTS,6,2,strVal,RGB(100, 0, 0));


	if(fIccVal < m_fIccMin_edit)						//读取到的 Icc 小于最小门限值
	{
		m_Method.OnCellModel(m_GridTS,3,1,1,FailColor);	//单元格颜色与模式
		g_FailType     = _T("Icc小");	
		g_FailItemCode = 114;
		return FALSE;
	}
	if(fIccVal > m_fIccMax_edit)						//读取到的 Icc 大于最大门限值
	{
		m_Method.OnCellModel(m_GridTS,3,2,1,FailColor);	//单元格颜色与模式
		g_FailType     = _T("Icc大");	
		g_FailItemCode = 115;
		return FALSE;
	}

	if(fIdVal < m_fIdMin_edit)	//读取到的 Id 小于最小门限值
	{
		m_Method.OnCellModel(m_GridTS,5,1,1,FailColor);		//单元格颜色与模式
		g_FailType     = _T("Id小");	
		g_FailItemCode = 116;
		return FALSE;
	}
	if(fIdVal > m_fIdMax_edit)	//读取到的 ID 大于最大门限值
	{
		m_Method.OnCellModel(m_GridTS,5,2,1,FailColor);		//单元格颜色与模式
		g_FailType     = _T("Id大");	
		g_FailItemCode = 117;
		return FALSE;
	}

	if(fVbrVal < m_fVbrMin_edit)	//读取到的 Vbr 小于最小门限值
	{
		m_Method.OnCellModel(m_GridTS,7,1,1,FailColor);		//单元格颜色与模式
		g_FailType     = _T("VBR偏小");	
		g_FailItemCode = 112;
		return FALSE;
	}
	if(fVbrVal > m_fVbrMax_edit)	//读取到的 Vbr 大于最大门限值
	{
		m_Method.OnCellModel(m_GridTS,7,2,1,FailColor);		//单元格颜色与模式
		g_FailType     = _T("VBR偏大");	
		g_FailItemCode = 113;
		return FALSE;
	}

	Data->m_IccVal = fIccVal;	//Icc
	Data->m_IdVal	= fIdVal;	//Id值
	Data->m_VbrVal	= fVbrVal;	//Vbr值
	Data->m_VbrTestPass = 1;	//Vbr测试结果（测试成功）

	return TRUE;
}

//灵敏度测试，成功返回 TRUE， 失败返回 FALSE
BOOL CMy10GAPDTESTDlg::SenTest()
{
	float x_Data[10] = {0};		//观察点x轴
	float y_Data[10] = {0};		//观察点y轴
	float Coefficient[10] = {0};//拟合计算的到的系数，分别是 a、b、c、等等

	CString strTemp = _T("");

	if(ObservationPoints(x_Data, y_Data, 5) == FALSE)//查找 5 个观察点 
	{
		return FALSE;
	}

	m_ctrlProgress.SetPos(70);			

	if(FittTest(x_Data, y_Data, Coefficient, 1) != 0)//对观察点进行拟合计算 
	{
		return FALSE;
	}

	m_ctrlProgress.SetPos(80);		

	strTemp.Format("%f",Coefficient[0]);
	m_Method.WriteGridCtrOne(m_GridFitt, 7, 1, strTemp, RGB(100, 0, 0));//保存系数a到表格中

	strTemp.Format("%f",Coefficient[1]);
	m_Method.WriteGridCtrOne(m_GridFitt, 8, 1, strTemp, RGB(100, 0, 0));//保存系数b到表格中

	float fSenVal = 0.0f;	//公式计算后得到的灵敏度 
	float fBerVal = 0.0f;	//选择的误码率经公式转化后得到的值

	fBerVal = (float)log10(-log10(atof(m_strVbrVal_edit)));
	//int IntBer = m_VbrVal_Combo.GetCurSel();	//获取选择的误码率规格
	//if(IntBer == 0)	//选取的误码率为10E-3
	//{
	//	fBerVal = (float)log10(-log10(1.0E-3));
	//}
	//else if(IntBer == 1)	//选取的误码率为10E-10
	//{
	//	fBerVal = (float)log10(-log10(1.0E-10));
	//}
	//else
	//{
	//	fBerVal = (float)log10(-log10(1.0E-12));
	//}

	fSenVal = ((fBerVal - Coefficient[1]) / Coefficient[0]) + m_CompenVal_edit;	//加补偿值

	Data->m_SenVal = fSenVal;	//灵敏度值

	strTemp.Format("%f",fSenVal);
	m_Method.WriteGridCtrOne(m_GridFitt, 9, 1, strTemp, RGB(100, 0, 0));

	m_Method.WriteGridCtrOne(m_GridTS,8,1,_T("PASS"),RGB(0, 0, 0));
	m_Method.OnCellModel(m_GridTS,8,1,1,PassColor);	//单元格颜色与模式
	m_Method.WriteGridCtrOne(m_GridTS,8,2,strTemp,RGB(0, 0, 0));

	if(fSenVal < m_fSenMin_edit)	//灵敏度小于最小门限值
	{
		m_Method.OnCellModel(m_GridTS,9,1,1,FailColor);		//单元格颜色与模式
		g_FailType     = _T("SEN小");	
		g_FailItemCode = 107;
		return FALSE;
	}

	if(fSenVal > m_fSenMax_edit)	//灵敏度大于最大门限值
	{
		m_Method.OnCellModel(m_GridTS,9,2,1,FailColor);		//单元格颜色与模式
		g_FailType     = _T("SEN大");	
		g_FailItemCode = 108;
		return FALSE;
	}

	Data->m_SenTestPass = 1;		//灵敏度测试结果（测试成功）

	return TRUE;
}

//通过调节光衰减器的衰减值，来设置光功率计的 光功率值(器件的接收光 = 设置值 - TanP)
DWORD CMy10GAPDTESTDlg::SetPowerVal(float PowerVal)
{
	float fPowerValBef = 0.0f;	//在调节光衰减器前 读取到的光功率值
	float fAttValBef   = 0.0f;	//在调节光衰减器前 读取到的光衰减值值
	int IntWaveLen = 0;			//当前光衰减器的波长
	float AttValSetp = 0.0f;	//衰减器变化的步长

	for(int i = 0; i < 10; i++)
	{
		if(m_PM7501->GetPower(fPowerValBef) == FALSE)	//获取光功率值
		{
			return 1;
		}
		
		if(m_AitVoa->GetWaveLengthAtt(IntWaveLen,fAttValBef) == FALSE)	//获取光衰减值
		{
			return 2;
		}

		if(fPowerValBef < PowerVal + 0.15 && fPowerValBef > PowerVal - 0.15)
		{
			break;
		}

		if(fabs(PowerVal - fPowerValBef) > 20)		//当前光功率值与目标光功率值差值大于20时步长为20
		{
			AttValSetp = 20.0f;
		}
		else if(fabs(PowerVal - fPowerValBef) > 10)	//当前光功率值与目标光功率值差值大于10时步长为10
		{
			AttValSetp = 10.0f;
		}
		else if(fabs(PowerVal - fPowerValBef) > 5)	//当前光功率值与目标光功率值差值大于5时步长为5
		{
			AttValSetp = 5.0f;
		}
		else if(fabs(PowerVal - fPowerValBef) > 1)	//当前光功率值与目标光功率值差值大于1时步长为1
		{
			AttValSetp = 1.0f;
		}
		else if(fabs(PowerVal - fPowerValBef) > 0.5)	//步长为0.5
		{
			AttValSetp = 0.50f;
		}
		else if(fabs(PowerVal - fPowerValBef) > 0.2)	//步长为0.2
		{
			AttValSetp = 0.20f;
		}
		else
		{
			AttValSetp = 0.10f;
		}
		
		if(fPowerValBef < PowerVal) //当前光功率值小于目标光功率值，需减小衰减值
		{
			if(m_AitVoa->SetAttVal(fAttValBef - AttValSetp) == FALSE)
			{
				return 3;
			}
		}

		if(fPowerValBef > PowerVal) //当前光功率值大于目标光功率值，需增大衰减值
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

//获取当前光功率下的同、失步状态和误码数量，成功返回 TRUE 
BOOL CMy10GAPDTESTDlg::GetBitErrState(int Channel, int & Sync, INT64 & BitErrNum)
{
	double dBer			 = 0.0;	//查询成功后得到的误码率

	if(m_Bert->BeginBitErrTest(Channel) == FALSE)	//开启误码测试
	{
		MessageBox(_T("开启误码测试失败！"));
		return FALSE;	
	}
	Sleep(1000);	//误码测试时间
	if(m_Bert->QueryBitErrState(Channel, Sync, BitErrNum, dBer) == FALSE)//查询误码数据
	{
		MessageBox(_T("查询误码数据失败！"));
		return FALSE;
	}
	
	if(m_Bert->StopBitErrTest(Channel) == FALSE)		//停止误码检测
	{
		MessageBox(_T("停止误码检测失败！"));
		return FALSE;
	}
	return TRUE;
}

//获取当前产品灵敏度测试光功率起始值,成功返回 TRUE
BOOL CMy10GAPDTESTDlg::GetSenTestBeginPowVal(float & fBeginPowVal)
{
	int IntChannel   = m_Channel_Combo.GetCurSel();		//误码仪通道选择
	int IntSync          = 0;	//查询成功后得到的同步、失步状态，为0时表示同步，1表示失步,2表示有误码
	INT64 Int64BitErrNum = 0;	//查询成功后得到的误码数

	if(SetPowerVal(m_fSenGoVal_edit + m_fTanP_edit) != 0)// 调节器件接收光功率到目标值
	{
		MessageBox(_T("调节器件接收光功率到目标值失败！"));
		return FALSE;
	}
	if(FALSE == GetBitErrState(IntChannel, IntSync,Int64BitErrNum))
	{
		return FALSE;
	}

	if(IntSync == 1)				//失步状态
	{
		for(int i = 1; i < 6; i++)
		{
			if(SetPowerVal(m_fSenGoVal_edit + i * m_PowStep1 + m_fTanP_edit) != 0)// 调节器件接收光功率到目标值
			{
				MessageBox(_T("调节器件接收光功率到目标值失败1！"));
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
		return FALSE;//找不到起始点
	}
	else if(Int64BitErrNum == 0)	//非失步状态且误码率为0
	{
		int i = 1;
		while(1)
		{
			if(SetPowerVal(m_fSenGoVal_edit - i * m_PowStep2 + m_fTanP_edit) != 0)// 调节器件接收光功率到目标值
			{
				MessageBox(_T("调节器件接收光功率到目标值失败2！"));
				return FALSE;
			}
			if(FALSE == GetBitErrState(IntChannel, IntSync,Int64BitErrNum))
			{
				return FALSE;
			}

			if(IntSync == 1)	//失步
			{
				return FALSE;
			}

			if(Int64BitErrNum > 0)	//有误码产生
			{
				fBeginPowVal = m_fSenGoVal_edit - i * m_PowStep1;
				return TRUE;
			}
		}
	}

	fBeginPowVal = m_fSenGoVal_edit;
	return TRUE;
}

//获取观察点,成功返回TRUE，失败返回FALSE
BOOL CMy10GAPDTESTDlg::ObservationPoints(float x_Data[], float y_Data[], int IntNum)
{
	int IntChannel   = m_Channel_Combo.GetCurSel();		//误码仪通道选择

	//首先找到 5 个观察点
	int IntSync          = 0;	//查询成功后得到的同步、失步状态，为0时表示同步，1表示失步,2表示有误码
	INT64 Int64BitErrNum = 0;	//查询成功后得到的误码数
	double dBer			 = 0.0;	//查询成功后得到的误码率

	if(FALSE == GetSenTestBeginPowVal(m_SenTestBeginPowVal))//查找当前产品灵敏度测试光功率起始值
	{
		m_Method.WriteGridCtrOne(m_GridTS,8,1,_T("FAIL"),RGB(0, 0, 0));
		m_Method.OnCellModel(m_GridTS,8,1,1,FailColor);	//单元格颜色与模式
		m_Method.WriteGridCtrOne(m_GridTS,8,2,_T("获取灵敏度测试起始光功率值失败"),RGB(0, 0, 0));
		return FALSE;
	}

	for(int i = 0; i < IntNum; i++)
	{
		if(SetPowerVal(m_SenTestBeginPowVal - m_fSenStep_edit * i + m_fTanP_edit) != 0)// 调节器件接收光功率到目标值
		{
			m_Method.WriteGridCtrOne(m_GridTS,8,1,_T("FAIL"),RGB(0, 0, 0));
			m_Method.OnCellModel(m_GridTS,8,1,1,FailColor);	//单元格颜色与模式
			m_Method.WriteGridCtrOne(m_GridTS,8,2,_T("调节器件接收光功率到目标值失败"),RGB(0, 0, 0));
			return FALSE;
		}

		Sleep(100);

		float fPowerVal = 0.0f;	//调节后读取到的光功率值
		if(m_PM7501->GetPower(fPowerVal) == FALSE)	//获取光功率值
		{
			m_Method.WriteGridCtrOne(m_GridTS,8,1,_T("FAIL"),RGB(0, 0, 0));
			m_Method.OnCellModel(m_GridTS,8,1,1,FailColor);	//单元格颜色与模式
			m_Method.WriteGridCtrOne(m_GridTS,8,2,_T("获取光功率值失败"),RGB(0, 0, 0));
			return FALSE;
		}

		CString strTemp = _T("");
		strTemp.Format(_T("%0.2f"),fPowerVal - m_fTanP_edit);
		m_Method.WriteGridCtrOne(m_GridFitt, i + 1,0, strTemp, RGB(100, 0, 0));
		x_Data[i] = fPowerVal - m_fTanP_edit;

		if(m_Bert->BeginBitErrTest(IntChannel) == FALSE)	//开启误码测试
		{
			m_Method.WriteGridCtrOne(m_GridTS,8,1,_T("FAIL"),RGB(0, 0, 0));
			m_Method.OnCellModel(m_GridTS,8,1,1,FailColor);	//单元格颜色与模式
			m_Method.WriteGridCtrOne(m_GridTS,8,2,_T("开启误码测试失败"),RGB(0, 0, 0));
			return FALSE;	
		}

		Sleep(m_SenTime_edit * 1000);	//误码测试时间

		if(m_Bert->QueryBitErrState(IntChannel, IntSync, Int64BitErrNum, dBer) == FALSE)//查询误码数据
		{
			m_Method.WriteGridCtrOne(m_GridTS,8,1,_T("FAIL"),RGB(0, 0, 0));
			m_Method.OnCellModel(m_GridTS,8,1,1,FailColor);	//单元格颜色与模式
			m_Method.WriteGridCtrOne(m_GridTS,8,2,_T("查询误码数据失败"),RGB(0, 0, 0));
			return FALSE;
		}

		if(IntSync == 1)		//失步状态误码率为0
		{
			m_Method.WriteGridCtrOne(m_GridTS,8,1,_T("FAIL"),RGB(0, 0, 0));
			m_Method.OnCellModel(m_GridTS,8,1,1,FailColor);	
			m_Method.WriteGridCtrOne(m_GridTS,8,2,_T("误码仪处于失步状态"),RGB(0, 0, 0));
			return FALSE;
		}
		
		if(m_Bert->StopBitErrTest(IntChannel) == FALSE)		//停止误码检测
		{
			m_Method.WriteGridCtrOne(m_GridTS,8,1,_T("FAIL"),RGB(0, 0, 0));
			m_Method.OnCellModel(m_GridTS,8,1,1,FailColor);	//单元格颜色与模式
			m_Method.WriteGridCtrOne(m_GridTS,8,2,_T("停止误码检测失败"),RGB(0, 0, 0));
			return FALSE;
		}
		
		strTemp.Format(_T("%0.8lf"),dBer);
		m_Method.WriteGridCtrOne(m_GridFitt, i + 1,1, strTemp, RGB(100, 0, 0));

		y_Data[i] = (float)log10(-log10(dBer));
	}

	return TRUE;
}

//根据观察点数据，进行直线拟合计算
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

	Coefficient[0] = C[1];	//系数 a
	Coefficient[1] = C[0];	//系数 b

	return 0;
}

//判断工单号是否存在，存在则返回TRUE
BOOL CMy10GAPDTESTDlg::JudgeWorkOrderNo()
{
	CString strCmd = _T("");	//Sql语句字符串
	strCmd.Format(_T("select count(*) as Number from Manufacture_info where Manufacture_ID = '%s'"),
				  m_strWorkOrderNo_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//
	{
		MessageBox(_T("数据库工单号查询失败！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	CString strNumber = g_SqlCon->Result("Number");

	int IntNumber = _ttoi(strNumber);

	if(IntNumber == 0)
		return FALSE;

	return TRUE;
}

//判断批次号是否存在(确认工单号存在后)，存在则返回TRUE
BOOL CMy10GAPDTESTDlg::JudgeBatchNo()
{
	CString strCmd = _T("");	//Sql语句字符串
	strCmd.Format(_T("select count(*) as Number from dbo.testdata where Manufacture_ID = '%s' AND Batch_ID = %.0f"),
				  m_strWorkOrderNo_edit,atof(m_strBatchNo_edit));
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//
	{
		MessageBox(_T("数据库批次号查询失败！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	CString strNumber = g_SqlCon->Result("Number");

	int IntNumber = _ttoi(strNumber);

	if(IntNumber == 0)
		return FALSE;

	return TRUE;
}

//判断输入的SN号是否正确，存在则返回TRUE
BOOL CMy10GAPDTESTDlg::JudgeSN()
{
	CString strCmd = _T("");	//Sql语句字符串
	strCmd.Format(_T("select count(*) as Number from dbo.testdata where Serial_No = '%s'"),m_strSn_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//
	{
		//MessageBox(_T("数据库批次号查询失败！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	CString strNumber = g_SqlCon->Result("Number");

	int IntNumber = _ttoi(strNumber);

	if(IntNumber == 0)
	{
		MessageBox(_T("SN号不存在，请重新输入！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
		StopTest();		//停止测试
		return FALSE;
	}

	return TRUE;
}

//判断SN号是否属于该工单号(确认工单号存在后)，存在则返回TRUE
BOOL CMy10GAPDTESTDlg::JudgeSNtoWorkOrder()
{
	CString strCmd = _T("");	//Sql语句字符串
	strCmd.Format(_T("select count(*) as Number from dbo.testdata where Manufacture_ID = '%s' AND Serial_No = '%s'"),
		m_strWorkOrderNo_edit,m_strSn_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//查
	{
		//MessageBox(_T("数据库批次号查询失败！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	CString strNumber = g_SqlCon->Result("Number");

	int IntNumber = _ttoi(strNumber);

	if(IntNumber == 0)
	{
		MessageBox(_T("该SN号不属于当前工单号，请重新输入工单号或 SN 号！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
		StopTest();		//停止测试

		g_Extractdata = FALSE;
		GetDlgItem(IDC_ExtractData_BUTTON)->SetWindowText(_T("规格数据提取"));
		m_ButtonExtractData.SetUpColor(FailColor);
		GetDlgItem(IDC_ExtractData_BUTTON)->Invalidate();
		return FALSE;
	}

	return TRUE;
}

//判断SN号是否属于该批次号(确认工单号、批次号存在后)，存在则返回TRUE
BOOL CMy10GAPDTESTDlg::JudgeSNtoBatch()
{
	CString strCmd = _T("");	//Sql语句字符串
	strCmd.Format(_T("select count(*) as Number from dbo.testdata where Manufacture_ID = '%s' AND Batch_ID = %.0f\
		AND Serial_No = '%s'"),m_strWorkOrderNo_edit,atof(m_strBatchNo_edit),m_strSn_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//
	{
		//MessageBox(_T("数据库批次号查询失败！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	CString strNumber = g_SqlCon->Result("Number");

	int IntNumber = _ttoi(strNumber);

	if(IntNumber == 0)
	{
		MessageBox(_T("该SN号不属于当前批次号，请重新输入批次号！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
		StopTest();		//停止测试
		g_Extractdata = FALSE;
		GetDlgItem(IDC_ExtractData_BUTTON)->SetWindowText(_T("规格数据提取"));
		m_ButtonExtractData.SetUpColor(FailColor);
		GetDlgItem(IDC_ExtractData_BUTTON)->Invalidate();
		return FALSE;
	}

	return TRUE;
}

//获取所属 输入的工单号与批次号下的 SN 号,成功返回 TRUE
BOOL CMy10GAPDTESTDlg::GetBatchToSN(CString Serial_No[])
{
	CString strCmd = _T("");	//Sql语句字符串
	strCmd.Format(_T("select Serial_No from dbo.testdata where Manufacture_ID = '%s' AND Batch_ID = '%0.f'"),
		m_strWorkOrderNo_edit,atof(m_strBatchNo_edit));
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//
	{
		//MessageBox(_T("数据库批次号查询失败！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
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

//获取工单所属型号的测试规格 成功返回 TRUE
BOOL CMy10GAPDTESTDlg::GetModelSpec()
{
	CString strCmd = _T("");	//Sql语句字符串
	strCmd.Format(_T("select model from Manufacture_info where Manufacture_ID = '%s'"),
		m_strWorkOrderNo_edit);			
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//获取型号
	{
		MessageBox(_T("数据库型号查询失败！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}
	m_strModel_edit = g_SqlCon->Result("model");	//获取型号
	UpdateData(false);

////////////////////////////////////////获取规格//////////////////////////////////////////////////////////

	strCmd.Format(_T("select Icc_Min,Icc_Max,Id_Min,Id_Max,Vbr_Min,Vbr_Max,SEN_Min,SEN_Max,Wavelength,Rx_inputWL,\
		SEN_Time,SEN_BER,SEN_DATA_RATE,SEN_Data_Pattern,SEN_Work_MODE,SEN_VOP,SEN_PM_STEP,Siganl_Range from \
		dbo.testsetup where model = '%s'"),m_strModel_edit);			
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//获取规格
	{
		MessageBox(_T("数据库器件规格查询失败！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
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

	int IntWaveLen = g_SqlCon->Result.Convertor("Rx_inputWL").ToInteger();	//接收波长
	if(IntWaveLen == 1310)
	{
		m_AttWaveLen_Combo.SetCurSel(0);		//光衰减器
		m_PowerWaveLen_Combo.SetCurSel(3);		//光功率计
	}
	else if(IntWaveLen == 1550)
	{
		m_AttWaveLen_Combo.SetCurSel(1);
		m_PowerWaveLen_Combo.SetCurSel(5);
	}
	else
	{
		MessageBox(_T("波长设置错误，将默认选择 1310 的波长！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
	}

	int IntRange = 0;
	IntRange = (int)g_SqlCon->Result.Convertor("Siganl_Range").ToDouble();	//误码仪幅度选择
	m_Amplitude_Combo.SetCurSel(IntRange);

	m_strVbrVal_edit = g_SqlCon->Result("SEN_BER");		//误码率选择
	UpdateData(FALSE);

	strTemp = g_SqlCon->Result("SEN_DATA_RATE");	//速率选择
	int IntRate = _ttoi(strTemp);

	if(IntRate < 8)
	{
		IntRate = 8;
		MessageBox(_T("速率设置错误，将默认选择 9.95G 的速率！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
	}
	m_Rate_Combo.SetCurSel(IntRate - 8);

	strTemp = g_SqlCon->Result("SEN_Data_Pattern");	//码型选择
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
		MessageBox(_T("码型设置错误，将默认选择 PRBS7 的码型！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
	}

	strTemp = g_SqlCon->Result("SEN_Work_MODE");	//工作模式选择(光、电通道)
	int IntMode = _ttoi(strTemp);
	if(IntMode == 1)		//光通道
	{
		m_Channel_Combo.SetCurSel(1);
	}
	else if(IntMode == 2)	//电通道
	{
		m_Channel_Combo.SetCurSel(0);
	}
	
	strTemp = g_SqlCon->Result("SEN_VOP");	//VOP取值方式
	int IntVop = _ttoi(strTemp);

	if(IntVop == 0 || IntVop == 3)
	{
		MessageBox(_T("VOP取值方式设置错误，将默认选择 Vbr-2！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
	}
	m_VopVal_Combo.SetCurSel(IntVop);

	strTemp = g_SqlCon->Result("SEN_PM_STEP");	//步长
	m_fSenStep_edit = (float)atof(strTemp);
	m_fSenGoVal_edit = ReadIniFileNum("Config.ini","TEST","SenPower");			//起始值
	m_CompenVal_edit = ReadIniFileNum("Config.ini","TEST","SenCompenVal");		//灵敏度补偿值
	m_PowStep1		 = ReadIniFileNum("Config.ini","TEST","PowStep1");
	m_PowStep2		 = ReadIniFileNum("Config.ini","TEST","PowStep2");

	UpdateData(false);

	return TRUE;
}

//获取当前输入工单的产品的测试规格（从TS文件中获取），成功返回 TRUE
BOOL CMy10GAPDTESTDlg::GetProductTestSpec_TS()
{
	///////////////////获取型号
	CString strCmd = _T("");	//Sql语句字符串
	strCmd.Format(_T("select model from Manufacture_info where Manufacture_ID = '%s'"),
		m_strWorkOrderNo_edit);			
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	
	{
		MessageBox(_T("数据库型号查询失败！！"),_T("错误"),MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	m_strModel_edit = g_SqlCon->Result("model");
	UpdateData(false);

	//////////////////下载TS文件
	if (!LoadTsFileFun(m_strModel_edit,g_strMaterialCode,g_strTsFileName,((CButton*)(GetDlgItem(IDC_TESTSPEC_CHECK)))->GetCheck()))
	{
		return FALSE;
	}

	////////////////读取TS文件的测试规格
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

//读取本地指定TS文件中的产品测试规格
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

	CUexcel->ReadTestSpec(_T("Icc"),_T("RX测试"),_T("25"),_T("3.3"),strTemp1,strTemp2,bEnabled[0]);
	m_fIccMin_edit = (float)(atof(strTemp1));
	m_fIccMax_edit = (float)(atof(strTemp2));

	CUexcel->ReadTestSpec(_T("暗电流(Id)"),_T("RX测试"),_T("25"),_T("3.3"),strTemp1,strTemp2,bEnabled[1]);
	m_fIdMin_edit = (float)(atof(strTemp1));
	m_fIdMax_edit = (float)(atof(strTemp2));

	CUexcel->ReadTestSpec(_T("击穿电压(VBR)"),_T("RX测试"),_T("25"),_T("3.3"),strTemp1,strTemp2,bEnabled[2]);
	m_fVbrMin_edit = (float)(atof(strTemp1));
	m_fVbrMax_edit = (float)(atof(strTemp2));

	CUexcel->ReadTestSpec(_T("灵敏度"),_T("RX测试"),_T("25"),_T("3.3"),strTemp1,strTemp2,bEnabled[3]);
	m_fSenMin_edit = (float)(atof(strTemp1));
	m_fSenMax_edit = (float)(atof(strTemp2));

	int nOrd = -1;
	CString SpecVal,Specdescribe;
	CUexcel->ReadBaseSpec(_T("SEN测试时长"),nOrd,SpecVal,Specdescribe);
	m_SenTime_edit = _ttoi(SpecVal);

	CUexcel->ReadBaseSpec(_T("接收波长-数字"),nOrd,SpecVal,Specdescribe);
	if(_ttoi(SpecVal) == 1310)
	{
		m_AttWaveLen_Combo.SetCurSel(0);		//光衰减器
		m_PowerWaveLen_Combo.SetCurSel(3);		//光功率计
	}
	else if(_ttoi(SpecVal) == 1550)
	{
		m_AttWaveLen_Combo.SetCurSel(1);
		m_PowerWaveLen_Combo.SetCurSel(5);
	}
	else
	{
		MessageBox(_T("波长设置错误，将默认选择 1310 的波长！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
	}

	CUexcel->ReadBaseSpec(_T("指定误码率"),nOrd,m_strVbrVal_edit,Specdescribe);	//误码率选择
	CUexcel->ReadBaseSpec(_T("接收速率-数字"),nOrd,SpecVal,Specdescribe);		//速率选择
	if(nOrd < 8)
	{
		nOrd = 8;
		MessageBox(_T("速率设置错误，将默认选择 9.95G 的速率！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
	}
	m_Rate_Combo.SetCurSel(nOrd - 8);

	CUexcel->ReadBaseSpec(_T("接收码型-数字"),nOrd,SpecVal,Specdescribe);//码型选择
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
		MessageBox(_T("码型设置错误，将默认选择 PRBS7 的码型！！"),_T("提示信息"),MB_OKCANCEL | MB_ICONQUESTION);
	}

	CUexcel->ReadBaseSpec(_T("误码仪工作模式"),nOrd,SpecVal,Specdescribe);//工作模式选择(光、电通道)
	if(nOrd == 1)		//光通道
	{
		m_Channel_Combo.SetCurSel(1);
	}
	else if(nOrd == 2)	//电通道
	{
		m_Channel_Combo.SetCurSel(0);
	}
	
	CUexcel->ReadBaseSpec(_T("SEN_Vop方式"),nOrd,SpecVal,Specdescribe);//VOP取值方式
	m_VopVal_Combo.SetCurSel(nOrd);

	CUexcel->ReadBaseSpec(_T("功率计设置步进"),nOrd,SpecVal,Specdescribe);//步长
	m_fSenStep_edit = (float)atof(SpecVal);

	CUexcel->m_CUexcel->CloseExcel();
	delete CUexcel;
	CUexcel = NULL;

	int IntRange = (int)(ReadIniFileNum("Config.ini","TEST","SiganlRange"));	//误码仪幅度选择	
	m_Amplitude_Combo.SetCurSel(IntRange);

	m_fSenGoVal_edit = ReadIniFileNum("Config.ini","TEST","SenPower");			//起始值
	m_CompenVal_edit = ReadIniFileNum("Config.ini","TEST","SenCompenVal");		//灵敏度补偿值
	m_PowStep1		 = ReadIniFileNum("Config.ini","TEST","PowStep1");
	m_PowStep2		 = ReadIniFileNum("Config.ini","TEST","PowStep2");

	UpdateData(false);

	return TRUE;
}

//获取输入的工单的测试信息，（已测试数量、良品率 等）
BOOL CMy10GAPDTESTDlg::GetTestInfoToWorkOrder()
{
	int IntTestNumber	= 0;	//测试的总数量
	int IntTestFailNum	= 0;	//不良品数量
	int IntTestPassNum	= 0;	//良品数量
	float fTestPass		= 0.0f;	//良品率

	//获取良品数量
	CString strCmd = _T("");	//Sql语句字符串
	strCmd.Format(_T("select count(*) as Number from dbo.testdata where  Sen_Pass_fail = '1' AND Manufacture_ID = '%s'"),
		m_strWorkOrderNo_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//
	{
		return FALSE;
	}
	CString strNumber = g_SqlCon->Result("Number");
	IntTestPassNum	  = _ttoi(strNumber);

	//获取不良品数量
	strCmd.Format(_T("select count(*) as Number from dbo.testdata where  Sen_Pass_fail = '0' AND Manufacture_ID = '%s'"),
		m_strWorkOrderNo_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	
	{
		return FALSE;
	}
	strNumber = g_SqlCon->Result("Number");
	IntTestFailNum	  = _ttoi(strNumber);

	//计算已测试数量和良品率
	IntTestNumber = IntTestPassNum + IntTestFailNum;
	if(IntTestNumber == 0)
	{
		fTestPass = 0.0f;
	}
	else
	{
		fTestPass = (float)(IntTestPassNum/IntTestNumber);
	}

	//将数据写入表格中
	CString strTemp = _T("");
	strTemp.Format("%d",IntTestNumber);
	m_Method.WriteGridCtrOne(m_GridDS,1,1,strTemp,RGB(100, 0, 0));	//测试总数量

	strTemp.Format("%d",IntTestFailNum);
	m_Method.WriteGridCtrOne(m_GridDS,2,1,strTemp,RGB(100, 0, 0));	//不良品数量

	strTemp.Format("%d",IntTestPassNum);
	m_Method.WriteGridCtrOne(m_GridDS,3,1,strTemp,RGB(100, 0, 0));	//良品数量

	strTemp.Format("%0.2f",fTestPass * 100);
	m_Method.WriteGridCtrOne(m_GridDS,4,1,strTemp,RGB(100, 0, 0));	//良品率
	return TRUE;
}

//开启批量测试
void CMy10GAPDTESTDlg::BeginTest()
{
	g_BeginTest = TRUE;
	GetDlgItem(IDC_BeginTest_BUTTON)->SetWindowText(_T("已开启批量测试"));
	m_ButtonBeginTest.SetUpColor(PassColor);
	GetDlgItem(IDC_BeginTest_BUTTON)->Invalidate();
	SetDebugParaCtrlStatus(false);
}

//停止批量测试
void CMy10GAPDTESTDlg::StopTest()
{
	g_BeginTest = FALSE;
	GetDlgItem(IDC_BeginTest_BUTTON)->SetWindowText(_T("已停止批量测试"));
	m_ButtonBeginTest.SetUpColor(FailColor);
	GetDlgItem(IDC_BeginTest_BUTTON)->Invalidate();
	SetDebugParaCtrlStatus(true);
}

//服务器数据库连接初始化
BOOL CMy10GAPDTESTDlg::SqlServerConnet()
{
	g_SqlCon	= new ADO_Connection();	//sql server数据库
	g_recordSql = new _RecordsetPtr;

	CString strServerIP = ReadIniFileStr("Config.ini","SQL","ServerIP");	//服务器IP地址
	CString strAccount  = ReadIniFileStr("Config.ini","SQL","Account");		//账号
	CString strPassWord = ReadIniFileStr("Config.ini","SQL","PassWord");	//密码
	CString strDataBase = ReadIniFileStr("Config.ini","SQL","DataBase");	//数据库名

	/*********************************对加密的密码进行解密*********************************************/
	/*
	 *密码加密方法：
	 *加密的Key值为 ：“APAT”每个字符的Assic码值相加
	 *加密方法：
	 *	将密码的每个字符的Assic值 乘以 Key值并用 ‘,’分隔起来；如密码“123”加密后就为 "14406,14700,14994"
	 */

	CString strPassWordTemp[10];						//用于保存分割后的未解密的子字符串
	SubCString(strPassWord,strPassWordTemp, _T(","));	//获取子字符串
	int i = 0;
	int Key = 'A' + 'P' + 'A' + 'T';					//加密的Key值
	char strTemp[1024] = {0};							//用于保存解密后的字符
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

	//获取工序标识代码
	CString strCmd = _T("");
	strCmd.Format(_T("SELECT Station_Code FROM  sys_info WHERE info_Item ='%s'"),g_strActiveStep);
	if( false == g_SqlCon->Query(strCmd, g_recordSql) )	
	{
		MessageBox(_T("获取当前工序标识代码失败！"), _T("错误提示"), MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}
	g_IntActiveStep = g_SqlCon->Result.Convertor("Station_Code").ToInteger();


	return TRUE;
}

//保存测试数据到服务器当中
BOOL CMy10GAPDTESTDlg::SaveTestDataServer()
{
	Data->m_PDNo		= m_strPDModel_edit;	//PD型号
	Data->m_TestEr		= m_strTestEr_edit;		//测试人员
	Data->m_WorkOrderNo = m_strWorkOrderNo_edit;//工单号
	Data->m_SerialNo	= m_strSn_edit;			//系列号

	SYSTEMTIME st;
	GetLocalTime(&st);
	Data->m_TestTime.Format("%4d-%2d-%2d %2d:%2d:%2d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);


	CString strCmd = _T("");	//Sql语句字符串
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

//本地数据库连接初始化
BOOL CMy10GAPDTESTDlg::AccessConnet()
{
	g_AccCon	= new ADO_Connection();	//Access数据库
	g_recordAcc = new _RecordsetPtr;

	CString FilePath = GetFilePath("SaveData.accdb");	//数据库文件路径
	CString StrConnect = "Provider = Microsoft.ACE.OLEDB.12.0;Data Source="  + FilePath;//Access2007

	if(false == g_AccCon->OpenConnection( StrConnect ))
	{
		return FALSE;
	}
	return TRUE;
}

//保存测试数据在本地的Access数据库中
BOOL CMy10GAPDTESTDlg::SaveTestDataAccess()
{
	CString strCmd = _T("");	//Sql语句字符串
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


	if(IntNumber > 0)	//已经有该 SN 号的产品数据，修改即可
	{
		Data->m_TestEr		= m_strTestEr_edit;		//测试人员
		Data->m_WorkOrderNo = m_strWorkOrderNo_edit;//工单号
		Data->m_SerialNo	= m_strSn_edit;			//系列号

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
	else					//该 SN 号的产品还未测试过，需要添加
	{
		Data->m_Model		= m_strModel_edit;		//型号
		Data->m_WorkOrderNo = m_strWorkOrderNo_edit;//工单号
		Data->m_BatchNo		= m_strBatchNo_edit;	//批次号
		Data->m_PDNo		= m_strPDModel_edit;	//PD型号
		Data->m_TestEr		= m_strTestEr_edit;		//测试人员
		Data->m_SerialNo	= m_strSn_edit;			//系列号
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

//测试失败，给出失败提示
void CMy10GAPDTESTDlg::TestFail()
{
	if(ApdVccOutPut(FALSE) == FALSE)//关闭APD、VCC电压输出
	{
		MessageBox(_T("设备操作出现错误，请手动关闭 VBR 电压输出，否则将会损坏器件！"), _T("错误提示"), MB_OKCANCEL | MB_ICONQUESTION);
	}

	GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("测试失败！"));
	g_ColorLogo = Color_Red;
	GetDlgItem(IDC_Results_STATIC)->SetWindowText(_T("FAIL"));
	GetDlgItem(IDC_Results_STATIC)->Invalidate();
	g_deviceTestState = FALSE;
	StopTest();
}

//测试成功，给出成功提示
void CMy10GAPDTESTDlg::TestPass()
{
	if(ApdVccOutPut(FALSE) == FALSE)//关闭APD、VCC电压输出
	{
		MessageBox(_T("设备操作出现错误，请手动关闭 VBR 电压输出，否则将会损坏器件！"), _T("错误提示"), MB_OKCANCEL | MB_ICONQUESTION);
	}

	GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("测试成功！请更换器件"));
	g_ColorLogo = Color_Green;
	GetDlgItem(IDC_Results_STATIC)->SetWindowText(_T("PASS"));
	GetDlgItem(IDC_Results_STATIC)->Invalidate();
	m_strSn_edit = _T("");
	UpdateData(false);
	StopTest();
}

//器件开始测试
void CMy10GAPDTESTDlg::BeginDeviceTest()
{
	Data->SaveDataInit();				//初始化将要保存的数据
	GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("正在测试，请稍候···"));
	g_ColorLogo = Color_BLUE;
	GetDlgItem(IDC_Results_STATIC)->SetWindowText(_T("······"));
	GetDlgItem(IDC_Results_STATIC)->Invalidate();
	g_deviceTestState = TRUE;
	g_TimeBegin = GetTickCount();		//取得当前系统运行时间(ms)
	m_ctrlProgress.SetPos(10);			//设置进度条的当前位置

	g_FailType     = _T("");	
	g_FailItemCode = -1;
}

//设置某个控件的可操作状态
void CMy10GAPDTESTDlg::SetControlsOptional(int ID, bool State)
{
	CWnd * pWnd = NULL;
	pWnd = GetDlgItem(ID);
	pWnd->EnableWindow(State);
}

//自动调试可选项控件状态控制
void CMy10GAPDTESTDlg::SetDebugParaCtrlStatus(bool setVal)
{
	CWnd * pWnd = NULL; 

	SetControlsOptional(IDC_CHECK1, setVal);
	SetControlsOptional(IDC_ExtractData_BUTTON, setVal);
	SetControlsOptional(IDC_DeviceInit_BUTTON, setVal);
	SetControlsOptional(IDC_Calibration1_BUTTON, setVal);
}

//开启/关闭 APD、VCC电压输出
BOOL CMy10GAPDTESTDlg::ApdVccOutPut(BOOL State)
{
	if(m_ApatVbr->VoltageOutputEnable(0, State) == FALSE)	//开启VAPD电压输出
	{
		return FALSE;
	}

	if(m_ApatVbr->VoltageOutputEnable(2, State) == FALSE)	//开启VCC电压输出
	{
		return FALSE;
	}

	if(State == FALSE)
	{
		if(m_ApatVbr->SetDeviceWorkMode(5) == FALSE)			//选择第5工作模式
		{
			return FALSE;
		}
	}

	return TRUE;
}

//进行工序判断，符合当前工序返回TRUE，否则返回FASLE
BOOL CMy10GAPDTESTDlg::JudgeStep(CString strSN)
{
	CString strCmd = _T("");	//Sql语句字符串
	strCmd.Format(_T("select Active_Step,Next_Step from dbo.testdata  where Serial_No = '%s'"),
				  strSN);

	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	
	{
		TestFail();
		GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("数据库获取器件工序出错"));
		return FALSE;
	}
	int IntActiveStep = g_SqlCon->Result("Active_Step");	//当前工序
	int IntNextStep   = g_SqlCon->Result("Next_Step");		//下一工序

	strCmd.Format(_T("select Pstep from pstepset where dutType = '%s' AND PIndex = %d"),m_strModel_edit,IntActiveStep);
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	
	{
		TestFail();
		return FALSE;
	}
	CString strActiveStep = g_SqlCon->Result("Pstep");	//当前工序

	if(strActiveStep != g_strActiveStep)
	{
		TestFail();
		GetDlgItem(IDC_STATIC1)->SetWindowTextA(_T("该器件不属于当前工序流程"));
		return FALSE;
	}
	
	return TRUE;
}

//进入返修
BOOL CMy10GAPDTESTDlg::GotoRepair()
{
	INT_PTR nRes = MessageBox(_T("该产品是否转入返修？"), _T("提示"), MB_OKCANCEL | MB_ICONQUESTION);
	if (IDCANCEL == nRes)	//不进返修
	{
		return FALSE;
	}

	//查询返修次数
	CString strCmd = _T("");	//Sql语句字符串
	strCmd.Format("select Repair from testdata where Manufacture_ID='%s' and Batch_ID= %.0f and Serial_No='%s' ",
		m_strWorkOrderNo_edit,atof(m_strBatchNo_edit),m_strSn_edit);

	if( false == g_SqlCon->Query(strCmd, g_recordSql) )	
	{
		MessageBox(_T("该器件进入返修失败1！"), _T("错误提示"), MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}
	float IntRepair = (float)g_SqlCon->Result.Convertor("Repair").ToInteger();	//返修次数

	//进入返修	
	strCmd.Format(_T("select count(*) as Number from Repair_info where Serial_No = '%s'"),m_strSn_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//
	{
		MessageBox(_T("返修数据查询失败！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	int IntNumber = g_SqlCon->Result.Convertor("Number").ToInteger();

	if(IntNumber == 0)	//未进入过返修
	{
		strCmd.Format( "INSERT INTO Repair_info (Serial_No,Model_Number,Rep_Index, Old_Manufacture_ID, Old_Batch, \
			Exp_Team,Exp_Operator, Exp_time, Fail_Item_Code,Fail_Station_Code,Fail_type) VALUES ('%s','%s',%f,'%s',\
			%f,'%s','%s','%s',%d,%d,'%s')",
			m_strSn_edit,m_strModel_edit,IntRepair +1,m_strWorkOrderNo_edit,atof(m_strBatchNo_edit),_T("测包"),
			m_strTestEr_edit,Data->m_TestTime,g_FailItemCode,6,g_FailType);
			
	}
	else				//已存在返修
	{
		strCmd.Format( "Update Repair_info set Model_Number = '%s',Rep_Index = %f,Old_Manufacture_ID = '%s',\
			Old_Batch = %f,Exp_Team = '%s',Exp_Operator = '%s',Exp_time = '%s',\
			Fail_Item_Code = %d,Fail_Station_Code = %d,Fail_type = '%s' where Serial_No = '%s'",
			m_strModel_edit,IntRepair +1,m_strWorkOrderNo_edit,atof(m_strBatchNo_edit),_T("测包"),
			m_strTestEr_edit,Data->m_TestTime,g_FailItemCode,6,g_FailType,m_strSn_edit);
	}

	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	
	{
		MessageBox(_T("该器件进入返修失败2！"), _T("错误提示"), MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	strCmd.Format("UPDATE testdata SET  Active_Step=0,Next_Step=0,Repair=%f where Serial_No='%s' ",IntRepair +1,m_strSn_edit);

	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	
	{
		MessageBox(_T("该器件进入返修失败3！"), _T("错误提示"), MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	strCmd.Format("SELECT FailCode_Num FROM Count_Product_Fail_Num WHERE Manufacture_ID='%s' AND Batch_ID= %d AND Fail_Code= %d\
		AND Fail_Station_Code=%d ",m_strWorkOrderNo_edit,(int)atof(m_strBatchNo_edit),g_FailItemCode,6);

	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	
	{
		MessageBox(_T("该器件进入返修失败4！"), _T("错误提示"), MB_OKCANCEL | MB_ICONQUESTION);
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
		MessageBox(_T("该器件进入返修失败5！"), _T("错误提示"), MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	return TRUE;
}

//测试结果处理
void CMy10GAPDTESTDlg::TestResultsDeal()
{
	if(g_FailItemCode == -1 && g_FailType == _T(""))
	{
		MessageBox(_T("设备操作出现错误，请检查设备后重新测试！"), _T("错误提示"), MB_OKCANCEL | MB_ICONQUESTION);
		TestFail();
		return ;
	}

	if(SaveTestDataAccess() == FALSE)		//测试数据保存在本地
	{
		MessageBox(_T("保存测试数据在本地失败！！"), _T("错误提示"), MB_OKCANCEL | MB_ICONQUESTION);
		TestFail();
		return ;
	}

	if(SaveTestDataServer() == FALSE)		//测试数据保存在服务器
	{
		MessageBox(_T("保存测试数据在服务器失败！！"), _T("错误提示"), MB_OKCANCEL | MB_ICONQUESTION);
		TestFail();
		return ;
	}

	if(GotoRepair() == TRUE)	//返修处理
	{
		MessageBox(_T("该器件进入返修成功！"), _T("提示"), MB_OKCANCEL | MB_ICONQUESTION);
	}
	TestFail();
	return ;
}

void CMy10GAPDTESTDlg::OnBnClickedTestspecCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	g_strModelType = _T("2efretyret```");
}
