
// GK_KinectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GK_Kinect.h"
#include "GK_KinectDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static int nPlayIndex = 0;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGK_KinectDlg �Ի���



CGK_KinectDlg::CGK_KinectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGK_KinectDlg::IDD, pParent)
	, m_nPlayInterval(1000)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGK_KinectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFOLIST, m_cInfoList);
	DDX_Text(pDX, IDC_INP_PLAY_INTERVAL, m_nPlayInterval);
	DDX_Control(pDX, IDC_DIS_TMR, m_cDisTmr);
	DDX_Control(pDX, IDC_DIS_INFO, m_cDisInfo);
	DDX_Control(pDX, IDC_INFO_VIEW, m_cInfoView);
}

BEGIN_MESSAGE_MAP(CGK_KinectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_KINECT_OPEN, &CGK_KinectDlg::OnBnClickedKinectOpen)
	ON_BN_CLICKED(IDC_KINECT_CLOSE, &CGK_KinectDlg::OnBnClickedKinectClose)
	ON_BN_CLICKED(IDC_KINECT_LOG, &CGK_KinectDlg::OnBnClickedKinectLog)
	ON_BN_CLICKED(IDC_DATA_LOAD, &CGK_KinectDlg::OnBnClickedDataLoad)
	ON_BN_CLICKED(IDC_DATA_PLAY, &CGK_KinectDlg::OnBnClickedDataPlay)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_DATA_PAUSE, &CGK_KinectDlg::OnBnClickedDataPause)
	ON_BN_CLICKED(IDC_DATA_ONE_FRAME, &CGK_KinectDlg::OnBnClickedDataOneFrame)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CGK_KinectDlg ��Ϣ�������

BOOL CGK_KinectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	InitGL();
	m_cRobotCore.m_cKinectDataStream.pInfoList = &m_cInfoList;
	m_cRobotCore.m_DataProcess.pInfoList = &m_cInfoList;
	m_cRobotCore.m_DataProcess.pInfoDis = &m_cDisInfo;
	m_cRobotCore.pDisShow = &m_cInfoView;

	m_kinect.pRobotCore = &m_cRobotCore;
	m_kinect.pInfoList = &m_cInfoList;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGK_KinectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGK_KinectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGK_KinectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGK_KinectDlg::OnBnClickedKinectOpen()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_kinect.Start();
}


void CGK_KinectDlg::OnBnClickedKinectClose()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_kinect.Stop();
}


void CGK_KinectDlg::OnBnClickedKinectLog()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_cRobotCore.StartLogKinectStream(50);
}


void CGK_KinectDlg::OnBnClickedDataLoad()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fdlg(TRUE, L"k2", L"Default.k2", NULL, L"Kinect2�������ļ����Ͷ�(*.k2)|*.k2", this);
	if (fdlg.DoModal() != IDOK)
	{
		return;
	}

	CString strFn = fdlg.GetPathName();
	m_cRobotCore.m_cKinectDataStream.LoadFromBinFile(strFn);

	//�ط�����
	OnBnClickedDataPlay();
}


void CGK_KinectDlg::OnBnClickedDataPlay()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	nPlayIndex = 0;
	SetTimer(TMR_PLAY, m_nPlayInterval, NULL);
}


void CGK_KinectDlg::InitGL()
{
	m_oglStatic.Create(L"openGLstatic", WS_VISIBLE, CRect(0, 0, 800, 600), this, IDC_GL_VIEW);
	m_oglStatic.pInfoList = &m_cInfoList;
	m_oglStatic.pDepthTo3D = &(m_cRobotCore.m_cDepthTo3D);
	m_cRobotCore.pGL = &m_oglStatic;
}


void CGK_KinectDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == TMR_PLAY)
	{
		if (nPlayIndex >= m_cRobotCore.m_cKinectDataStream.m_ar3Dobj.size())
		{
			nPlayIndex = 0;
			KillTimer(TMR_PLAY);
			return;
		}

		m_cRobotCore.m_cDepthTo3D.csSrc3DObject.Lock();
		memcpy(m_cRobotCore.m_cDepthTo3D.pSrc3DObject, &(m_cRobotCore.m_cKinectDataStream.m_ar3Dobj[nPlayIndex]), sizeof(WP_K_3D_Object));
		m_cRobotCore.m_cDepthTo3D.csSrc3DObject.Unlock();
		m_cRobotCore.ProcessEvent();

		CString strTmp;
		strTmp.Format(L"���ڻطŵ� [%d] ֡", nPlayIndex + 1);
		m_cDisTmr.SetWindowTextW(strTmp);

		nPlayIndex++;
		
		//////////////////////////////////////////////////////////
		/*if (nPlayIndex %2 == 0)
		{
		m_cRobotCore.m_cDepthTo3D.csSrc3DObject.Lock();
		m_3dFile.Load3DObjFile(L"../../_3D_DataLog/��˾����0.3df2", m_cRobotCore.m_cDepthTo3D.pSrc3DObject, 1);
		m_cRobotCore.m_cDepthTo3D.csSrc3DObject.Unlock();

		m_cRobotCore.m_cDepthTo3D.m_fCameraAboveGround = 1.5f;
		m_cRobotCore.m_cDepthTo3D.m_fCameraPitch = -28;

		m_cRobotCore.ProcessEvent();
		}
		else
		{
		m_cRobotCore.m_cDepthTo3D.csSrc3DObject.Lock();
		m_3dFile.Load3DObjFile(L"../../_3D_DataLog/��������1.3df2", m_cRobotCore.m_cDepthTo3D.pSrc3DObject, 1);
		m_cRobotCore.m_cDepthTo3D.csSrc3DObject.Unlock();

		m_cRobotCore.m_cDepthTo3D.m_fCameraAboveGround = 1.5f;
		m_cRobotCore.m_cDepthTo3D.m_fCameraPitch = -28;

		m_cRobotCore.ProcessEvent();
		}*/
		/////////////////////////////////////////////////////////
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CGK_KinectDlg::OnBnClickedDataPause()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	KillTimer(TMR_PLAY);
}


void CGK_KinectDlg::OnBnClickedDataOneFrame()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int nCurPlayIndex = nPlayIndex - 1;
	if (nCurPlayIndex >= 0 && nCurPlayIndex < m_cRobotCore.m_cKinectDataStream.m_ar3Dobj.size())
	{
		CString m_strFile;
		CTime tm;
		tm = CTime::GetCurrentTime();
		m_strFile.Format(L"%d-%d-%d_%d-%d-%d.k2", tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
		m_strFile = L"../../_KinectStream/" + m_strFile;

		CFile fileWrite;		//��¼�ļ�
		BOOL fo_res;			//��¼�ļ����Ƿ�ɹ�

		//���ļ�
		fo_res = fileWrite.Open(m_strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
		if (fo_res == TRUE)
		{
			//��ʼд������
			fileWrite.Write(&(m_cRobotCore.m_cKinectDataStream.m_ar3Dobj[nCurPlayIndex]), sizeof(stWP_K_3D_Object));

			fileWrite.Close();

			CString strTmp;
			strTmp.Format(L"��ǰ֡���ݱ���%s", m_strFile);
			m_cRobotCore.m_cKinectDataStream.PrintInfo(strTmp);
		}
	}
}


void CGK_KinectDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_kinect.Stop();
	Sleep(500);
	CDialogEx::OnClose();
}
