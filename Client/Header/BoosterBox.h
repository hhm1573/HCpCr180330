#pragma once

#include "Model.h"

namespace Engine
{
	class CCollisioner;
}

class CBoosterBox final
	:public CModel
{
private:
	explicit CBoosterBox(LPDIRECT3DDEVICE9 pDevice);
	explicit CBoosterBox(const CBoosterBox& Ref);
	virtual ~CBoosterBox() = default;

public:
	void SetIsMoving(My_Bool flag);
	void SetPos(My_Vec3 vecPos) { mVecPos = vecPos; }

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

private:
	CCollisioner*		m_pCollisioner = nullptr;

private:
	My_Float			mSpeed;
	My_Int				mState;
	My_Bool				mIsMoving;
	My_Float			mAngle;

	My_Vec3				mVecPos;
	const My_Matrix*	mMatParent;

public:
	static CBoosterBox*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

