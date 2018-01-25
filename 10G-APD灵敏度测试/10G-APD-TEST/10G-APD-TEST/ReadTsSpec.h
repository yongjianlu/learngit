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

	CUXls * m_CUexcel;	//Exce�ļ���������

	int ReadBaseSpec(CString strFieldName,int & Ord,CString & SpecVal,CString & Specdescribe);
	/*
	 *��ȡ TS �ļ��ڲ�Ʒ����������ݣ�������BaseSpec����Ʒ�������ԣ��е�ָ���ֶ����ƣ���������ֵ���Ĺ������
	 *������
	 *	strFieldName��	�ֶ����ƣ���Ʒ���������еĻ�������ֵ��
	 *	Ord��			��ȡ���Ĺ������ڹ������������ֵ����0��ʼ��
	 *					�� SpecVal = B,Specdescribe = A,B,C,D���� Ord = 1��Ĭ�ϵ���-1
	 *	SpecVal��		��ȡ���Ķ�Ӧ���ֵ
	 *	Specdescribe��	��ȡ���Ķ�Ӧ�������
	 *����ֵ��
	 *	 0����ʾ�ɹ���ȡ
	 *	-1��û��ָ�������Ĺ������
	 *	-2��CUexcelΪ��ֵ��������ļ������������
	 */

	int ReadTestSpec(CString strFieldName,CString strType,CString strTemperatrue,
		CString strVoltage,CString & strMinVal,CString & strMaxVal,BOOL & bEnabled);
	/*
	 *��ȡ TS �ļ��ڲ�Ʒ���Թ�����ݣ�������TestSpec���е�ָ���ֶ����ƣ�����ֵ���Ĺ������
	 *������
	 *	strFieldName��	�ֶ����ƣ���Ʒ���Թ���еĲ���ֵ parameter��
	 *	strType��		����
	 *	strTemperatrue���¶�����
	 *	strVoltage��	��ѹ
	 *	strMinVal��		��ȡ������С����ֵ
	 *	strMaxVal��		��ȡ�����������ֵ
	 *	bEnabled��		TRUE��ʾ������������ã�FALSE ���෴
	 *����ֵ��
	 *	 0����ʾ�ɹ���ȡ
	 *	-1��û��ָ�������Ĺ������
	 *	-2��CUexcelΪ��ֵ��������ļ������������
	 */

	void SubCString(CString SrcCString,vector<CString> & destCString,CString Mark);
	/*
	 *����һ����־��ȡһ���ַ����е� ���ַ���������ȡ ��123,456,789���е� ��123������456������789������־���� ","
	 *������
	 *	SrcCString��Դ�ַ���
	 *	destCString�����ȡ�õ��ַ���
	 *	Mark����ȡ�ı�־
	 */
};

BOOL GteMaterialCode(CString strProductModel,CString & strMaterialCode);
/*
 *��ȡָ���ͺŵ����ϱ���
 */

BOOL GetTsFileName(CString strMaterialCode,CString & strFileName);
/*
 *����ָ�������ϱ��룬��121��������ȡ��Ӧ��TS�ļ������ƣ��ɹ����� TRUE
 *������
 *	strMaterialCode��ָ�������ϱ���
 *	strFileName����ȡ����TS�ļ�����
 */

CString GetSysRunPath();
/*
 *��ȡ��ǰϵͳ����·��
 */

BOOL GetFileFromFTP( CString FtpPath, CString LocalPath ,CString ftpAdd, CString ftpUser, 
					CString ftpPass, CString &sErr );
/************************************************************************/
/*
����ָ��Ŀ¼�µ�Ftp�����µ��ļ�������ָ��Ŀ¼��,�ɹ����� TRUE
����˵��: 
FtpPath		Ҫ���ص�ftp Ŀ¼���ļ�
localPath	���浽���ص�Ŀ¼
ftpAdd		ftp��ַ
ftpSuser������½ftp���û���
ftpPass		��½ftp������
sErr		��������ʱ�����صĴ�����Ϣ*/                                                  
/************************************************************************/

CString GetDriver();

BOOL LoadTsFile_Ftp(CString SrcFilePath);
/*
*��107�������������ļ���Ĭ��·������Ftp�ķ�ʽ���ɹ����� TRUE��ʧ�ܷ��� FALSE��
*������
*	SrcFilePath ��Դ�ļ�·��
*/

BOOL LoadTsFile_Shared(CString strFileName);
/*
 *��107�Ĺ����ļ����������ļ���Ĭ��·�����ɹ����� TRUE
 */

BOOL LoadTsFileFun(CString strModelType,CString & strMaterialCode,CString & strFileName,BOOL CheckStatu);
/************************************************************************/
/* ������Ӧ����������TS�ļ�   LYJ-2018-01-13
/************************************************************************/