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

class CTimeUI final : public CGameObject
{
private:
	explicit CTimeUI(LPDIRECT3DDEVICE9 pDevice);
	explicit CTimeUI(const CTimeUI& Ref);
	virtual ~CTimeUI() = default;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CViewPort_Buffer*		m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

public:
	void		SetIsStart() { mIsStart = true; }

public:
	void		SetIsAward(My_Bool flag) { mIsAward = flag; }

public:
	HRESULT		Initialize();
	HRESULT		Initialize_Prototype();
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void		Render();

private:
	My_Float		mTime;
	My_Int			mMiliSec;
	My_Int			mSec;
	My_Int			mMinute;	
	My_Int			mTimeDigit[6];
	My_Bool			mIsStart = false;
	My_Bool			mIsAward = false;


private:
	HRESULT		Initialize_Component();

private:
	void		Set_RenderState();
	void		Release_RenderState();

public:
	static		CTimeUI*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*		Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

