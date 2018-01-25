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
	 *将PCI命令的相关字符串都转化为16进制的数据
	 *str:		将要进行转换的字符串
	 *senddata：存放转换后的16进制数据
	 *返回值：  返回senddata重有效数据长度
	 */

	BYTE ConvertHexChar(char ch);
	/*
	 *该函数是一个将字符转换为相应的十六进制值的函数
	 *功能：若是在0-F之间的字符，则转换为相应的十六进制字符，否则返回-1
	 */

public:

	HANDLE  m_hCom;//串口句柄

	SerialPort(void);
	~SerialPort(void);

	
	int HexStrToHex(string str,BYTE WriteData[]);
	/*
	 *将每一个类16进制字符都转化为16进制的数据
	 *str:		将要进行转换的字符串
	 *senddata：存放转换后的16进制数据
	 *返回值：  返回senddata重有效数据长度
	 */

	BOOL OpenSerialPort(int SerialPortCom);
	/*
	 *打开指定的串口
	 *SerialPortCom：将要打开的串口
	 *返回值：		 打开成功返回TRUE，失败返回FALSE
	 */

	BOOL CloseSerialPort();
	/*
	 *关闭已打开的串口。成功返回TRUE
	 */

	BOOL SetSerialPort(DWORD dwBaudRate);
	/*
	 *设置指定的波特率，并默认设置为 无校验位、8数据位、1停止位
	 *dwBaudRate ： 指定的波特率
	 *返回值：		成功返回TRUE，失败返回FALSE
	 */

	BOOL SetSerialPort(DWORD dwBaudRate, BYTE btParity, BYTE btByteSize, BYTE btStopBits);
	/*
	 *设置串口为指定的数据
	 *dwBaudRate ：波特率
	 *btParity：   校验位
	 *btByteSize： 数据位
	 *btStopBits： 停止位   0表示1停止位、 1表示1.5停止位、  2表示2停止位
	 *返回值：	   成功返回TRUE，失败返回FALSE
	 */

	BOOL SetTimeOut(void);
	/*
	 *设置串口收发缓冲区大小与超时设置
	 *返回值：	成功返回TRUE，失败返回FALSE
	 */

	DWORD WriteSerialPort(BYTE WriteBuffer[] , DWORD dwLength);
	/*
	 *向串口发送16进制的命令
	 *WriteBuffer： 所将要发送的命令数据
	 *dwLength：	所发送数据的长度
	 *返回值：		发送成功返回TRUE，失败返回FALSE
	 */

	DWORD WriteSerialPort(char * WritePciStr);
	/*
	 *向串口发送PCI字符串命令（参数是字符串，但仍以16进制数据发送，需转换）
	 *WritePciStr： 所将要发送的命令数据
	 *返回值：		发送成功返回TRUE，失败返回FALSE
	 */

	DWORD ReadSerialPort(BYTE ReadBuff[],DWORD dwLength);
	/*
	 *从串口中读取数据
	 *ReadBuff：存放从串口读到的数据
	 *dwLength：想要读取的数据的长度，单位：字节
	 *返回值：  成功返回实际读到的字节数，失败返回-1
	 */
	
};

#endif