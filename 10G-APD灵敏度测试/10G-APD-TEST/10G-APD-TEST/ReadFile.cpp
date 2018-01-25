#include "stdafx.h"
#include "ReadFile.h"

#define MAX_SW_BOARD_STR_LEN			31

//打开.txt配置文件，并按条件得到文件中相应的内容
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
	CString filecontent("");		//存储从文件中读取的数据 （初始化） 
	CString sSubStr("");			//用于获得文件中两标志间字符串，并将其返回给调用者（初始化）

	CString start_str = StaStr;		//目标字符串前标志
	CString end_str   = EndStr;		//目标字符串后标志

	int startLenth=start_str.GetLength();//目标字符串前标志长度
	int endLenth=end_str.GetLength();//目标字符串后标志长度

	//文件内容读入内存  
	while(!feof(fp_statfile))
	{  
		filecontent.AppendChar(getc(fp_statfile));  
	}  //将整个文件当中的数据都读到filecontent内存当中

	int first,//目标字符串前标志在整个字符串当中的位置
		last; //目标字符串后标志在整个字符串当中的位置

	first= filecontent.Find(start_str);
	if (first != -1)
	{
		last= filecontent.Find(end_str,first + startLenth);//获取位置
	}

	if (first!= -1 && last!= -1)
	{
		int nCount = last-first-startLenth;
		sSubStr = filecontent.Mid(first + startLenth,nCount);//从first + startLenth开始拷贝nCount个字符到缓存中
	}

	fclose(fp_statfile);
	return sSubStr;
}



//获取文件的路径
CString GetFilePath(CString FileName)
{
	CString strPath = "";
	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH在WINDEF.h中定义了，等于260  
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


//读取*.ini配置文件中的某个节点下的某一项数值
float ReadIniFileNum(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName)
{
	char szBuff[100] = {0};
	CString FilePath = GetFilePath(FileName);
	GetPrivateProfileString( lpAppName,lpKeyName, "0", szBuff, MAX_SW_BOARD_STR_LEN, FilePath );
	return (float)atof( szBuff );
}

//读取*.ini配置文件中的某个节点下的某一项字符串数值
CString ReadIniFileStr(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName)
{
	char szBuff[100] = {0};
	CString FilePath = GetFilePath(FileName);
	GetPrivateProfileString( lpAppName,lpKeyName, "0", szBuff, MAX_SW_BOARD_STR_LEN, FilePath );

	CString RetuStr = "";
	RetuStr.Format("%s",szBuff);
	return RetuStr;
}

//将某个float数据写入*.ini配置文件中的某个节点下的某一项
void WriteIniFileNum(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName,float WriteVal)
{
	CString WriteStr = "";
	WriteStr.Format("%f",WriteVal);

	CString FilePath = GetFilePath(FileName);
	WritePrivateProfileString(lpAppName,lpKeyName,WriteStr,FilePath);
}

//将某个Int数据写入*.ini配置文件中的某个节点下的某一项
void WriteIniFileInt(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName,int WriteVal)
{
	CString WriteStr = "";
	WriteStr.Format("%d",WriteVal);

	CString FilePath = GetFilePath(FileName);
	WritePrivateProfileString(lpAppName,lpKeyName,WriteStr,FilePath);
}

//将某个字符串数据写入*.ini配置文件中的某个节点下的某一项
void WriteIniFileStr(CString FileName,LPCSTR lpAppName,LPCSTR lpKeyName,CString WriteStr)
{
	CString FilePath = GetFilePath(FileName);
	WritePrivateProfileString(lpAppName,lpKeyName,WriteStr,FilePath);
}

//按照一个标志提取一个字符串中的 子字符串，如提取 “123,456,789”中的 “123”，“456”，“789”，标志就是 ","
void SubCString(CString SrcCString,CString destCString[], CString Mark)
{
	CArray<CString,CString>strArray;  
	int i = 0;
	while(SrcCString.Find(Mark)+1)	//当找不到标志时返回-1，所以这里我用它返回值加1来循环
	{
		strArray.Add(SrcCString.Left(SrcCString.Find(Mark)));//找到标志的索引，截取空格左边的字符串,并将它添加到strArray动态数组中。
		destCString[i] = strArray[i];
		SrcCString.Delete(0,strArray[i++].GetLength()+1);//返回删除空格和其左边的字符串的得到的新字符串。
	}
	strArray.Add(SrcCString);//最后加上经过最终删除后剩下的字符串；
	destCString[i] = strArray[i];
}