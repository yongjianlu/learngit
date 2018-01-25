#pragma once
#include "afxwin.h"
#include "Button/MyButton.h"


// Login 对话框

class Login : public CDialog
{
	DECLARE_DYNAMIC(Login)

public:
	Login(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Login();

// 对话框数据
	enum { IDD = IDD_LOGIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();

	CString m_Account_edit;		//账号
	CString m_PassWord_edit;	//密码

	BOOL LoginState;			//登录状态, TRUE 表示登录成功， FALSE 表示未登录

	CComboBox m_Model_Combo;	//模式选择

	CMyButton m_ButtonLonging;		//“用户登录”按钮重绘
	CMyButton m_ButtonTestModel;	//“进入测试界面”按钮重绘

	afx_msg void OnBnClickedGotestmodelButton();
	/*
	 *进入 “测试界面”
	 */

	afx_msg void OnBnClickedGospecconfigButton();
	/*
	 *进入 “规格配置”界面 
	 */

	afx_msg void OnEnChangeAccountEdit();
	/*
	 *“账号”编辑框消息函数
	 */

	afx_msg void OnEnChangePasswordEdit();
	/*
	 *“密码”编辑框消息函数
	 */

	afx_msg void OnBnClickedLoginButton();
	/*
	 *"登录"按钮消息函数
	 */

	BOOL JudgeUser();
	/*
	 *判断用户输入的 账号 是否正确（数据库中是否存在），正确返回TRUE，否则返回FALSE
	 */

	BOOL JudgePassword();
	/*
	 *确定账号正确后对用户输入的密码进行判断，正确返回 TRUE
	 */

	BOOL GetUserName();
	/*
	 *获取正确账号的 测试人员姓名 和 用户的权限,成功返回 TRUE
	 */
};
