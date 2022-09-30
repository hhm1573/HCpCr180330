#pragma once

#include "Model.h"

class CRotateHouse final
	:public CModel
{
private:
	explicit CRotateHouse(LPDIRECT3DDEVICE9 pDevice);
	explicit CRotateHouse(const CRotateHouse& Ref);
	virtual ~CRotateHouse() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	const My_Matrix*	GetWorldMatrix() const { return m_pComponent_Transform->Get_WorldMatrix(); }

public:
	void				SetOriPos(My_Vec3 vecOri) { mOriPos = vecOri; }

private:
	My_Float				mAngleX;
	My_Float				mAngleY;
	My_Float				mAngleZ;

private:
	My_Float				mSpeed;

private:
	My_Float				mWaitTime;

private:
	My_Bool					mIsMediaHouse;
	My_Bool					mIsWait;

private:
	My_Vec3					mOriPos;

public:
	static CRotateHouse*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

