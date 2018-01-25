#pragma once

class SaveData
{
public:
	SaveData(void);
	~SaveData(void);

public:

	CString m_Model;			//模块型号 
	CString m_WorkOrderNo;		//工单号 
	CString m_BatchNo;			//批次号 
	CString m_PDNo;				//PD型号
	CString m_SerialNo;			//SN号
	CString m_TestEr;			//测试人员 
	CString m_TestTime;			//测试时间  

	int m_VbrTestPass;		//VBR测试结果
	int m_SenTestPass;		//灵敏度测试结果
	int m_RxTestPass;		//Rx测试结果

	float m_IccVal;			//Icc值
	float m_IdVal;			//Id值
	float m_VbrVal;			//Vbr值
	float m_SenVal;			//灵敏度值

	void SaveDataInit();
	/*
	 *初始化成员函数
	 */
};
