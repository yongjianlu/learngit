#include "stdafx.h"
#include "ReadTsSpec.h"
#include <afxinet.h>

//#pragma comment(lib,"UExcel.lib")	//

ReadTsSpec::ReadTsSpec(void)
{
	m_CUexcel = new CUXls();
}

ReadTsSpec::~ReadTsSpec(void)
{
	delete m_CUexcel;
	m_CUexcel = NULL;
}

/*
 *获取 TS 文件内产品基础规格数据（别名：BaseSpec，产品基础特性）中的指定字段名称（基本参数值）的规格数据
 *参数：
 *	CUexcel：		Exce文件操作对象
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
int ReadTsSpec::ReadBaseSpec(CString strFieldName,int & Ord,CString & SpecVal,CString & Specdescribe)
{
	Ord = -1;SpecVal = _T("");Specdescribe = _T("");
	int row = 1;
	int EndRow = 1;

	if(m_CUexcel == NULL)
	{
		return -2;
	}

	long * pRow = new long;
	long * pCol = new long;

	int IntReturn = -1;

	if( -1 == m_CUexcel->SearchText(_T("产品规格  Product Spec."),pRow,pCol))
	{
		IntReturn =  -1;
		goto _RETURN_FUN_;
	}

	EndRow = m_CUexcel->GetUsedRow();
	for(row = *pRow + 3; row < EndRow; row++)
	{
		if(m_CUexcel->GetCellText(row,*pCol) == strFieldName)
		{
			SpecVal = m_CUexcel->GetCellText(row,*pCol + 2);
			Specdescribe = m_CUexcel->GetCellText(row,*pCol + 5);
		}
	}

	//分析获取排序值
	if(SpecVal != _T("") && Specdescribe != _T(""))
	{
		vector<CString> strSpecdescribe;
		SubCString(Specdescribe,strSpecdescribe,_T(","));
		int i = 0;
		for(vector<CString>::iterator it = strSpecdescribe.begin();it!= strSpecdescribe.end();it++)
		{
			if(*it == SpecVal)
			{
				Ord = i;
				IntReturn = 0;
				break;
			}
			i++;
		}
	}
	else if(SpecVal != _T("") && Specdescribe == _T(""))
	{
		IntReturn = 0;
	}

_RETURN_FUN_ :
	delete pRow;delete pCol;
	pRow = NULL;pCol = NULL;
	return IntReturn;
}

/*
 *获取 TS 文件内产品测试规格数据（别名：TestSpec）中的指定字段名称（参数值）的规格数据
 *参数：
 *	CUexcel：		Exce文件操作对象
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
int ReadTsSpec::ReadTestSpec(CString strFieldName,CString strType,CString strTemperatrue,
		CString strVoltage,CString & strMinVal,CString & strMaxVal,BOOL & bEnabled)
{
	if(NULL == m_CUexcel)
	{
		return -2;
	}
	int IntReturn = -1;
	long row = 1;
	long col = 1;
	strMinVal = _T("");strMaxVal = _T("");bEnabled = FALSE;
	CString strReadTsSpecTemp("");

	long BeginRow = 1;
	long EndRow = m_CUexcel->GetUsedRow();
	BOOL bStatu = FALSE;

	long * pRow = new long;
	long * pCol = new long;

	if( -1 == m_CUexcel->SearchText(_T("测试规格  Test Spec."),pRow,pCol))
	{
		IntReturn =  -1;
		goto _RETURN_FUN_;
	}
	BeginRow = *pRow + 3;
	col = *pCol;
	//获取Type的范围
	bStatu = FALSE;//为TRUE时表示已找到Type的起始位置
	for(row = BeginRow; row < EndRow + 1; row++)
	{
		strReadTsSpecTemp = m_CUexcel->GetCellText(row,col);

		if(strReadTsSpecTemp == strType)
		{
			BeginRow = row;
			bStatu = TRUE;
			continue;
		}

		if(bStatu == TRUE && strReadTsSpecTemp != _T(""))
		{
			EndRow = row - 1;
			break;
		}
	}

	if(bStatu == FALSE)
	{
		IntReturn =  -1;
		goto _RETURN_FUN_;
	}

	//获取Temperatrue范围
	bStatu = FALSE;//为TRUE时表示已找到strTemperatrue的起始位置
	for(row = BeginRow; row <= EndRow; row++)
	{
		strReadTsSpecTemp = m_CUexcel->GetCellText(row,*pCol + 2);
		if(row == BeginRow && strReadTsSpecTemp == _T(""))	//此时Temperatrue范围 大于 Type的范围
		{
			bStatu = TRUE;
			break;
		}

		if(bStatu == FALSE && strReadTsSpecTemp == strTemperatrue)
		{
			BeginRow = row;
			bStatu = TRUE;
			continue;
		}

		if(bStatu == TRUE && strReadTsSpecTemp != _T(""))
		{
			EndRow = row - 1;
			break;
		}
	}
	if(bStatu == FALSE)
	{
		IntReturn =  -1;
		goto _RETURN_FUN_;
	}

	//获取Voltage范围
	bStatu = FALSE;//为TRUE时表示已找到strVoltage的起始位置
	for(row = BeginRow; row <= EndRow; row++)
	{
		strReadTsSpecTemp = m_CUexcel->GetCellText(row,*pCol + 3);
		if(row == BeginRow && strReadTsSpecTemp == _T(""))	//此时Voltage范围 大于 Type的范围
		{
			bStatu = TRUE;
			break;
		}

		if(bStatu == FALSE && strReadTsSpecTemp == strVoltage)
		{
			BeginRow = row;
			bStatu = TRUE;
			continue;
		}

		if(bStatu == TRUE && strReadTsSpecTemp != _T(""))
		{
			EndRow = row - 1;
			break;
		}
	}
	if(bStatu == FALSE)
	{
		IntReturn =  -1;
		goto _RETURN_FUN_;
	}

	//获取指定strFieldName项的相关值
	for(row = BeginRow; row <= EndRow; row++)
	{
		strReadTsSpecTemp = m_CUexcel->GetCellText(row,*pCol + 1);
		if(strReadTsSpecTemp == strFieldName)
		{
			strMinVal = m_CUexcel->GetCellText(row,*pCol + 4);
			strMaxVal = m_CUexcel->GetCellText(row,*pCol + 6);
			if(_T("Y") == m_CUexcel->GetCellText(row,*pCol + 8))
			{
				bEnabled = TRUE;
			}
			IntReturn = 0;
			break;
		}

	}

_RETURN_FUN_ :
	delete pRow;delete pCol;
	pRow = NULL;pCol = NULL;
	return IntReturn ;
}

/*
 *按照一个标志提取一个字符串中的 子字符串，如提取 “123,456,789”中的 “123”，“456”，“789”，标志就是 ","
 *参数：
 *	SrcCString：源字符串
 *	destCString：存放取得的字符串
 *	Mark：提取的标志
 */
void ReadTsSpec::SubCString(CString SrcCString,vector<CString> & destCString,CString Mark)
{
	destCString.clear();
	CArray<CString,CString>strArray;  
	int i = 0;
	while(SrcCString.Find(Mark)+1)	//当找不到标志时返回-1，所以这里我用它返回值加1来循环
	{
		strArray.Add(SrcCString.Left(SrcCString.Find(Mark)));//找到标志的索引，截取空格左边的字符串,并将它添加到strArray动态数组中。
		destCString.push_back(strArray[i]);
		SrcCString.Delete(0,strArray[i++].GetLength()+1);//返回删除空格和其左边的字符串的得到的新字符串。
	}
	strArray.Add(SrcCString);//最后加上经过最终删除后剩下的字符串；
	destCString.push_back(strArray[i]);
}

//获取指定型号的物料编码
BOOL GteMaterialCode(CString strProductModel,CString & strMaterialCode)
{
	ADO_Connection * SqlCon    = new ADO_Connection();	//sql server数据库对象
	_RecordsetPtr  * recordSql = new _RecordsetPtr;
	BOOL State = FALSE;
	char * P = new char[1024];
	CString strSqlCmd;

	CString strIniFilePath,strServerAdd;
	strIniFilePath.Format(_T("%s\\Config.ini"),GetSysRunPath());
	::GetPrivateProfileString("TSPATH","ServerIP","192.168.0.203",strServerAdd.GetBuffer(255),255,strIniFilePath);

	if( false == SqlCon->OpenConnection(strServerAdd,_T("test"),_T("123"),_T("test"),1433) )
	{
		goto GOTORETURN;
	}
	
	strSqlCmd.Format(_T("select MaterialCode from testsetup where model = '%s'"),strProductModel);
	if( false == SqlCon->Query(strSqlCmd, recordSql) )	
	{
		SqlCon->Close();
		goto GOTORETURN;
	}
	strMaterialCode.Format(_T("%s"),SqlCon->Result.Convertor("MaterialCode").ToString(P));
	State = TRUE;
	SqlCon->Close();

GOTORETURN:
	delete SqlCon;
	SqlCon = NULL;
	delete recordSql;
	recordSql = NULL;
	delete P; 
	P = NULL;
	return State;
}

//根据指定的物料编码，从121服务器获取对应的TS文件的名称，成功返回 TRUE
BOOL GetTsFileName(CString strMaterialCode,CString & strFileName)
{
	ADO_Connection * SqlCon    = new ADO_Connection();	//sql server数据库对象
	_RecordsetPtr  * recordSql = new _RecordsetPtr;
	BOOL State = FALSE;
	char * P = new char[1024];
	CString strSqlCmd;

	CString strIniFilePath,strServerAdd;
	strIniFilePath.Format(_T("%s\\Config.ini"),GetSysRunPath());
	::GetPrivateProfileString("TSPATH","ServerIP1","192.168.0.121",strServerAdd.GetBuffer(255),255,strIniFilePath);

	if( false == SqlCon->OpenConnection(strServerAdd,_T("usrActive"),_T("usractive2017"),_T("APAT_Active"),1433) )
	{
		goto GOTORETURN;
	}

	strSqlCmd.Format(_T("select * from TF_ExcelInfo where F_ProductNo='%s' AND F_Id = \
(select max(F_Id) from TF_ExcelInfo where F_ProductNo ='%s')"),_T("TS_") + strMaterialCode,_T("TS_") + strMaterialCode);

	if( false == SqlCon->Query(strSqlCmd, recordSql) )	
	{
		SqlCon->Close();
		goto GOTORETURN;
	}

	strFileName.Format(_T("%s"),SqlCon->Result.Convertor("F_ExcelName").ToString(P));
	State = TRUE;
	SqlCon->Close();
	
GOTORETURN:
	delete SqlCon;
	SqlCon = NULL;
	delete recordSql;
	recordSql = NULL;
	delete P; 
	P = NULL;
	return State;
}

//获取当前系统运行路径
CString GetSysRunPath()
{
	CString strPath;
	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH在WINDEF.h中定义了，等于260  
	memset(exeFullPath,0,MAX_PATH);    
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);  
	for(int i = strlen(exeFullPath) - 1; i >= 0;i--) 
    { 
        if (exeFullPath[i] == '\\') 
        { 
            exeFullPath[i] = '\0'; 
            break; 
        } 
    }
	strPath.Format("%s",exeFullPath);

	return strPath;
}

//下载指定目录下的Ftp服务下的文件到本地指定目录下,成功返回 TRUE
BOOL GetFileFromFTP( CString FtpPath, CString LocalPath ,CString ftpAdd, CString ftpUser, CString ftpPass, CString &sErr )
{
	CString sTemp ;
	CString sLocalTemp ;
	CString sTempFtpPath ;
	CInternetSession  pInternetCon(AfxGetAppName(),1,PRE_CONFIG_INTERNET_ACCESS); 
	CFtpConnection * pFtpCon ;
	try 
	{
		pFtpCon =pInternetCon.GetFtpConnection( ftpAdd, ftpUser, ftpPass, 21 );
		if ( pFtpCon != NULL)
		{
			CFtpFileFind FtpFinder( pFtpCon ) ;
			BOOL bWork = FtpFinder.FindFile( FtpPath );
			if(!bWork)
			{
				sErr.Format( "没有找到 %s 文件，请确认验证服务器的路径设置是否正确！", FtpPath ) ;
				pFtpCon=NULL;
				return FALSE;
			}
			while ( bWork )
			{
				bWork = FtpFinder.FindNextFile() ;
				if ( FtpFinder.IsDots() ) 
				continue;
				if(FtpFinder.IsDirectory() )
				{  
					//是目录
					sTemp.Empty() ;
					sTemp = FtpFinder.GetFileName() ;
					sTempFtpPath.Empty() ;
					sTempFtpPath = FtpPath.Left( FtpPath.GetLength() - 3 )  ;
					sTempFtpPath += sTemp + "//*.*" ;
					sLocalTemp.Empty() ;
					sLocalTemp = LocalPath + "\\" + sTemp ;
					CFileStatus   rStatus;
					if( !( CFile::GetStatus( sLocalTemp, rStatus ) ) )
					{
						//文件夹不存在, 创建之
						if( !(CreateDirectory((LPCTSTR)sLocalTemp, NULL)) )
						{
							//创建失败
							DWORD dErr = GetLastError() ;
							sErr.Format( "创建新目录失败: ERROR  %d" ,dErr ) ;
							return FALSE ; 
						}
					}
					GetFileFromFTP( sTempFtpPath, sLocalTemp, ftpAdd, ftpUser, ftpPass, sErr ) ;
				}
				else
				{
					sTemp.Empty() ;
					sTemp = FtpFinder.GetFileName();
					sLocalTemp.Empty() ;
					sLocalTemp = LocalPath + "\\" + sTemp ;
					sTemp.Empty() ;
					sTemp = FtpFinder.GetFilePath();//
					CFileStatus   rStatus;
					if( ( CFile::GetStatus( sLocalTemp, rStatus ) ) )//文件已存在，则删除原文件
					{
						// 删除文件，成功返回0，否则返回-1
						if (-1 == remove(sLocalTemp))
						{
							sErr = _T("下载失败，失败原因：删除原文件失败！");
							return FALSE;
						}
					}

					//下载文件
					if( !( pFtpCon->GetFile( sTemp, sLocalTemp, true, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY, 1) ) )
					{
						//下载失败
						DWORD dErr = GetLastError() ;
						sErr.Format( "下载失败: ERROR  %d" ,dErr ) ;
						return FALSE ;
					}
				}
			}
			FtpFinder.Close() ;
		}     
	}
	catch (CInternetException * pEx)
	{
		TCHAR szError[1024];
		pEx->GetErrorMessage(szError,1024) ;
		sErr.Format( "连接FTP失败:%s", pEx ) ;
		pEx->Delete();
		pFtpCon=NULL;
		return FALSE ;
	}
	return TRUE ;
}

//从107服务器中(验证服务器)下载文件到默认路径，以Ftp的方式；成功返回 TRUE，失败返回 FALSE。
BOOL LoadTsFile_Ftp(CString SrcFilePath)
{
	CString strIniFilePath,strServerAdd;
	strIniFilePath.Format(_T("%s\\Config.ini"),GetSysRunPath());
	::GetPrivateProfileString("TSPATH","ServerIP2","192.168.0.107",strServerAdd.GetBuffer(255),255,strIniFilePath);

	CString FtpPath		= _T("");
	CString ftpAdd		= strServerAdd;
	CString ftpSuser	= _T("utest");
	CString ftpPass		= _T("4rtg%7$F");
	CString sErr,DestFilePath;

	DestFilePath = GetSysRunPath() + _T("\\TsFile");
	if(FALSE == GetFileFromFTP(SrcFilePath,DestFilePath,ftpAdd,ftpSuser,ftpPass,sErr))
	{
		AfxMessageBox(sErr); 
		return FALSE;
	}

	return TRUE;
}

CString GetDriver()
{
	char buf[100];

	DWORD len = GetLogicalDriveStrings(sizeof(buf)/sizeof(TCHAR),buf);

	if( len == 0 )
		return "";

	CString strDisk;
	for(  TCHAR* s = buf;*s; s+=_tcslen(s)+1)
	{
		LPCTSTR sDrivePath = s; 
		strDisk += sDrivePath;      
		strDisk += " "; 
	}

	strDisk.MakeLower();

	CString tmpStr,strRes;
	for( char c = 'z'; c>='d'; c-- )
	{	
		tmpStr.Format("%c",c);
		if( strDisk.Find( tmpStr,0 ) < 0 )
		{
			strRes.Format("%s:",tmpStr );
			return strRes;
		}
	}

	return strRes;
}

//从107的共享文件夹(生产服务器)中下载文件到默认路径，成功返回 TRUE
BOOL LoadTsFile_Shared(CString strFileName)
{
	CString strIniFilePath,strServerAdd;
	strIniFilePath.Format(_T("%s\\Config.ini"),GetSysRunPath());
	::GetPrivateProfileString("TSPATH","ServerIP2","192.168.0.107",strServerAdd.GetBuffer(255),255,strIniFilePath);
	strServerAdd.Format(_T("\\\\%s\\TestingRelease"),strServerAdd);

	CString tmpStr,strDriver;
	BOOL	bRes = FALSE;
	
	strDriver = GetDriver();
	CString DestFilePath = GetSysRunPath() + _T("TsFile\\") + strFileName;//
	CString SrcFilePath;
	
	NETRESOURCE  nr;   
    nr.dwType=RESOURCETYPE_ANY;   
    nr.lpLocalName=strDriver.GetBuffer(0);   //此处可以是某个盘符比如:"z:"   
    nr.lpRemoteName=(LPSTR)(LPCSTR)strServerAdd;
    nr.lpComment="";   
    nr.lpProvider="";   
    DWORD   dwRes;   
	
	strDriver.ReleaseBuffer();
	//CPublic::strNetPath.ReleaseBuffer();
	
#ifdef _DEBUG
	dwRes=WNetAddConnection2(&nr,"4rtg%7$F","ftest",CONNECT_INTERACTIVE);
#else
	dwRes=WNetAddConnection2(&nr,"4rtg%7$F","ftest",CONNECT_INTERACTIVE);
#endif
	
	if(dwRes == 0 || dwRes == 85 )
	{
		CString strProductionPath;
		::GetPrivateProfileString("TSPATH","ProductionPath","\\OSA\\",strProductionPath.GetBuffer(255),255,strIniFilePath);
		SrcFilePath.Format(_T("%s%s%s"),strDriver,strProductionPath,strFileName);

		bRes = CopyFile(SrcFilePath,DestFilePath,FALSE);
		dwRes = WNetCancelConnection(strDriver,TRUE);		
	}	
	
	return bRes;
}

/************************************************************************/
/* 根据相应条件，下载TS文件   LYJ-2018-01-13
/************************************************************************/
BOOL LoadTsFileFun(CString strModelType,CString & strMaterialCode,CString & strFileName,BOOL CheckStatu)
{
	//获取产品物料编码
	if(!GteMaterialCode(strModelType,strMaterialCode))
	{
		AfxMessageBox( "系统消息: 获取产品物料编码失败! ", MB_ICONERROR  );
		return FALSE;
	}
	if(strMaterialCode.IsEmpty())
	{
		AfxMessageBox( "系统消息: 该产品未添加物料编码! ", MB_ICONERROR  );
		return FALSE;
	}
	
	//获取物料编码对应的TS文件名
	if(!GetTsFileName(strMaterialCode,strFileName))
	{
		AfxMessageBox( "系统消息: 获取产品的TS文件名失败! ", MB_ICONERROR  );
		return FALSE;
	}
	if(strFileName.IsEmpty())
	{
		AfxMessageBox( "系统消息: 不存在该型号产品的TS文件! ", MB_ICONERROR  );
		return FALSE;
	}
	
	if(CheckStatu)	//已勾选
	{
		CString strIniFilePath,strTestPath,strSrcPath;
		strIniFilePath.Format(_T("%s\\Config.ini"),GetSysRunPath());
		::GetPrivateProfileString("TSPATH","TestPath","OSA\\",strTestPath.GetBuffer(255),255,strIniFilePath); 
		strSrcPath.Format(_T("%s%s"),strTestPath,strFileName);
		if(!LoadTsFile_Ftp(strSrcPath))
		{
			return FALSE;
		}
	}
	else
	{
		if(!LoadTsFile_Shared(strFileName))
		{
			AfxMessageBox( "系统消息: 从共享文件夹中下载TS文件失败! ", MB_ICONERROR  );
			return FALSE;
		}
	}

	return TRUE;
}