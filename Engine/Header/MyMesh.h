#pragma once

#include "Component.h"

namespace Engine {
	class CCubeCol;
	class CMyLocalInfo;
}

BEGIN(Engine)

class ENGINE_DLL CMyMesh final
	:public CComponent
{
public:
	enum BUFFER_TYPE { TYPE_CUBE_COL, TYPE_CUBE_TEX, TYPE_END };
private:
	explicit CMyMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CMyMesh(const CMyMesh& Ref);
	virtual ~CMyMesh(void) = default;

private:
	CCubeCol*									m_pCom_CubeCol = nullptr;
	map<const My_TChar*, CMyLocalInfo*>			m_map_pCubeColInfo;

public:
	HRESULT				Initialize(CCubeCol* pCubeCol);
	void				Render(const My_Matrix* pMatWorld);
public:
	HRESULT				Refresh_ALL(void);
	HRESULT				Refresh_Single(BUFFER_TYPE eType, const My_TChar* szKey);
private:
	HRESULT				Refresh_CubeCol(void);

public:
	HRESULT				Add_CubeCol(const My_TChar* szKey, D3DCOLOR dwColor);
	HRESULT				Delete_CubeCol(const My_TChar* szKey);

public:
	HRESULT				Move_Local_Forward(BUFFER_TYPE eType, const My_TChar* szKey, const My_Float& fSpd, const My_Float& fTime);
	HRESULT				Move_Local_Backward(BUFFER_TYPE eType, const My_TChar* szKey, const My_Float& fSpd, const My_Float& fTime);
	HRESULT				Move_Local_Right(BUFFER_TYPE eType, const My_TChar* szKey, const My_Float& fSpd, const My_Float& fTime);
	HRESULT				Move_Local_Left(BUFFER_TYPE eType, const My_TChar* szKey, const My_Float& fSpd, const My_Float& fTime);
	HRESULT				Move_Local_Up(BUFFER_TYPE eType, const My_TChar* szKey, const My_Float& fSpd, const My_Float& fTime);
	HRESULT				Move_Local_Down(BUFFER_TYPE eType, const My_TChar* szKey, const My_Float& fSpd, const My_Float& fTime);
	
	HRESULT				Rotation_Local_X(BUFFER_TYPE eType, const My_TChar* szKey, const My_Float& fRad, const My_Float& fTime);
	HRESULT				Rotation_Local_Y(BUFFER_TYPE eType, const My_TChar* szKey, const My_Float& fRad, const My_Float& fTime);
	HRESULT				Rotation_Local_Z(BUFFER_TYPE eType, const My_TChar* szKey, const My_Float& fRad, const My_Float& fTime);

	HRESULT				Scale_Local_X(BUFFER_TYPE eType, const My_TChar* szKey, const My_Float& fSpd, const My_Float& fTime);
	HRESULT				Scale_Local_Y(BUFFER_TYPE eType, const My_TChar* szKey, const My_Float& fSpd, const My_Float& fTime);
	HRESULT				Scale_Local_Z(BUFFER_TYPE eType, const My_TChar* szKey, const My_Float& fSpd, const My_Float& fTime);
public:
	HRESULT				Move_Local_Forward_All	(const My_Float& fSpd, const My_Float& fTime);
	HRESULT				Move_Local_Backward_All	(const My_Float& fSpd, const My_Float& fTime);
	HRESULT				Move_Local_Right_All	(const My_Float& fSpd, const My_Float& fTime);
	HRESULT				Move_Local_Left_All		(const My_Float& fSpd, const My_Float& fTime);
	HRESULT				Move_Local_Up_All		(const My_Float& fSpd, const My_Float& fTime);
	HRESULT				Move_Local_Down_All		(const My_Float& fSpd, const My_Float& fTime);

	HRESULT				Rotation_Local_X_All	(const My_Float& fRad, const My_Float& fTime);
	HRESULT				Rotation_Local_Y_All	(const My_Float& fRad, const My_Float& fTime);
	HRESULT				Rotation_Local_Z_All	(const My_Float& fRad, const My_Float& fTime);

	HRESULT				Scale_Local_X_All		(const My_Float& fSpd, const My_Float& fTime);
	HRESULT				Scale_Local_Y_All		(const My_Float& fSpd, const My_Float& fTime);
	HRESULT				Scale_Local_Z_All		(const My_Float& fSpd, const My_Float& fTime);
public:
	HRESULT				Set_Position(BUFFER_TYPE eType, const My_TChar* szKey, const My_Vec3& vPos);
	HRESULT				Set_Rotation(BUFFER_TYPE eType, const My_TChar* szKey, const My_Vec3& vRotation);
	HRESULT				Set_Scale(BUFFER_TYPE eType, const My_TChar* szKey, const My_Vec3& vScale);
public:
	HRESULT				Set_Position_All(const My_Vec3& vPos);
	HRESULT				Set_Rotation_All(const My_Vec3& vRotation);
	HRESULT				Set_Scale_All(const My_Vec3& vScale);

private:
	My_Bool												CheckData(BUFFER_TYPE eType, const My_TChar* szKey);
	CMyLocalInfo*										Find_LocalInfo(BUFFER_TYPE eType, const My_TChar* szKey);
	map<const My_TChar*, CMyLocalInfo*>::iterator		Find_Iter(BUFFER_TYPE eType, const My_TChar* szKey);
	map<const My_TChar*, CMyLocalInfo*>*				Get_Container(BUFFER_TYPE eType);

public:
	static CMyMesh*				Create(LPDIRECT3DDEVICE9 pDevice, CCubeCol* pCubeCol);
	virtual CComponent*			Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

END