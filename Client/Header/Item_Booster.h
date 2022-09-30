#pragma once

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CViewPort_Buffer;
	class CTransform;
	class CTexture;
	class CVehicle;
}

class CItem_Booster final : public CGameObject
{
private:
	explicit CItem_Booster(LPDIRECT3DDEVICE9 pDevice);
	explicit CItem_Booster(const CItem_Booster& Ref);
	virtual ~CItem_Booster() = default;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CViewPort_Buffer*		m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

public:
	void			Set_Player_Vehicle(CVehicle* pVehicle);
	void			Set_IsMagnet(My_Bool isMagnet) {
		m_bIsMagnet = isMagnet;
	}

	void SetIsPlayer(My_Bool flag);

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Component();
	HRESULT		Initialize_Prototype();

public:
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void			Render();

private:
	void			Set_RenderState();
	void			Release_RenderState();

private:
	CGameObject*		m_pCamrea;
	FRAME				m_tFrame;
	CVehicle*			m_pPlayer_Vehicle;
	My_Bool				m_bIsMagnet = false;

	My_Bool				mIsPlayer = true;
	My_Float			mBoosterTime;

public:
	static		CItem_Booster*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*			Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

