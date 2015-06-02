#include "stdafx.h"
#include "DataProcess.h"

static unsigned char dataShow[320*240*3];	//�������Ͻ�������ʾ������
static BITMAPINFOHEADER bih;

CDataProcess::CDataProcess()
{
	pInfoList = NULL;
	pInfoDis = NULL;

	memset(dataShow, 0, 320 * 240 * 3);
	memset(&bih, 0, sizeof(bih));
	bih.biSize = sizeof(bih);
	bih.biWidth = 320;
	bih.biHeight = 240;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
}


CDataProcess::~CDataProcess()
{
}


void CDataProcess::PrintList(CString inStr)
{
	if (pInfoList != NULL)
	{
		pInfoList->AddString(inStr);
		pInfoList->SetCurSel(pInfoList->GetCount() - 1);
	}
}


void CDataProcess::PrintDis(CString inStr)
{
	if (pInfoDis != NULL)
	{
		pInfoDis->SetWindowTextW(inStr);
	}
}

//��ʾ����dataShow
void CDataProcess::DisplayPic(CStatic *inDis)
{
	HWND hwnd = inDis->GetSafeHwnd();
	RECT rc;
	::GetWindowRect(hwnd, &rc);
	long lWidth = rc.right - rc.left;
	long lHeight = rc.bottom - rc.top;

	HDC hdc = ::GetDC(hwnd);
	PAINTSTRUCT ps;
	::BeginPaint(hwnd, &ps);

	SetStretchBltMode(hdc, COLORONCOLOR);
	StretchDIBits(
		hdc, 0, 0,
		lWidth, lHeight,
		0, 0, 320, 240,
		dataShow,
		(BITMAPINFO*)&bih,
		DIB_RGB_COLORS,
		SRCCOPY);

	::EndPaint(hwnd, &ps);
	::ReleaseDC(hwnd, hdc);
}

/*
typedef struct stWP_K_3D_Object
{
unsigned char srcRGB[COLOR_WIDTH*COLOR_HEIGHT * 3];  //RGB��ɫ���ݣ�1092*1080��
unsigned short depthD16[DEPTH_WIDTH*DEPTH_HEIGHT];	//���ֵ���ݣ�512*424��
int DepthToColorX[DEPTH_WIDTH*DEPTH_HEIGHT];		//�����������ӳ�䵽RGB��ɫ�����������Xֵ
int DepthToColorY[DEPTH_WIDTH*DEPTH_HEIGHT];		//�����������ӳ�䵽RGB��ɫ�����������Yֵ
tdPoint ColorToDepth[COLOR_WIDTH*COLOR_HEIGHT];		//RGB��ɫ���ݶ�Ӧ�������������

bool en[DEPTH_WIDTH][DEPTH_HEIGHT];			//������ά�������Ч��ǣ����Ϊtrueʱ��ʾ����ά�������Ч�����һ���OpenGL����ʾ
float x[DEPTH_WIDTH][DEPTH_HEIGHT];			//���ƶ�Ӧ����ά�����xֵ
float y[DEPTH_WIDTH][DEPTH_HEIGHT];			//���ƶ�Ӧ����ά�����yֵ
float z[DEPTH_WIDTH][DEPTH_HEIGHT];			//���ƶ�Ӧ����ά�����zֵ
unsigned char R[DEPTH_WIDTH][DEPTH_HEIGHT];	//��ά������Ӧ�Ĳ�ɫ���ݵĺ�ɫ����
unsigned char G[DEPTH_WIDTH][DEPTH_HEIGHT];	//��ά������Ӧ�Ĳ�ɫ���ݵ���ɫ����
unsigned char B[DEPTH_WIDTH][DEPTH_HEIGHT];	//��ά������Ӧ�Ĳ�ɫ���ݵ���ɫ����
}WP_K_3D_Object;
*/

/************************************************************************/
/*����δ����ת�任������                                                */
/************************************************************************/
void CDataProcess::ProcessSrc(stWP_K_3D_Object* in3DObj)
{
	//////////////////////////////////////////////
	//ʾ�����ı���������пؼ�
	/*CString strTmp;
	int a = 1;
	strTmp.Format(L"���ڴ���ԭʼ���� a= %d", a);
	PrintDis(strTmp);*/
	///////////////////////////////////////////////
}

static int n = 0;
/************************************************************************/
/*�����Ѿ���ת�任������                                                */
/************************************************************************/
void CDataProcess::ProcessTransfom(stWP_K_3D_Object* in3DObj)
{
	//////////////////////////////////////////////
	//ʾ�����ı�����������б�
	/*CString strTmp;
	strTmp.Format(L"���ڴ���任���� n= %d", n);
	PrintList(strTmp);*/

	//////////////////////////////////
	//ʾ���������������ϽǺ�ɫ�������ʾ
	/*if (n>=240)
	{
	n = 0;
	}
	for (int i = 0; i < 320 * 3 ;i++)
	{
	if (i%3 == 2)
	{
	dataShow[n * 320 * 3*2 + i] = 0xff;
	}
	}
	n++;*/
	//////////////////////////////////
	//ʾ������������3D��ʾ

	//��һЩ�е���ά���ɫ
	/*for (int y = 10; y < 150;y++)
	{
	for (int x = 0; x < 512;x++)
	{
	in3DObj->R[x][y] = 0;
	in3DObj->G[x][y] = 0xff;
	in3DObj->B[x][y] = 0;
	}
	}*/

	//��һЩ�е���ά�㲻��ʾ
	/*for (int y = 0; y < 424; y++)
	{
	for (int x = 100; x < 200; x++)
	{
	in3DObj->en[x][y] = false;
	}
	}*/
	/////////////////////////////////
}
