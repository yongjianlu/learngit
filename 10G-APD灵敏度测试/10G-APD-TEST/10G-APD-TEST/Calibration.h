#pragma once


// Calibration �Ի���

class Calibration : public CDialog
{
	DECLARE_DYNAMIC(Calibration)

public:
	Calibration(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Calibration();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	CString m_PowerVal_edit; //�ֶ����Ե���ʵ�ʵĹ���ֵ
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangePowerEdit();
};
