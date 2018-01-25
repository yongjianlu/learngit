#include "stdafx.h"
#include "GuidCtrlMethod.h"

#include "NewCellTypes/GridURLCell.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellDateTime.h"

static COLORREF NormalColor		= RGB(211, 211,211);//����
static COLORREF ReadonlyColor	= RGB(190,190,190);	//ֻ��
static COLORREF NumericColor	= RGB(193, 205,193);//��ֵ
static COLORREF DataTimeColor	= RGB(144 ,238,255);//����
static COLORREF ComboColor		= RGB(255 ,187,255);//�����б�
static COLORREF UrlColor		= RGB(187 ,255,255);//��վ
static COLORREF CheckboxColor	= RGB(122 ,139,139);//��ѡ��

GuidCtrlMethod::GuidCtrlMethod()
{

}

GuidCtrlMethod::~GuidCtrlMethod()
{

}

//�Ա��ؼ����г�ʼ��
void GuidCtrlMethod::GridCtrlInit(CGridCtrl &pGrid, UINT Row, UINT Col, COLORREF color, UINT LineHeig, UINT ColumnWidth)
{
	if(Row * Col == 0)
		return;

	pGrid.SetEditable(true);

    pGrid.SetTextBkColor(color);			//������ɫ����
    pGrid.SetRowCount(Row);					//��ʼΪ Row ��
    pGrid.SetColumnCount(Col);				//��ʼ��Ϊ Col ��
    pGrid.SetFixedRowCount(1);				//��ͷΪ 1 ��
    pGrid.SetFixedColumnCount(1);			//��ͷΪ 1 ��

	for (int row = 0; row < pGrid.GetRowCount(); row++)			//��ȡ����
	{
		for (int col = 0; col < pGrid.GetColumnCount(); col++)	//��ȡ����
		{ 
			if(row == 0)
				pGrid.SetRowHeight(row,LineHeig + 10);		//���ñ�ͷ�и�  
			else
				pGrid.SetRowHeight(row,LineHeig);			//���ø��и�  

			if(col == 0)
				pGrid.SetColumnWidth(col,ColumnWidth + 30);	//���ñ�ͷ�п�
			else
				pGrid.SetColumnWidth(col,ColumnWidth + 5);		//���ø��п�
		}
	}
}

//�����ĳһ����Ԫ��дĳ������
void GuidCtrlMethod::WriteGridCtrOne(CGridCtrl &pGrid, int Row, int Col, CString WriteStr, COLORREF TextColor)
{
	if((Row > pGrid.GetRowCount() -1) || (Col > pGrid.GetColumnCount() -1))
		return ;
	
	GV_ITEM Item; 
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = Row;	//��
	Item.col = Col;	//��
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.strText.Format(WriteStr);
	pGrid.SetItem(&Item); 

	pGrid.SetItemFgColour(Row,Col,TextColor);		//��Ԫ����������ɫ
	pGrid.Invalidate();
}

//��ȡĳ����Ԫ�������
CString GuidCtrlMethod::ReadGridCtrOne(CGridCtrl &pGrid, int Row, int Col)
{
	CString ReadStr = "";
	ReadStr = pGrid.GetItemText(Row,Col);

	return ReadStr;
}

//���õ�Ԫ���ģʽ
void GuidCtrlMethod::OnCellModel(CGridCtrl &pGrid, int Row, int Col, int Model, COLORREF TextColor)
{
	//����ʲô���������������Ϊ����ģʽ
	pGrid.SetCellType(Row, Col, RUNTIME_CLASS(CGridCell));
    pGrid.SetItemState(Row, Col, pGrid.GetItemState(Row,Col) & ~GVIS_READONLY);
    pGrid.Invalidate();

	if(Model == 0)
	{
		pGrid.SetItemBkColour(Row,Col,TextColor);	//��Ԫ�񱳾���ɫ
	}
	else if(Model == 1)			//Readonly��ֻ����
	{
		pGrid.SetItemBkColour(Row,Col,TextColor);	//��Ԫ�񱳾���ɫ(��ɫ)
		pGrid.SetItemState(Row, Col, pGrid.GetItemState(Row,Col) | GVIS_READONLY);
	}
	else if(Model == 2)		//Checkbox����ѡ��
	{
		pGrid.SetItemBkColour(Row,Col,TextColor);	//��Ԫ�񱳾���ɫ������ɫ��
		pGrid.SetCellType(Row, Col, RUNTIME_CLASS(CGridCellCheck));
	}
	else if(Model == 3)		//Numeric��ֻ�����ֵ���ݣ�
	{
		pGrid.SetItemBkColour(Row,Col,TextColor);	//��Ԫ�񱳾���ɫ������ɫ��
		pGrid.SetCellType(Row, Col, RUNTIME_CLASS(CGridCellNumeric));
	}
	else if(Model == 4)		//DateTime���������ʱ�䣩
	{
		pGrid.SetItemBkColour(Row,Col,TextColor);	//��Ԫ�񱳾���ɫ
		pGrid.SetCellType(Row, Col, RUNTIME_CLASS(CGridCellDateTime));
		CGridCellDateTime * pCell = (CGridCellDateTime*) pGrid.GetCell(Row, Col);
		pCell->SetTime(CTime::GetCurrentTime());
		pGrid.RedrawCell(Row, Col);
		pGrid.SetColumnWidth(1,100);
	}

}

//���øõ�Ԫ��Ϊ����ѡ���
void GuidCtrlMethod::OnCellCombo(CGridCtrl &pGrid, int Row, int Col, CString StrData)
{ 
	CString Str[1024];
	int i = 0;
	while(StrData.Find(_T(";")) + 1)//���Ҳ����ո�ʱ����-1��������������������ֵ��1��ѭ��
	{
		Str[i] = StrData.Left(StrData.Find(_T(";")));//�ҵ��ֺŵ���������ȡ�ֺ���ߵ��ַ���,��������ӵ�Str�����С�
		StrData.Delete(0,Str[i].GetLength() + 1);	//����ɾ���ֺź�����ߵ��ַ����ĵõ������ַ�����
		i++;
	}

	i = 0;
	OnCellModel(pGrid, Row, Col, 0,NormalColor);	//��������Ϊ����ģʽ

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

	pGrid.SetItemBkColour(Row,Col,ComboColor);	//��Ԫ�񱳾���ɫ

    CGridCellCombo *pCell = (CGridCellCombo*) pGrid.GetCell(Row, Col);
    pCell->SetOptions(options);
    pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
}

//���øõ�Ԫ��ΪUrlģʽ�����һ����ַ���������ɽ������վ��
void GuidCtrlMethod::OnCellUrl(CGridCtrl &pGrid, int Row, int Col, CString UrlStr)
{
	OnCellModel(pGrid, Row, Col, 0, NormalColor);	//��������Ϊ����ģʽ
	pGrid.SetItemBkColour(Row,Col,UrlColor);	//��Ԫ�񱳾���ɫ

	if (!pGrid.SetCellType(Row, Col, RUNTIME_CLASS(CGridURLCell)))
        return;
    pGrid.SetItemText(Row, Col, UrlStr);
}