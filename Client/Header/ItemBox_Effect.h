#pragma once

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CViewPort_Buffer;
	class CTransform;
	class CTexture;
}

class CItemBox_Effect final : public CGameObject
{
private:
	explicit CItemBox_Effect(LPDIRECT3DDEVICE9 pDevice);
	explicit CItemBox_Effect(const CItemBox_Effect& rhs);
	virtual ~CItemBox_Effect() = default;

public:
	CTransform*		Get_TransformCom() const {
		return m_pTransformCom;
	}

public:
	void		Set_Look(My_Vec3 vLook) {
		m_vLook = vLook;
	}
	void		Set_JumpPow(My_Float fJumpPow) {
		m_fJumpPow = fJumpPow;
	}
	void		Set_Position(My_Vec3 vPos) {
		m_vPosition = vPos;
		m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &m_vPosition);
	}

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CViewPort_Buffer*		m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

private:
	My_Vec3			m_vPosition;
	My_Vec3			m_vRight;
	My_Vec3			m_vUp;
	My_Vec3			m_vLook;
	My_Float			m_fAccTime = 0.f;
	My_Float			m_fFirstTime = 0.f;
	My_Bool			m_bIsFirst = true;
	My_Int			m_iRand = 0;
	My_Float			m_fJumpAcc = 0.f;
	My_Float			m_fJumpPow = 0.f;

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

public:
	static		CItemBox_Effect*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*		Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

