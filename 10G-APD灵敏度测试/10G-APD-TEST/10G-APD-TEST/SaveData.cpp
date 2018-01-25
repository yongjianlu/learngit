#include "StdAfx.h"
#include "SaveData.h"

SaveData::SaveData(void)
{
	SaveDataInit();
}

SaveData::~SaveData(void)
{

}

//初始化成员函数
void SaveData::SaveDataInit()
{
	m_Model			= _T("");		//模块型号 
	m_WorkOrderNo	= _T("");		//工单号 
	m_BatchNo		= _T("");		//批次号 
	m_PDNo			= _T("");		//PD型号
	m_SerialNo		= _T("");		//SN号
	m_TestEr		= _T("");		//测试人员 
	m_TestTime		= _T("");		//测试时间  

	m_VbrTestPass = 0;		//VBR测试结果
	m_SenTestPass = 0;		//灵敏度测试结果
	m_RxTestPass  = 0;		//Rx测试结果

	m_IccVal = 0.0f;		//Icc值
	m_IdVal  = 0.0f;		//Id值
	m_VbrVal = 0.0f;		//Vbr值
	m_SenVal = 0.0f;		//灵敏度值
}