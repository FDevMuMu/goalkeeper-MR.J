
// GK_KinectDlg.h : ͷ�ļ�
//

#pragma once

#include "Kinect2.h"
#include "RobotCore.h"
#include "OpenGLStatic.h"
#include "afxwin.h"
#include "C3DObjFile.h"

#define TMR_PLAY 1

// CGK_KinectDlg �Ի���
class CGK_KinectDlg : public CDialogEx
{
// ����
public:
	CGK_KinectDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GK_KINECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	CKinect2 m_kinect;
	CRobotCore m_cRobotCore;
	COpenGLStatic m_oglStatic; 
	C3DObjFile m_3dFile;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedKinectOpen();
	afx_msg void OnBnClickedKinectClose();
	afx_msg void OnBnClickedKinectLog();
	afx_msg void OnBnClickedDataLoad();
	afx_msg void OnBnClickedDataPlay();
	void InitGL();
	CListBox m_cInfoList;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_nPlayInterval;
	afx_msg void OnBnClickedDataPause();
	CEdit m_cDisTmr;
	CEdit m_cDisInfo;
	CStatic m_cInfoView;
	afx_msg void OnBnClickedDataOneFrame();
	afx_msg void OnClose();
};
