
// GK_Kinect.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGK_KinectApp: 
// �йش����ʵ�֣������ GK_Kinect.cpp
//

class CGK_KinectApp : public CWinApp
{
public:
	CGK_KinectApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGK_KinectApp theApp;