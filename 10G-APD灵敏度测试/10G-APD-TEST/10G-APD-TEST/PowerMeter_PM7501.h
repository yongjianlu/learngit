// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� POWERMETER_PM7501_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// POWERMETER_PM7501_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifndef POWERMETER_PM7501_H_H_H
#define POWERMETER_PM7501_H_H_H

#include "SerialPort.h"

#ifdef POWERMETER_PM7501_EXPORTS
#define POWERMETER_PM7501_API __declspec(dllexport)
#else
#define POWERMETER_PM7501_API __declspec(dllimport)
#endif

class POWERMETER_PM7501_API CPowerMeter_PM7501 {
public:
	CPowerMeter_PM7501(void);
   ~CPowerMeter_PM7501(void);

private:
	BOOL m_DeviceConnect;	//�豸����״̬��Ϊtrueʱ��ʾ�����ӣ�false��ʾδ���ӣ��Ͽ����ӣ�
	SerialPort * m_Comm;	//����ָ�򴮿�ͨ�������

public:

	BOOL GetConnectState();
	/*
	 *��ȡ����״̬������TRUE��ʾ�����ӣ�FALSE��ʾδ���ӻ��ѶϿ�����
	 */
	
	BOOL OpenDevice(int ComNum[], int ComNumLen);
	/*
	 *���ӹ��ʼƣ�����TRUE��ʾ���ӳɹ���FALSE����ʧ��
	 *������
	 *	ComNum��	���±� i = ż�� ʱ��ֵԪ�ر�ʾ����ʹ��״̬��ȫ����ʼ��Ϊ-1����ʾ�ô��ں�δʹ�ù���
	 *				��Ӧ�� �±�Ϊ i + 1 ������Ԫ�ر�ʾ���ں�
	 *	ComNumLen:	����ĳ���
	 */
	
	BOOL CloseDevice();
	/*
	 *�Ͽ��豸�����ʼƣ������ӣ��ɹ�����TRUE��ʧ�ܷ���FALSE
	 */
	
	BOOL GetPower(float & fPowerVal);
	/*
	 *��ȡ�⹦�ʼƵ�ǰ�⹦�ʣ�ʧ�ܷ���FALSE
	 */

	BOOL SetWaveLength(int IntWaveLength);
	/*
	 *���ù⹦�ʼƵĲ������ɹ�����true
	 *IntWaveLength������ѡ��0 ~ 6 ���α�ʾ850,980,1046,1310,1480,1550,1610 nm
	 */

	BOOL GetWaveLength(int & IntWaveLength);
	/*
	 *���ȡ�⹦�ʼƵ�ǰ�Ĳ������ɹ�����true
	 *IntWaveLength������ѡ��0 ~ 6 ���α�ʾ850,980,1046,1310,1480,1550,1610 nm
	 */

};

#endif
