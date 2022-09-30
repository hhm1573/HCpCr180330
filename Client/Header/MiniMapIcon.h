#pragma once

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CRectTex;
	class CTransform;
	class CTexture;
}

class CMiniMap;

class CMiniMapIcon 
	: public CGameObject
{
protected:
	explicit CMiniMapIcon(LPDIRECT3DDEVICE9 pDevice);
	explicit CMiniMapIcon(const CMiniMapIcon& rhs);
	virtual ~CMiniMapIcon() = default;

protected:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRectTex*				m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

public:
	void		SetIsAward(My_Bool flag) { mIsAward = flag; }

protected:
	HRESULT		Initialize();
	HRESULT		Initialize_Component();
	HRESULT		Initialize_Prototype();

public:
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void		Render();

protected:
	const CMiniMap*		mMiniMap;
	const CGameObject*	mTarget;
	My_Bool		mIsAward = false;


private:
	void			Set_RenderState();
	void			Release_RenderState();

protected:
	virtual		My_uLong			ReleaseMemory();
};

