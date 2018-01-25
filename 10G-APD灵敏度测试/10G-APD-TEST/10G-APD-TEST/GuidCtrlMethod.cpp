#include "stdafx.h"
#include "GuidCtrlMethod.h"

#include "NewCellTypes/GridURLCell.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellDateTime.h"

static COLORREF NormalColor		= RGB(211, 211,211);//正常
static COLORREF ReadonlyColor	= RGB(190,190,190);	//只读
static COLORREF NumericColor	= RGB(193, 205,193);//数值
static COLORREF DataTimeColor	= RGB(144 ,238,255);//日期
static COLORREF ComboColor		= RGB(255 ,187,255);//下拉列表
static COLORREF UrlColor		= RGB(187 ,255,255);//网站
static COLORREF CheckboxColor	= RGB(122 ,139,139);//单选框

GuidCtrlMethod::GuidCtrlMethod()
{

}

GuidCtrlMethod::~GuidCtrlMethod()
{

}

//对表格控件进行初始化
void GuidCtrlMethod::GridCtrlInit(CGridCtrl &pGrid, UINT Row, UINT Col, COLORREF color, UINT LineHeig, UINT ColumnWidth)
{
	if(Row * Col == 0)
		return;

	pGrid.SetEditable(true);

    pGrid.SetTextBkColor(color);			//背景颜色设置
    pGrid.SetRowCount(Row);					//初始为 Row 行
    pGrid.SetColumnCount(Col);				//初始化为 Col 列
    pGrid.SetFixedRowCount(1);				//表头为 1 行
    pGrid.SetFixedColumnCount(1);			//表头为 1 列

	for (int row = 0; row < pGrid.GetRowCount(); row++)			//获取行数
	{
		for (int col = 0; col < pGrid.GetColumnCount(); col++)	//获取列数
		{ 
			if(row == 0)
				pGrid.SetRowHeight(row,LineHeig + 10);		//设置表头行高  
			else
				pGrid.SetRowHeight(row,LineHeig);			//设置各行高  

			if(col == 0)
				pGrid.SetColumnWidth(col,ColumnWidth + 30);	//设置表头列宽
			else
				pGrid.SetColumnWidth(col,ColumnWidth + 5);		//设置各列宽
		}
	}
}

//向表格的某一个单元格写某个数据
void GuidCtrlMethod::WriteGridCtrOne(CGridCtrl &pGrid, int Row, int Col, CString WriteStr, COLORREF TextColor)
{
	if((Row > pGrid.GetRowCount() -1) || (Col > pGrid.GetColumnCount() -1))
		return ;
	
	GV_ITEM Item; 
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = Row;	//行
	Item.col = Col;	//列
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.strText.Format(WriteStr);
	pGrid.SetItem(&Item); 

	pGrid.SetItemFgColour(Row,Col,TextColor);		//单元格内字体颜色
	pGrid.Invalidate();
}

//读取某个单元格的数据
CString GuidCtrlMethod::ReadGridCtrOne(CGridCtrl &pGrid, int Row, int Col)
{
	CString ReadStr = "";
	ReadStr = pGrid.GetItemText(Row,Col);

	return ReadStr;
}

//设置单元格的模式
void GuidCtrlMethod::OnCellModel(CGridCtrl &pGrid, int Row, int Col, int Model, COLORREF TextColor)
{
	//无论什么情况，首先设置其为正常模式
	pGrid.SetCellType(Row, Col, RUNTIME_CLASS(CGridCell));
    pGrid.SetItemState(Row, Col, pGrid.GetItemState(Row,Col) & ~GVIS_READONLY);
    pGrid.Invalidate();

	if(Model == 0)
	{
		pGrid.SetItemBkColour(Row,Col,TextColor);	//单元格背景颜色
	}
	else if(Model == 1)			//Readonly（只读）
	{
		pGrid.SetItemBkColour(Row,Col,TextColor);	//单元格背景颜色(灰色)
		pGrid.SetItemState(Row, Col, pGrid.GetItemState(Row,Col) | GVIS_READONLY);
	}
	else if(Model == 2)		//Checkbox（单选框）
	{
		pGrid.SetItemBkColour(Row,Col,TextColor);	//单元格背景颜色（蓝紫色）
		pGrid.SetCellType(Row, Col, RUNTIME_CLASS(CGridCellCheck));
	}
	else if(Model == 3)		//Numeric（只存放数值数据）
	{
		pGrid.SetItemBkColour(Row,Col,TextColor);	//单元格背景颜色（淡蓝色）
		pGrid.SetCellType(Row, Col, RUNTIME_CLASS(CGridCellNumeric));
	}
	else if(Model == 4)		//DateTime（存放日期时间）
	{
		pGrid.SetItemBkColour(Row,Col,TextColor);	//单元格背景颜色
		pGrid.SetCellType(Row, Col, RUNTIME_CLASS(CGridCellDateTime));
		CGridCellDateTime * pCell = (CGridCellDateTime*) pGrid.GetCell(Row, Col);
		pCell->SetTime(CTime::GetCurrentTime());
		pGrid.RedrawCell(Row, Col);
		pGrid.SetColumnWidth(1,100);
	}

}

//设置该单元格为下拉选择框
void GuidCtrlMethod::OnCellCombo(CGridCtrl &pGrid, int Row, int Col, CString StrData)
{ 
	CString Str[1024];
	int i = 0;
	while(StrData.Find(_T(";")) + 1)//当找不到空格时返回-1，所以这里我用它返回值加1来循环
	{
		Str[i] = StrData.Left(StrData.Find(_T(";")));//找到分号的索引，截取分号左边的字符串,并将它添加到Str数组中。
		StrData.Delete(0,Str[i].GetLength() + 1);	//返回删除分号和其左边的字符串的得到的新字符串。
		i++;
	}

	i = 0;
	OnCellModel(pGrid, Row, Col, 0,NormalColor);	//首先设置为正常模式

    if (!pGrid.SetCellType(Row, Col, RUNTIME_CLASS(CGridCellCombo)))
        return;

    pGrid.SetItemText(Row, Col, Str[0]);

    CStringArray options;
	while(1)
	{
		if(i == 1024 || Str[i] == _T(""))
			break;
		options.Add(Str[i]);
		i++;
	}

	pGrid.SetItemBkColour(Row,Col,ComboColor);	//单元格背景颜色

    CGridCellCombo *pCell = (CGridCellCombo*) pGrid.GetCell(Row, Col);
    pCell->SetOptions(options);
    pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
}

//设置该单元格为Url模式（存放一个网址，单击即可进入该网站）
void GuidCtrlMethod::OnCellUrl(CGridCtrl &pGrid, int Row, int Col, CString UrlStr)
{
	OnCellModel(pGrid, Row, Col, 0, NormalColor);	//首先设置为正常模式
	pGrid.SetItemBkColour(Row,Col,UrlColor);	//单元格背景颜色

	if (!pGrid.SetCellType(Row, Col, RUNTIME_CLASS(CGridURLCell)))
        return;
    pGrid.SetItemText(Row, Col, UrlStr);
}