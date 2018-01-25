#pragma once

class SaveData
{
public:
	SaveData(void);
	~SaveData(void);

public:

	CString m_Model;			//ģ���ͺ� 
	CString m_WorkOrderNo;		//������ 
	CString m_BatchNo;			//���κ� 
	CString m_PDNo;				//PD�ͺ�
	CString m_SerialNo;			//SN��
	CString m_TestEr;			//������Ա 
	CString m_TestTime;			//����ʱ��  

	int m_VbrTestPass;		//VBR���Խ��
	int m_SenTestPass;		//�����Ȳ��Խ��
	int m_RxTestPass;		//Rx���Խ��

	float m_IccVal;			//Iccֵ
	float m_IdVal;			//Idֵ
	float m_VbrVal;			//Vbrֵ
	float m_SenVal;			//������ֵ

	void SaveDataInit();
	/*
	 *��ʼ����Ա����
	 */
};
