#pragma once

#include "Include.h"
#include "Model.h"

namespace Engine
{
	class CCollisioner;
}

class CItem_First final : public CModel
{
public:
	explicit CItem_First(LPDIRECT3DDEVICE9 pDevice);
	explicit CItem_First(const CItem_First& Ref);
	virtual ~CItem_First() = default;

public:
	const My_Vec3*		Get_Position() const {
		return m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
	}

public:
	void		Set_Player_Transform(const My_Matrix* pPlayerMatrix);
	void		Set_Num(const int& iNum);
	void		Set_CenterPos(const My_Vec3* vCenterPos) {
		m_vCenter = *vCenterPos;
	}

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

private:
	CCollisioner*	m_pCollisioner = nullptr;
	My_Vec3			m_vPosition;
	My_Vec3			m_vRight;
	My_Vec3			m_vUp;
	My_Vec3			m_vLook;
	My_Vec3			m_vCenter;
	My_Int			m_iNum;
	My_Float		m_fAccTime;
	My_Int			m_iState = 0;

public:
	static CItem_First*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);

protected:
	virtual My_uLong			ReleaseMemory(void);
};

