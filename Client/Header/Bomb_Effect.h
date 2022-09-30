#pragma once

#include "Include.h"
#include "GameObject.h"
#include "GameObjectMgr.h"
#include "Item_Missile.h"

class CBomb_Effect final : public CGameObject
{
private:
	explicit CBomb_Effect(LPDIRECT3DDEVICE9 pDevice);
	explicit CBomb_Effect(const CBomb_Effect& Ref);
	virtual ~CBomb_Effect() = default;

public:
	void	Set_Pos(My_Vec3 pPosition) {

		m_vPos = { pPosition.x, pPosition.y + 4.f, pPosition.z };

		CGameObject* pGameObject = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Item_Missile", 0));
		CTransform*	pTransform = dynamic_cast<CItem_Missile*>(pGameObject)->Get_Transform();
		My_Vec3		vRight = *pTransform->Get_Information(CTransform::INFO_RIGHT);
		My_Vec3		vUp = *pTransform->Get_Information(CTransform::INFO_UP);
		m_vLook = *pTransform->Get_Information(CTransform::INFO_LOOK);

		if (true == m_bIsBoss)
			m_vPos = m_vPos - m_vLook * 50.f;

		m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &m_vPos);
	}
	void	Set_IsBoss(My_Bool bIsBoss) {
		m_bIsBoss = bIsBoss;
	}

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRectTex*				m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

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
	My_Vec3				m_vPos;
	My_Vec3				m_vLook;
	My_Vec3				m_vScale;
	My_TChar*			m_szName = nullptr;
	My_Float			m_fAccTime = 0.f;
	My_Float			m_fFirstTime = 0.f;
	My_Bool				m_bIsFirst = true;
	My_Float			m_fY = 0.f;
	My_Bool				m_bIsBoss = false;


public:
	static		CBomb_Effect*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*			Clone();

protected:
	virtual		My_uLong			ReleaseMemory();

};

