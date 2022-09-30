#pragma once

#include "Model.h"

namespace Engine {
	class CCollisioner;
}

class CItemBox final
	:public CModel
{
private:
	explicit CItemBox(LPDIRECT3DDEVICE9 pDevice);
	explicit CItemBox(const CItemBox& Ref);
	virtual ~CItemBox() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

private:
	CCollisioner*			m_pCollisioner = nullptr;

private:
	My_Int				m_iState = 0;
	My_Float			mRespawnTime = 0.f;
	My_Bool				mIsCol = false;

private:
	My_Float				mAngleX;
	My_Float				mAngleY;
	My_Float				mAngleZ;

private:
	My_Float				mSpeed;

public:
	static CItemBox*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

