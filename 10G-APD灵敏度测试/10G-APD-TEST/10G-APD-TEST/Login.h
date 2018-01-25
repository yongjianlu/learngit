#pragma once
#include "afxwin.h"
#include "Button/MyButton.h"


// Login �Ի���

class Login : public CDialog
{
	DECLARE_DYNAMIC(Login)

public:
	Login(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Login();

// �Ի�������
	enum { IDD = IDD_LOGIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();

	CString m_Account_edit;		//�˺�
	CString m_PassWord_edit;	//����

	BOOL LoginState;			//��¼״̬, TRUE ��ʾ��¼�ɹ��� FALSE ��ʾδ��¼

	CComboBox m_Model_Combo;	//ģʽѡ��

	CMyButton m_ButtonLonging;		//���û���¼����ť�ػ�
	CMyButton m_ButtonTestModel;	//��������Խ��桱��ť�ػ�

	afx_msg void OnBnClickedGotestmodelButton();
	/*
	 *���� �����Խ��桱
	 */

	afx_msg void OnBnClickedGospecconfigButton();
	/*
	 *���� ��������á����� 
	 */

	afx_msg void OnEnChangeAccountEdit();
	/*
	 *���˺š��༭����Ϣ����
	 */

	afx_msg void OnEnChangePasswordEdit();
	/*
	 *�����롱�༭����Ϣ����
	 */

	afx_msg void OnBnClickedLoginButton();
	/*
	 *"��¼"��ť��Ϣ����
	 */

	BOOL JudgeUser();
	/*
	 *�ж��û������ �˺� �Ƿ���ȷ�����ݿ����Ƿ���ڣ�����ȷ����TRUE�����򷵻�FALSE
	 */

	BOOL JudgePassword();
	/*
	 *ȷ���˺���ȷ����û��������������жϣ���ȷ���� TRUE
	 */

	BOOL GetUserName();
	/*
	 *��ȡ��ȷ�˺ŵ� ������Ա���� �� �û���Ȩ��,�ɹ����� TRUE
	 */
};
