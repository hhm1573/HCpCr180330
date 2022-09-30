#pragma once

#include "Base.h"

BEGIN(Engine)

class CComponentMgr;
class CGameObjectMgr;

class ENGINE_DLL CScene
	:public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CScene() = default;

protected:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
	CComponentMgr*			m_pComponentMgr = nullptr;
	CGameObjectMgr*			m_pObjectMgr = nullptr;

public:
	virtual HRESULT			Initialize();
	virtual My_Int			Update(const My_Float& fTimeDelta);
	virtual My_Int			LastUpdate(const My_Float& fTimeDelta);
	virtual void			Render();

protected:
	virtual My_uLong		ReleaseMemory();
};

END