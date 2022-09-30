#pragma once

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CRectTex;
	class CTransform;
	class CTexture;
}

class CEngine_Effect final : public CGameObject
{
private:
	explicit CEngine_Effect(LPDIRECT3DDEVICE9 pDevice);
	explicit CEngine_Effect(const CEngine_Effect& Ref);
	virtual ~CEngine_Effect() = default;

public:
	void	Set_Name(My_TChar* name) {
		m_szName = name;
	}

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRectTex*				m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Component();
	HRESULT		Initialize_Prototype();

public:
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void			Render();

private:
	void			Set_RenderState();
	void			Release_RenderState();

private:
	FRAME				m_tFrame;
	My_Vec3				m_vPos;
	My_Vec3				m_vLook;
	My_TChar*			m_szName = nullptr;
	My_Float			m_fAccTime = 0.f;
	My_Float			m_fFirstTime = 0.f;
	My_Bool				m_bIsFirst = true;

public:
	static		CEngine_Effect*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*			Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

