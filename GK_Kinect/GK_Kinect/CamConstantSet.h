//CamConstantSet.h
//ע�⣺���˶�����Ŀ���л�ʱ ԭ���Ļ������ٶȼ�¼ �Ȼ��ڱ��� ��������˶�������
//#include "stdafx.h"

#ifndef CamConstantSet_h
#define CamConstantSet_h

//��������һЩ ������	21:39 2008-11-3
const int OMNI_VISION_CAM_WIDTH = 640;//320;//ȫ���ֱ���x
const int OMNI_VISION_CAM_HEIGHT= 480;//240;//ȫ���ֱ���y
const int MIRROR_CENTER_X		= OMNI_VISION_CAM_WIDTH/2;//��������x
const int MIRROR_CENTER_Y		= OMNI_VISION_CAM_HEIGHT/2;//��������y
const int MIRROR_RADIUS			= OMNI_VISION_CAM_HEIGHT/2;//���Ӱ뾶r
const int MIRROR_LIMIT_L		= MIRROR_CENTER_X-MIRROR_RADIUS;//������߽�	�Զ�������ĳ���
const int MIRROR_LIMIT_R		= MIRROR_CENTER_X+MIRROR_RADIUS;//������߽�	�Զ�������ĳ���
const int MIRROR_LIMIT_UP		= MIRROR_CENTER_Y+MIRROR_RADIUS;//�����ϱ߽�	�Զ�������ĳ���
const int MIRROR_LIMIT_DOWN		= MIRROR_CENTER_Y-MIRROR_RADIUS;//�����±߽�	�Զ�������ĳ���

const int circle_mask_radius_min =0;//20//0 //����ȫ����Ĥ �뾶��С����		//Ӱ����� HSVת�� ��ֵ�� ��ͨ��
const int circle_mask_radius_max =240;//176;//90  //����ȫ����Ĥ �뾶��󳤶�

//��Ȳ�������� ������
const int DEPTH_VISION_CAM_WIDTH = 640;//320;//ȫ���ֱ���x
const int DEPTH_VISION_CAM_HEIGHT= 480;//240;//ȫ���ֱ���y
const int DEPTH_VISION_CENTER_X		= DEPTH_VISION_CAM_WIDTH/2;//��Ұ����x
const int DEPTH_VISION_CENTER_Y		= DEPTH_VISION_CAM_HEIGHT/2;//��Ұ����y
//����ԭ���������ͷ ƽ����ccd y������Ϊy  ƽ����ccd x������Ϊx  ��ֱ��ccd (ƽ���ڹ���)������(��ǰ)Ϊz   ע:����ο�Ϊƽ���ڹ��ῴ��������� ����ͬ���ڹ���
const float DEPTH_VISION_Z_DATA2METER_P	    = 1.0;//��λ ����mm   z�����ݱ�ɺ����Ƴ˵ı���ϵ�� z_meter = z_data * DEPTH_VISION_Z_DATA2METER_P
//x_meter = z_data * DEPTH_VISION_ZX_DATA2METER_P  *(x_���� - DEPTH_VISION_CENTER_X)
const float DEPTH_VISION_ZX_DATA2METER_P	= 0.001512;//float(1.0/320.0);// x�����ݱ�ɺ����Ƴ˵ı���ϵ��   �궨���� z=1240mm ��ƽ��x��ֱ���� ������320���� ��������600mm   600=1240*320*0.001512
//y_meter = y_data * DEPTH_VISION_ZX_DATA2METER_P  *(x_���� - DEPTH_VISION_CENTER_X)
const float DEPTH_VISION_ZY_DATA2METER_P	= 0.001512;//float(1.0/240.0);// y�����ݱ�ɺ����Ƴ˵ı���ϵ�� 

#endif CamConstantSet_h