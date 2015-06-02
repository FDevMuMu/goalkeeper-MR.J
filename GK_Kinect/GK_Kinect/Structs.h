#if !defined(HOME_STRUCTS_INCLUDED)
#define HOME_STRUCTS_INCLUDED

#include <vector>
using namespace std;

#define DEPTH_WIDTH		512
#define DEPTH_HEIGHT	424

#define COLOR_WIDTH		1920
#define COLOR_HEIGHT	1080

typedef struct st_2DPoint
{
	long x;
	long y;
}tdPoint;

typedef struct st_trPoint
{
	float x;
	float y;
	float z;
}trPoint;

typedef struct st_trVector
{
	double x;
	double y;
	double z;
}trVector;

//�Զ���3D���ݸ�ʽ
typedef struct stWP_K_3D_Object
{
	unsigned char srcRGB[COLOR_WIDTH*COLOR_HEIGHT * 3];
	unsigned short depthD16[DEPTH_WIDTH*DEPTH_HEIGHT];
	int DepthToColorX[DEPTH_WIDTH*DEPTH_HEIGHT];
	int DepthToColorY[DEPTH_WIDTH*DEPTH_HEIGHT];
	tdPoint ColorToDepth[COLOR_WIDTH*COLOR_HEIGHT];

	bool en[DEPTH_WIDTH][DEPTH_HEIGHT];
	float x[DEPTH_WIDTH][DEPTH_HEIGHT];
	float y[DEPTH_WIDTH][DEPTH_HEIGHT];
	float z[DEPTH_WIDTH][DEPTH_HEIGHT];
	unsigned char R[DEPTH_WIDTH][DEPTH_HEIGHT];
	unsigned char G[DEPTH_WIDTH][DEPTH_HEIGHT];
	unsigned char B[DEPTH_WIDTH][DEPTH_HEIGHT];
}WP_K_3D_Object;

//��������
typedef struct trObject
{
	float nCenterX;	//���ĵ�����
	float nCenterY;
	float nCenterZ;

	float nWidthX;	//�ߴ�
	float nWidthY;
	float nWidthZ;

	int nBlobIndex;	//��Blob�����ж�Ӧ��index
	int nPointNum;	//����

	float nMaxX;	//����X���ֵ
	float nMinX;	//����X��Сֵ
	float nMaxY;	//����Y���ֵ
	float nMinY;	//����Y��Сֵ
	float nMaxZ;	//����Z���ֵ
	float nMinZ;	//����Z��Сֵ

	tdPoint pntDepthLeft;
	tdPoint pntDepthRight;
	tdPoint pntDepthTop;
	tdPoint pntDepthBottom;

	bool bEn;	//���

	string strObjectName;
}trObject;

#endif // !defined(HOME_STRUCTS_INCLUDED)