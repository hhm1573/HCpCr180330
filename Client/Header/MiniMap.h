#pragma once

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CViewPort_Buffer;
	class CTransform;
	class CTexture;
	class CCamera;
}

class CMiniMap final : public CGameObject
{
private:
	explicit CMiniMap(LPDIRECT3DDEVICE9 pDevice);
	explicit CMiniMap(const CMiniMap& Ref);
	virtual ~CMiniMap() = default;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CViewPort_Buffer*		m_pBufferCom = nullptr;

public:
	HRESULT		Initialize();
	HRESULT		Initialize_Prototype();
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void		Render();

private:
	LPDIRECT3DSURFACE9		mOriBackBufferSurface = nullptr;
	LPDIRECT3DSURFACE9		mMiniMapSurface = nullptr;
	LPDIRECT3DTEXTURE9		mMiniMapTexture = nullptr;

private:
	My_Matrix				mTargetViewMat;
	My_Matrix				mTargetProjMat;
	My_Matrix				mMiniMapViewMat;
	My_Matrix				mMiniMapProjMat;

public:
	void		SetIsAward(My_Bool flag) { mIsAward = flag; }

private:
	const CCamera*	mCamTarget;
	const CCamera*	mCamMiniMap;

private:
	HRESULT		Initialize_Component();
	My_Bool		mIsAward = false;

public:
	void		SetRenderTargetMiniMap();
	void		ReleaseRenderTargetMiniMap();

private:
	void		Set_RenderState();
	void		Release_RenderState();

public:
	static		CMiniMap*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*		Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

