// Login.cpp : 实现文件
//

#include "stdafx.h"
#include "10G-APD-TEST.h"
#include "Login.h"
#include "10G-APD-TESTDlg.h"
#include "SpecConfig.h"
#include "ReadFile.h"


ADO_Connection * g_SqlCon = NULL;		//sql server数据库对象
_RecordsetPtr  * g_recordSql = NULL;

ADO_Connection * g_AccCon;				//Access数据库对象
_RecordsetPtr  * g_recordAcc ;

#define MODEL1 _T("10G-APD")
#define MODEL2 _T("10G-PIN-TIA")


#pragma comment(lib,"ADODB.lib")			//数据库Dll

// Login 对话框

IMPLEMENT_DYNAMIC(Login, CDialog)

Login::Login(CWnd* pParent /*=NULL*/)
	: CDialog(Login::IDD, pParent)
	, m_Account_edit(_T(""))
	, m_PassWord_edit(_T(""))
{
	LoginState = FALSE;
}

Login::~Login()
{
}

void Login::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Account_EDIT, m_Account_edit);
	DDX_Text(pDX, IDC_PassWord_EDIT, m_PassWord_edit);
	DDX_Control(pDX, IDC_Model_COMBO, m_Model_Combo);
}


BEGIN_MESSAGE_MAP(Login, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_GoTestModel_BUTTON, &Login::OnBnClickedGotestmodelButton)
	ON_BN_CLICKED(IDC_GoSpecConfig_BUTTON, &Login::OnBnClickedGospecconfigButton)
	ON_EN_CHANGE(IDC_Account_EDIT, &Login::OnEnChangeAccountEdit)
	ON_EN_CHANGE(IDC_PassWord_EDIT, &Login::OnEnChangePasswordEdit)
	ON_BN_CLICKED(IDC_Login_BUTTON, &Login::OnBnClickedLoginButton)
END_MESSAGE_MAP()


// Login 消息处理程序
void Login::OnOK()
{
	if(LoginState == FALSE)
	{
		if(m_Account_edit == _T(""))
		{
			MessageBox(_T("账号不能为空，请先输入账号！"),_T("错误提示"),MB_OKCANCEL | MB_ICONQUESTION);
			return ;
		}

		if(m_PassWord_edit == _T(""))
		{
			MessageBox(_T("密码不能为空，请先输入密码！"),_T("错误提示"),MB_OKCANCEL | MB_ICONQUESTION);
			return ;
		}

		if(JudgeUser() == FALSE)
		{
			MessageBox(_T("账号错误，不存在该用户账号！"),_T("错误提示"),MB_OKCANCEL | MB_ICONQUESTION);
			return ;
		}

		if(JudgePassword() == FALSE)
		{
			MessageBox(_T("密码错误，请重新输入密码！"),_T("错误提示"),MB_OKCANCEL | MB_ICONQUESTION);
			return ;
		}

		if(GetUserName() == FALSE)
		{
			MessageBox(_T("获取用户姓名和用户权限失败！"),_T("错误提示"),MB_OKCANCEL | MB_ICONQUESTION);
			return ;
		}

		GetDlgItem(IDC_Login_BUTTON)->SetWindowTextA(_T("登录成功"));
		LoginState = TRUE;
		m_ButtonLonging.SetUpColor(RGB(144,238,144));
		GetDlgItem(IDC_Login_BUTTON)->Invalidate();
	}
	else
	{
		g_IntModelState = m_Model_Combo.GetCurSel();	//选择的测试模式
		CString strModel = _T("");

		if(LoginState == FALSE)
		{
			MessageBox(_T("用户未登录成功，请先登录！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
			return ;
		}
		
		if(g_IntModelState == 0)
		{
			strModel.Format(_T("您确定要进入“%s测试界面”吗？"),MODEL1); 
		}
		else if(g_IntModelState == 1)
		{
			//strModel.Format(_T("您确定要进入“%s测试界面”吗？"),MODEL2); 
			strModel.Format(_T("软件暂不支持“%s”类型器件的测试！"),MODEL2);
			MessageBox(strModel);
			return ;
		}
		INT_PTR nRes = MessageBox(strModel, _T("模式确认"), MB_OKCANCEL | MB_ICONQUESTION);
		if (IDCANCEL == nRes)   
			return; 
		OnClose();
		CMy10GAPDTESTDlg dlg;
		INT_PTR nResponse = dlg.DoModal();
	}
}

void Login::OnCancel()
{

}

void Login::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	g_SqlCon->Close();		//关闭数据库

	delete g_SqlCon;
	delete g_recordSql;
	g_SqlCon	= NULL;
	g_recordSql = NULL;

	EndDialog(IDCANCEL);		//关闭窗口 
	CDialog::OnClose();
}

BOOL Login::OnInitDialog()
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
		MessageBox(_T("服务器数据库连接失败，程序退出！"),_T("错误提示"),MB_OKCANCEL | MB_ICONQUESTION);
		exit(0);
	}

	m_Model_Combo.InsertString(0,MODEL1);
	m_Model_Combo.InsertString(1,MODEL2);
	m_Model_Combo.SetCurSel(0);


	//“用户登录”按钮控件
	GetDlgItem(IDC_Login_BUTTON)->ModifyStyle(0,BS_OWNERDRAW,0);	//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
	m_ButtonLonging.Attach(IDC_Login_BUTTON,this);					//绑定控件IDC_BUTTON1与类CMyButton，响应重载函数DrawItem()
	m_ButtonLonging.SetDownColor(RGB(255,255,224));					//设置Button Down的背景色
	m_ButtonLonging.SetUpColor(RGB(205,192,112));					//设置Button Up的背景色
	m_ButtonLonging.SetTextColor(RGB(0,0,128));						//Button的字体颜色

	//“进入测试界面”按钮控件
	GetDlgItem(IDC_GoTestModel_BUTTON)->ModifyStyle(0,BS_OWNERDRAW,0);
	m_ButtonTestModel.Attach(IDC_GoTestModel_BUTTON,this);
	m_ButtonTestModel.SetDownColor(RGB(255,255,224));
	m_ButtonTestModel.SetUpColor(RGB(205,192,112));
	m_ButtonTestModel.SetTextColor(RGB(0,0,128));


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Login::OnEnChangeAccountEdit()
{
	UpdateData(true);
}

void Login::OnEnChangePasswordEdit()
{
	UpdateData(true);
}

//登录按钮消息函数
void Login::OnBnClickedLoginButton()
{
	// TODO: 在此添加控件通知处理程序代码

	if(m_Account_edit == _T(""))
	{
		MessageBox(_T("账号不能为空，请先输入账号！"),_T("错误提示"),MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(m_PassWord_edit == _T(""))
	{
		MessageBox(_T("密码不能为空，请先输入密码！"),_T("错误提示"),MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(JudgeUser() == FALSE)
	{
		MessageBox(_T("账号错误，不存在该用户账号！"),_T("错误提示"),MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(JudgePassword() == FALSE)
	{
		MessageBox(_T("密码错误，请重新输入密码！"),_T("错误提示"),MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(GetUserName() == FALSE)
	{
		MessageBox(_T("获取用户姓名和用户权限失败！"),_T("错误提示"),MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	GetDlgItem(IDC_Login_BUTTON)->SetWindowTextA(_T("登录成功"));
	LoginState = TRUE;
	m_ButtonLonging.SetUpColor(RGB(144,238,144));
	GetDlgItem(IDC_Login_BUTTON)->Invalidate();
}

//判断用户输入的 用户名 是否正确
BOOL Login::JudgeUser()
{
	CString strCmd = _T("");	//Sql语句字符串
	strCmd.Format(_T("select count(*) as Number from dbo.UserInfor where manuser = '%s'"),m_Account_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//查询该账号的记录数
	{
		MessageBox(_T("数据库账号查询失败！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	CString strNumber = g_SqlCon->Result("Number");

	int IntNumber = _ttoi(strNumber);

	if(IntNumber == 0)
		return FALSE;

	return TRUE;
}

//确定账号正确后对用户输入的密码进行判断，正确返回 TRUE
BOOL Login::JudgePassword()
{
	CString strCmd = _T("");	//Sql语句字符串
	strCmd.Format(_T("select count(*) as Number from dbo.UserInfor where manuser = '%s' AND password = '%s'"),
				  m_Account_edit,m_PassWord_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//查询该账号、密码的记录数
	{
		MessageBox(_T("数据库账号与密码查询失败！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	CString strNumber = g_SqlCon->Result("Number");

	int IntNumber = _ttoi(strNumber);

	if(IntNumber == 0)
		return FALSE;

	return TRUE;
}

//获取正确账号的 测试人员姓名 和 用户的权限,成功返回 TRUE
BOOL Login::GetUserName()
{
	CString strCmd = _T("");	//Sql语句字符串
	strCmd.Format(_T("select UserEnable,UserName from dbo.UserInfor where manuser = '%s' AND password = '%s'"),
				  m_Account_edit,m_PassWord_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//查询该账号的用户姓名与用户权限
	{
		MessageBox(_T("数据库用户姓名与用户权限查询失败！！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	g_UserName			= g_SqlCon->Result("UserName");		//用户姓名
	g_UserAccount = m_Account_edit;
	g_UserPermissions	= g_SqlCon->Result("UserEnable");	//用户权限

	int IntOrd = 0;
	for(int i = 0; i < 48;i++)
	{
		g_EnabledArray[i].Format(_T("%c"),g_UserPermissions[IntOrd]);
		IntOrd += 2;
	}

	return TRUE;
}


//进入 测试界面
void Login::OnBnClickedGotestmodelButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if(LoginState == FALSE)
	{
		MessageBox(_T("用户未登录成功，请先登录！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	g_IntModelState = m_Model_Combo.GetCurSel();	//选择的测试模式
	CString strModel = _T("");

	GetDlgItem(IDC_Model_COMBO)->GetWindowTextA(strModel);

	
	if(g_IntModelState == 0)
	{
		strModel.Format(_T("您确定要进入“%s测试界面”吗？"),MODEL1); 
	}
	else if(g_IntModelState == 1)
	{
		//strModel.Format(_T("您确定要进入“%s测试界面”吗？"),MODEL2); 
		strModel.Format(_T("软件暂不支持“%s”类型器件的测试！"),MODEL2);
		MessageBox(strModel); 
		return ;
	}

	INT_PTR nRes = MessageBox(strModel, _T("模式确认"), MB_OKCANCEL | MB_ICONQUESTION);
	if (IDCANCEL == nRes)   
		return; 
	OnClose();
	CMy10GAPDTESTDlg dlg;
	INT_PTR nResponse = dlg.DoModal();
}

//“规格配置界面”
void Login::OnBnClickedGospecconfigButton()
{
	// TODO: 在此添加控件通知处理程序代码

	if(LoginState == FALSE)
	{
		MessageBox(_T("用户未登录成功，请先登录！"),_T("错误"),MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	INT_PTR nRes = MessageBox(_T("您确定要进入“规格配置界面”吗？"), _T("模式确认"), MB_OKCANCEL | MB_ICONQUESTION);
	if (IDCANCEL == nRes)   
		return; 
	OnClose();
	SpecConfig dlg;
	INT_PTR nResponse = dlg.DoModal();
}