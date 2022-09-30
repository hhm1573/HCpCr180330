#pragma once

#include "Model.h"

namespace Engine {
	class CVehicle;
	class CNavMesh;
	class CCollisioner;
}

class CCrocodile;

class CCrocodile_Face final
	:public CModel
{
private:
	explicit CCrocodile_Face(LPDIRECT3DDEVICE9 pDevice);
	explicit CCrocodile_Face(const CCrocodile_Face& Ref);
	virtual ~CCrocodile_Face() = default;

public:
	void	Set_IsBoss(My_Bool bIsBoss) 
	{
		m_bIsBoss = bIsBoss;
		//m_pComponent_Transform->Scaling(10.f, 10.f, 10.f);
		m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(422.f, 6.f, 285.f));
	}

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	void SetAngle(My_Float angleX, My_Float angleY) { mAngleX = angleX; mAngleY = angleY; }
	void SetBody(CCrocodile* body) 
	{ 
		mBody = body;  
		m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(422.f, 6.f, 285.f));
	}

private:
	My_Float			mSpeed;
	My_Float			mAngleX;
	My_Float			mAngleY;
	My_Bool				mIsOpen;
	My_Bool				m_bLanded;

	CCrocodile*			mBody;

private:
	CVehicle*				m_pComponent_Vehicle = nullptr;
	CNavMesh*				m_pComponent_NavMesh = nullptr;
	CCollisioner*			m_pCollisioner = nullptr;

private:
	My_Bool		m_bIsBoss = false;

public:
	static CCrocodile_Face*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

