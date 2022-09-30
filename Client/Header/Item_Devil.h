#pragma once

#include "Include.h"
#include "Model.h"

class CItem_Devil final : public CModel
{
private:
	explicit CItem_Devil(LPDIRECT3DDEVICE9 pDevice);
	explicit CItem_Devil(const CItem_Devil& Ref);
	virtual ~CItem_Devil() = default;

public:
	const My_Vec3*		Get_Position() const {
		return m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
	}

public:
	void		Set_Player_Tramsform(CTransform* pPlayerTrans){
		m_pPlayerTrans = pPlayerTrans;
	}
	void		Set_Num(const int& iNum) {
		m_iNum = iNum;
	}

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

private:
	CTransform*			m_pPlayerTrans = nullptr;
	int					m_iNum = 0;
	float				m_fAccTime = 0.f;
	float				m_fFirstTime = 0.f;
	bool				m_bIsFirst = true;

public:
	static	 CItem_Devil*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*			Clone(void);

protected:
	virtual My_uLong			ReleaseMemory(void);
};

