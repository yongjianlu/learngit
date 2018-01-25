#ifndef READFILE_H_
#define READFILE_H_

#pragma once

CString ReadConfigFile(CString StaStr,CString EndStr);
/*
 *读取txt文件中的数据
 */

CString GetFilePath(CString FileName);
/*
 *获取文件的路径
 */

float ReadIniFileNum(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName);
/*
 *读取*.ini配置文件中的某个节点下的某一项数值
 *FileName：	配置文件名
 *lpAppName：	节点
 *lpKeyName：	项
 *返回值：		返回读取到的数值数据
 */

CString ReadIniFileStr(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName);
/*
 *读取*.ini配置文件中的某个节点下的某一项字符串数值
 *FileName：	配置文件名
 *lpAppName：	节点
 *lpKeyName：	项
 *返回值：		返回读取到的字符串数据
 */

void WriteIniFileNum(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName,float WriteVal);
/*
 *将某个float数据写入*.ini配置文件中的某个节点下的某一项
 *FileName：	配置文件名
 *lpAppName：	节点
 *lpKeyName：	项
 *WriteVal:		将要写入的float值
 */

void WriteIniFileInt(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName,int WriteVal);
/*
 *将某个Int数据写入*.ini配置文件中的某个节点下的某一项
 *FileName：	配置文件名
 *lpAppName：	节点
 *lpKeyName：	项
 *WriteVal:		将要写入的int值
 */


void WriteIniFileStr(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName,CString WriteStr);
/*
 *将某个字符串数据写入*.ini配置文件中的某个节点下的某一项
 *FileName：	配置文件名
 *lpAppName：	节点
 *lpKeyName：	项
 *WriteStr:		将要写入的字符串
 */


void SubCString(CString SrcCString,CString destCString[], CString Mark);
/*
 *按照一个标志提取一个字符串中的 子字符串，如提取 “123,456,789”中的 “123”，“456”，“789”，标志就是 ","
 *参数：
 *	SrcCString：源字符串
 *	destCString：存放取得的字符串
 *	Mark：提取的标志
 */


#endif