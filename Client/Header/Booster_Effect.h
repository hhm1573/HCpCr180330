#pragma once

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CViewPort_Buffer;
	class CTransform;
	class CTexture;
	class CVehicle;
}

class CBooster_Effect final : public CGameObject
{
private:
	explicit CBooster_Effect(LPDIRECT3DDEVICE9 pDevice);
	explicit CBooster_Effect(const CBooster_Effect& Ref);
	virtual ~CBooster_Effect() = default;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CViewPort_Buffer*		m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Component();
	HRESULT		Initialize_Prototype();

public:
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void		Render();

private:
	void			Set_RenderState();
	void			Release_RenderState();

private:
	CGameObject*		m_pCamrea;
	FRAME				m_tFrame;

public:
	static		CBooster_Effect*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*			Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

