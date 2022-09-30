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

class CAward_Box final : public CGameObject
{
private:
	explicit CAward_Box(LPDIRECT3DDEVICE9 pDevice);
	explicit CAward_Box(const CAward_Box& Ref);
	virtual ~CAward_Box() = default;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CViewPort_Buffer*		m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

public:
	CTransform*		Get_TransformCom() const {
		return m_pTransformCom;
	}

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Component();
	HRESULT		Initialize_Prototype();

private:
	My_Vec3		m_vPosition;

public:
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void			Render();

private:
	void			Set_RenderState();
	void			Release_RenderState();

public:
	static		CAward_Box*				Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*			Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

