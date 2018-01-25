// stdafx.cpp : 只包括标准包含文件的源文件
// 10G-APD-TEST.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

int ComNum[1024] = {0};			//查询到的主机可以串口以及使用状态

float g_PowerVal = -100.0f;		//光校准中 手动测试到的 功率值

CString g_UserAccount		= _T("");		//测试账号
CString g_UserName			= _T("");		//测试人员姓名
CString g_UserPermissions	= _T("");		//登录用户权限
int g_IntModelState = 0;					//选择的测试模式
CString g_EnabledArray[48];					//用户登录时读取到的权限