// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� ADODB_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// ADODB_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

#include <vector>


//#import "c:\program files\common files\system\ado\msado15.dll" \
//			no_namespace rename("EOF","adoEOF") 

#import "msado15.dll" 	no_namespace rename("EOF","adoEOF") 
using namespace std; 

#define MAX_CONNECT_STRING_LEN				512
#define MAX_READ_LEN						10240



#ifdef ADODB_EXPORTS
#define ADODB_API __declspec(dllexport)
#else
#define ADODB_API __declspec(dllimport)
#endif

//==================================================================================
class ADODB_API _variant_t;
class ADODB_API _convertor_t
{
public:
	_convertor_t( _variant_t& val );
	~_convertor_t();
public:
	char*	ToString( char* pVal );
	int		ToInteger();
	bool	ToBoolean();
	double	ToDouble();
private:
	_variant_t	m_value;
};

//===============================================================================================================
class ADODB_API _t_PositionEnum_Param_t
{
public:
	_t_PositionEnum_Param_t( );
	_t_PositionEnum_Param_t( _RecordsetPtr* rs );
	~_t_PositionEnum_Param_t();


public:

	void	Reset( _RecordsetPtr* rs );

	_t_PositionEnum_Param_t&		operator= ( PositionEnum_Param param ); 
	_t_PositionEnum_Param_t&		operator= ( int param ); 

	operator		PositionEnum_Param()const;	

private:
	_RecordsetPtr*	m_prsThis;
};


//=================================================================================================================


class ADODB_API RstResult 
{	
public:
	RstResult();
	~RstResult();
public:

	bool			Query( const char* cSQL, _ConnectionPtr* pConn, _RecordsetPtr* rs, int cursortype = 0, int lockType = 1, long options = 1 );

	bool			IsBOF();
	bool			IsEOF();

	long			GetRows();
	long			State();

	//�ƶ��α���
	bool			MoveToFirst();					
	bool			MoveToNext();

	RstResult&		operator= ( RstResult res ); 

	_variant_t		operator()( const char* szFieldName );
	_variant_t		operator()( long sIndex );			

	_convertor_t	Convertor( const char* szFieldName );
	_convertor_t	Convertor( long sIndex );

	_variant_t		GetVariant( const char* szFieldName );
	_variant_t		GetVariant( long sIndex );	

	bool			GetString( char* pVal, const char* szFieldName );
	bool			GetString( char* pVal, long sIndex );					

	bool			GetInt( int& nVal, const char* szFieldName );
	bool			GetInt( int& nVal, long sIndex );

	bool			GetBoolean( bool& bVal, const char* szFieldName );
	bool			GetBoolean( bool& bVal, long sIndex );

	bool			GetDouble( double& dblVal, const char* szFieldName );
	bool			GetDouble( double& dblVal, long sIndex );

	_t_PositionEnum_Param_t	AbsolutePosition;

private:
	_RecordsetPtr*			m_prsThis;
	char					Valstring[MAX_READ_LEN];
};


class ADODB_API ADO_Connection
{
public:
	ADO_Connection();
	ADO_Connection( char* cConnectString, int nID );
	virtual					~ADO_Connection();

public:
	enum	DataBaseType
	{
		DBT_SQL,
		DBT_ACCESS,
	};

public:
	RstResult				Result;

	bool					IsConnect()const;

	int						SetConnTimeOut( long lngTimeOut ){ return (*m_pCon)->put_ConnectionTimeout( lngTimeOut ); } 
	int						SetCommTimeOut( long lngTimeOut ){ return (*m_pCon)->put_CommandTimeout( lngTimeOut ); }


	//�����ַ���
	void					SetConnectString( char* szConnString );
	char*					GetConnectString(){ return m_szConnString; }

	//��/�ر�����
	bool					OpenConnection( const char* sConnString );
	bool					OpenConnection( const char*	sDBIP,const char* sAccount, const char* cPassword, const char* DBName, int DBPort );
	void					Close();

	long					BeginTrans();
	HRESULT					CommitTrans();
	HRESULT					RollbackTrans();

	//ִ��SQL
	//������Ӱ��ļ�¼����.  ʧ�ܷ��أ�1   
	long					Execute( const char* szCmd );
	long					Execute( std::vector<std::string> &szArray );

	bool					Query( const char* cSQL, _RecordsetPtr* rs, int cursortype = 0, int lockType = 1, long options = 1  );


	_ConnectionPtr*			GetConnectionPtr(){ return m_pCon; }

private:
	_ConnectionPtr*			m_pCon; 
	char					m_szConnString[MAX_CONNECT_STRING_LEN];
};

//��������
ADODB_API void		variant2string(  TCHAR* szVal, VARIANT var );
ADODB_API double	variant2Double( _variant_t *pVal, double default_val = 0.0f );

