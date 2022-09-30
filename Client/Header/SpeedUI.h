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

class CSpeedUI abstract 
	: public CGameObject
{
protected:
	explicit CSpeedUI(LPDIRECT3DDEVICE9 pDevice);
	explicit CSpeedUI(const CSpeedUI& Ref);
	virtual ~CSpeedUI() = default;

protected:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CViewPort_Buffer*		m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

public:
	void		SetIsAward(My_Bool flag) { mIsAward = flag; }

protected:
	HRESULT		Initialize();
	HRESULT		Initialize_Prototype();
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void		Render();

protected:
	FRAME				mFrame;
	My_Float			mPlayerSpeed;
	const CGameObject*	mPlayer;

protected:
	My_Bool		mIsAward = false;

private:
	HRESULT		Initialize_Component();

protected:
	void		Set_RenderState();
	void		Release_RenderState();

protected:
	virtual		My_uLong			ReleaseMemory();
};

