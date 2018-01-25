#ifndef SERIALPORT_H_H_
#define SERIALPORT_H_H_


#include <string>
#include <windows.h>

using namespace std;
#define LEN 1024

#ifdef SERIALPORT_EXPORTS
#define SERIALPORT_API __declspec(dllexport)
#else
#define SERIALPORT_API __declspec(dllimport)
#endif


class SERIALPORT_API SerialPort
{

private:

	COMSTAT ComStat;
	DWORD	dwErrorFlags;

	DWORD PciStrToHex(string str,BYTE (&WriteData)[LEN]);
	/*
	 *��PCI���������ַ�����ת��Ϊ16���Ƶ�����
	 *str:		��Ҫ����ת�����ַ���
	 *senddata�����ת�����16��������
	 *����ֵ��  ����senddata����Ч���ݳ���
	 */

	BYTE ConvertHexChar(char ch);
	/*
	 *�ú�����һ�����ַ�ת��Ϊ��Ӧ��ʮ������ֵ�ĺ���
	 *���ܣ�������0-F֮����ַ�����ת��Ϊ��Ӧ��ʮ�������ַ������򷵻�-1
	 */

public:

	HANDLE  m_hCom;//���ھ��

	SerialPort(void);
	~SerialPort(void);

	
	int HexStrToHex(string str,BYTE WriteData[]);
	/*
	 *��ÿһ����16�����ַ���ת��Ϊ16���Ƶ�����
	 *str:		��Ҫ����ת�����ַ���
	 *senddata�����ת�����16��������
	 *����ֵ��  ����senddata����Ч���ݳ���
	 */

	BOOL OpenSerialPort(int SerialPortCom);
	/*
	 *��ָ���Ĵ���
	 *SerialPortCom����Ҫ�򿪵Ĵ���
	 *����ֵ��		 �򿪳ɹ�����TRUE��ʧ�ܷ���FALSE
	 */

	BOOL CloseSerialPort();
	/*
	 *�ر��Ѵ򿪵Ĵ��ڡ��ɹ�����TRUE
	 */

	BOOL SetSerialPort(DWORD dwBaudRate);
	/*
	 *����ָ���Ĳ����ʣ���Ĭ������Ϊ ��У��λ��8����λ��1ֹͣλ
	 *dwBaudRate �� ָ���Ĳ�����
	 *����ֵ��		�ɹ�����TRUE��ʧ�ܷ���FALSE
	 */

	BOOL SetSerialPort(DWORD dwBaudRate, BYTE btParity, BYTE btByteSize, BYTE btStopBits);
	/*
	 *���ô���Ϊָ��������
	 *dwBaudRate ��������
	 *btParity��   У��λ
	 *btByteSize�� ����λ
	 *btStopBits�� ֹͣλ   0��ʾ1ֹͣλ�� 1��ʾ1.5ֹͣλ��  2��ʾ2ֹͣλ
	 *����ֵ��	   �ɹ�����TRUE��ʧ�ܷ���FALSE
	 */

	BOOL SetTimeOut(void);
	/*
	 *���ô����շ���������С�볬ʱ����
	 *����ֵ��	�ɹ�����TRUE��ʧ�ܷ���FALSE
	 */

	DWORD WriteSerialPort(BYTE WriteBuffer[] , DWORD dwLength);
	/*
	 *�򴮿ڷ���16���Ƶ�����
	 *WriteBuffer�� ����Ҫ���͵���������
	 *dwLength��	���������ݵĳ���
	 *����ֵ��		���ͳɹ�����TRUE��ʧ�ܷ���FALSE
	 */

	DWORD WriteSerialPort(char * WritePciStr);
	/*
	 *�򴮿ڷ���PCI�ַ�������������ַ�����������16�������ݷ��ͣ���ת����
	 *WritePciStr�� ����Ҫ���͵���������
	 *����ֵ��		���ͳɹ�����TRUE��ʧ�ܷ���FALSE
	 */

	DWORD ReadSerialPort(BYTE ReadBuff[],DWORD dwLength);
	/*
	 *�Ӵ����ж�ȡ����
	 *ReadBuff����ŴӴ��ڶ���������
	 *dwLength����Ҫ��ȡ�����ݵĳ��ȣ���λ���ֽ�
	 *����ֵ��  �ɹ�����ʵ�ʶ������ֽ�����ʧ�ܷ���-1
	 */
	
};

#endif