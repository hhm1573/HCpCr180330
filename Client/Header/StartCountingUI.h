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

class CStartCountingUI final : public CGameObject
{
private:
	explicit CStartCountingUI(LPDIRECT3DDEVICE9 pDevice);
	explicit CStartCountingUI(const CStartCountingUI& Ref);
	virtual ~CStartCountingUI() = default;

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
	My_Int		GetCounting() { return mCounting; }

public:
	void		SetIsStart() { mIsStart = true; }

private:
	My_Int		mCounting;
	My_Float	mTime;
	My_Bool		mIsStart;

private:
	HRESULT		Initialize_Component();

private:
	void		Set_RenderState();
	void		Release_RenderState();

public:
	static		CStartCountingUI*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*				Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

