#include <string>
#include <stdlib.h>  
#include <iostream> 

//��1��  
//#include <XnCppWrapper.h>  
#include "opencv/cv.h"  
#include "opencv/highgui.h"  
#include "CamConstantSet.h"

using namespace std;

extern  unsigned char dataShow[320 * 240 * 3];	//�������Ͻ�������ʾ������
extern unsigned int DepthBuf_O_T[DEPTH_VISION_CAM_HEIGHT][DEPTH_VISION_CAM_WIDTH];
extern unsigned int  area_grow_data_obj[DEPTH_VISION_CAM_HEIGHT*DEPTH_VISION_CAM_WIDTH][3];
extern int connect_area_s_e_w[100000][3];//[100000][3];//[DEPTH_VISION_CAM_HEIGHT*DEPTH_VISION_CAM_WIDTH][3];//�洢1000����ͨ��� �� β ��� [��]connect_area_s_e[n][0] [β]connect_area_s_e[n][1] [����]connect_area_s_e[n][2]
extern int Num_track_L;//�洢��ǰ��������ͨ��ĸ���
extern int Z_min_diff_set;	  //�˲��������弰��Χ����������������������							
extern IplImage *pOut01;
extern IplImage *pOut02;
extern int DepthBuf_O[480][640];//ԭ���ͼ
extern int DepthBuf_O_msy[480][640];//���Բ��������ͼ
extern IplImage* pCannyImg1;
float AXc = 0, AYc = 0, Xc = 0, Yc = 0, weight_msy= 0;//////ÿ��X��˵������ĳ˻�֮�ͣ�ÿ��Y��˵������ĳ˻�֮�ͣ�����X��λ�ã�����Y��λ��,������
extern long int center_msy;//abc.h�з��ص�center,��ʾ��x,y)�ǵ��Ӧ�ھ������λ��
//extern const openni::DepthPixel* pDepth;
char str[50];
/////////////////////////////////MSY TEST////////////////////////////
extern IplImage *showxy_msy;
extern IplImage *showxz_msy;
void DrawPoint(int x, int y, unsigned char R, unsigned char G, unsigned char B, IplImage *img)
{
	if (x>img->width)		x = img->width;
	if (x<0)				x = 0;
	if (y>img->height)		y = img->height;
	if (y<0)				y = 0;
	char *	data = img->imageData + (img->width*y + x) * 3;
	*data++ = B;
	*data++ = G;
	*data++ = R;
}


float ball_move_distance(float x1, float y1, float z1, float x2, float y2, float z2)
{
	float distance;
	distance = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2));
	return distance;
}

int ball_move_judge(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
	float distance1, distance2;
	distance1 = ball_move_distance(x1, y1, z1, x2, y2, z2);
	distance2 = ball_move_distance(x2, y2, z2, x3, y3, z3);
	if (distance1>50 && distance1<500 && distance2>50 && distance2<500)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


float least_square_method(float *posX, float *posY, float *posW, int num)
{
	double Exy = 0.0;
	double Ex = 0.0;
	double Ey = 0.0;
	double Ex2 = 0.0;
	double totalnum = 0.0;
	double a, b;//ax+by+c=0
	double k;
	if (num <= 1)
	{
		return(0);
	}
	for (int ct = 0; ct<num; ct++)
	{
		Exy = Exy + posX[ct] * posY[ct] * posW[ct];
		Ex = Ex + posX[ct] * posW[ct];
		Ey = Ey + posY[ct] * posW[ct];
		Ex2 = Ex2 + posX[ct] * posX[ct] * posW[ct];
		totalnum = totalnum + posW[ct];
	}
	a = totalnum*Exy - Ex*Ey;
	b = totalnum*Ex2 - Ex*Ex;
	//	k=-a/b;
	return (-atan2(a, b));  //����hu��ֵ
	//	return (-atan2(a,b)*180.0/3.1415926);  //���ؽǶ�ֵ
	//	return k;  //���ؽǶ�ֵ
}


void clean(IplImage *img)
{
	char *	data = img->imageData;
	for (int i = 0; i<img->width*img->height * 3; i++)
		*data++ = 0;

}





long int bool_max_connectivity_analyze2_1_OBJ()
{
	long int center = 0;

	register unsigned long int  boolset = 3000;//��ֵ

	register unsigned long int  area_grow_data_arel = 0;//��¼currently��ǰ��һȦ�м�������


	int Z_min_diff = Z_min_diff_set;//50;	//����һ���ɵ�ֵ��ϵ����ȵķֱ���Գ̶�
	const char X_step = 2;		//���ص�Ĳ��� ����ɨ�� ���ڼ���������
	const char Y_step = 2;		//���ص�Ĳ��� ����ɨ�� ���ڼ���������


	const int x320 = DEPTH_VISION_CAM_WIDTH;//320;//320;//����ͼ�����ݵĴ�С����  xֵ�����ԭ������ͬ������� �д��� 
	const int y240 = DEPTH_VISION_CAM_HEIGHT;//240;//240;//����ͼ�����ݵĴ�С����
	const int datalong = DEPTH_VISION_CAM_WIDTH*DEPTH_VISION_CAM_HEIGHT;
	const int x320r1 = x320 - 1;//320;//����ͼ�����ݵĴ�С����  xֵ�����ԭ������ͬ������� �д��� 
	const int y240r1 = y240 - 1;//240;//����ͼ�����ݵĴ�С����

	register int i, j, k, l, x, y;//seedx,seedy,,used_num

	register int    x_add, x_reduce, y_add, y_reduce, xg, yg, zg, zt, maxW, maxN;



	register int connect_area_num = 1;//��¼�õ�����ͨ���	ע��:����ı�� 0��Ч�� 1δ���������Ч�� 2��ͨ��ĵ�һ����� 3 4 5....
	register int connect_max_area_num = 1;//�����ͨ��ı��	Ĭ����Чֵ1
	register int connect_area_w = 0;//��¼��ͨ������ֵ
	register int connect_max_area_w = 0;//��¼��ͨ���������ֵ

	register int connect_area_layer_l = 0;//�洢��ͨ�� ��� ���� ����
	register int connect_area_layer_s = 0;//�洢��ͨ�� ��� ��
	register int connect_area_layer_e = 0;//�洢��ͨ�� ��� β
	register int connect_area_layer_pl = 0;//�洢��ͨ�� ��� ���� ����
	register int connect_area_layer_ps = 0;//�洢��ͨ�� �ϲ�� ��
	register int connect_area_layer_pe = 0;//�洢��ͨ�� �ϲ�� β



	Num_track_L = 0;//�洢��ǰ��������ͨ��ĸ�������

	////////////////////////////////////
	//�ڱߴ���  ��ֹ���ֱ�Եð��//////       
	for (x = 0; x<x320; x++)
	{
		DepthBuf_O[0][x] = 0;
		DepthBuf_O[1][x] = 0;
		DepthBuf_O[y240r1][x] = 0;
		DepthBuf_O[y240r1 - 1][x] = 0;
	}
	for (y = 0; y<y240; y++)
	{
		DepthBuf_O[y][0] = 0;
		DepthBuf_O[y][1] = 0;
		DepthBuf_O[y][x320r1] = 0;
		DepthBuf_O[y][x320r1 - 1] = 0;
	}
	//�ڱߴ���  ��ֹ���ֱ�Եð��//////


	unsigned int *area_grow_data_are_PO = &area_grow_data_obj[0][0];
	area_grow_data_arel = 0;//������ͨ���¼ λ������
	//1�Զ���������������������
	//for(i=0;i<circle_mask_long;i++)
	//i=0;while(i<datalong)


	//�ֺŲ��� ���һ���� ���ٹ� һ���ھ�Ҫ��ʹ���ۼӼ�1 �ӵ�һ����ֵ�Ͳ�����   ����˺�û�е��˴˺ŵ�ʹ���ۼ����� ����ʱ��Խ����ΪԽ�ɿ�
	//��ǰ���ڲο������ڵķֺŽ�� �Ӹ���ʱ�䳤������ʼ�Ȳ���
#ifndef WIN01_MOUSE_SET
	for (y = 0; y<DEPTH_VISION_CAM_HEIGHT; y += Y_step)
	{
		for (x = 0; x<DEPTH_VISION_CAM_WIDTH; x += X_step)
		{
#endif
#ifdef WIN01_MOUSE_SET
			for (y = win01_mouse_set_Y; y<win01_mouse_set_Y + 1; y += Y_step)//�궨������
			{
				for (x = win01_mouse_set_X; x<win01_mouse_set_X + 1; x += X_step)//�궨������
				{
#endif
					//y=i/DEPTH_VISION_CAM_WIDTH;
					//x=i%DEPTH_VISION_CAM_WIDTH;
					if (DepthBuf_O[y][x]>0)//û������ ������Ч��
					{
						connect_area_num++;//��¼�õ�����ͨ���	ע��:����ı�� 0��Ч�� 1δ���������Ч�� 2��ͨ��ĵ�һ����� 3 4 5....
						connect_area_w = 0;//��¼��ͨ������ֵ	����
						connect_area_s_e_w[connect_area_num][0] = area_grow_data_arel;//�洢1000����ͨ��� �� β ��� [��]connect_area_s_e[n][0] [β]connect_area_s_e[n][1] [����]connect_area_s_e[n][2]
						//���������������������������������������������
						//1��������������������
						//			area_grow_data_arex[area_grow_data_arel]=x;//��ǰ��������һȦ�ļ�¼
						//			area_grow_data_arey[area_grow_data_arel++]=y;//��ǰ��������һȦ�ļ�¼
						*area_grow_data_are_PO++ = y;
						*area_grow_data_are_PO++ = x;
						*area_grow_data_are_PO++ = DepthBuf_O[y][x];
						area_grow_data_arel++;
						connect_area_layer_l = 1;
						connect_area_layer_pe = connect_area_layer_ps + 1;
						//    		area_grow_dataAtpx[0]=x;//seedx;////�ֶ���������
						//    		area_grow_dataAtpy[0]=y;//seedy;////�ֶ���������
						//    		area_grow_dataAtpl=1;////�ֶ���������
						connect_area_w++;//��ͨ������+1

						//��ʼ����
						while (1)//for (i=0;i<1500;i++)//������������ ����������
						{
							connect_area_layer_s = area_grow_data_arel;
							j = connect_area_layer_ps; while (j<connect_area_layer_pe)
							{   //��Χ����A�е���Ч���ҷ�����������
								//    				xg=area_grow_data_arex[j];//��ǰ��������һȦ�ļ�¼
								//    				yg=area_grow_data_arey[j];//��ǰ��������һȦ�ļ�¼
								zg = area_grow_data_obj[j][2];//��ǰ��������һȦ�ļ�¼
								xg = area_grow_data_obj[j][1];//��ǰ��������һȦ�ļ�¼
								yg = area_grow_data_obj[j][0];//��ǰ��������һȦ�ļ�¼
								DepthBuf_O_T[yg][xg] = connect_area_num;//һ��ԭ������ û������ȥ�� ��ʹ��ͨ���1����
								x_add = xg + X_step;
								x_reduce = xg - X_step;
								y_add = yg + Y_step;
								y_reduce = yg - Y_step;

								zt = DepthBuf_O[y_add][x_reduce];
								if ((abs(zt - zg)<Z_min_diff))//&&(zt>0)
								{
									*area_grow_data_are_PO++ = y_add;
									*area_grow_data_are_PO++ = x_reduce;
									*area_grow_data_are_PO++ = zt;
									area_grow_data_arel++;
									DepthBuf_O[y_add][x_reduce] = -connect_area_num;
								}
								zt = DepthBuf_O[y_add][xg];
								if ((abs(zt - zg)<Z_min_diff))//&&(zt>0)
								{
									*area_grow_data_are_PO++ = y_add;
									*area_grow_data_are_PO++ = xg;
									*area_grow_data_are_PO++ = zt;
									area_grow_data_arel++;
									DepthBuf_O[y_add][xg] = -connect_area_num;
								}
								zt = DepthBuf_O[y_add][x_add];
								if ((abs(zt - zg)<Z_min_diff))//&&(zt>0)
								{
									*area_grow_data_are_PO++ = y_add;
									*area_grow_data_are_PO++ = x_add;
									*area_grow_data_are_PO++ = zt;
									area_grow_data_arel++;
									DepthBuf_O[y_add][x_add] = -connect_area_num;
								}
								zt = DepthBuf_O[yg][x_reduce];
								if ((abs(zt - zg)<Z_min_diff))//&&(zt>0)
								{
									*area_grow_data_are_PO++ = yg;
									*area_grow_data_are_PO++ = x_reduce;
									*area_grow_data_are_PO++ = zt;
									area_grow_data_arel++;
									DepthBuf_O[yg][x_reduce] = -connect_area_num;
								}
								zt = DepthBuf_O[yg][x_add];
								if ((abs(zt - zg)<Z_min_diff))//&&(zt>0)
								{
									*area_grow_data_are_PO++ = yg;
									*area_grow_data_are_PO++ = x_add;
									*area_grow_data_are_PO++ = zt;
									area_grow_data_arel++;
									DepthBuf_O[yg][x_add] = -connect_area_num;
								}
								zt = DepthBuf_O[y_reduce][x_reduce];
								if ((abs(zt - zg)<Z_min_diff))//&&(zt>0)
								{
									*area_grow_data_are_PO++ = y_reduce;
									*area_grow_data_are_PO++ = x_reduce;
									*area_grow_data_are_PO++ = zt;
									area_grow_data_arel++;
									DepthBuf_O[y_reduce][x_reduce] = -connect_area_num;
								}
								zt = DepthBuf_O[y_reduce][xg];
								if ((abs(zt - zg)<Z_min_diff))//&&(zt>0)
								{
									*area_grow_data_are_PO++ = y_reduce;
									*area_grow_data_are_PO++ = xg;
									*area_grow_data_are_PO++ = zt;
									area_grow_data_arel++;
									DepthBuf_O[y_reduce][xg] = -connect_area_num;
								}
								zt = DepthBuf_O[y_reduce][x_add];
								if ((abs(zt - zg)<Z_min_diff))//&&(zt>0)
								{
									*area_grow_data_are_PO++ = y_reduce;
									*area_grow_data_are_PO++ = x_add;
									*area_grow_data_are_PO++ = zt;
									area_grow_data_arel++;
									DepthBuf_O[y_reduce][x_add] = -connect_area_num;////////////////////////////////////////////���������� �Ӳ���
								}
								j++;
							}
							connect_area_layer_e = area_grow_data_arel;
							connect_area_layer_l = connect_area_layer_e - connect_area_layer_s;//��ͨ������+��ǰһȦ�ĵ���
							connect_area_layer_ps = connect_area_layer_s;//�洢��ͨ�� �ϲ�� ��
							connect_area_layer_pe = connect_area_layer_e;//�洢��ͨ�� �ϲ�� β

							connect_area_w += connect_area_layer_l;
							if (connect_area_layer_l == 0)
								break;


						}


						//���������������������������������������������
						connect_area_s_e_w[connect_area_num][1] = area_grow_data_arel;//�洢1000����ͨ��� �� β ��� [��]connect_area_s_e[n][0] [β]connect_area_s_e[n][1] [����]connect_area_s_e[n][2]
						connect_area_s_e_w[connect_area_num][2] = connect_area_w;//�洢1000����ͨ��� �� β ��� [��]connect_area_s_e[n][0] [β]connect_area_s_e[n][1] [����]connect_area_s_e[n][2]
						if (connect_max_area_w<connect_area_w)
						{
							connect_max_area_w = connect_area_w;
							connect_max_area_num = connect_area_num;
						}



					}


				}
			}

			Num_track_L = Num_track_L;//


			int t;
			unsigned char R, G, B;



			if (1)//������ͨ�� �����ʾ
			{
				register int   min_x, max_x, min_y, max_y;
				i = 1;
				clean(pOut01);//��������ͼ
				clean(pOut02);
				for (j = 2; j <= connect_area_num; j++)//j ��ͨ��ĸ���
				{
					R = (i * 90 % 200) + 40;
					G = (i * 50 % 200) + 55;
					B = (i * 30 % 200) + 55;//����ͨ���������ɫ   		
					if (1)
					{
					//	if ((connect_area_s_e_w[j][1] - connect_area_s_e_w[j][0])> 100 && (connect_area_s_e_w[j][1] - connect_area_s_e_w[j][0])<3300)
						if ((connect_area_s_e_w[j][1] - connect_area_s_e_w[j][0])> 100 && (connect_area_s_e_w[j][1] - connect_area_s_e_w[j][0])<3300)
							//��ͨ���Сɸѡ   100<  <3300
						{	////////�˴������ɵ�(��ͨ)


							min_x = 640;
							max_x = 0;
							min_y = 480;
							max_y = 0;
							//////////////////////
							weight_msy = 0;
							AXc = 0;
							AYc = 0;
							/////////////////
							for (i = connect_area_s_e_w[j][0]; i<connect_area_s_e_w[j][1]; i++)
							{
								if (min_x>area_grow_data_obj[i][1])
									min_x = area_grow_data_obj[i][1];
								if (max_x<area_grow_data_obj[i][1])
									max_x = area_grow_data_obj[i][1];
								if (min_y>area_grow_data_obj[i][0])
									min_y = area_grow_data_obj[i][0];
								if (max_y<area_grow_data_obj[i][0])
									max_y = area_grow_data_obj[i][0];

/////////////////////////////////////////////////////////////////////////////////////////
								AXc = AXc + area_grow_data_obj[i][1] * DepthBuf_O_msy[area_grow_data_obj[i][0]][area_grow_data_obj[i][1]];
								AYc = AYc + area_grow_data_obj[i][0] * DepthBuf_O_msy[area_grow_data_obj[i][0]][area_grow_data_obj[i][1]];
								weight_msy = weight_msy + DepthBuf_O_msy[area_grow_data_obj[i][0]][area_grow_data_obj[i][1]];
/////////////////////////////////////////////////////////////////////////////

							}
							float P_xy, area, P_area;   //P_xyΪ�������
							if (abs(max_x - min_x)>abs(max_y - min_y))
								P_xy = (float)abs(max_y - min_y) / (float)abs(max_x - min_x);
							else
								P_xy = (float)abs(max_x - min_x) / (float)abs(max_y - min_y);
							//���� ͼ����� �� ��С����� ��
							area = abs((max_y - min_y)*(max_x - min_x));
							if (area != 0)
								P_area = connect_area_s_e_w[j][2] * X_step*Y_step / area;
							else
								P_area = 0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////
						
						
							
								
							Xc = AXc / weight_msy;
							Yc = AYc / weight_msy;
/////////////////////////////////////////////////////////////////////////////////////////////////
							//if ((((float)(weight_msy / (float)connect_area_s_e_w[j][2])*sqrt((connect_area_s_e_w[j][2] * X_step*Y_step)))>90000) && (((float)(weight_msy / (float)connect_area_s_e_w[j][2])*sqrt((connect_area_s_e_w[j][2] * X_step*Y_step)))<110000))

							if ((fabs(P_xy - 1)<1 * 0.15) && (fabs(P_area - 0.7854)<0.7854*0.15)&& (((float)(weight_msy / (float)connect_area_s_e_w[j][2])*sqrt((connect_area_s_e_w[j][2] * X_step*Y_step)))>90000) && (((float)(weight_msy / (float)connect_area_s_e_w[j][2])*sqrt((connect_area_s_e_w[j][2] * X_step*Y_step)))<110000))
							{	//Բ�ĳ����Ϊ1  Բ������������С���������Ϊ 2*r*2*r/Pi*r*r =4/pi 					
								//cout << "�����" << P_xy << " �����" << P_area << " ʵ����� " << connect_area_s_e_w[j][2] * X_step*Y_step << "���ĵ����" << DepthBuf_O_msy[(max_y + min_y)/2][(max_x + min_x) / 2] << endl;
/////////////////////////////////////////////////////���������������ȣ�����ȣ�ʵ����������ľ���////////////////////////////
							
								for (i = connect_area_s_e_w[j][0]; i < connect_area_s_e_w[j][1]; i++)
								{
									//DrawPoint(area_grow_data_obj[i][1], area_grow_data_obj[i][0], R, G, 0, pOut02);//SetColor(y,x,cPoint);//���Դ���
									dataShow[area_grow_data_obj[i][0] * 320 * 3 * 2 + area_grow_data_obj[i][1]] = 0xff;
									center = (max_x + min_x) / 2 + (max_y + min_y) * 320;
								}
///////////////////////////////������ȱ����ͼ��MSY///////////////////////////////////////////
//								CvFont font;
//								cvInitFont(&font, 5,
//									0.6f, 0.6f,
//									0, 1, CV_AA);
//								sprintf(str, "P_area %.3f ", P_area);
//								cvPutText(pOut01, str, cvPoint(min_x + 10, min_y + 10), &font, CV_RGB(255, 255, 255));
//								sprintf(str, "length-width ratio  %.3f  ", P_xy);
//								cvPutText(pOut01, str, cvPoint(min_x + 10, min_y + 30), &font, CV_RGB(255, 255, 255));
//								sprintf(str, "  Figure area  %d  ", connect_area_s_e_w[j][2] * X_step*Y_step);
//								cvPutText(pOut01, str, cvPoint(min_x + 10, min_y + 50), &font, CV_RGB(255, 255, 255));
//								sprintf(str, "  depth  %f  ", weight_msy / (float)connect_area_s_e_w[j][2]);// DepthBuf_O_msy[(int)Yc][(int)Xc]);
//								cvPutText(pOut01, str, cvPoint(min_x + 10, min_y + 70), &font, CV_RGB(255, 255, 255));
////								sprintf(str, "  end  %f  ", weight_msy / (float)connect_area_s_e_w[j][2] / (sqrt(connect_area_s_e_w[j][2] * X_step*Y_step)));// DepthBuf_O_msy[(int)Yc][(int)Xc]);
//								sprintf(str, "  end  %f  ", weight_msy / (float)((float)connect_area_s_e_w[j][2])* sqrt((connect_area_s_e_w[j][2] * X_step*Y_step)));// DepthBuf_O_msy[(int)Yc][(int)Xc]);
//								cvPutText(pOut01, str, cvPoint(min_x + 10, min_y + 90), &font, CV_RGB(255, 255, 255));
//////////////////////////////////////////////////////////////MSY TEST////////////////////////////
							/*	CvPoint   centerpointxy;
								centerpointxy.x = (int)Xc;
								centerpointxy.y = (int)Yc;
								cvCircle(showxy_msy, centerpointxy, 3, CV_RGB(0, 0, 0), 1, 8, 3);
								
								CvPoint   centerpointxz;
								centerpointxz.x = (int)Xc;
								centerpointxz.y = DepthBuf_O_msy[(int)Yc][(int)Xc] / 12;
								cvCircle(showxz_msy, centerpointxz ,3, CV_RGB(0, 0, 0), 1, 8, 3);*/


								//DrawPoint((int)Xc, (int)Yc, 0, 255, 255, showxy_msy);
								//cout << (max_y + min_y) / 2<<"     "<<(max_x + min_x) / 2 <<"     "<<Yc<<"       "<<Xc << endl;
								//cout  << " ʵ����� " << connect_area_s_e_w[j][2] * X_step*Y_step << "���ĵ����" << DepthBuf_O_msy[(int)Yc][(int)Xc] << endl;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								/*if ( ((weight_msy / (float)connect_area_s_e_w[j][2])*sqrt((connect_area_s_e_w[j][2] * X_step*Y_step)) >= 80000) && ((weight_msy / (float)connect_area_s_e_w[j][2])*sqrt((connect_area_s_e_w[j][2] * X_step*Y_step)) < 150000))
								{
									weight_msy / (float)((float)connect_area_s_e_w[j][2])*sqrt((connect_area_s_e_w[j][2] * X_step*Y_step)
								}*/
							}

						}
					}
				}
//				cvCvtColor(pOut02, pCannyImg1, CV_RGB2GRAY);
//				cvCanny(pCannyImg1, pCannyImg1, 50, 100, 3);////////@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@�˴������ɵ�
				//	cvShowImage("canny1", pCannyImg1);
#ifndef WIN01_MOUSE_SET
				//cvShowImage("win01", pOut01);
				//cvShowImage("win02", pOut02);//������������
				/*cvShowImage("msy_xy", showxy_msy);
			    cvShowImage("msy_xz", showxz_msy);*/
#endif
			}




			x = x;

			//�����ڰ�ͼ�����ĺ�����  obj_properties *All  All.x��������ĵ�x���� All.y��������ĵ�y����
			//��Ҫ�����ֵ���Ĥ����  int *yan_mo_x,int *yan_mo_y
			//��Ҫ�����ֵ���Ĥ���鳤��int yan_mo_long,
			register float OBJ_temp_b = 0;
			register float OBJ_temp_b_ = 0;
			register float OBJ_temp_c = 0;
			register float ball_op_x = 0;
			register float ball_op_y = 0;
			register float ball_op_weight = 0;
			if (connect_max_area_num>1)//�����ͨ��ı��	Ĭ����Чֵ1
			{
				for (i = connect_area_s_e_w[connect_max_area_num][0]; i<connect_area_s_e_w[connect_max_area_num][1]; i++)
				{//	SetColor(area_grow_data_arey[i],area_grow_data_arex[i],cPointB);
					OBJ_temp_c++;
					OBJ_temp_b += area_grow_data_obj[i][1];//x
					OBJ_temp_b_ += area_grow_data_obj[i][0];//y


				}
			}

			if (OBJ_temp_c != 0)  //���ճ���  ��ͬ������ʱ  ��ʱCapture����ͼ�� �ͻ�����������ض�Ϊ�� ��ʱ��OBJ_temp_b / OBJ_temp_c�ĳ����ͻ������ĸΪ��Ĵ���
			{
				ball_op_x = OBJ_temp_b / OBJ_temp_c;//(int)(OBJ_temp_b / OBJ_temp_c);  //����1+2+3+...+320
				ball_op_y = OBJ_temp_b_ / OBJ_temp_c;//(int)(OBJ_temp_b / OBJ_temp_c);  //����1+2+3+...+240
			}
			ball_op_weight = OBJ_temp_c;//׼�������������������߼�����
			ball_op_y = ball_op_y;
			ball_op_x = ball_op_x;
			ball_op_weight = ball_op_weight;

			//��ɶ�ֵ��ͼ������ĳ���///////////////////////////////////////////////////////////////////

			//�������� �ش���ͬ������
			//memset(DepthBuf_O, 0, DEPTH_VISION_CAM_HEIGHT*DEPTH_VISION_CAM_WIDTH*sizeof(int));





			
			if (0)// display_process_image == 1)//�ж� ��ʾ����ͼ��	//��ʾ�����ͨ��
			for (y = 0; y<DEPTH_VISION_CAM_HEIGHT; y++)
			for (x = 0; x<DEPTH_VISION_CAM_WIDTH; x++)
			{
				if (DepthBuf_O[y][x] == 1)
					DrawPoint(x, y, 0, 255, 255, pOut01);//SetColor(y,x,cPoint);//���Դ���
				else
					DrawPoint(x, y, 0, 0, 0, pOut01);//SetColor(y,x,cPoint);//���Դ���

			}

			//			cvShowImage("win01", pOut01);
			return center;

		}

		// area_grow_data_obj[��ǰ�Զ�������ͨ���][0 1 2]=y x z   trackobj[���������]= ��ǰ�Զ�������ͨ���
		// ��������� ������� һ������ֵ�һ���� ��ǰ��������� ���һ�������³��ľ��·ָ����ĺ� ���һ�������һ���㶼û���� ������ͷŲ�����������ĺ�ǰ��
		// �����֪ ������� ÿ���������� ��ǰ���Ƕ��� �����ں��Ƕ���



		/*void CheckOpenNIError(XnStatus result, string status)
		{
			if (result != XN_STATUS_OK)
				cerr << status << " Error: " << xnGetStatusString(result) << endl;
		}*/








