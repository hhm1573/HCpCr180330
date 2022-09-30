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

class CItemSlot final : public CGameObject
{
private:
	explicit CItemSlot(LPDIRECT3DDEVICE9 pDevice);
	explicit CItemSlot(const CItemSlot& Ref);
	virtual ~CItemSlot() = default;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CViewPort_Buffer*		m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

public:
	HRESULT		Initialize();
	HRESULT		Initialize_Prototype();
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void		Render();

public:
	void		SetIsAward(My_Bool flag) { mIsAward = flag; }

private:
	HRESULT		Initialize_Component();

private:
	My_Bool		mIsAward = false;
	
private:
	void		Set_RenderState();
	void		Release_RenderState();

public:
	static		CItemSlot*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*		Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

