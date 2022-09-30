#pragma once

#include "Model.h"

namespace Engine {
	class CVehicle;
	class CNavMesh;
	class CCollisioner;
}

class CCrocodile;

class CCrocodile_Tail final
	:public CModel
{
private:
	explicit CCrocodile_Tail(LPDIRECT3DDEVICE9 pDevice);
	explicit CCrocodile_Tail(const CCrocodile_Tail& Ref);
	virtual ~CCrocodile_Tail() = default;

public:
	void	Set_IsBoss(My_Bool bIsBoss) {
		m_bIsBoss = bIsBoss;

		//m_pComponent_Transform->Scaling(1.f, 10.f, 10.f);
		m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(422.f, 7.f, 285.f));
	}

	void SetBody(CCrocodile* body) 
	{ 
		mBody = body; 
		m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(422.f, 7.f, 285.f));
	}

private:
	CVehicle*				m_pComponent_Vehicle = nullptr;
	CNavMesh*				m_pComponent_NavMesh = nullptr;
	CCollisioner*			m_pCollisioner = nullptr;

	My_Bool					m_bLanded;
	CCrocodile*			mBody;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

private:
	My_Bool		m_bIsBoss = false;

public:
	static CCrocodile_Tail*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

