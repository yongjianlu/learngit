#pragma once

// StatusLampPropPage.h : CStatusLampPropPage 属性页类的声明。


// CStatusLampPropPage : 有关实现的信息，请参阅 StatusLampPropPage.cpp。

class CStatusLampPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CStatusLampPropPage)
	DECLARE_OLECREATE_EX(CStatusLampPropPage)

// 构造函数
public:
	CStatusLampPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_STATUSLAMP };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

