// 10G-APD-TESTDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "GuidCtrlMethod.h"
#include "Button/MyButton.h"

#include "AIT-VOA-01.h"
#include "APAT-VBR.h"
#include "PowerMeter_PM7501.h"
#include "PSS-BERT-1-II.h"
#include "SaveData.h"
#include "afxcmn.h"

// CMy10GAPDTESTDlg �Ի���
class CMy10GAPDTESTDlg : public CDialog
{
// ����
public:
	CMy10GAPDTESTDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY10GAPDTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	virtual void OnOK();
	virtual void OnCancel();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


	SaveData * Data;	//��Ҫ��������ݵĽṹ

	CAITVOA01			* m_AitVoa;		//��˥����
	CAPATVBR			* m_ApatVbr;	//VBR������
	CPowerMeter_PM7501	* m_PM7501;		//�⹦�ʼ�
	PssBert1II			* m_Bert;		//������

	GuidCtrlMethod m_Method;//���ؼ�������������

	CGridCtrl m_GridDS;		//"����ͳ��"���ؼ�����
	CGridCtrl m_GridFitt;	//"�����Ȳ������"���ؼ�����
	CGridCtrl m_GridTS;		//"���Խ��"���ؼ�����

	CProgressCtrl m_ctrlProgress;		//������

	CMyButton m_ButtonExtractData;		//����ȡ���ݡ���ť�ػ�
	CMyButton m_ButtonConfigInit;		//���豸��ʼ������ť�ػ�
	CMyButton m_ButtonBeginTest;		//�������������ԡ���ť�ػ�
	CMyButton m_ButtonCalibration1;		//����У׼1����ť�ػ�

	CComboBox m_Channel_Combo;		//������ͨ��ѡ��Combo�ؼ�
	CComboBox m_Type_Combo;			//����������ѡ��Combo�ؼ�
	CComboBox m_Rate_Combo;			//����������ѡ��Combo�ؼ�
	CComboBox m_Amplitude_Combo;	//�����Ƿ���ѡ��Combo�ؼ�
	CComboBox m_VopVal_Combo;		//Vbr������VOPȡֵ��ʽѡ��Combo�ؼ�
	CString   m_strVbrVal_edit;		//������
	CComboBox m_PowerWaveLen_Combo;	//�⹦�ʼƲ���ѡ��Combo�ؼ�
	CComboBox m_AttWaveLen_Combo;	//��˥��������ѡ��Combo�ؼ�

	CString m_strWorkOrderNo_edit;	//������
	CString m_strBatchNo_edit;		//���κ�
	CString m_strPDModel_edit;		//PD�ͺ�
	CString m_strModel_edit;		//��Ʒ����
	CString m_strSn_edit;			//SN�ţ�ϵ�кţ�
	CString m_strTestEr_edit;		//������Ա

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
	float m_fTanP_edit;				//�⹦��ʵ��ֵ�� �豸��ȡ����ֵ �Ĳ�ֵ
	int   m_SenTime_edit;			//SEN����ʱ��
	float m_CompenVal_edit;			//�����Ȳ���ֵ

	float m_SenTestBeginPowVal;		//��ǰ��Ʒ�����Ȳ��Թ⹦����ʼֵ
	float m_PowStep1;				//���������Ȳ����еĹ⹦�ʲ���ֵ1(ʧ��״̬ʱ)
	float m_PowStep2;				//���������Ȳ����еĹ⹦�ʲ���ֵ2(������Ϊ0ʱ)

	CButton m_Check1;				//ѡ���ؼ�

	void ControlsTxetInit();
	/*
	 *�ؼ���ʼ��
	 */

	void DeviceConfigControls();
	/*
	 *�豸���ÿؼ���ʼ��
	 */

	void GridInit();
	/*
	 *���ؼ���ʼ��
	 */

	void ButtonInit();
	/*
	 *Button�ؼ���ʼ������Button�ؼ������ػ�
	 */

	afx_msg void OnEnChangeWorkordernoEdit();
	/*
	 *�����ű༭����Ϣ����
	 */

	afx_msg void OnEnChangeBatchnoEdit();
	/*
	 *���κű༭����Ϣ����
	 */

	afx_msg void OnEnChangePdmodelEdit();
	/*
	 *PD�ͺ� �༭��
	 */

	afx_msg void OnEnChangeSnEdit();
	/*
	 *ϵ�кű༭����Ϣ����
	 */

	afx_msg void OnEnChangePowervalEdit();
	/*
	 *ʵ�ʹ⹦�ʱ༭����Ϣ����
	 */

	afx_msg void OnBnClickedCalibration1Button();
	/*
	 *����ʼ��У׼����ť��Ϣ����
	 */

	afx_msg void OnBnClickedExtractdataButton();
	/*
	 *�����������ȡ�� ��ť��Ϣ����
	 */

	afx_msg void OnBnClickedDeviceinitButton();
	/*
	 *���豸��ʼ������ť��Ϣ����
	 */

	afx_msg void OnBnClickedBegintestButton();
	/*
	 *�������������ԡ���ť��Ϣ����
	 */

	void GetComNum();
	/*
	 *��ȡ������ǰ���ô��ڣ����������� ComNum[]����� �±�Ϊ������Ԫ���У��±�Ϊż����ʾ���ڵ�ʹ��״̬ 
	 *�� ComNum[] = {-1, 1, 1, 2};��ʾ COM1 ����δʹ�ã����� COM2 �ѱ�ʹ��
	 */

	BOOL DeviceConnet();
	/*
	 *���Ӹ����豸���ɹ�����TRUE��ʧ�ܷ���FALSE
	 */

	void GetCalState();
	/*
	 *��ȡ��У׼��״̬
	 */

	void SetCalState();
	/*
	 *���ù�У׼��״̬
	 */

	void CalStatePass();
	/*
	 *У׼�ɹ�
	 */

	void CalStateFail();
	/*
	 *У׼ʧ��
	 */

	static UINT ThreadTimeRecord( LPVOID lParam );
	/*
	 *����ʱ���¼�߳�
	 */

	static UINT ThreadTest( LPVOID lParam );
	/*
	 *�����߳�
	 */

	BOOL IccTest();
	/*
	 *����Icc���ԣ���ȡIcc��ֵ�����޹������£�
	 *����ֵ��
	 *	�ɹ����� TRUE��ʧ�ܷ��� FASLE
	 */

	BOOL VbrTest();
	/*
	 *����VBR���ԣ���ȡ Id ֵ�� Vbr ֵ
	 *����ֵ��
	 *�ɹ����� TRUE��ʧ�ܷ��� FALSE
	 */

	BOOL SenTest();
	/*
	 *�����Ȳ��ԣ��ɹ����� TRUE�� ʧ�ܷ��� FALSE
	 */

	DWORD SetPowerVal(float PowerVal);
	/*
	 *ͨ�����ڹ�˥������˥��ֵ�������ù⹦�ʼƵ� �⹦��ֵ(�����Ľ��չ�)
	 *������
	 *	PowerVal��Ŀ��⹦��ֵ
	 *����ֵ��
	 *	0�����óɹ�
	 *	1����ȡ�⹦��ֵʧ��
	 *	2����ȡ˥��ֵʧ��
	 *	3������˥��ֵʧ��
	 */

	BOOL GetBitErrState(int Channel, int & Sync, INT64 & BitErrNum);
	/*
	 *��ȡ��ǰ�⹦���µ�ͬ��ʧ��״̬�������������ɹ����� TRUE 
	 */

	BOOL GetSenTestBeginPowVal(float & fBeginPowVal);
	/*
	 *��ȡ��ǰ��Ʒ�����Ȳ��Թ⹦����ʼֵ,�ɹ����� TRUE
	 */

	BOOL ObservationPoints(float x_Data[], float y_Data[], int IntNum);
	/*
	 *��ȡ�۲��,�ɹ�����TRUE��ʧ�ܷ���FALSE
	 *������
	 *	x_Data����ȡ�Ĺ۲���x������
	 *	y_Data����ȡ�Ĺ۲���y������
	 *	IntNum����Ҫ��ȡ�Ĺ۲�����
	 */

	DWORD FittTest(float x_Data[], float y_Data[], float Coefficient[], int Poly_n);
	/*
	 *���ݹ۲�����ݣ�����ֱ����ϼ���
	 *������
	 *	x_Data���۲�� x ������
	 *	y_Data���۲�� y ������
	 *	Coefficient��������ϼ����ĵ������ݣ��ֱ��� a,b,c,d,e �ȴ��������� Poly_n ���йأ�
	 *	Poly_n����Ҫ��ϵ��������� Poly_n = 1ʱ���� y = a * x^1 + b; ������ϵõ� a��b ϵ��
	 *����ֵ��
	 *	0����ϼ���ɹ�
	 */

	BOOL JudgeWorkOrderNo();
	/*
	 *�жϹ������Ƿ���ڣ������򷵻�TRUE
	 */

	BOOL JudgeBatchNo();
	/*
	 *�ж����κ��Ƿ����(ȷ�Ϲ����Ŵ��ں�)�������򷵻�TRUE
	 */

	BOOL JudgeSN();
	/*
	 *�ж������SN���Ƿ���ȷ�������򷵻�TRUE
	 */

	BOOL JudgeSNtoWorkOrder();
	/*
	 *�ж�SN���Ƿ����ڸù�����(ȷ�Ϲ����Ŵ��ں�)�������򷵻�TRUE
	 */
	
	BOOL JudgeSNtoBatch();
	/*
	 *�ж�SN���Ƿ����ڸ����κ�(ȷ�Ϲ����š����κŴ��ں�)�������򷵻�TRUE
	 */

	BOOL GetBatchToSN(CString Serial_No[]);
	/*
	 *��ȡ���� ����Ĺ����������κ��µ� SN ��,�ɹ����� TRUE
	 */

	BOOL GetModelSpec();
	/*
	 *��ȡ���������ͺŵĲ��Թ�� �ɹ����� TRUE
	 */

	BOOL GetProductTestSpec_TS();
	/*
	 *��ȡ��ǰ���빤���Ĳ�Ʒ�Ĳ��Թ�񣨴�TS�ļ��л�ȡ�����ɹ����� TRUE
	 */

	BOOL ReadTsFileTestSpec();
	/*
	 *��ȡ����ָ��TS�ļ��еĲ�Ʒ���Թ��
	 */

	BOOL GetTestInfoToWorkOrder();
	/*
	 *��ȡ����Ĺ����Ĳ�����Ϣ�����Ѳ�����������Ʒ�� �ȣ�
	 */

	void BeginTest();
	/*
	 *������������
	 */

	void StopTest();
	/*
	 *ֹͣ��������
	 */

	BOOL SqlServerConnet();
	/*
	 *���������ݿ����ӳ�ʼ��
	 */

	BOOL SaveTestDataServer();
	/*
	 *����������ݵ�����������
	 */

	BOOL AccessConnet();
	/*
	 *�������ݿ����ӳ�ʼ��
	 */

	BOOL SaveTestDataAccess();
	/*
	 *������������ڱ��ص�Access���ݿ���
	 */

	void TestFail();
	/*
	 *����ʧ�ܣ�����ʧ����ʾ
	 */

	void TestPass();
	/*
	 *���Գɹ��������ɹ���ʾ
	 */

	void BeginDeviceTest();
	/*
	 *������ʼ����
	 */

	void SetControlsOptional(int ID, bool State);
	/*
	 *����ĳ���ؼ��Ŀɲ���״̬
	 *ID���ؼ���ID
	 *State��״̬���ƣ�true��ʾ�ɲ�����false��ʾ���ɲ���
	 */

	void SetDebugParaCtrlStatus(bool setVal);
	/*
	 *�Զ����Կ�ѡ��ؼ�״̬����
	 */ 

	BOOL ApdVccOutPut(BOOL State);
	/*
	 *����/�ر� APD��VCC��ѹ���
	 *������
	 *	State ��Ϊ TRUE �Ǳ�ʾ������Ϊ FALSE ʱ��ʾ�ر�
	 */

	BOOL JudgeStep(CString strSN);
	/*
	 *��ָ��ϵ�кŵ��������й����жϣ����ϵ�ǰ���򷵻�TRUE�����򷵻�FASLE
	 */

	BOOL GotoRepair();
	/*
	 *���뷵��
	 */
	
	void TestResultsDeal();
	/*
	 *���Խ������
	 */
	afx_msg void OnBnClickedTestspecCheck();
};
