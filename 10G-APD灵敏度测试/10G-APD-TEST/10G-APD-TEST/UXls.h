#if !defined UXLS_H
#define UXLS_H

#include "excel.h"
#include "comdef.h" 

#define xlText -4158

class AFX_EXT_CLASS CUXls
{
public:
	void DeletSheet(long lngSheetItem);
	void DeletSheet(CString strSheetName);
	void SetSheetName(CString strOldName,CString strNewName);
	void SetSheetName(long lngSheetItem,CString strNewName);
	long GetSheetCount();
	void GetSheet(long sheetItem);
	void GetSheet(CString strSheetName);
	void Delet(long lngFirstRow, long lngFirstCol,long lngLastRow,long lngLastCol);
	void InserRow(long lngRow);
	int SearchText(LPCTSTR strText,long* lngRow,long* lngCol);
	void Save();
    void SaveAs(LPCTSTR lpstrFileName);
    void SaveAs(LPCTSTR lpstrFileName,long lngFileFormat = 0);
	void CreateFile(LPCTSTR AddressTempName,BOOL bVisible);
	void SetCellText(long lngRow,long lngCol,LPCTSTR strText);
	void SetCellValue( long lngRow, long lngCol, _variant_t varVal );

	CString GetCellText(long lngRow,long lngCol);
	long GetUsedRow();
	void CloseExcel();
	int OpenXlsFile(LPCTSTR strFileName,BOOL bVisible);

public:
	void SetCellFormat(long lngRow,long lngCol,LPCTSTR strFormat);
	void DeleteSeriesCollection(long lngItem);
	void GetActiveChart(CString strSheetName);
	void DeleteColumn(long lngCol);
	void DeletRow(long lngRow);
	void Clear(long lngSheetItem,long FirstRow,long FirstCol,long LastRow,long LastCol);
	void Clear(CString strSheetName,long FirstRow,long FirstCol,long LastRow,long LastCol);
	void Clear(long lngSheetItem);
	void Clear(CString strSheetName);
	CString GetSheetName(long lngSheetItem);
   CUXls();
   ~CUXls();  

private:
	_Application exlapp;   //组件服务器的各个classes
	_Workbook wbk;
    Workbooks wbks;
	_Worksheet wht;
	Worksheets whts;
	Range range;
	LPDISPATCH lpDisp;

	_Chart crt;
	Charts crts;

private:
	CString strName;
	BOOL bApp;

};

#endif