// Login.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "10G-APD-TEST.h"
#include "Login.h"
#include "10G-APD-TESTDlg.h"
#include "SpecConfig.h"
#include "ReadFile.h"


ADO_Connection * g_SqlCon = NULL;		//sql server���ݿ����
_RecordsetPtr  * g_recordSql = NULL;

ADO_Connection * g_AccCon;				//Access���ݿ����
_RecordsetPtr  * g_recordAcc ;

#define MODEL1 _T("10G-APD")
#define MODEL2 _T("10G-PIN-TIA")


#pragma comment(lib,"ADODB.lib")			//���ݿ�Dll

// Login �Ի���

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


// Login ��Ϣ�������
void Login::OnOK()
{
	if(LoginState == FALSE)
	{
		if(m_Account_edit == _T(""))
		{
			MessageBox(_T("�˺Ų���Ϊ�գ����������˺ţ�"),_T("������ʾ"),MB_OKCANCEL | MB_ICONQUESTION);
			return ;
		}

		if(m_PassWord_edit == _T(""))
		{
			MessageBox(_T("���벻��Ϊ�գ������������룡"),_T("������ʾ"),MB_OKCANCEL | MB_ICONQUESTION);
			return ;
		}

		if(JudgeUser() == FALSE)
		{
			MessageBox(_T("�˺Ŵ��󣬲����ڸ��û��˺ţ�"),_T("������ʾ"),MB_OKCANCEL | MB_ICONQUESTION);
			return ;
		}

		if(JudgePassword() == FALSE)
		{
			MessageBox(_T("��������������������룡"),_T("������ʾ"),MB_OKCANCEL | MB_ICONQUESTION);
			return ;
		}

		if(GetUserName() == FALSE)
		{
			MessageBox(_T("��ȡ�û��������û�Ȩ��ʧ�ܣ�"),_T("������ʾ"),MB_OKCANCEL | MB_ICONQUESTION);
			return ;
		}

		GetDlgItem(IDC_Login_BUTTON)->SetWindowTextA(_T("��¼�ɹ�"));
		LoginState = TRUE;
		m_ButtonLonging.SetUpColor(RGB(144,238,144));
		GetDlgItem(IDC_Login_BUTTON)->Invalidate();
	}
	else
	{
		g_IntModelState = m_Model_Combo.GetCurSel();	//ѡ��Ĳ���ģʽ
		CString strModel = _T("");

		if(LoginState == FALSE)
		{
			MessageBox(_T("�û�δ��¼�ɹ������ȵ�¼��"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
			return ;
		}
		
		if(g_IntModelState == 0)
		{
			strModel.Format(_T("��ȷ��Ҫ���롰%s���Խ��桱��"),MODEL1); 
		}
		else if(g_IntModelState == 1)
		{
			//strModel.Format(_T("��ȷ��Ҫ���롰%s���Խ��桱��"),MODEL2); 
			strModel.Format(_T("����ݲ�֧�֡�%s�����������Ĳ��ԣ�"),MODEL2);
			MessageBox(strModel);
			return ;
		}
		INT_PTR nRes = MessageBox(strModel, _T("ģʽȷ��"), MB_OKCANCEL | MB_ICONQUESTION);
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	g_SqlCon->Close();		//�ر����ݿ�

	delete g_SqlCon;
	delete g_recordSql;
	g_SqlCon	= NULL;
	g_recordSql = NULL;

	EndDialog(IDCANCEL);		//�رմ��� 
	CDialog::OnClose();
}

BOOL Login::OnInitDialog()
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
		MessageBox(_T("���������ݿ�����ʧ�ܣ������˳���"),_T("������ʾ"),MB_OKCANCEL | MB_ICONQUESTION);
		exit(0);
	}

	m_Model_Combo.InsertString(0,MODEL1);
	m_Model_Combo.InsertString(1,MODEL2);
	m_Model_Combo.SetCurSel(0);


	//���û���¼����ť�ؼ�
	GetDlgItem(IDC_Login_BUTTON)->ModifyStyle(0,BS_OWNERDRAW,0);	//����ť�޸�ΪBS_OWNERDRAW���,����button�Ĳ����Ի�ģʽ
	m_ButtonLonging.Attach(IDC_Login_BUTTON,this);					//�󶨿ؼ�IDC_BUTTON1����CMyButton����Ӧ���غ���DrawItem()
	m_ButtonLonging.SetDownColor(RGB(255,255,224));					//����Button Down�ı���ɫ
	m_ButtonLonging.SetUpColor(RGB(205,192,112));					//����Button Up�ı���ɫ
	m_ButtonLonging.SetTextColor(RGB(0,0,128));						//Button��������ɫ

	//��������Խ��桱��ť�ؼ�
	GetDlgItem(IDC_GoTestModel_BUTTON)->ModifyStyle(0,BS_OWNERDRAW,0);
	m_ButtonTestModel.Attach(IDC_GoTestModel_BUTTON,this);
	m_ButtonTestModel.SetDownColor(RGB(255,255,224));
	m_ButtonTestModel.SetUpColor(RGB(205,192,112));
	m_ButtonTestModel.SetTextColor(RGB(0,0,128));


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Login::OnEnChangeAccountEdit()
{
	UpdateData(true);
}

void Login::OnEnChangePasswordEdit()
{
	UpdateData(true);
}

//��¼��ť��Ϣ����
void Login::OnBnClickedLoginButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(m_Account_edit == _T(""))
	{
		MessageBox(_T("�˺Ų���Ϊ�գ����������˺ţ�"),_T("������ʾ"),MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(m_PassWord_edit == _T(""))
	{
		MessageBox(_T("���벻��Ϊ�գ������������룡"),_T("������ʾ"),MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(JudgeUser() == FALSE)
	{
		MessageBox(_T("�˺Ŵ��󣬲����ڸ��û��˺ţ�"),_T("������ʾ"),MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(JudgePassword() == FALSE)
	{
		MessageBox(_T("��������������������룡"),_T("������ʾ"),MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	if(GetUserName() == FALSE)
	{
		MessageBox(_T("��ȡ�û��������û�Ȩ��ʧ�ܣ�"),_T("������ʾ"),MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	GetDlgItem(IDC_Login_BUTTON)->SetWindowTextA(_T("��¼�ɹ�"));
	LoginState = TRUE;
	m_ButtonLonging.SetUpColor(RGB(144,238,144));
	GetDlgItem(IDC_Login_BUTTON)->Invalidate();
}

//�ж��û������ �û��� �Ƿ���ȷ
BOOL Login::JudgeUser()
{
	CString strCmd = _T("");	//Sql����ַ���
	strCmd.Format(_T("select count(*) as Number from dbo.UserInfor where manuser = '%s'"),m_Account_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//��ѯ���˺ŵļ�¼��
	{
		MessageBox(_T("���ݿ��˺Ų�ѯʧ�ܣ���"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	CString strNumber = g_SqlCon->Result("Number");

	int IntNumber = _ttoi(strNumber);

	if(IntNumber == 0)
		return FALSE;

	return TRUE;
}

//ȷ���˺���ȷ����û��������������жϣ���ȷ���� TRUE
BOOL Login::JudgePassword()
{
	CString strCmd = _T("");	//Sql����ַ���
	strCmd.Format(_T("select count(*) as Number from dbo.UserInfor where manuser = '%s' AND password = '%s'"),
				  m_Account_edit,m_PassWord_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//��ѯ���˺š�����ļ�¼��
	{
		MessageBox(_T("���ݿ��˺��������ѯʧ�ܣ���"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	CString strNumber = g_SqlCon->Result("Number");

	int IntNumber = _ttoi(strNumber);

	if(IntNumber == 0)
		return FALSE;

	return TRUE;
}

//��ȡ��ȷ�˺ŵ� ������Ա���� �� �û���Ȩ��,�ɹ����� TRUE
BOOL Login::GetUserName()
{
	CString strCmd = _T("");	//Sql����ַ���
	strCmd.Format(_T("select UserEnable,UserName from dbo.UserInfor where manuser = '%s' AND password = '%s'"),
				  m_Account_edit,m_PassWord_edit);
	
	if( false == g_SqlCon->Query( strCmd, g_recordSql  ) )	//��ѯ���˺ŵ��û��������û�Ȩ��
	{
		MessageBox(_T("���ݿ��û��������û�Ȩ�޲�ѯʧ�ܣ���"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}

	g_UserName			= g_SqlCon->Result("UserName");		//�û�����
	g_UserAccount = m_Account_edit;
	g_UserPermissions	= g_SqlCon->Result("UserEnable");	//�û�Ȩ��

	int IntOrd = 0;
	for(int i = 0; i < 48;i++)
	{
		g_EnabledArray[i].Format(_T("%c"),g_UserPermissions[IntOrd]);
		IntOrd += 2;
	}

	return TRUE;
}


//���� ���Խ���
void Login::OnBnClickedGotestmodelButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(LoginState == FALSE)
	{
		MessageBox(_T("�û�δ��¼�ɹ������ȵ�¼��"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	g_IntModelState = m_Model_Combo.GetCurSel();	//ѡ��Ĳ���ģʽ
	CString strModel = _T("");

	GetDlgItem(IDC_Model_COMBO)->GetWindowTextA(strModel);

	
	if(g_IntModelState == 0)
	{
		strModel.Format(_T("��ȷ��Ҫ���롰%s���Խ��桱��"),MODEL1); 
	}
	else if(g_IntModelState == 1)
	{
		//strModel.Format(_T("��ȷ��Ҫ���롰%s���Խ��桱��"),MODEL2); 
		strModel.Format(_T("����ݲ�֧�֡�%s�����������Ĳ��ԣ�"),MODEL2);
		MessageBox(strModel); 
		return ;
	}

	INT_PTR nRes = MessageBox(strModel, _T("ģʽȷ��"), MB_OKCANCEL | MB_ICONQUESTION);
	if (IDCANCEL == nRes)   
		return; 
	OnClose();
	CMy10GAPDTESTDlg dlg;
	INT_PTR nResponse = dlg.DoModal();
}

//��������ý��桱
void Login::OnBnClickedGospecconfigButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(LoginState == FALSE)
	{
		MessageBox(_T("�û�δ��¼�ɹ������ȵ�¼��"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);
		return ;
	}

	INT_PTR nRes = MessageBox(_T("��ȷ��Ҫ���롰������ý��桱��"), _T("ģʽȷ��"), MB_OKCANCEL | MB_ICONQUESTION);
	if (IDCANCEL == nRes)   
		return; 
	OnClose();
	SpecConfig dlg;
	INT_PTR nResponse = dlg.DoModal();
}