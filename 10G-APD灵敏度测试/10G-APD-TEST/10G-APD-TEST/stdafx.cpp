// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// 10G-APD-TEST.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

int ComNum[1024] = {0};			//��ѯ�����������Դ����Լ�ʹ��״̬

float g_PowerVal = -100.0f;		//��У׼�� �ֶ����Ե��� ����ֵ

CString g_UserAccount		= _T("");		//�����˺�
CString g_UserName			= _T("");		//������Ա����
CString g_UserPermissions	= _T("");		//��¼�û�Ȩ��
int g_IntModelState = 0;					//ѡ��Ĳ���ģʽ
CString g_EnabledArray[48];					//�û���¼ʱ��ȡ����Ȩ��