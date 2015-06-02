#include "stdafx.h"
#include "KinectDataStream.h"


CKinectDataStream::CKinectDataStream()
{
	m_ar3Dobj.resize(100);
	pInfoList = NULL;
}


CKinectDataStream::~CKinectDataStream()
{

}


void CKinectDataStream::BufOneFrame(stWP_K_3D_Object* in3DObj)
{
	m_ar3Dobj.push_back(*in3DObj);
}


void CKinectDataStream::SaveToBinFile(CString inFileName)
{
	if (m_ar3Dobj.size() > 0)
	{
		CFile fileWrite;		//��¼�ļ�
		BOOL fo_res;			//��¼�ļ����Ƿ�ɹ�

		//���ļ�
		fo_res = fileWrite.Open(inFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
		if (fo_res == TRUE)
		{
			//��ʼд������
			int nNum = m_ar3Dobj.size();

			//����д��ģ������
			for (int i = 0; i < nNum; i++)
			{
				fileWrite.Write(&(m_ar3Dobj[i]), sizeof(stWP_K_3D_Object));
			}

			fileWrite.Close();

			CString strTmp;
			strTmp.Format(L"������%d֡���ݵ�%s", nNum, inFileName);
			PrintInfo(strTmp);
		}
	}
}


void CKinectDataStream::PrintInfo(CString inStr)
{
	if (pInfoList != NULL)
	{
		pInfoList->AddString(inStr);
		pInfoList->SetCurSel(pInfoList->GetCount() - 1);
	}
}


void CKinectDataStream::SaveToDefault()
{
	CString m_strFile;
	CTime tm;
	tm = CTime::GetCurrentTime();
	m_strFile.Format(L"%d-%d-%d_%d-%d-%d.k2", tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
	m_strFile = L"../../_KinectStream/" + m_strFile;

	SaveToBinFile(m_strFile);
}

static WP_K_3D_Object tmpObj;
void CKinectDataStream::LoadFromBinFile(CString inFileName)
{
	PrintInfo(inFileName);
	CFile fileRead;		//��¼�ļ�
	BOOL fo_res;			//��¼�ļ����Ƿ�ɹ�

	//���ļ�
	fo_res = fileRead.Open(inFileName, CFile::modeRead);
	if (fo_res == TRUE)
	{
		m_ar3Dobj.clear();

		//��ʼ��ȡ����
		ULONGLONG nFileSize = fileRead.GetLength();
		int nNum = nFileSize / sizeof(tmpObj);

		CString str;
		str.Format(L"��ȡ%d������(%s)", nNum, inFileName);
		PrintInfo(str);

		//ѭ����ȡ
		for (int i = 0; i < nNum; i++)
		{
			fileRead.Read(&tmpObj, sizeof(WP_K_3D_Object));
			m_ar3Dobj.push_back(tmpObj);
		}

		fileRead.Close();
	}
}
