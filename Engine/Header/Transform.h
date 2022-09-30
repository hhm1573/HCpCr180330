#pragma once

#include "Component.h"

BEGIN(Engine)

class CVIBuffer;
class CVehicle;

class ENGINE_DLL CTransform
	:public CComponent
{
public:
	enum INFORMATION {
		INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POSITION
	};

private:
	explicit CTransform(LPDIRECT3DDEVICE9 pDevice);
	explicit CTransform(const CTransform& Ref);
	virtual ~CTransform() = default;

private:
	My_Matrix	m_matWorld;
	const NAVMESH*		m_pNavMesh = nullptr;
	My_Int		m_iTest = 0;

public:
	HRESULT		Initialize(void);
	void		SetUp_OnGraphicDev();

public:
	inline const My_Matrix*		Get_WorldMatrix()const { return &m_matWorld; }
	inline const My_Vec3*		Get_Information(INFORMATION eInfo) { return (My_Vec3*)&m_matWorld.m[eInfo][0]; }
	My_Vec3						Get_Scale(void);
	My_Float					Get_Height_OnBuffer(const CVIBuffer* pBuffer);
	My_Float					Get_Height_OnBuffer(const CVIBuffer* pBuffer, const My_Vec3& vPos);
	My_Float					Get_Height_OnNav();

public:
	void			Set_World(My_Matrix& world) {
		m_matWorld = world;
	}

public:
	void		Move_Forward(const My_Float& fSpdPerSec);
	void		Move_Backward(const My_Float& fSpdPerSec);
	void		Move_Right(const My_Float& fSpdPerSec);
	void		Move_Left(const My_Float& fSpdPerSec);
	void		Move_Up(const My_Float& fSpdPerSec);
	void		Move_Down(const My_Float& fSpdPerSec);
	void		Move_Position(const My_Vec3& vPos);	//Teleport to target position
	void		Move_Free(const My_Vec3* pvVelocity, const My_Float& fTimeDelta);

	My_Bool		Set_NavMesh(const NAVMESH* pNavMesh);
	My_Bool		Move_On_NavMesh(const My_Vec3* pvVelocity, const CVIBuffer* pBuffer, const My_Float& fTimeDelta, CVehicle* pVehicle, My_Bool*);

	void		Rotation_X(const My_Float& fRadPerSec);
	void		Rotation_Y(const My_Float& fRadPerSec);
	void		Rotation_Z(const My_Float& fRadPerSec);
	void		Rotation_Free(const My_Vec3& vAxis, const My_Float& fRadPerSec);
	void		RotationYawPitchRoll(const My_Float radianX, const My_Float radianY, const My_Float radianZ);

	void		Scaling(const My_Float& fX, const My_Float& fY, const My_Float& fZ);

	void		Set_HeightOnBuffer(const CVIBuffer* pBuffer);

	void		Set_Information(const My_Matrix* pMatWorld) { m_matWorld = *pMatWorld; }
	void		Set_Information(INFORMATION eInfo, const My_Vec3* pVec) { memcpy_s(&m_matWorld.m[eInfo][0], sizeof(My_Vec3), pVec, sizeof(My_Vec3)); }

public:
	static CTransform*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent*		Clone();
protected:
	virtual My_uLong		ReleaseMemory();
};

END