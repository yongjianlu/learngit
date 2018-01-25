// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� PSSBERT1II_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// PSSBERT1II_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifndef PSS_BERT_1_II_H_H_
#define PSS_BERT_1_II_H_H_

#include "SerialPort.h"

#ifdef PSSBERT1II_EXPORTS
#define PSSBERT1II_API __declspec(dllexport)
#else
#define PSSBERT1II_API __declspec(dllimport)
#endif

// �����Ǵ� PSS-BERT-1-II.dll ������
class PSSBERT1II_API PssBert1II {
public:
	PssBert1II();
	~PssBert1II();

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
	 *���������ǣ�����TRUE��ʾ���ӳɹ���FALSE����ʧ��
	 *������
	 *	ComNum��	���±� i = ż�� ʱ��ֵԪ�ر�ʾ����ʹ��״̬��ȫ����ʼ��Ϊ-1����ʾ�ô��ں�δʹ�ù���
	 *				��Ӧ�� �±�Ϊ i + 1 ������Ԫ�ر�ʾ���ں�
	 *	ComNumLen:	����ĳ���
	 */
	
	BOOL CloseDevice();
	/*
	 *�Ͽ��豸�������ǣ������ӣ��ɹ�����TRUE��ʧ�ܷ���FALSE
	 */

	BOOL InitDevice(int Channel, int Type, int Rate, int Amplitude);
	/*
	 *���豸���г�ʼ�����������������ǵĹ���ģʽ���ɹ�����TRUE
	 *������
	 *	Channel��ѡ���ͨ�����ͣ�0��ʾ��ͨ����1��ʾ��ͨ��
	 *	Type   ��ѡ������ͣ�0��ʾPRBS7,1��ʾPRBS31
	 *	Rate   ��ѡ������ʣ�0 ~ 6�ֱ��ʾ9.95Gbps,10.31Gbps,10.52Gbps,10.70Gbps,11.09Gbps,11.32Gbps,10Gbps
	 *	Amplitude: ѡ��ķ��ȣ�0 ~ 7 �ֱ��ʾ 100 ~ 800 mVppd
	 */

	BOOL BeginBitErrTest(int Channel);
	/*
	 *���������⣬�ɹ�����TRUE
	 *������
	 *	Channel����������ͨ����0��ʾ��ͨ����1��ʾ��ͨ����2��ʾ���߶�����
	 */

	BOOL StopBitErrTest(int Channel);
	/*
	 *ֹͣ�����⣬�ɹ�����TRUE
	 *������
	 *	Channel��ֹͣ����ͨ����0��ʾ��ͨ��ֹͣ��1��ʾ��ͨ��ֹͣ��2��ʾ���߶�ֹͣ
	 */

	BOOL ResetDevice();
	/*
	 *��λ�豸��ʹ�����ǽ�����������״̬��������Ӧ�����������ɹ�����TRUE
	 */

	BOOL QueryBitErrState(int Channel, int & Sync, INT64 & BitErrNum, double & Ber);
	/*
	 *��ѯ��ǰ������״̬���ɹ�����TRUE
	 *������
	 *	Channel����Ҫ��ѯ��ͨ����0��ʾ��ͨ����1��ʾ��ͨ����2��ʾ���߶���ѯ
	 *	Sync   ����ѯ�ɹ���õ���ͬ����ʧ��״̬��Ϊ0ʱ��ʾͬ����1��ʾʧ��,2��ʾ������
	 *	BitErrNum: ��ѯ�ɹ���õ���������
	 *	Ber		 ����ѯ�ɹ���õ���������
	 */

	BOOL QueryDeviceConfig(int Channel, int & Type, int & Rate, int & Amplitude);
	/*
	 *��ѯ�豸��ǰ��������Ϣ���ɹ�����TRUE
	 *������
	 *	Channel��ѡ���ѯ��ͨ����0��ʾ��ͨ����1��ʾ��ͨ��
	 *	Type   ����ѯ�ɹ���õ������ͣ�0��ʾPRBS7,1��ʾPRBS31
	 *	Rate   ����ѯ�ɹ���õ������ʣ�0 ~ 6�ֱ��ʾ9.95Gbps,10.31Gbps,10.52Gbps,10.70Gbps,11.09Gbps,11.32Gbps,10Gbps
	 *	Amplitude: ��ѯ�ɹ���õ��ķ��ȣ�0 ~ 7 �ֱ��ʾ 100 ~ 800 mVppd
	 */
};

#endif