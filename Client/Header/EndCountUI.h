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

class CEndCountingUI final : public CGameObject
{
private:
	explicit CEndCountingUI(LPDIRECT3DDEVICE9 pDevice);
	explicit CEndCountingUI(const CEndCountingUI& Ref);
	virtual ~CEndCountingUI() = default;

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
	My_Bool		GetIsEnd() { return mIsEnd; }

public:
	void		SetIsStart() { mIsStart = true; }
	void		SetIsGoal() { mIsPlayerGoal = true; mTime = 0.f; }

private:
	My_Int		mCounting;
	My_Float	mTime;
	My_Bool		mIsStart;
	My_Bool		mIsPlayerGoal;
	My_Bool		mIsEnd = false;

private:
	HRESULT		Initialize_Component();

private:
	void		Set_RenderState();
	void		Release_RenderState();

public:
	static		CEndCountingUI*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*			Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

