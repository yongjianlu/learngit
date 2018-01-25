#pragma once
#include "afxwin.h"


// SpecConfig 对话框

class SpecConfig : public CDialog
{
	DECLARE_DYNAMIC(SpecConfig)

public:
	SpecConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SpecConfig();

// 对话框数据
	enum { IDD = IDD_SpecConfig_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedNexthigleveButton();

	CComboBox m_Channel_Combo;		//误码仪通道选择Combo控件
	CComboBox m_Type_Combo;			//误码仪码型选择Combo控件
	CComboBox m_Rate_Combo;			//误码仪速率选择Combo控件
	CComboBox m_Amplitude_Combo;	//误码仪幅度选择Combo控件
	CComboBox m_VopVal_Combo;		//Vbr测试仪VOP取值方式选择Combo控件
	CComboBox m_PowerWaveLen_Combo;	//光功率计波长选择Combo控件
	CComboBox m_AttWaveLen_Combo;	//光衰减器波长选择Combo控件

	float m_fIccMax_edit;			//Icc门限最大值
	float m_fIccMin_edit;			//Icc门限最小值
	float m_fVbrMax_edit;			//Vbr门限最大值
	float m_fVbrMin_edit;			//Vbr门限最小值
	float m_fSenGoVal_edit;			//Sen起始值
	float m_fSenStep_edit;			//Sen变化步长（找观察点时）
	float m_fSenMax_edit;			//Sen门限值最大值
	float m_fSenMin_edit;			//Sen门限最小值
	float m_fIdMax_edit;			//Id门限最大值
	float m_fIdMin_edit;			//Id门限最小值
	float m_fPowerVal_edit;			//实际光功率值（光校准时用）
	float m_fTanP_edit;				//光功率实际值与 设备读取到的值 的差值

	void DeviceConfigControls();
	/*
	 *设备配置控件初始化
	 */

	afx_msg void OnEnChangeIccmaxEdit();
	afx_msg void OnEnChangeIccminEdit();
	afx_msg void OnEnChangeVbrmaxEdit();
	afx_msg void OnEnChangeVbrminEdit();
	afx_msg void OnEnChangeSengoEdit();
	afx_msg void OnEnChangeIdmaxEdit();
	afx_msg void OnEnChangeIdminEdit();
	afx_msg void OnEnChangeSenmaxEdit();
	afx_msg void OnEnChangeSenminEdit();
	afx_msg void OnEnChangeSenstepEdit();

	afx_msg void OnDeltaposIccmaxSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposIccminSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposVbrmaxSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposVbrminSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSengoSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSenstepSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposIdmaxSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposIdminSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSenmaxSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSenminSpin(NMHDR *pNMHDR, LRESULT *pResult);

	void SpinOperation(LPNMUPDOWN pNMUpDownTemp,float & fTemp);
	/*
	 *Spin控件的公共操作
	 */
};
