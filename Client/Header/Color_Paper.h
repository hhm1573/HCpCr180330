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

class CColor_Paper final : public CGameObject
{
private:
	explicit CColor_Paper(LPDIRECT3DDEVICE9 pDevice);
	explicit CColor_Paper(const CColor_Paper& rhs);
	virtual ~CColor_Paper() = default;

public:
	CTransform*		Get_TransformCom() const {
		return m_pTransformCom;
	}

public:
	void		Set_Look(My_Vec3 vLook) {
		m_vLook = vLook;
	}
	void		Set_Position(My_Vec3 vPos) {
		m_vPosition = vPos;
		m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &m_vPosition);
	}

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CViewPort_Buffer*				m_pBufferCom = nullptr;
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
	static		CColor_Paper*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*		Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

