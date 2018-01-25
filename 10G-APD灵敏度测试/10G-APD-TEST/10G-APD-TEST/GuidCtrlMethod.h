#ifndef GUIDCTRLMETHOD_H_
#define GUIDCTRLMETHOD_H_

#pragma once
#include "GridCtrl_src/GridCtrl.h"


class GuidCtrlMethod{

public:
	GuidCtrlMethod();
   ~GuidCtrlMethod();

	void GridCtrlInit(CGridCtrl & pGrid,UINT Row,UINT Col,COLORREF color,UINT LineHeig,UINT ColumnWidth);	
	/*
	 *对表格控件进行初始化
	 *参数：
	 *	p_Grid：		将要进行初始化的表格控件
	 *	Row：			表格将要初始化为Row行
	 *	Col:			表格将要初始化为Col列
	 *	color：			表格背景颜色
	 *	LineHeig：		行高，即是表格单元格的高度
	 *	ColumnWidth		列宽，即是表格单元格的长度
	 *					默认表格有一个表头行与一个表头列
	 */

	void WriteGridCtrOne(CGridCtrl & pGrid,int Row,int Col,CString WriteStr,COLORREF TextColor);
	/*
	 *向表格的某一个单元格写某个数据
	 *参数：
	 *	Row、Col ：	代表所将要写的单元格所属行列
	 *	pGrid    ：	将要写的表格控件变量
	 *	WriteStr：	将要写的数据
	 *	TextColor:	单元格字体颜色
	 */

	CString ReadGridCtrOne(CGridCtrl & pGrid,int Row,int Col);
	/*
	 *读取某个单元格的数据
	 *参数：
	 *	Row、Col ：	代表所将要读的单元格所属行列
	 *	pGrid    ：	将要读的表格控件变量
	 *返回值：
	 *	返回读取到的字符串
	 */

	void OnCellModel(CGridCtrl & pGrid,int Row,int Col,int Model,COLORREF TextColor);
	/*
	 *设置单元格的模式
	 *参数：
	 *	Row、Col：	代表所将要更改模式的单元格所属行列
	 *	pGrid   ：	表格控件变量
	 *	Model:		
	 *		0：表示Normal（正常模式,可能是可读写的编辑框）
	 *		1：表示Readonly（只读）
	 *		2：表示Checkbox（单选框）
	 *		3：表示Numeric（只存放数值数据）
	 *		4：表示DateTime（存放日期时间）
	 *
	 *TextColor:控件表格颜色
	 */

	void OnCellCombo(CGridCtrl & pGrid,int Row,int Col,CString StrData);
	/*
	 *设置该单元格为下拉选择框
	 *参数：
	 *	Row、Col：	代表所将要更改模式的单元格所属行列
	 *	pGrid   ：	表格控件变量
	 *	StrData:	下拉选择框中的可选项，每一项以分号分割,并且要以分号结尾
	 */

	void OnCellUrl(CGridCtrl & pGrid,int Row,int Col,CString UrlStr);
	/*
	 *设置该单元格为Url模式（存放一个网址，单击即可进入该网站）
	 *参数：
	 *	Row、Col：	代表所将要更改模式的单元格所属行列
	 *	pGrid   ：	表格控件变量
	 *	UrlStr	:	网站网址
	 */

};


#endif