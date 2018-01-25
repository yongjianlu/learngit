// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 POWERMETER_PM7501_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// POWERMETER_PM7501_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
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
	BOOL m_DeviceConnect;	//设备连接状态，为true时表示已连接，false表示未连接（断开连接）
	SerialPort * m_Comm;	//用于指向串口通信类对象

public:

	BOOL GetConnectState();
	/*
	 *获取连接状态，返回TRUE表示已连接，FALSE表示未连接或已断开连接
	 */
	
	BOOL OpenDevice(int ComNum[], int ComNumLen);
	/*
	 *连接功率计，返回TRUE表示连接成功，FALSE连接失败
	 *参数：
	 *	ComNum：	当下标 i = 偶数 时数值元素表示串口使用状态，全部初始化为-1，表示该串口号未使用过；
	 *				对应的 下标为 i + 1 的数组元素表示串口号
	 *	ComNumLen:	数组的长度
	 */
	
	BOOL CloseDevice();
	/*
	 *断开设备（功率计）的连接，成功返回TRUE，失败返回FALSE
	 */
	
	BOOL GetPower(float & fPowerVal);
	/*
	 *获取光功率计当前光功率，失败返回FALSE
	 */

	BOOL SetWaveLength(int IntWaveLength);
	/*
	 *设置光功率计的波长，成功返回true
	 *IntWaveLength：波长选择，0 ~ 6 依次表示850,980,1046,1310,1480,1550,1610 nm
	 */

	BOOL GetWaveLength(int & IntWaveLength);
	/*
	 *设获取光功率计当前的波长，成功返回true
	 *IntWaveLength：波长选择，0 ~ 6 依次表示850,980,1046,1310,1480,1550,1610 nm
	 */

};

#endif
