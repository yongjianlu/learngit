// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� AITVOA01_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// AITVOA01_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

#ifndef AIT_VOA_01_H_H
#define AIT_VOA_01_H_H

#include "SerialPort.h"

#ifdef AITVOA01_EXPORTS
#define AITVOA01_API __declspec(dllexport)
#else
#define AITVOA01_API __declspec(dllimport)
#endif

class AITVOA01_API CAITVOA01 {
public:
	CAITVOA01(void);
   ~CAITVOA01(void);

private:
	BOOL m_DeviceConnect;	//�豸����״̬��Ϊtrueʱ��ʾ�����ӣ�false��ʾδ���ӣ��Ͽ����ӣ�
	SerialPort * m_Comm;	//����ָ�򴮿�ͨ�������

public:

	
	BOOL GetConnectState();
	/*
	 *��ȡ����״̬������TRUE��ʾ�����ӣ�FALSE��ʾδ���ӻ��ѶϿ�����
	 */
	
	BOOL OpenDevice(int * ComNum, int ComNumLen);
	/*
	 *���ӹ�˥����������TRUE��ʾ���ӳɹ���FALSE����ʧ��
	 *������
	 *	ComNum��	���±� i = ż�� ʱ��ֵԪ�ر�ʾ����ʹ��״̬��ȫ����ʼ��Ϊ-1����ʾ�ô��ں�δʹ�ù���
	 *				��Ӧ�� �±�Ϊ i + 1 ������Ԫ�ر�ʾ���ں�
	 *	ComNumLen:	����ĳ���
	 */
	
	BOOL CloseDevice();
	/*
	 *�Ͽ��豸����˥�����������ӣ��ɹ�����TRUE��ʧ�ܷ���FALSE
	 */

	BOOL SetAttVal(float fAttVal);
	/*
	 *����˥������˥��ֵ���ɹ�����TRUE
	 *������
	 *	fAttVal��˥������Ŀ��˥��ֵ
	 */

	BOOL SetWaveLength(int IntWaveLength);
	/*
	 *���ù�˥�����Ĳ������ɹ�����true
	 *������
	 *	IntWaveLength������ѡ��0 ��ʾ 1310 nm��1 ��ʾ 1550 nm
	 */

	BOOL GetWaveLengthAtt(int & IntWaveLength, float & fAttVal);
	/*
	 *��ȡ��˥������ǰ�Ĳ�����˥��ֵ���ɹ�����true
	 *������
	 *	IntWaveLength��0 ��ʾ 1310 nm��1 ��ʾ 1550 nm
	 *	fAttVal����ȡ���ĵ�ǰ˥��ֵ
	 */

};

#endif