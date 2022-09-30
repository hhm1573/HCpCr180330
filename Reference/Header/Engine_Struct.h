#pragma once

namespace Engine
{
	typedef struct tagProjection_Desc
	{
		float			fFovy; // ī�޶��� �þ߰���.
		float			fAspect; // �� ������� ��Ⱦ��
		float			fNear; // �����
		float			fFar; // �����

		tagProjection_Desc()
		{

		}

		tagProjection_Desc(float _fFovy, float _fAspect, float _fNear, float _fFar)
			: fFovy(_fFovy), fAspect(_fAspect), fNear(_fNear), fFar(_fFar)
		{

		}

	}PROJ_DESC;

	typedef struct tagCamera_Desc
	{
		D3DXVECTOR3		vEye; // ī�޶��� ��ġ
		D3DXVECTOR3		vAt; // ī�޶� �ٶ󺸰� �ִ� ��.
		D3DXVECTOR3		vAxisY; // ��ǥ����� y�຤��

		tagCamera_Desc()
		{

		}

		tagCamera_Desc(const D3DXVECTOR3& _vEye, const D3DXVECTOR3& _vAt, const D3DXVECTOR3& _vAxisY)
			: vEye(_vEye), vAt(_vAt), vAxisY(_vAxisY)
		{

		}
	}CAMERA_DESC;

	typedef struct tagVertex_Color
	{
		D3DXVECTOR3		vPosition; // ��ġ
		unsigned long	dwColor; // ��
	}VTXCOL;
	const unsigned long		VTXFVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef struct tagVertex_NormalColor {
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		unsigned long	dwColor;
	}VTXNORCOL;
	const unsigned long		VTXFVF_NORCOL = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;

	typedef struct tagVertex_Texture
	{
		D3DXVECTOR3		vPosition; // ��ġ
		D3DXVECTOR2		vTexUV;
	}VTXTEX;

	typedef struct tagVertex_NormalTexture {
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR2		vUV;
	}VTXNORTEX;
	const unsigned long		VTXFVF_NORTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertex_ViewPortTexture {
		D3DXVECTOR4		vPosition;
		D3DXVECTOR2		vTexUV;
	}VTXVIEWPORTTEX;
	const unsigned long		VTXFVF_VIEWPORTTEX = D3DFVF_XYZRHW | D3DFVF_TEX1;

	typedef struct tagVertex_ViewPortColor {
		D3DXVECTOR4		vPosition;
		unsigned long		dwColor; // ��
	}VTXVIEWPORTCOL;
	const unsigned long		VTXFVF_VIEWPORTCOL = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

	typedef struct tagVertex_CubeTexture
	{
		D3DXVECTOR3		vPosition; // ��ġ
		D3DXVECTOR3		vTexUV;
	}VTXCUBETEX;

	typedef struct tagPolygon16
	{
		unsigned short		_1, _2, _3;
	}POLYGON16;

	typedef struct tagPolygon32
	{
		unsigned long		_1, _2, _3;
	}POLYGON32;
	


	typedef struct tagModelInfo {
		bool			bIsTexture;
		D3DXMATRIX		matLocal;
		D3DXCOLOR		dwColor;
		TCHAR			szTexture[256];
		TCHAR			szName[256];
	}INFO_MODEL;

	typedef struct tagMapInfo {
		unsigned int	ID;
		unsigned int	SubID;
		D3DXMATRIX		matWorld;
	}INFO_MAP;

	typedef struct tagNavMesh {
		D3DXVECTOR3		vLT;
		D3DXVECTOR3		vRT;
		D3DXVECTOR3		vLB;
		D3DXVECTOR3		vRB;
		tagNavMesh*		pL;
		tagNavMesh*		pR;
		tagNavMesh*		pU;
		tagNavMesh*		pB;
		unsigned short	wState;
	}NAVMESH;

	typedef struct tagCollisionSphere {
		int*			piReceive;
		int				iTransmit;
		D3DXVECTOR3		vCenter;
		float			fRadius;
		float			fCol_CenterDist;
	}COL_SPHERE;

	typedef struct tagCollisionAABB {
		int*			piReceive;
		int				iTransmit;
		D3DXVECTOR3		vMin;
		D3DXVECTOR3		vMax;
		D3DXVECTOR3*	pvIntersect;
	}COL_AABB;
}