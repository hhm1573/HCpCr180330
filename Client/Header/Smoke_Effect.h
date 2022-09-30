#pragma once

#include "Include.h"
#include "GameObject.h"
#include "Bazzi.h"
#include "GameObjectMgr.h"

namespace Engine
{
	class CRenderer;
	class CRectTex;
	class CTransform;
	class CTexture;
}

class CSmoke_Effect final : public CGameObject
{
private:
	explicit CSmoke_Effect(LPDIRECT3DDEVICE9 pDevice);
	explicit CSmoke_Effect(const CSmoke_Effect& Ref);
	virtual ~CSmoke_Effect() = default;

public:
	void	Set_Name(My_TChar* name) {
		m_szName = name;

		CGameObject* pBazzi = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0));
		CTransform*	pTransform = dynamic_cast<CBazzi*>(pBazzi)->Get_Transform();
		My_Vec3		vRight = *pTransform->Get_Information(CTransform::INFO_RIGHT);
		m_vPos = dynamic_cast<CBazzi*>(pBazzi)->Get_ModelPos(m_szName);
		My_Vec3		vUp = *pTransform->Get_Information(CTransform::INFO_UP);
		m_vLook = *pTransform->Get_Information(CTransform::INFO_LOOK);

		m_pTransformCom->Set_Information(CTransform::INFO_RIGHT, &(vRight));
		m_pTransformCom->Set_Information(CTransform::INFO_UP, &(vUp));
		m_pTransformCom->Set_Information(CTransform::INFO_LOOK, &(m_vLook));
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
	FRAME				m_tFrame;
	My_Vec3				m_vPos;
	My_Vec3				m_vLook;
	My_TChar*			m_szName = nullptr;
	My_Float			m_fAccTime = 0.f;
	My_Float			m_fFirstTime = 0.f;
	My_Bool				m_bIsFirst = true;
	My_Int				m_iRand = 0;
	My_Vec3				m_vScale;

public:
	static		CSmoke_Effect*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*			Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

