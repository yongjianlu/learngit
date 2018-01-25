#ifndef READFILE_H_
#define READFILE_H_

#pragma once

CString ReadConfigFile(CString StaStr,CString EndStr);
/*
 *��ȡtxt�ļ��е�����
 */

CString GetFilePath(CString FileName);
/*
 *��ȡ�ļ���·��
 */

float ReadIniFileNum(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName);
/*
 *��ȡ*.ini�����ļ��е�ĳ���ڵ��µ�ĳһ����ֵ
 *FileName��	�����ļ���
 *lpAppName��	�ڵ�
 *lpKeyName��	��
 *����ֵ��		���ض�ȡ������ֵ����
 */

CString ReadIniFileStr(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName);
/*
 *��ȡ*.ini�����ļ��е�ĳ���ڵ��µ�ĳһ���ַ�����ֵ
 *FileName��	�����ļ���
 *lpAppName��	�ڵ�
 *lpKeyName��	��
 *����ֵ��		���ض�ȡ�����ַ�������
 */

void WriteIniFileNum(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName,float WriteVal);
/*
 *��ĳ��float����д��*.ini�����ļ��е�ĳ���ڵ��µ�ĳһ��
 *FileName��	�����ļ���
 *lpAppName��	�ڵ�
 *lpKeyName��	��
 *WriteVal:		��Ҫд���floatֵ
 */

void WriteIniFileInt(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName,int WriteVal);
/*
 *��ĳ��Int����д��*.ini�����ļ��е�ĳ���ڵ��µ�ĳһ��
 *FileName��	�����ļ���
 *lpAppName��	�ڵ�
 *lpKeyName��	��
 *WriteVal:		��Ҫд���intֵ
 */


void WriteIniFileStr(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName,CString WriteStr);
/*
 *��ĳ���ַ�������д��*.ini�����ļ��е�ĳ���ڵ��µ�ĳһ��
 *FileName��	�����ļ���
 *lpAppName��	�ڵ�
 *lpKeyName��	��
 *WriteStr:		��Ҫд����ַ���
 */


void SubCString(CString SrcCString,CString destCString[], CString Mark);
/*
 *����һ����־��ȡһ���ַ����е� ���ַ���������ȡ ��123,456,789���е� ��123������456������789������־���� ","
 *������
 *	SrcCString��Դ�ַ���
 *	destCString�����ȡ�õ��ַ���
 *	Mark����ȡ�ı�־
 */


#endif