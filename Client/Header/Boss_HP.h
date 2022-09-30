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

class CBoss_HP final : public CGameObject
{
private:
	explicit CBoss_HP(LPDIRECT3DDEVICE9 pDevice);
	explicit CBoss_HP(const CBoss_HP& Ref);
	virtual ~CBoss_HP() = default;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CViewPort_Buffer*		m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

public:
	CTransform*		Get_TransformCom() const {
		return m_pTransformCom;
	}
	void		Set_IsCol(My_Bool bIsCol) {
		m_bIsCol = bIsCol;
	}

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Component();
	HRESULT		Initialize_Prototype();

private:
	My_Vec3			m_vPosition;
	My_Bool			m_bIsCol = false;

public:
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void			Render();

private:
	void			Set_RenderState();
	void			Release_RenderState();

public:
	static		CBoss_HP*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*			Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

