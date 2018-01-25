#include "stdafx.h"
#include "ReadFile.h"

#define MAX_SW_BOARD_STR_LEN			31

//��.txt�����ļ������������õ��ļ�����Ӧ������
CString ReadConfigFile(CString StaStr,CString EndStr)
{
	errno_t err;
	FILE *fp_statfile;
	CString FilePath = GetFilePath("ConfigFile.txt");
	err=fopen_s(&fp_statfile,FilePath,"rb");
	if(err!=0)
	{
		return _T("");
	}
	CString filecontent("");		//�洢���ļ��ж�ȡ������ ����ʼ���� 
	CString sSubStr("");			//���ڻ���ļ�������־���ַ����������䷵�ظ������ߣ���ʼ����

	CString start_str = StaStr;		//Ŀ���ַ���ǰ��־
	CString end_str   = EndStr;		//Ŀ���ַ������־

	int startLenth=start_str.GetLength();//Ŀ���ַ���ǰ��־����
	int endLenth=end_str.GetLength();//Ŀ���ַ������־����

	//�ļ����ݶ����ڴ�  
	while(!feof(fp_statfile))
	{  
		filecontent.AppendChar(getc(fp_statfile));  
	}  //�������ļ����е����ݶ�����filecontent�ڴ浱��

	int first,//Ŀ���ַ���ǰ��־�������ַ������е�λ��
		last; //Ŀ���ַ������־�������ַ������е�λ��

	first= filecontent.Find(start_str);
	if (first != -1)
	{
		last= filecontent.Find(end_str,first + startLenth);//��ȡλ��
	}

	if (first!= -1 && last!= -1)
	{
		int nCount = last-first-startLenth;
		sSubStr = filecontent.Mid(first + startLenth,nCount);//��first + startLenth��ʼ����nCount���ַ���������
	}

	fclose(fp_statfile);
	return sSubStr;
}



//��ȡ�ļ���·��
CString GetFilePath(CString FileName)
{
	CString strPath = "";
	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH��WINDEF.h�ж����ˣ�����260  
	memset(exeFullPath,0,MAX_PATH);    
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);  
	for(int i = strlen(exeFullPath) - 1; i >= 0;i--) 
    { 
        if (exeFullPath[i] == '\\') 
        { 
            exeFullPath[i+1] = '\0'; 
            break; 
        } 
    }
	strPath.Format("%s",exeFullPath);
	strPath = strPath + FileName; 

	return strPath;
}


//��ȡ*.ini�����ļ��е�ĳ���ڵ��µ�ĳһ����ֵ
float ReadIniFileNum(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName)
{
	char szBuff[100] = {0};
	CString FilePath = GetFilePath(FileName);
	GetPrivateProfileString( lpAppName,lpKeyName, "0", szBuff, MAX_SW_BOARD_STR_LEN, FilePath );
	return (float)atof( szBuff );
}

//��ȡ*.ini�����ļ��е�ĳ���ڵ��µ�ĳһ���ַ�����ֵ
CString ReadIniFileStr(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName)
{
	char szBuff[100] = {0};
	CString FilePath = GetFilePath(FileName);
	GetPrivateProfileString( lpAppName,lpKeyName, "0", szBuff, MAX_SW_BOARD_STR_LEN, FilePath );

	CString RetuStr = "";
	RetuStr.Format("%s",szBuff);
	return RetuStr;
}

//��ĳ��float����д��*.ini�����ļ��е�ĳ���ڵ��µ�ĳһ��
void WriteIniFileNum(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName,float WriteVal)
{
	CString WriteStr = "";
	WriteStr.Format("%f",WriteVal);

	CString FilePath = GetFilePath(FileName);
	WritePrivateProfileString(lpAppName,lpKeyName,WriteStr,FilePath);
}

//��ĳ��Int����д��*.ini�����ļ��е�ĳ���ڵ��µ�ĳһ��
void WriteIniFileInt(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName,int WriteVal)
{
	CString WriteStr = "";
	WriteStr.Format("%d",WriteVal);

	CString FilePath = GetFilePath(FileName);
	WritePrivateProfileString(lpAppName,lpKeyName,WriteStr,FilePath);
}

//��ĳ���ַ�������д��*.ini�����ļ��е�ĳ���ڵ��µ�ĳһ��
void WriteIniFileStr(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName,CString WriteStr)
{
	CString FilePath = GetFilePath(FileName);
	WritePrivateProfileString(lpAppName,lpKeyName,WriteStr,FilePath);
}

//����һ����־��ȡһ���ַ����е� ���ַ���������ȡ ��123,456,789���е� ��123������456������789������־���� ","
void SubCString(CString SrcCString,CString destCString[], CString Mark)
{
	CArray<CString,CString>strArray;  
	int i = 0;
	while(SrcCString.Find(Mark)+1)	//���Ҳ�����־ʱ����-1��������������������ֵ��1��ѭ��
	{
		strArray.Add(SrcCString.Left(SrcCString.Find(Mark)));//�ҵ���־����������ȡ�ո���ߵ��ַ���,��������ӵ�strArray��̬�����С�
		destCString[i] = strArray[i];
		SrcCString.Delete(0,strArray[i++].GetLength()+1);//����ɾ���ո������ߵ��ַ����ĵõ������ַ�����
	}
	strArray.Add(SrcCString);//�����Ͼ�������ɾ����ʣ�µ��ַ�����
	destCString[i] = strArray[i];
}