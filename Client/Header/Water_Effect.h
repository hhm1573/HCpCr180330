#pragma once

#include "Include.h"
#include "GameObject.h"

class CWater_Effect final : public CGameObject
{
private:
	explicit CWater_Effect(LPDIRECT3DDEVICE9 pDevice);
	explicit CWater_Effect(const CWater_Effect& rhs);
	virtual ~CWater_Effect() = default;

public:
	void	Set_Pos(My_Vec3 pPosition) {

		m_vPos = { pPosition.x, pPosition.y + 7.5f, pPosition.z };

		m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &m_vPos);
	}
	void	Set_IsBoss(My_Bool bIsBoss) {
		m_bIsBoss = bIsBoss;
	}

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CViewPort_Buffer*				m_pBufferCom = nullptr;
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
	My_Vec3				m_vPos;
	My_Vec3				m_vLook;
	My_Vec3				m_vScale;
	My_TChar*			m_szName = nullptr;
	My_Float			m_fAccTime = 0.f;
	My_Float			m_fFirstTime = 0.f;
	My_Bool				m_bIsFirst = true;
	My_Float			m_fY = 0.f;
	My_Bool				m_bIsBoss = false;


public:
	static		CWater_Effect*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*			Clone();

protected:
	virtual		My_uLong			ReleaseMemory();

};

