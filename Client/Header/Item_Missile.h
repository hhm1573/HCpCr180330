#pragma once

#include "Include.h"
#include "Model.h"

namespace Engine
{
	class CColisioner;
}

class CItem_Missile final : public CModel
{
private:
	explicit CItem_Missile(LPDIRECT3DDEVICE9 pDevice);
	explicit CItem_Missile(const CItem_Missile& Ref);
	virtual ~CItem_Missile() = default;

public:
	void		Set_Player_Tramsform(CTransform* pPlayerTrans);
	void		Set_Target_Tramsform(CTransform* pTrans) { mTargetTrans = pTrans; }
	void		Set_Target_Num(int iTargetNum);
	void		Set_IsBoss(My_Bool IsBoss) {
		m_bIsBoss = IsBoss;
	}

public:
	CTransform*		Get_Transform() {
		return m_pComponent_Transform;
	}
	My_Vec3			Get_ModelPos(My_TChar* name)
	{
		My_Vec3 pPos;
		CModel::Get_Model_Pos(name, &pPos);
		D3DXVec3TransformCoord(&pPos, &pPos, m_pComponent_Transform->Get_WorldMatrix());
		return pPos;
	}

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void			Render() override;

private:
	CTransform*		m_pPlayerTrans = nullptr;
	My_Vec3			m_vPosition;
	My_Vec3			m_vPivot;
	My_Vec3			m_vLook;
	My_Float		m_fAccTime;
	My_Float		m_fZ = 0.f;
	CCollisioner*	m_pCollisioner = nullptr;
	My_Int			m_iState = 0;
	CTransform*		mTargetTrans = nullptr;
	My_Int			m_iTargetNum = 0;
	My_Bool			m_bIsBoss = false;

public:
	static	 CItem_Missile*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*			Clone(void);

protected:
	virtual My_uLong			ReleaseMemory(void);
};

