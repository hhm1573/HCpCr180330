#pragma once

#include "Camera.h"

class CCamera_Free final
	:public CCamera
{
private:
	explicit CCamera_Free(LPDIRECT3DDEVICE9 pDevice);
	explicit CCamera_Free(const CCamera_Free& Ref);
	virtual ~CCamera_Free() = default;

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Prototype(const CAMERA_DESC* pCameraDesc, const PROJ_DESC* pProjDesc);
public:
	virtual My_Int	Update(const My_Float& fTimeDelta);
	virtual My_Int	LastUpdate(const My_Float& fTimeDelta);

public:
	static CCamera_Free*		Create(LPDIRECT3DDEVICE9 pDevice, const CAMERA_DESC* pCameraDesc, const PROJ_DESC* pProjDesc);
	virtual CGameObject*		Clone();
protected:
	virtual My_uLong			ReleaseMemory();
};

