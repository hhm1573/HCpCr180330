#pragma once

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CSphere_Buffer;
	class CTransform;
	class CTexture;
	class CCollisioner;
}

class CBoss_Skill final : public CGameObject
{
private:
	explicit CBoss_Skill(LPDIRECT3DDEVICE9 pDevice);
	explicit CBoss_Skill(const CBoss_Skill& rhs);
	virtual ~CBoss_Skill() = default;

public:
	void		Set_Boss_Matrix(const My_Matrix* pPlayerMatrix);

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CSphere_Buffer*		m_pBufferCom = nullptr;
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
	CTransform*		m_pPlayerTrans = nullptr;
	CCollisioner*	m_pCollisioner = nullptr;
	My_Vec3			m_vPosition = { 0.f, 0.f, 0.f };
	My_Vec3			m_vLook = { 0.f, 0.f, 0.f };
	My_Int			m_iState = 0;
	My_Float			m_fAccTime = 0.f;
	My_Float			m_fFirstTime = 0.f;
	My_Bool			m_bIsFirst = true;
	My_Int			m_iRand = 0;

private:
	void			Set_RenderState();
	void			Release_RenderState();

public:
	static		CBoss_Skill*					Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*				Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

