// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 APATVBR_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// APATVBR_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifndef APATVBR_H_H_H
#define APATVBR_H_H_H

#include "SerialPort.h"

#ifdef APATVBR_EXPORTS
#define APATVBR_API __declspec(dllexport)
#else
#define APATVBR_API __declspec(dllimport)
#endif

// 此类是从 APAT-VBR.dll 导出的
class APATVBR_API CAPATVBR {

public:
	CAPATVBR();
	~CAPATVBR();

private:
	BOOL m_DeviceConnect;	//设备连接状态，为true时表示已连接，false表示未连接（已断开连接）
	SerialPort * m_Comm;	//用于指向串口通信类对象

public:

	BOOL GetConnectState();
	/*
	 *获取连接状态，返回TRUE表示已连接，FALSE表示未连接或已断开连接
	 */

	BOOL OpenDevice(int ComNum[], int ComNumLen);
	/*
	 *连接Vbr设备，返回TRUE表示连接成功，FALSE连接失败
	 *参数：
	 *	ComNum：	当下标 i = 偶数 时数值元素表示串口使用状态，全部初始化为-1，表示该串口号未使用过；
	 *				对应的 下标为 i + 1 的数组元素表示串口号
	 *	ComNumLen:	数组的长度
	 */
	
	BOOL CloseDevice();
	/*
	 *断开设备（Vbr测试仪）的连接，成功返回TRUE，失败返回FALSE
	 */

	BOOL ReadVoltageVal(int IntReadState, float & fVoltageVal);
	/*
	 *读取电压上报量，成功返回TRUE
	 *参数：
	 *	IntReadState：将要读取的类型，0 ~ 3 分别表示 VAPD电压、VBR电压、VPD电压、VCC电压
	 *	fVoltageVal：存放读取到的电压数据，单位是 （V）
	 */

	BOOL ReadCurrentVal(int IntReadState, float & fCurrentVal);
	/*
	 *读取电流上报量，成功返回TRUE
	 *参数：
	 *	IntReadState：将要读取的类型，0 ~ 3 分别表示 VAPD电流、VPD电流、VCC电流、RSSI电流
	 *	fCurrentVal：存放读取到的电压数据，单位是 （mA）
	 */

	BOOL SetDeviceWorkMode(int IntModeState);
	/*
	 *设置设备工作模式,成功返回TRUE 
	 *参数：
	 *	IntModeState：想要设置的工作模式，取值如下
	 *		1：VBR测试模式，测试完毕后不输出任何 VAPD电压（默认模式）
	 *		2：VBR测试模式，测试完毕后输出任何 VAPD电压 = VBR
	 *		3：VBR测试模式，测试完毕后输出任何 VAPD电压 = A * VBR，A默认值为0.9
	 *		4：VBR测试模式，测试完毕后输出任何 VAPD电压 = VBR - OffSet，OffSet默认值为 3(V)
	 *		5：VAPD固定输出模式1，此模式固定输出预设的 VAPD = 10 （V）
	 *		6：VAPD固定输出模式2，此模式固定输出预设的 VAPD = 36 （V）
	 *		7：VAPD用户调整输出模式，输出设置范围为 5 V ~ 60 V，默认是 5 V
	 *		·
	 *		·
	 *		·
	 *		15：共有15个模式
	 */

	BOOL SetVoltageOutputVal(int IntOutputMode, float fVoltageVal);
	/*
	 *设置 VAPD 电压输出量（在非VBE测试模式下），成功返回TRUE
	 *参数：
	 *	fVoltageVal：想要设置的输出值，单位是 （V）
	 *	IntOutputMode：输出的电压类型，取值如下
	 *		0：表示VAPD电压，fVoltageVal取值范围是 5 ~ 60 V
	 *		1：表示APD电压， fVoltageVal取值范围是 3 ~ 5.2 V
	 *		2：表示VCC电压， fVoltageVal取值范围是 3 ~ 5.2 V
	 */

	BOOL VoltageOutputEnable(int IntOutputMode, BOOL EnableState);
	/*
	 *电压输出使能，即是开启/关闭电压输出，成功返回TRUE
	 *参数：
	 *	IntOutputMode：使能输出的电压类型，取值如下
	 *		0：表示VAPD电压，
	 *		1：表示APD电压，
	 *		2：表示VCC电压，
	 *	EnableState：为TRUE时表示启用，为 FALSE时表示禁用
	 */

	BOOL GotoVbrTestMode();
	/*
	 *设置面板进入VBR测试界面（前提是设备选择了VBR测试模式），成功返回TRUE
	 */

	BOOL BeginVbrTest();
	/*
	 *发送 VBR 测试命令，即是开启 VBR 测试，至少延时 2 秒以上，成功返回TRUE
	 */

	DWORD IccTestResult(float & IccVal);
	/*
	 *进行Icc测试，获取Icc值
	 *参数：
	 *	IccVal：成功后获取的Icc的值
	 *
	 *返回值：
	 *	0：测试读取成功
	 *	1：选择第5工作模式失败
	 *	2：开启VAPD电压输出失败
	 *	3：开启VCC电压输出失败
	 *	4：读取Icc值（Vcc电流值失败
	 *	5：关闭VAPD电压输出失败
	 *	6：关闭VCC电压输出失败
	 */

	DWORD VbrTestResult(float & VbrVal,float & IdVal, float & IccVal);
	/*
	 *进行Vbr测试，读取 Vbr 电压值、 Id 值（VAPD电流）、Icc 值（Vcc电流值）
	 *参数：
	 *	VbrVal：读取到的 Vbr 值
	 *	IdVal： 读取到的 Id  值
	 *	IccVal: 读取到的 Icc 值
	 *
	 *返回值：
	 *	0：测试读取数据成功
	 *	1：选择第3工作模式失败
	 *	2：开启VAPD电压输出失
	 *	3：开启VCC电压输出失败
	 *	4：设置面板进入VBR测试界面失败
	 *	5：开启 VBR 测试失败
	 *	6：读取 Vbr 值（Vbr电压值）失败
	 *	7：读取 Id  值（VAPD电流值）失败
	 *	8：读取 Icc 值失败
	 */

};


#endif