#pragma once

#include "Scene.h"

class CTetrisManager;
class CRankingManager;
class CStartButton;
class CGameStartButton;

namespace Engine {
	class CCollisioner;
}

class CStage final
	:public CScene
{
	enum SceneState { LOGO, WATINGROOM, STAGE };

private:
	explicit CStage(LPDIRECT3DDEVICE9 pDevice);
	explicit CStage(const CStage& Ref);
	virtual ~CStage() = default;

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Component_Prototype();
	HRESULT		Initialize_Component_Texture();
	HRESULT		Initialize_GameObject_Prototype();
	HRESULT		Initialize_GameObject();
	HRESULT		Initialize_LoadObject();
	HRESULT		Initialize_Lighting();
	HRESULT		Initialize_Tetris();
	HRESULT		Initialize_UI();
	HRESULT		Initialize_Box();
	HRESULT		Initialize_Award();

public:
	virtual My_Int		Update(const My_Float& fTimeDelta);
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta);
	virtual void		Render();

private:
	CTetrisManager*		mTetrisManager = nullptr;
	CRankingManager*	mRankingManager = nullptr;
	CCollisioner*		m_pCollisioner = nullptr;
	CStartButton*		mStartButton = nullptr;
	CGameStartButton*	mGameStartButton = nullptr;

private:
	My_Bool				mIsState = false;

private:
	My_Float			m_fFirstTime = 0.f;
	My_Float			m_fAccTime = 0.f;
	My_Bool				m_bIsFirst = true;
	My_Bool				mIsAwardFirst = true;
	My_Bool				mIsAward = false;

private:
	HRESULT				SetLogo();
	HRESULT				SetWatingRoom();
	HRESULT				SetStage();
	HRESULT				SetBossStage();

public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	virtual My_uLong	ReleaseMemory();
};

