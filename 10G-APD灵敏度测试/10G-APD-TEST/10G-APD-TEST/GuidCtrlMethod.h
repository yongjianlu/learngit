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
	 *�Ա��ؼ����г�ʼ��
	 *������
	 *	p_Grid��		��Ҫ���г�ʼ���ı��ؼ�
	 *	Row��			���Ҫ��ʼ��ΪRow��
	 *	Col:			���Ҫ��ʼ��ΪCol��
	 *	color��			��񱳾���ɫ
	 *	LineHeig��		�иߣ����Ǳ��Ԫ��ĸ߶�
	 *	ColumnWidth		�п����Ǳ��Ԫ��ĳ���
	 *					Ĭ�ϱ����һ����ͷ����һ����ͷ��
	 */

	void WriteGridCtrOne(CGridCtrl & pGrid,int Row,int Col,CString WriteStr,COLORREF TextColor);
	/*
	 *�����ĳһ����Ԫ��дĳ������
	 *������
	 *	Row��Col ��	��������Ҫд�ĵ�Ԫ����������
	 *	pGrid    ��	��Ҫд�ı��ؼ�����
	 *	WriteStr��	��Ҫд������
	 *	TextColor:	��Ԫ��������ɫ
	 */

	CString ReadGridCtrOne(CGridCtrl & pGrid,int Row,int Col);
	/*
	 *��ȡĳ����Ԫ�������
	 *������
	 *	Row��Col ��	��������Ҫ���ĵ�Ԫ����������
	 *	pGrid    ��	��Ҫ���ı��ؼ�����
	 *����ֵ��
	 *	���ض�ȡ�����ַ���
	 */

	void OnCellModel(CGridCtrl & pGrid,int Row,int Col,int Model,COLORREF TextColor);
	/*
	 *���õ�Ԫ���ģʽ
	 *������
	 *	Row��Col��	��������Ҫ����ģʽ�ĵ�Ԫ����������
	 *	pGrid   ��	���ؼ�����
	 *	Model:		
	 *		0����ʾNormal������ģʽ,�����ǿɶ�д�ı༭��
	 *		1����ʾReadonly��ֻ����
	 *		2����ʾCheckbox����ѡ��
	 *		3����ʾNumeric��ֻ�����ֵ���ݣ�
	 *		4����ʾDateTime���������ʱ�䣩
	 *
	 *TextColor:�ؼ������ɫ
	 */

	void OnCellCombo(CGridCtrl & pGrid,int Row,int Col,CString StrData);
	/*
	 *���øõ�Ԫ��Ϊ����ѡ���
	 *������
	 *	Row��Col��	��������Ҫ����ģʽ�ĵ�Ԫ����������
	 *	pGrid   ��	���ؼ�����
	 *	StrData:	����ѡ����еĿ�ѡ�ÿһ���Էֺŷָ�,����Ҫ�ԷֺŽ�β
	 */

	void OnCellUrl(CGridCtrl & pGrid,int Row,int Col,CString UrlStr);
	/*
	 *���øõ�Ԫ��ΪUrlģʽ�����һ����ַ���������ɽ������վ��
	 *������
	 *	Row��Col��	��������Ҫ����ģʽ�ĵ�Ԫ����������
	 *	pGrid   ��	���ؼ�����
	 *	UrlStr	:	��վ��ַ
	 */

};


#endif