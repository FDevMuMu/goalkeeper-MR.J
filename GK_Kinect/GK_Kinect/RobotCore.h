#pragma once
#include "DepthTo3D.h"
#include "OpenGLStatic.h"
#include "KinectDataStream.h"
#include "DataProcess.h"

#define RTASK_NONE				0
#define RTASK_GOTO				1
#define RTASK_FIND_OBJ_DESK		2
#define RTASK_GRAB_OBJ			3
#define RTASK_PASS_OBJ_PERSON	4

#define RTASK_FOLLOW			5

#define MAX_LIFT_POS		9500	//����λ�����ֵ
#define MAX_SHOULDER_POS	4200	//���̧�����ֵ

class CRobotCore
{
public:
	CRobotCore();
	~CRobotCore();


	COpenGLStatic* pGL;
	CDepthTo3D m_cDepthTo3D;
	CDataProcess m_DataProcess;

	HANDLE	m_hToProcessEvent;
	HANDLE	m_hShutdownEvent;
	HANDLE	m_hEventArray[2];
	BOOL	m_bThreadAlive;

	int nAutoTask;		//�Զ�������ģʽ

	CKinectDataStream m_cKinectDataStream;	//���ݴ洢����
	int nNumToSave;		//������֡��洢
	bool bLogFrames;	//��¼Kinect����
	CStatic* pDisShow;	

	void Procedure();
	void TackAction();
	static UINT PointCloudsThread(LPVOID pParam);
	void ProcessDepth(USHORT* inBuf, int inWidth, int inHeight, USHORT nMinDepth, USHORT nMaxDepth);
	void ProcessRGB(UCHAR* inBuf, int inWidth, int inHeight);
	void m_ProcessOneFrame();
	void ProcessEvent();
	void StartLogKinectStream(int inFrameNum);
};

