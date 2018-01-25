// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� APATVBR_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// APATVBR_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifndef APATVBR_H_H_H
#define APATVBR_H_H_H

#include "SerialPort.h"

#ifdef APATVBR_EXPORTS
#define APATVBR_API __declspec(dllexport)
#else
#define APATVBR_API __declspec(dllimport)
#endif

// �����Ǵ� APAT-VBR.dll ������
class APATVBR_API CAPATVBR {

public:
	CAPATVBR();
	~CAPATVBR();

private:
	BOOL m_DeviceConnect;	//�豸����״̬��Ϊtrueʱ��ʾ�����ӣ�false��ʾδ���ӣ��ѶϿ����ӣ�
	SerialPort * m_Comm;	//����ָ�򴮿�ͨ�������

public:

	BOOL GetConnectState();
	/*
	 *��ȡ����״̬������TRUE��ʾ�����ӣ�FALSE��ʾδ���ӻ��ѶϿ�����
	 */

	BOOL OpenDevice(int ComNum[], int ComNumLen);
	/*
	 *����Vbr�豸������TRUE��ʾ���ӳɹ���FALSE����ʧ��
	 *������
	 *	ComNum��	���±� i = ż�� ʱ��ֵԪ�ر�ʾ����ʹ��״̬��ȫ����ʼ��Ϊ-1����ʾ�ô��ں�δʹ�ù���
	 *				��Ӧ�� �±�Ϊ i + 1 ������Ԫ�ر�ʾ���ں�
	 *	ComNumLen:	����ĳ���
	 */
	
	BOOL CloseDevice();
	/*
	 *�Ͽ��豸��Vbr�����ǣ������ӣ��ɹ�����TRUE��ʧ�ܷ���FALSE
	 */

	BOOL ReadVoltageVal(int IntReadState, float & fVoltageVal);
	/*
	 *��ȡ��ѹ�ϱ������ɹ�����TRUE
	 *������
	 *	IntReadState����Ҫ��ȡ�����ͣ�0 ~ 3 �ֱ��ʾ VAPD��ѹ��VBR��ѹ��VPD��ѹ��VCC��ѹ
	 *	fVoltageVal����Ŷ�ȡ���ĵ�ѹ���ݣ���λ�� ��V��
	 */

	BOOL ReadCurrentVal(int IntReadState, float & fCurrentVal);
	/*
	 *��ȡ�����ϱ������ɹ�����TRUE
	 *������
	 *	IntReadState����Ҫ��ȡ�����ͣ�0 ~ 3 �ֱ��ʾ VAPD������VPD������VCC������RSSI����
	 *	fCurrentVal����Ŷ�ȡ���ĵ�ѹ���ݣ���λ�� ��mA��
	 */

	BOOL SetDeviceWorkMode(int IntModeState);
	/*
	 *�����豸����ģʽ,�ɹ�����TRUE 
	 *������
	 *	IntModeState����Ҫ���õĹ���ģʽ��ȡֵ����
	 *		1��VBR����ģʽ��������Ϻ�����κ� VAPD��ѹ��Ĭ��ģʽ��
	 *		2��VBR����ģʽ��������Ϻ�����κ� VAPD��ѹ = VBR
	 *		3��VBR����ģʽ��������Ϻ�����κ� VAPD��ѹ = A * VBR��AĬ��ֵΪ0.9
	 *		4��VBR����ģʽ��������Ϻ�����κ� VAPD��ѹ = VBR - OffSet��OffSetĬ��ֵΪ 3(V)
	 *		5��VAPD�̶����ģʽ1����ģʽ�̶����Ԥ��� VAPD = 10 ��V��
	 *		6��VAPD�̶����ģʽ2����ģʽ�̶����Ԥ��� VAPD = 36 ��V��
	 *		7��VAPD�û��������ģʽ��������÷�ΧΪ 5 V ~ 60 V��Ĭ���� 5 V
	 *		��
	 *		��
	 *		��
	 *		15������15��ģʽ
	 */

	BOOL SetVoltageOutputVal(int IntOutputMode, float fVoltageVal);
	/*
	 *���� VAPD ��ѹ��������ڷ�VBE����ģʽ�£����ɹ�����TRUE
	 *������
	 *	fVoltageVal����Ҫ���õ����ֵ����λ�� ��V��
	 *	IntOutputMode������ĵ�ѹ���ͣ�ȡֵ����
	 *		0����ʾVAPD��ѹ��fVoltageValȡֵ��Χ�� 5 ~ 60 V
	 *		1����ʾAPD��ѹ�� fVoltageValȡֵ��Χ�� 3 ~ 5.2 V
	 *		2����ʾVCC��ѹ�� fVoltageValȡֵ��Χ�� 3 ~ 5.2 V
	 */

	BOOL VoltageOutputEnable(int IntOutputMode, BOOL EnableState);
	/*
	 *��ѹ���ʹ�ܣ����ǿ���/�رյ�ѹ������ɹ�����TRUE
	 *������
	 *	IntOutputMode��ʹ������ĵ�ѹ���ͣ�ȡֵ����
	 *		0����ʾVAPD��ѹ��
	 *		1����ʾAPD��ѹ��
	 *		2����ʾVCC��ѹ��
	 *	EnableState��ΪTRUEʱ��ʾ���ã�Ϊ FALSEʱ��ʾ����
	 */

	BOOL GotoVbrTestMode();
	/*
	 *����������VBR���Խ��棨ǰ�����豸ѡ����VBR����ģʽ�����ɹ�����TRUE
	 */

	BOOL BeginVbrTest();
	/*
	 *���� VBR ����������ǿ��� VBR ���ԣ�������ʱ 2 �����ϣ��ɹ�����TRUE
	 */

	DWORD IccTestResult(float & IccVal);
	/*
	 *����Icc���ԣ���ȡIccֵ
	 *������
	 *	IccVal���ɹ����ȡ��Icc��ֵ
	 *
	 *����ֵ��
	 *	0�����Զ�ȡ�ɹ�
	 *	1��ѡ���5����ģʽʧ��
	 *	2������VAPD��ѹ���ʧ��
	 *	3������VCC��ѹ���ʧ��
	 *	4����ȡIccֵ��Vcc����ֵʧ��
	 *	5���ر�VAPD��ѹ���ʧ��
	 *	6���ر�VCC��ѹ���ʧ��
	 */

	DWORD VbrTestResult(float & VbrVal,float & IdVal, float & IccVal);
	/*
	 *����Vbr���ԣ���ȡ Vbr ��ѹֵ�� Id ֵ��VAPD��������Icc ֵ��Vcc����ֵ��
	 *������
	 *	VbrVal����ȡ���� Vbr ֵ
	 *	IdVal�� ��ȡ���� Id  ֵ
	 *	IccVal: ��ȡ���� Icc ֵ
	 *
	 *����ֵ��
	 *	0�����Զ�ȡ���ݳɹ�
	 *	1��ѡ���3����ģʽʧ��
	 *	2������VAPD��ѹ���ʧ
	 *	3������VCC��ѹ���ʧ��
	 *	4������������VBR���Խ���ʧ��
	 *	5������ VBR ����ʧ��
	 *	6����ȡ Vbr ֵ��Vbr��ѹֵ��ʧ��
	 *	7����ȡ Id  ֵ��VAPD����ֵ��ʧ��
	 *	8����ȡ Icc ֵʧ��
	 */

};


#endif