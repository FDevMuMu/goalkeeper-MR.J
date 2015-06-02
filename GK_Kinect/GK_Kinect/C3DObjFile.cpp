#include "StdAfx.h"
#include "C3DObjFile.h"

#define WP_K_3DFILE_HEADER		0x5A
#define WP_K_DEPTHFILE_HEADER	0x5B

typedef struct stWP3dFileHeader
{
	UINT header;
	int num;
}WP3dFileHeader;

C3DObjFile::C3DObjFile(void)
{

}


C3DObjFile::~C3DObjFile(void)
{
}


void C3DObjFile::Save3DObjFile(CString inFileName, WP_K_3D_Object* inp3DObj,int inNum)
{
	if (inp3DObj != NULL)
	{
		CFile fileWrite;		//��¼�ļ�
		BOOL fo_res;			//��¼�ļ����Ƿ�ɹ�

		//���ļ�
		fo_res = fileWrite.Open(inFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
		if (fo_res == TRUE)
		{
			//��ʼд������
			WP3dFileHeader fileHeader;
			fileHeader.header = WP_K_3DFILE_HEADER;
			fileHeader.num = inNum;
			//д���ļ�ͷ
			fileWrite.Write(&fileHeader,sizeof(fileHeader));

			//����д��ģ������
			for (int i=0;i<inNum;i++)
			{
				fileWrite.Write(&(inp3DObj[i]),sizeof(WP_K_3D_Object));
			}

			fileWrite.Close();
		} 
	}
}


int C3DObjFile::Load3DObjFile(CString inFileName, WP_K_3D_Object* inArObjects, int inMax)
{
	int num =0;
	if (inArObjects != NULL)
	{
		CFile fileRead;		//��¼�ļ�
		BOOL fo_res;			//��¼�ļ����Ƿ�ɹ�

		//���ļ�
		fo_res = fileRead.Open(inFileName,CFile::modeRead);
		if (fo_res == TRUE)
		{
			//��ʼ��ȡ����
			WP3dFileHeader fileHeader;
			
			fileRead.Read(&fileHeader,sizeof(fileHeader));

			if (fileHeader.header != WP_K_3DFILE_HEADER)
			{
				AfxMessageBox(L"����3D�ļ�ʧ�ܣ��ļ�ͷ��Ϣ����ȷ��");
				fileRead.Close();
				return 0;
			}
			if (inMax >= fileHeader.num)
			{
				num = fileHeader.num;
			} 
			else
			{
				num = inMax;
			}

			//һ���Զ�������
			fileRead.Read(inArObjects,num*sizeof(WP_K_3D_Object));

			fileRead.Close();
		} 
	}
	return num;
}


