#pragma once

#include "GameObject.h"
#include "InputDev.h"

BEGIN(Engine)

class ENGINE_DLL CCamera
	:public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pDevice);
	explicit CCamera(const CCamera& Ref);
	virtual ~CCamera() = default;

protected:
	CAMERA_DESC		m_CameraDesc;
	My_Matrix		m_matWorld;
	My_Matrix		m_matView;
protected:
	PROJ_DESC		m_ProjDesc;
	My_Matrix		m_matProj;
protected:
	CInputDev*		m_pInputDev = nullptr;
	
public:
	HRESULT			Initialize(const CAMERA_DESC* pCameraDesc, const PROJ_DESC* pProjDesc);
	virtual My_Int	Update(const My_Float& fTimeDelta);
	virtual My_Int	LastUpdate(const My_Float& fTimeDelta);
	virtual void	Render() {}

public:
	My_Matrix		GetViewMat() const { return m_matView; }
	My_Matrix		GetProjMat() const { return m_matProj; }

protected:
	HRESULT			Compute_ViewMat();
	HRESULT			Compute_ProjMat();

public:
	virtual CGameObject*	Clone() PURE;
protected:
	virtual My_uLong		ReleaseMemory();
};

END