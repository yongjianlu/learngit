// 10G-APD-TESTDlg.h : 头文件
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

// CMy10GAPDTESTDlg 对话框
class CMy10GAPDTESTDlg : public CDialog
{
// 构造
public:
	CMy10GAPDTESTDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY10GAPDTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	virtual void OnOK();
	virtual void OnCancel();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


	SaveData * Data;	//将要保存的数据的结构

	CAITVOA01			* m_AitVoa;		//光衰减器
	CAPATVBR			* m_ApatVbr;	//VBR测试仪
	CPowerMeter_PM7501	* m_PM7501;		//光功率计
	PssBert1II			* m_Bert;		//误码仪

	GuidCtrlMethod m_Method;//表格控件操作方法变量

	CGridCtrl m_GridDS;		//"数据统计"表格控件变量
	CGridCtrl m_GridFitt;	//"灵敏度测试拟合"表格控件变量
	CGridCtrl m_GridTS;		//"测试结果"表格控件变量

	CProgressCtrl m_ctrlProgress;		//进度条

	CMyButton m_ButtonExtractData;		//“提取数据”按钮重绘
	CMyButton m_ButtonConfigInit;		//“设备初始化”按钮重绘
	CMyButton m_ButtonBeginTest;		//“开启批量测试”按钮重绘
	CMyButton m_ButtonCalibration1;		//“光校准1”按钮重绘

	CComboBox m_Channel_Combo;		//误码仪通道选择Combo控件
	CComboBox m_Type_Combo;			//误码仪码型选择Combo控件
	CComboBox m_Rate_Combo;			//误码仪速率选择Combo控件
	CComboBox m_Amplitude_Combo;	//误码仪幅度选择Combo控件
	CComboBox m_VopVal_Combo;		//Vbr测试仪VOP取值方式选择Combo控件
	CString   m_strVbrVal_edit;		//误码率
	CComboBox m_PowerWaveLen_Combo;	//光功率计波长选择Combo控件
	CComboBox m_AttWaveLen_Combo;	//光衰减器波长选择Combo控件

	CString m_strWorkOrderNo_edit;	//工单号
	CString m_strBatchNo_edit;		//批次号
	CString m_strPDModel_edit;		//PD型号
	CString m_strModel_edit;		//产品类型
	CString m_strSn_edit;			//SN号（系列号）
	CString m_strTestEr_edit;		//测试人员

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
	float m_fTanP_edit;				//光功率实际值与 设备读取到的值 的差值
	int   m_SenTime_edit;			//SEN测试时间
	float m_CompenVal_edit;			//灵敏度补偿值

	float m_SenTestBeginPowVal;		//当前产品灵敏度测试光功率起始值
	float m_PowStep1;				//查找灵敏度测试中的光功率步进值1(失步状态时)
	float m_PowStep2;				//查找灵敏度测试中的光功率步进值2(误码率为0时)

	CButton m_Check1;				//选择框控件

	void ControlsTxetInit();
	/*
	 *控件初始化
	 */

	void DeviceConfigControls();
	/*
	 *设备配置控件初始化
	 */

	void GridInit();
	/*
	 *表格控件初始化
	 */

	void ButtonInit();
	/*
	 *Button控件初始化，对Button控件进行重绘
	 */

	afx_msg void OnEnChangeWorkordernoEdit();
	/*
	 *工单号编辑框消息函数
	 */

	afx_msg void OnEnChangeBatchnoEdit();
	/*
	 *批次号编辑框消息函数
	 */

	afx_msg void OnEnChangePdmodelEdit();
	/*
	 *PD型号 编辑框
	 */

	afx_msg void OnEnChangeSnEdit();
	/*
	 *系列号编辑框消息函数
	 */

	afx_msg void OnEnChangePowervalEdit();
	/*
	 *实际光功率编辑框消息函数
	 */

	afx_msg void OnBnClickedCalibration1Button();
	/*
	 *“开始光校准”按钮消息函数
	 */

	afx_msg void OnBnClickedExtractdataButton();
	/*
	 *“规格数据提取” 按钮消息函数
	 */

	afx_msg void OnBnClickedDeviceinitButton();
	/*
	 *“设备初始化”按钮消息函数
	 */

	afx_msg void OnBnClickedBegintestButton();
	/*
	 *“开启批量测试”按钮消息函数
	 */

	void GetComNum();
	/*
	 *获取主机当前可用串口，并将其存放在 ComNum[]数组的 下标为奇数的元素中，下标为偶数表示串口的使用状态 
	 *如 ComNum[] = {-1, 1, 1, 2};表示 COM1 串口未使用，串口 COM2 已被使用
	 */

	BOOL DeviceConnet();
	/*
	 *连接各个设备，成功返回TRUE，失败返回FALSE
	 */

	void GetCalState();
	/*
	 *获取光校准的状态
	 */

	void SetCalState();
	/*
	 *设置光校准的状态
	 */

	void CalStatePass();
	/*
	 *校准成功
	 */

	void CalStateFail();
	/*
	 *校准失败
	 */

	static UINT ThreadTimeRecord( LPVOID lParam );
	/*
	 *调测时间记录线程
	 */

	static UINT ThreadTest( LPVOID lParam );
	/*
	 *调试线程
	 */

	BOOL IccTest();
	/*
	 *进行Icc测试，读取Icc的值。（无光条件下）
	 *返回值：
	 *	成功返回 TRUE，失败返回 FASLE
	 */

	BOOL VbrTest();
	/*
	 *进行VBR测试，读取 Id 值和 Vbr 值
	 *返回值：
	 *成功返回 TRUE，失败返回 FALSE
	 */

	BOOL SenTest();
	/*
	 *灵敏度测试，成功返回 TRUE， 失败返回 FALSE
	 */

	DWORD SetPowerVal(float PowerVal);
	/*
	 *通过调节光衰减器的衰减值，来设置光功率计的 光功率值(器件的接收光)
	 *参数：
	 *	PowerVal：目标光功率值
	 *返回值：
	 *	0：设置成功
	 *	1：读取光功率值失败
	 *	2：读取衰减值失败
	 *	3：设置衰减值失败
	 */

	BOOL GetBitErrState(int Channel, int & Sync, INT64 & BitErrNum);
	/*
	 *获取当前光功率下的同、失步状态和误码数量，成功返回 TRUE 
	 */

	BOOL GetSenTestBeginPowVal(float & fBeginPowVal);
	/*
	 *获取当前产品灵敏度测试光功率起始值,成功返回 TRUE
	 */

	BOOL ObservationPoints(float x_Data[], float y_Data[], int IntNum);
	/*
	 *获取观察点,成功返回TRUE，失败返回FALSE
	 *参数：
	 *	x_Data：获取的观察点的x轴数据
	 *	y_Data：获取的观察点的y轴数据
	 *	IntNum：想要获取的观察点个数
	 */

	DWORD FittTest(float x_Data[], float y_Data[], float Coefficient[], int Poly_n);
	/*
	 *根据观察点数据，进行直线拟合计算
	 *参数：
	 *	x_Data：观察点 x 轴数据
	 *	y_Data：观察点 y 轴数据
	 *	Coefficient：经过拟合计算后的到的数据，分别是 a,b,c,d,e 等待（个数与 Poly_n 项有关）
	 *	Poly_n：将要拟合的项数，如 Poly_n = 1时，按 y = a * x^1 + b; 进行拟合得到 a、b 系数
	 *返回值：
	 *	0：拟合计算成功
	 */

	BOOL JudgeWorkOrderNo();
	/*
	 *判断工单号是否存在，存在则返回TRUE
	 */

	BOOL JudgeBatchNo();
	/*
	 *判断批次号是否存在(确认工单号存在后)，存在则返回TRUE
	 */

	BOOL JudgeSN();
	/*
	 *判断输入的SN号是否正确，存在则返回TRUE
	 */

	BOOL JudgeSNtoWorkOrder();
	/*
	 *判断SN号是否属于该工单号(确认工单号存在后)，存在则返回TRUE
	 */
	
	BOOL JudgeSNtoBatch();
	/*
	 *判断SN号是否属于该批次号(确认工单号、批次号存在后)，存在则返回TRUE
	 */

	BOOL GetBatchToSN(CString Serial_No[]);
	/*
	 *获取所属 输入的工单号与批次号下的 SN 号,成功返回 TRUE
	 */

	BOOL GetModelSpec();
	/*
	 *获取工单所属型号的测试规格 成功返回 TRUE
	 */

	BOOL GetProductTestSpec_TS();
	/*
	 *获取当前输入工单的产品的测试规格（从TS文件中获取），成功返回 TRUE
	 */

	BOOL ReadTsFileTestSpec();
	/*
	 *读取本地指定TS文件中的产品测试规格
	 */

	BOOL GetTestInfoToWorkOrder();
	/*
	 *获取输入的工单的测试信息，（已测试数量、良品率 等）
	 */

	void BeginTest();
	/*
	 *开启批量测试
	 */

	void StopTest();
	/*
	 *停止批量测试
	 */

	BOOL SqlServerConnet();
	/*
	 *服务器数据库连接初始化
	 */

	BOOL SaveTestDataServer();
	/*
	 *保存测试数据到服务器当中
	 */

	BOOL AccessConnet();
	/*
	 *本地数据库连接初始化
	 */

	BOOL SaveTestDataAccess();
	/*
	 *保存测试数据在本地的Access数据库中
	 */

	void TestFail();
	/*
	 *测试失败，给出失败提示
	 */

	void TestPass();
	/*
	 *测试成功，给出成功提示
	 */

	void BeginDeviceTest();
	/*
	 *器件开始测试
	 */

	void SetControlsOptional(int ID, bool State);
	/*
	 *设置某个控件的可操作状态
	 *ID：控件的ID
	 *State：状态控制，true表示可操作，false表示不可操作
	 */

	void SetDebugParaCtrlStatus(bool setVal);
	/*
	 *自动调试可选项控件状态控制
	 */ 

	BOOL ApdVccOutPut(BOOL State);
	/*
	 *开启/关闭 APD、VCC电压输出
	 *参数：
	 *	State ：为 TRUE 是表示开启，为 FALSE 时表示关闭
	 */

	BOOL JudgeStep(CString strSN);
	/*
	 *对指定系列号的器件进行工序判断，符合当前工序返回TRUE，否则返回FASLE
	 */

	BOOL GotoRepair();
	/*
	 *进入返修
	 */
	
	void TestResultsDeal();
	/*
	 *测试结果处理
	 */
	afx_msg void OnBnClickedTestspecCheck();
};
