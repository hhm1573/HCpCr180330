#pragma once

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CRectTex;
	class CTransform;
	class CTexture;
	class CCollisioner;
}

class CItem_Magnet final : public CGameObject
{
private:
	explicit CItem_Magnet(LPDIRECT3DDEVICE9 pDevice);
	explicit CItem_Magnet(const CItem_Magnet& Ref);
	virtual ~CItem_Magnet() = default;

public:
	void		Set_Transform(CTransform* pTransform) {
		m_pPlayer_Transform = pTransform;
	}
	void		Set_Num(int iNum) {
		m_iNum = iNum;
	}
	void		Set_Destroy(My_Bool isDestroy) {
		m_bIsDestroy = isDestroy;
	}
	void		Set_IsMagnet(My_Bool bIsMagnet) {
		m_bIsMagnet = bIsMagnet;
	}
	void		Set_IsMissile(My_Bool bIsMissile) {
		m_bIsMissile = bIsMissile;
	}
	void		Set_Target_Num(int num) {
		m_iTargetNum = num;
	}
	void		Set_IsBoss(My_Bool bIsBoss) {
		m_bIsBoss = bIsBoss;
	}

public:
	const int&	Get_Num() {
		return m_iNum;
	}
	const int&	Get_TargetNum() {
		return m_iTargetNum;
	}

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRectTex*				m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Component();
	HRESULT		Initialize_Prototype();

public:
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void			Render();

private:
	CTransform*			m_pPlayer_Transform = nullptr;
	My_Vec3				m_vPosition;
	My_Int				m_iNum = 0;
	My_Float			m_fAccSpeed = 0.f;
	My_Bool				m_bIsDestroy = false;
	My_Bool				m_bIsBooster = false;
	My_Bool				m_bIsMagnet = false;
	My_Bool				m_bIsMissile = false;
	My_Int				m_iTargetNum = 0;
	My_Float			mTime = 0.f;
	My_Float			m_fFirstTime = 0.f;
	My_Float			m_fAccTime = 0.f;
	My_Bool				m_bIsFirst = true;
	My_Bool				m_bIsBoss = false;

private:
	void			Set_RenderState();
	void			Release_RenderState();

public:
	static		CItem_Magnet*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*			Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

