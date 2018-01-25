#pragma once


// Calibration 对话框

class Calibration : public CDialog
{
	DECLARE_DYNAMIC(Calibration)

public:
	Calibration(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Calibration();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CString m_PowerVal_edit; //手动测试到的实际的功率值
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangePowerEdit();
};
