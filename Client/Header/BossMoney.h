#pragma once

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CRectTex;
	class CTransform;
	class CTexture;
	class CCollisioner;
}

class CBossMoney final : public CGameObject
{
private:
	explicit CBossMoney(LPDIRECT3DDEVICE9 pDevice);
	explicit CBossMoney(const CBossMoney& Ref);
	virtual ~CBossMoney() = default;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRectTex*				m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

public:
	CTransform*		Get_TransformCom() const {
		return m_pTransformCom;
	}
	void		Set_Position(My_Vec3 vPos) {
		m_vPosition = vPos;
		m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &vPos);
	}

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Component();
	HRESULT		Initialize_Prototype();

public:
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void		Render();

private:
	CCollisioner*			m_pCollisioner = nullptr;
	My_Int					m_iState = 0;

private:
	My_Float				m_fAccSpeed;
	My_Vec3					m_vPosition;
	My_Float				m_fAccTime = 0.f;

private:
	void			Set_RenderState();
	void			Release_RenderState();

public:
	static		CBossMoney*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*			Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

