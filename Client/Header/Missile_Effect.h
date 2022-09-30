#pragma once

#include "Include.h"
#include "GameObject.h"
#include "GameObjectMgr.h"
#include "Item_Missile.h"

namespace Engine
{
	class CRenderer;
	class CRectTex;
	class CTransform;
	class CTexture;
}

class CMissile_Effect final : public CGameObject
{
private:
	explicit CMissile_Effect(LPDIRECT3DDEVICE9 pDevice);
	explicit CMissile_Effect(const CMissile_Effect& Ref);
	virtual ~CMissile_Effect() = default;

public:
	void	Set_Name(My_TChar* name) {
		m_szName = name;


		CGameObject* pGameObject = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Item_Missile", 0));
		CTransform*	pTransform = dynamic_cast<CItem_Missile*>(pGameObject)->Get_Transform();
		My_Vec3		vRight = *pTransform->Get_Information(CTransform::INFO_RIGHT);
		m_vPos = dynamic_cast<CItem_Missile*>(pGameObject)->Get_ModelPos(m_szName);
		My_Vec3		vUp = *pTransform->Get_Information(CTransform::INFO_UP);
		m_vLook = *pTransform->Get_Information(CTransform::INFO_LOOK);

		m_pTransformCom->Set_Information(CTransform::INFO_RIGHT, &(vRight * 4.f));
		m_pTransformCom->Set_Information(CTransform::INFO_UP, &(vUp * 4.f));
		m_pTransformCom->Set_Information(CTransform::INFO_LOOK, &(m_vLook * 4.f));
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
	My_TChar*			m_szName = nullptr;
	My_Float			m_fAccTime = 0.f;
	My_Float			m_fFirstTime = 0.f;
	My_Bool				m_bIsFirst = true;

public:
	static		CMissile_Effect*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*			Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

