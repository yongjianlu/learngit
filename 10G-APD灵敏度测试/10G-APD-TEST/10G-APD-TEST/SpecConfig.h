#pragma once
#include "afxwin.h"


// SpecConfig �Ի���

class SpecConfig : public CDialog
{
	DECLARE_DYNAMIC(SpecConfig)

public:
	SpecConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SpecConfig();

// �Ի�������
	enum { IDD = IDD_SpecConfig_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedNexthigleveButton();

	CComboBox m_Channel_Combo;		//������ͨ��ѡ��Combo�ؼ�
	CComboBox m_Type_Combo;			//����������ѡ��Combo�ؼ�
	CComboBox m_Rate_Combo;			//����������ѡ��Combo�ؼ�
	CComboBox m_Amplitude_Combo;	//�����Ƿ���ѡ��Combo�ؼ�
	CComboBox m_VopVal_Combo;		//Vbr������VOPȡֵ��ʽѡ��Combo�ؼ�
	CComboBox m_PowerWaveLen_Combo;	//�⹦�ʼƲ���ѡ��Combo�ؼ�
	CComboBox m_AttWaveLen_Combo;	//��˥��������ѡ��Combo�ؼ�

	float m_fIccMax_edit;			//Icc�������ֵ
	float m_fIccMin_edit;			//Icc������Сֵ
	float m_fVbrMax_edit;			//Vbr�������ֵ
	float m_fVbrMin_edit;			//Vbr������Сֵ
	float m_fSenGoVal_edit;			//Sen��ʼֵ
	float m_fSenStep_edit;			//Sen�仯�������ҹ۲��ʱ��
	float m_fSenMax_edit;			//Sen����ֵ���ֵ
	float m_fSenMin_edit;			//Sen������Сֵ
	float m_fIdMax_edit;			//Id�������ֵ
	float m_fIdMin_edit;			//Id������Сֵ
	float m_fPowerVal_edit;			//ʵ�ʹ⹦��ֵ����У׼ʱ�ã�
	float m_fTanP_edit;				//�⹦��ʵ��ֵ�� �豸��ȡ����ֵ �Ĳ�ֵ

	void DeviceConfigControls();
	/*
	 *�豸���ÿؼ���ʼ��
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
	 *Spin�ؼ��Ĺ�������
	 */
};
