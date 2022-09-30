#pragma once

#include "Include.h"
#include "Model.h"

namespace Engine
{
	class CCollisioner;
}

class CItem_UFO final : public CModel
{
public:
	explicit CItem_UFO(LPDIRECT3DDEVICE9 pDevice);
	explicit CItem_UFO(const CItem_UFO& Ref);
	virtual ~CItem_UFO() = default;

public:
	void			Set_Transform(CTransform* pTransform);
	void			SetTarget(CGameObject* target) { mTarget = target; }
	void			SetTargetNum(My_Int num) { mTargetNum = num; }
	void			Set_IsBoss(My_Bool bIsBoss) { m_bIsBoss = bIsBoss; }

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

private:
	CTransform*				m_pPlayerTrans = nullptr;
	CCollisioner*			m_pCollisioner = nullptr;
	My_Vec3					m_vPosition;
	My_Float				m_fAccTime = 0.f;
	My_Float				m_fAccTime2 = 0.f;
	My_Float				m_fFirstTime = 0.f;
	My_Float				m_fDis = 0.f;
	My_Vec3					m_vDir;
	My_Bool					m_bIsFirst = true;
	My_Int					m_iState = 0;
	CGameObject*			mTarget = nullptr;
	My_Int					mTargetNum = 0;
	My_Bool					m_bIsBoss = false;


public:
	static CItem_UFO*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);

protected:
	virtual My_uLong			ReleaseMemory(void);
};

