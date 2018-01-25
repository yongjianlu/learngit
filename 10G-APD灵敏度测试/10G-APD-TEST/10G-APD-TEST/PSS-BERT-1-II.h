// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 PSSBERT1II_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// PSSBERT1II_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifndef PSS_BERT_1_II_H_H_
#define PSS_BERT_1_II_H_H_

#include "SerialPort.h"

#ifdef PSSBERT1II_EXPORTS
#define PSSBERT1II_API __declspec(dllexport)
#else
#define PSSBERT1II_API __declspec(dllimport)
#endif

// 此类是从 PSS-BERT-1-II.dll 导出的
class PSSBERT1II_API PssBert1II {
public:
	PssBert1II();
	~PssBert1II();

private:
	
	BOOL m_DeviceConnect;	//设备连接状态，为true时表示已连接，false表示未连接（断开连接）
	SerialPort * m_Comm;	//用于指向串口通信类对象

public:

	BOOL GetConnectState();
	/*
	 *获取连接状态，返回TRUE表示已连接，FALSE表示未连接或已断开连接
	 */

	BOOL OpenDevice(int ComNum[], int ComNumLen);
	/*
	 *连接误码仪，返回TRUE表示连接成功，FALSE连接失败
	 *参数：
	 *	ComNum：	当下标 i = 偶数 时数值元素表示串口使用状态，全部初始化为-1，表示该串口号未使用过；
	 *				对应的 下标为 i + 1 的数组元素表示串口号
	 *	ComNumLen:	数组的长度
	 */
	
	BOOL CloseDevice();
	/*
	 *断开设备（误码仪）的连接，成功返回TRUE，失败返回FALSE
	 */

	BOOL InitDevice(int Channel, int Type, int Rate, int Amplitude);
	/*
	 *对设备进行初始化，即是配置误码仪的工作模式，成功返回TRUE
	 *参数：
	 *	Channel：选择的通道类型，0表示电通道，1表示光通道
	 *	Type   ：选择的码型，0表示PRBS7,1表示PRBS31
	 *	Rate   ：选择的速率，0 ~ 6分别表示9.95Gbps,10.31Gbps,10.52Gbps,10.70Gbps,11.09Gbps,11.32Gbps,10Gbps
	 *	Amplitude: 选择的幅度，0 ~ 7 分别表示 100 ~ 800 mVppd
	 */

	BOOL BeginBitErrTest(int Channel);
	/*
	 *开启误码检测，成功返回TRUE
	 *参数：
	 *	Channel：开启检测的通道，0表示电通道，1表示光通道，2表示两者都开启
	 */

	BOOL StopBitErrTest(int Channel);
	/*
	 *停止误码检测，成功返回TRUE
	 *参数：
	 *	Channel：停止检测的通道，0表示电通道停止，1表示光通道停止，2表示两者都停止
	 */

	BOOL ResetDevice();
	/*
	 *复位设备，使误码仪进入自主控制状态，重新响应按键操作，成功返回TRUE
	 */

	BOOL QueryBitErrState(int Channel, int & Sync, INT64 & BitErrNum, double & Ber);
	/*
	 *查询当前的误码状态，成功返回TRUE
	 *参数：
	 *	Channel：将要查询的通道，0表示电通道，1表示光通道，2表示两者都查询
	 *	Sync   ：查询成功后得到的同步、失步状态，为0时表示同步，1表示失步,2表示有误码
	 *	BitErrNum: 查询成功后得到的误码数
	 *	Ber		 ：查询成功后得到的误码率
	 */

	BOOL QueryDeviceConfig(int Channel, int & Type, int & Rate, int & Amplitude);
	/*
	 *查询设备当前的配置信息，成功返回TRUE
	 *参数：
	 *	Channel：选择查询的通道，0表示电通道，1表示光通道
	 *	Type   ：查询成功后得到的码型，0表示PRBS7,1表示PRBS31
	 *	Rate   ：查询成功后得到的速率，0 ~ 6分别表示9.95Gbps,10.31Gbps,10.52Gbps,10.70Gbps,11.09Gbps,11.32Gbps,10Gbps
	 *	Amplitude: 查询成功后得到的幅度，0 ~ 7 分别表示 100 ~ 800 mVppd
	 */
};

#endif