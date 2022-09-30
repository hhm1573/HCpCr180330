#pragma once

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CSphere_Buffer;
	class CTransform;
	class CTexture;
	class CCollisioner;
}

class CItem_Water_Bomb final : public CGameObject
{
private:
	explicit CItem_Water_Bomb(LPDIRECT3DDEVICE9 pDevice);
	explicit CItem_Water_Bomb(const CItem_Water_Bomb& rhs);
	virtual ~CItem_Water_Bomb() = default;

public:
	CTransform*		Get_TransformCom() const {
		return m_pTransformCom;
	}

public:
	void		Set_Player_Matrix(const My_Matrix* pPlayerMatrix);
	void		Set_Player_Transform(CTransform* pPlayerTrans)
	{
		m_pPlayerTrans = pPlayerTrans;
		if (true == m_bIsBoss)
			m_pTransformCom->Scaling(10.f, 10.f, 10.f);
		else
			m_pTransformCom->Scaling(1.5f, 1.5f, 1.5f);
	}
	void		Set_Number(int iNum) {
		m_iNumber = iNum;
	}
	void			Set_IsBoss(My_Bool bIsBoss) { m_bIsBoss = bIsBoss; }

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CSphere_Buffer*			m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Component();
	HRESULT		Initialize_Prototype();

public:
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void		Render();

private:
	void			Set_RenderState();
	void			Release_RenderState();

private:
	CTransform*		m_pPlayerTrans = nullptr;
	CCollisioner*	m_pCollisioner = nullptr;
	My_Float	 	m_fAccTime = 0.f;
	My_Float	 	m_fFirstTime = 0.f;
	My_Float	 	m_fXZ;
	My_Float	 	m_fY;
	My_Bool			m_bIsFirst = true;
	My_Bool			m_bIsFull;
	My_Int			m_iNumber = 0;
	My_Int			m_iState = 0;
	My_Bool			m_bIsBoss = false;

public:
	static		CItem_Water_Bomb*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*				Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

