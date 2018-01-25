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
 *��ȡ TS �ļ��ڲ�Ʒ����������ݣ�������BaseSpec����Ʒ�������ԣ��е�ָ���ֶ����ƣ���������ֵ���Ĺ������
 *������
 *	CUexcel��		Exce�ļ���������
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

	if( -1 == m_CUexcel->SearchText(_T("��Ʒ���  Product Spec."),pRow,pCol))
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

	//������ȡ����ֵ
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
 *��ȡ TS �ļ��ڲ�Ʒ���Թ�����ݣ�������TestSpec���е�ָ���ֶ����ƣ�����ֵ���Ĺ������
 *������
 *	CUexcel��		Exce�ļ���������
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

	if( -1 == m_CUexcel->SearchText(_T("���Թ��  Test Spec."),pRow,pCol))
	{
		IntReturn =  -1;
		goto _RETURN_FUN_;
	}
	BeginRow = *pRow + 3;
	col = *pCol;
	//��ȡType�ķ�Χ
	bStatu = FALSE;//ΪTRUEʱ��ʾ���ҵ�Type����ʼλ��
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

	//��ȡTemperatrue��Χ
	bStatu = FALSE;//ΪTRUEʱ��ʾ���ҵ�strTemperatrue����ʼλ��
	for(row = BeginRow; row <= EndRow; row++)
	{
		strReadTsSpecTemp = m_CUexcel->GetCellText(row,*pCol + 2);
		if(row == BeginRow && strReadTsSpecTemp == _T(""))	//��ʱTemperatrue��Χ ���� Type�ķ�Χ
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

	//��ȡVoltage��Χ
	bStatu = FALSE;//ΪTRUEʱ��ʾ���ҵ�strVoltage����ʼλ��
	for(row = BeginRow; row <= EndRow; row++)
	{
		strReadTsSpecTemp = m_CUexcel->GetCellText(row,*pCol + 3);
		if(row == BeginRow && strReadTsSpecTemp == _T(""))	//��ʱVoltage��Χ ���� Type�ķ�Χ
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

	//��ȡָ��strFieldName������ֵ
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
 *����һ����־��ȡһ���ַ����е� ���ַ���������ȡ ��123,456,789���е� ��123������456������789������־���� ","
 *������
 *	SrcCString��Դ�ַ���
 *	destCString�����ȡ�õ��ַ���
 *	Mark����ȡ�ı�־
 */
void ReadTsSpec::SubCString(CString SrcCString,vector<CString> & destCString,CString Mark)
{
	destCString.clear();
	CArray<CString,CString>strArray;  
	int i = 0;
	while(SrcCString.Find(Mark)+1)	//���Ҳ�����־ʱ����-1��������������������ֵ��1��ѭ��
	{
		strArray.Add(SrcCString.Left(SrcCString.Find(Mark)));//�ҵ���־����������ȡ�ո���ߵ��ַ���,��������ӵ�strArray��̬�����С�
		destCString.push_back(strArray[i]);
		SrcCString.Delete(0,strArray[i++].GetLength()+1);//����ɾ���ո������ߵ��ַ����ĵõ������ַ�����
	}
	strArray.Add(SrcCString);//�����Ͼ�������ɾ����ʣ�µ��ַ�����
	destCString.push_back(strArray[i]);
}

//��ȡָ���ͺŵ����ϱ���
BOOL GteMaterialCode(CString strProductModel,CString & strMaterialCode)
{
	ADO_Connection * SqlCon    = new ADO_Connection();	//sql server���ݿ����
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

//����ָ�������ϱ��룬��121��������ȡ��Ӧ��TS�ļ������ƣ��ɹ����� TRUE
BOOL GetTsFileName(CString strMaterialCode,CString & strFileName)
{
	ADO_Connection * SqlCon    = new ADO_Connection();	//sql server���ݿ����
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

//��ȡ��ǰϵͳ����·��
CString GetSysRunPath()
{
	CString strPath;
	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH��WINDEF.h�ж����ˣ�����260  
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

//����ָ��Ŀ¼�µ�Ftp�����µ��ļ�������ָ��Ŀ¼��,�ɹ����� TRUE
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
				sErr.Format( "û���ҵ� %s �ļ�����ȷ����֤��������·�������Ƿ���ȷ��", FtpPath ) ;
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
					//��Ŀ¼
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
						//�ļ��в�����, ����֮
						if( !(CreateDirectory((LPCTSTR)sLocalTemp, NULL)) )
						{
							//����ʧ��
							DWORD dErr = GetLastError() ;
							sErr.Format( "������Ŀ¼ʧ��: ERROR  %d" ,dErr ) ;
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
					if( ( CFile::GetStatus( sLocalTemp, rStatus ) ) )//�ļ��Ѵ��ڣ���ɾ��ԭ�ļ�
					{
						// ɾ���ļ����ɹ�����0�����򷵻�-1
						if (-1 == remove(sLocalTemp))
						{
							sErr = _T("����ʧ�ܣ�ʧ��ԭ��ɾ��ԭ�ļ�ʧ�ܣ�");
							return FALSE;
						}
					}

					//�����ļ�
					if( !( pFtpCon->GetFile( sTemp, sLocalTemp, true, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY, 1) ) )
					{
						//����ʧ��
						DWORD dErr = GetLastError() ;
						sErr.Format( "����ʧ��: ERROR  %d" ,dErr ) ;
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
		sErr.Format( "����FTPʧ��:%s", pEx ) ;
		pEx->Delete();
		pFtpCon=NULL;
		return FALSE ;
	}
	return TRUE ;
}

//��107��������(��֤������)�����ļ���Ĭ��·������Ftp�ķ�ʽ���ɹ����� TRUE��ʧ�ܷ��� FALSE��
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

//��107�Ĺ����ļ���(����������)�������ļ���Ĭ��·�����ɹ����� TRUE
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
    nr.lpLocalName=strDriver.GetBuffer(0);   //�˴�������ĳ���̷�����:"z:"   
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
/* ������Ӧ����������TS�ļ�   LYJ-2018-01-13
/************************************************************************/
BOOL LoadTsFileFun(CString strModelType,CString & strMaterialCode,CString & strFileName,BOOL CheckStatu)
{
	//��ȡ��Ʒ���ϱ���
	if(!GteMaterialCode(strModelType,strMaterialCode))
	{
		AfxMessageBox( "ϵͳ��Ϣ: ��ȡ��Ʒ���ϱ���ʧ��! ", MB_ICONERROR  );
		return FALSE;
	}
	if(strMaterialCode.IsEmpty())
	{
		AfxMessageBox( "ϵͳ��Ϣ: �ò�Ʒδ������ϱ���! ", MB_ICONERROR  );
		return FALSE;
	}
	
	//��ȡ���ϱ����Ӧ��TS�ļ���
	if(!GetTsFileName(strMaterialCode,strFileName))
	{
		AfxMessageBox( "ϵͳ��Ϣ: ��ȡ��Ʒ��TS�ļ���ʧ��! ", MB_ICONERROR  );
		return FALSE;
	}
	if(strFileName.IsEmpty())
	{
		AfxMessageBox( "ϵͳ��Ϣ: �����ڸ��ͺŲ�Ʒ��TS�ļ�! ", MB_ICONERROR  );
		return FALSE;
	}
	
	if(CheckStatu)	//�ѹ�ѡ
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
			AfxMessageBox( "ϵͳ��Ϣ: �ӹ����ļ���������TS�ļ�ʧ��! ", MB_ICONERROR  );
			return FALSE;
		}
	}

	return TRUE;
}