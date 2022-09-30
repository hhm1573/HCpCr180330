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

class CDizny_Ani final : public CGameObject
{
private:
	explicit CDizny_Ani(LPDIRECT3DDEVICE9 pDevice);
	explicit CDizny_Ani(const CDizny_Ani& Ref);
	virtual ~CDizny_Ani() = default;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CViewPort_Buffer*		m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

public:
	CTransform*		Get_TransformCom() const {
		return m_pTransformCom;
	}
	void		Set_WaitingRoomPos() {
		m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(300.f, 300.f, 0.f));
	}
	void		SetRanking(My_Int rank);

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Component();
	HRESULT		Initialize_Prototype();

private:
	My_Vec3				m_vPosition;
	FRAME				m_tFrame;

public:
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void			Render();

private:
	void			Set_RenderState();
	void			Release_RenderState();

public:
	static		CDizny_Ani*				Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*			Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

