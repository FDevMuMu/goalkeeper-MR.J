#include "stdafx.h"
#include "DataProcess.h"

static unsigned char dataShow[320*240*3];	//界面右上角用来显示的数组
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

//显示数组dataShow
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
unsigned char srcRGB[COLOR_WIDTH*COLOR_HEIGHT * 3];  //RGB彩色数据（1092*1080）
unsigned short depthD16[DEPTH_WIDTH*DEPTH_HEIGHT];	//深度值数据（512*424）
int DepthToColorX[DEPTH_WIDTH*DEPTH_HEIGHT];		//深度数据坐标映射到RGB彩色数据里的坐标X值
int DepthToColorY[DEPTH_WIDTH*DEPTH_HEIGHT];		//深度数据坐标映射到RGB彩色数据里的坐标Y值
tdPoint ColorToDepth[COLOR_WIDTH*COLOR_HEIGHT];		//RGB彩色数据对应的深度数据坐标

bool en[DEPTH_WIDTH][DEPTH_HEIGHT];			//点云三维坐标点有效标记，标记为true时表示该三维坐标点有效，而且会在OpenGL里显示
float x[DEPTH_WIDTH][DEPTH_HEIGHT];			//点云对应的三维坐标点x值
float y[DEPTH_WIDTH][DEPTH_HEIGHT];			//点云对应的三维坐标点y值
float z[DEPTH_WIDTH][DEPTH_HEIGHT];			//点云对应的三维坐标点z值
unsigned char R[DEPTH_WIDTH][DEPTH_HEIGHT];	//三维坐标点对应的彩色数据的红色分量
unsigned char G[DEPTH_WIDTH][DEPTH_HEIGHT];	//三维坐标点对应的彩色数据的绿色分量
unsigned char B[DEPTH_WIDTH][DEPTH_HEIGHT];	//三维坐标点对应的彩色数据的蓝色分量
}WP_K_3D_Object;
*/

/************************************************************************/
/*处理未曾旋转变换的数据                                                */
/************************************************************************/
void CDataProcess::ProcessSrc(stWP_K_3D_Object* in3DObj)
{
	//////////////////////////////////////////////
	//示例：文本输出到单行控件
	/*CString strTmp;
	int a = 1;
	strTmp.Format(L"正在处理原始数据 a= %d", a);
	PrintDis(strTmp);*/
	///////////////////////////////////////////////
}

static int n = 0;
/************************************************************************/
/*处理已经旋转变换的数据                                                */
/************************************************************************/
void CDataProcess::ProcessTransfom(stWP_K_3D_Object* in3DObj)
{
	//////////////////////////////////////////////
	//示例：文本输出到多行列表
	/*CString strTmp;
	strTmp.Format(L"正在处理变换数据 n= %d", n);
	PrintList(strTmp);*/

	//////////////////////////////////
	//示例：操作界面右上角黑色区域的显示
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
	//示例：操作最后的3D显示

	//让一些行的三维点变色
	/*for (int y = 10; y < 150;y++)
	{
	for (int x = 0; x < 512;x++)
	{
	in3DObj->R[x][y] = 0;
	in3DObj->G[x][y] = 0xff;
	in3DObj->B[x][y] = 0;
	}
	}*/

	//让一些列的三维点不显示
	/*for (int y = 0; y < 424; y++)
	{
	for (int x = 100; x < 200; x++)
	{
	in3DObj->en[x][y] = false;
	}
	}*/
	/////////////////////////////////
}
