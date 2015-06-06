#include "stdafx.h"
#include "DataProcess.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/cv.h"  
#include "opencv/highgui.h"  
#include "CamConstantSet.h"
#include "abc.h"
#include "cv.h"
#include "highgui.h"
using namespace std;
using namespace cv;

static unsigned char dataShow[320*240*3];	//�������Ͻ�������ʾ������
static BITMAPINFOHEADER bih;
////variable from abc.h
unsigned int DepthBuf_O_T[DEPTH_VISION_CAM_HEIGHT][DEPTH_VISION_CAM_WIDTH];
unsigned int  area_grow_data_obj[DEPTH_VISION_CAM_HEIGHT*DEPTH_VISION_CAM_WIDTH][3];
int connect_area_s_e_w[100000][3];//[100000][3];//[DEPTH_VISION_CAM_HEIGHT*DEPTH_VISION_CAM_WIDTH][3];//�洢1000����ͨ��� �� β ��� [��]connect_area_s_e[n][0] [β]connect_area_s_e[n][1] [����]connect_area_s_e[n][2]
int Num_track_L;//�洢��ǰ��������ͨ��ĸ���
int Z_min_diff_set = 100;	  //��ͨ��������Ȳ�����Զ�Ķ�����Ϊ�ǲ�ͬ�����壩							
IplImage *pOut01;
IplImage *pOut02;

int DepthBuf_O[480][640];//ԭ���ͼ
int DepthBuf_O_msy[480][640];//����������ͼ
IplImage *pCannyImg1;
unsigned short *tempp;
long int xx = 0;
long int yy = 0;
long int msy = 0;
long int center_msy;//abc.h�з��ص�center,��ʾ��x,y)�ǵ��Ӧ�ھ������λ��
//const openni::DepthPixel* pDepth;
IplImage *showxy_msy;
IplImage *showxz_msy;

///////////////


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
	///////creat windows
	//cvNamedWindow("win02", 0);

	//cvNamedWindow("pOut02", 0);
	//cvNamedWindow("pOut01", 0);
	CvSize size; //=  cvGetSize(BGR);//�õ�����ͷͼ���С
	size.height = 480;
	size.width = 640;
	pOut01 = cvCreateImage(size, 8, 3);
	pOut02 = cvCreateImage(size, 8, 3);
	//pCannyImg1 = cvCreateImage(size, 8, 3);
	char key = 0;
	//showxy_msy = cvCreateImage(size, 8, 3);
	//showxz_msy = cvCreateImage(size, 8, 3);
	

		int i = 0, j = 0;
		long int temp = 0;
		//tempp = (unsigned short*)in3DObj->depthD16[DEPTH_WIDTH*DEPTH_HEIGHT];

		for (i = 0; i < 480; i++)
		{
			for (j = 0; j<640; j++)
			{
				if (i>0 && i<424 && j>0 && j < 512)
				{
					//temp = 512 * i + j;
					DepthBuf_O[i][j] = in3DObj->depthD16[temp];//5 >> 3;//ԭʼ��ȴ���DepthBuf_O������
					DepthBuf_O_msy[i][j] = in3DObj->depthD16[temp];//5 >> 3;//ԭʼ��ȴ���DepthBuf_O������
					temp = temp + 1;
				}

				else
				{
					DepthBuf_O[i][j] = 0;
					DepthBuf_O_msy[i][j] = 0;
				}

			}
		}

		bool_max_connectivity_analyze2_1_OBJ();
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
	//if (n>=240)
	//{
	//n = 0;
	//}
	//for (int i = 0; i < 320 * 3 ;i++)
	//{
	//if (i%3 == 2)
	//{
	//dataShow[n * 320 * 3*2 + i] = 0xff;
	//}
	//}
	//n++;
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
