#pragma once

#include <gl\gl.h> 
#include <gl\glu.h> 
#include "Structs.h"
#include "DepthTo3D.h"

// COpenGLStatic

class COpenGLStatic : public CStatic
{
	DECLARE_DYNAMIC(COpenGLStatic)

public:
	COpenGLStatic();
	virtual ~COpenGLStatic();

	void InitGL(void);
	void GlDrawLine(float x1,float y1,float z1,float x2,float y2,float z2);
	void GlRotatef(float angle, char axes);
	void GlDrawSolidCube(float x,float y,float z,float inSize);	//�����������
	void GlDrawStandCubiodLines(float x,float y,float z,float inXWidth, float inYWidth, float inZWidth);	//�������߿�
	void SetupLights(void);
	void GlDrawAxe(void);
	void Init2(void);
	BOOL SetupPixelFormat(void);
	void PrintInfo(CString inInfo);

	float m_fK;		//3D��ʾ�����ű���
	CDepthTo3D* pDepthTo3D;

	HGLRC m_hRC; //Rendering Context
	CDC* m_pDC; //Device Context

	CListBox* pInfoList;
	bool m_bLBtnDown;
	bool m_bRBtnDown;

	float m_fBaseRotY;
	float m_fBaseRotX;
	float m_fBaseDist;
	float m_fTempRotY;
	float m_fTempRotX;
	float m_fTempRotZ;
	float m_fTempDist;

	float m_fX_Fix;
	float m_fY_Fix;
	float m_fZ_Fix;

	CPoint lastLPnt;
	CPoint lastRPnt;

	//���λ�ú���Ұ
	trPoint m_CameraPos;
	trPoint m_arViewRect[4];

	//��ǵ������
	bool m_arEnMarkPoint[10];
	trPoint* m_arMarkPoint;

	GLUquadricObj *quadratic;
	bool m_arEnMarkCylinder[10];
	trPoint* m_arMarkCylinder;

	//����ʶ�����ʾ
	trPoint* m_posFace3D;
	int m_nFaceNum;

	//������ʾ


	float m_fMarkK;

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	void GLDrawCamView(void);
	void SetCamPos(float inHeight, float inPitch, float inRotate);
	void MarkPoint(int inIndex, int inX, int inY, int inZ);
	void CleanMarkPoints(void);
	void MarkCylinder(int inIndex, int inX, int inY, int inZ);
	void CleanMarkCylinder(void);
	UINT isFaceIn(int inCubiodIndex);
	void GlSelectFont(int size, int charset, const char* face);
	void GlDrawTxt(CString inStr);
	void m_DrawLine(int inSKIndex, int inFrom, int inTo);
};


