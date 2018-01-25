#pragma once
#include<vector>
#include "UXls.h"
#include<iostream>

using namespace std;

class ReadTsSpec
{
public:
	ReadTsSpec(void);
	~ReadTsSpec(void);

	CUXls * m_CUexcel;	//Exce文件操作对象

	int ReadBaseSpec(CString strFieldName,int & Ord,CString & SpecVal,CString & Specdescribe);
	/*
	 *获取 TS 文件内产品基础规格数据（别名：BaseSpec，产品基础特性）中的指定字段名称（基本参数值）的规格数据
	 *参数：
	 *	strFieldName：	字段名称（产品基础特性中的基本参数值）
	 *	Ord：			获取到的规格相对于规格描述的排序值，从0开始；
	 *					如 SpecVal = B,Specdescribe = A,B,C,D，则 Ord = 1；默认等于-1
	 *	SpecVal：		获取到的对应规格值
	 *	Specdescribe：	读取到的对应规格描述
	 *返回值：
	 *	 0：表示成功读取
	 *	-1：没有指定条件的规格数据
	 *	-2：CUexcel为空值，传入的文件操作对象错误
	 */

	int ReadTestSpec(CString strFieldName,CString strType,CString strTemperatrue,
		CString strVoltage,CString & strMinVal,CString & strMaxVal,BOOL & bEnabled);
	/*
	 *获取 TS 文件内产品测试规格数据（别名：TestSpec）中的指定字段名称（参数值）的规格数据
	 *参数：
	 *	strFieldName：	字段名称（产品测试规格中的参数值 parameter）
	 *	strType：		类型
	 *	strTemperatrue：温度条件
	 *	strVoltage：	电压
	 *	strMinVal：		读取到的最小门限值
	 *	strMaxVal：		读取到的最大门限值
	 *	bEnabled：		TRUE表示该项测试已启用，FALSE 则相反
	 *返回值：
	 *	 0：表示成功读取
	 *	-1：没有指定条件的规格数据
	 *	-2：CUexcel为空值，传入的文件操作对象错误
	 */

	void SubCString(CString SrcCString,vector<CString> & destCString,CString Mark);
	/*
	 *按照一个标志提取一个字符串中的 子字符串，如提取 “123,456,789”中的 “123”，“456”，“789”，标志就是 ","
	 *参数：
	 *	SrcCString：源字符串
	 *	destCString：存放取得的字符串
	 *	Mark：提取的标志
	 */
};

BOOL GteMaterialCode(CString strProductModel,CString & strMaterialCode);
/*
 *获取指定型号的物料编码
 */

BOOL GetTsFileName(CString strMaterialCode,CString & strFileName);
/*
 *根据指定的物料编码，从121服务器获取对应的TS文件的名称，成功返回 TRUE
 *参数：
 *	strMaterialCode：指定的物料编码
 *	strFileName：获取到的TS文件名称
 */

CString GetSysRunPath();
/*
 *获取当前系统运行路径
 */

BOOL GetFileFromFTP( CString FtpPath, CString LocalPath ,CString ftpAdd, CString ftpUser, 
					CString ftpPass, CString &sErr );
/************************************************************************/
/*
下载指定目录下的Ftp服务下的文件到本地指定目录下,成功返回 TRUE
参数说明: 
FtpPath		要下载的ftp 目录或文件
localPath	保存到本地的目录
ftpAdd		ftp地址
ftpSuser　　登陆ftp的用户名
ftpPass		登陆ftp的密码
sErr		发生错误时，返回的错误信息*/                                                  
/************************************************************************/

CString GetDriver();

BOOL LoadTsFile_Ftp(CString SrcFilePath);
/*
*从107服务器中下载文件到默认路径，以Ftp的方式；成功返回 TRUE，失败返回 FALSE。
*参数：
*	SrcFilePath ：源文件路径
*/

BOOL LoadTsFile_Shared(CString strFileName);
/*
 *从107的共享文件夹中下载文件到默认路径，成功返回 TRUE
 */

BOOL LoadTsFileFun(CString strModelType,CString & strMaterialCode,CString & strFileName,BOOL CheckStatu);
/************************************************************************/
/* 根据相应条件，下载TS文件   LYJ-2018-01-13
/************************************************************************/