#pragma once

// StatusLampPropPage.h : CStatusLampPropPage ����ҳ���������


// CStatusLampPropPage : �й�ʵ�ֵ���Ϣ������� StatusLampPropPage.cpp��

class CStatusLampPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CStatusLampPropPage)
	DECLARE_OLECREATE_EX(CStatusLampPropPage)

// ���캯��
public:
	CStatusLampPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_STATUSLAMP };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

