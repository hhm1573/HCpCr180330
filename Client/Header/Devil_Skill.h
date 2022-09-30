#pragma once

#include "Include.h"
#include "Model.h"

namespace Engine {
	class CCollisioner;
}

class CDevil_Skill final : public CModel
{
private:
	explicit CDevil_Skill(LPDIRECT3DDEVICE9 pDevice);
	explicit CDevil_Skill(const CDevil_Skill& Ref);
	virtual ~CDevil_Skill() = default;

public:
	void		Set_Boss_Transform(CTransform* pBoss) {
		m_pBossTransform = pBoss;
	}
	void		Set_Position(My_Vec3 vPos) {
		m_vPos = vPos;
		m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &vPos);
	}

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void			Render() override;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRectTex*				m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

private:
	CTransform*	m_pBossTransform = nullptr;
	CCollisioner*			m_pCollisioner = nullptr;
	My_Vec3			m_vPos;
	int					m_iNum = 0;
	float				m_fAccTime = 0.f;
	float				m_fFirstTime = 0.f;
	bool				m_bIsFirst = true;
	My_Float			m_fJumpAcc = 0.f;
	My_Int			m_iState = 0;

public:
	static	 CDevil_Skill*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*			Clone(void);

protected:
	virtual My_uLong			ReleaseMemory(void);
};

