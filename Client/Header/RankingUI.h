#pragma once

#include "Include.h"
#include "GameObject.h"

#include "RankingManager.h"

namespace Engine
{
	class CRenderer;
	class CViewPort_Buffer;
	class CTransform;
	class CTexture;
}

class CRankingUI final : public CGameObject
{
private:
	explicit CRankingUI(LPDIRECT3DDEVICE9 pDevice);
	explicit CRankingUI(const CRankingUI& Ref);
	virtual ~CRankingUI() = default;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CViewPort_Buffer*		m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

private:
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
	void		Set_RenderState();
	void		Release_RenderState();

private:
	vector<CRankingManager::Racer>*		mRanking;
	My_Int								mPlayerRank;
	My_Bool		mIsAward = false;


public:
	static		CRankingUI*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*		Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

