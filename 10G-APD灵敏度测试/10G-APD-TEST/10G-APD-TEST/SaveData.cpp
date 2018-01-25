#include "StdAfx.h"
#include "SaveData.h"

SaveData::SaveData(void)
{
	SaveDataInit();
}

SaveData::~SaveData(void)
{

}

//��ʼ����Ա����
void SaveData::SaveDataInit()
{
	m_Model			= _T("");		//ģ���ͺ� 
	m_WorkOrderNo	= _T("");		//������ 
	m_BatchNo		= _T("");		//���κ� 
	m_PDNo			= _T("");		//PD�ͺ�
	m_SerialNo		= _T("");		//SN��
	m_TestEr		= _T("");		//������Ա 
	m_TestTime		= _T("");		//����ʱ��  

	m_VbrTestPass = 0;		//VBR���Խ��
	m_SenTestPass = 0;		//�����Ȳ��Խ��
	m_RxTestPass  = 0;		//Rx���Խ��

	m_IccVal = 0.0f;		//Iccֵ
	m_IdVal  = 0.0f;		//Idֵ
	m_VbrVal = 0.0f;		//Vbrֵ
	m_SenVal = 0.0f;		//������ֵ
}