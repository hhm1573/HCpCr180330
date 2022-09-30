#pragma once

#include "Camera.h"

namespace Engine 
{
	class CTransform;
}

class CCamera_MiniMap final
	:public CCamera
{
private:
	explicit CCamera_MiniMap(LPDIRECT3DDEVICE9 pDevice);
	explicit CCamera_MiniMap(const CCamera_MiniMap& Ref);
	virtual ~CCamera_MiniMap() = default;

private:
	CTransform*			m_pTarget = nullptr;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype(const CAMERA_DESC& tCameraDesc, const PROJ_DESC& tProjDesc);
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta)override;
	virtual void		Render() override;

public:
	HRESULT				SetTarget(CTransform* pTarget);
	void				SetRotate(bool bIsRotate) { m_bIsRotate = bIsRotate; }

private:
	My_Vec3			m_vPrePos;
	bool			m_bIsRotate;

public:
	static CCamera_MiniMap*		Create(LPDIRECT3DDEVICE9 pDevice, const CAMERA_DESC& tCameraDesc, const PROJ_DESC& tProjDesc);
	virtual CGameObject*		Clone() override;
protected:
	virtual My_uLong			ReleaseMemory() override;
};

