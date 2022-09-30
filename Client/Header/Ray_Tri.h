#pragma once

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTriCol;
	class CTransform;
	class CTexture;
}

class CRay_Tri final : public CGameObject
{
private:
	explicit CRay_Tri(LPDIRECT3DDEVICE9 pDevice);
	explicit CRay_Tri(const CRay_Tri& Ref);
	virtual ~CRay_Tri() = default;

public:
	void	Set_Target_Transform(CTransform* pTransform) {
		m_pTargetTrans = pTransform;
	}
	void	Set_Target_Num(int iNum) {
		m_iTargetNum = iNum;
	}
	void	Set_IsBoss(My_Bool bIsBoss) {
		m_bIsBoss = bIsBoss;
	}

public:
	CTransform*		Get_Component() {
		return m_pTransformCom;
	}
	My_Bool				Get_IsOrange() {
		return m_bIsOrange;
	}

private:
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTriCol*			m_pBufferCom = nullptr;

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Component();
	HRESULT		Initialize_Prototype();

private:
	CTransform*		m_pTargetTrans = nullptr;
	CGameObject*	m_pBazzi = nullptr;
	My_Vec3			m_vPosition;
	My_Vec3			m_vLook;
	My_Float		m_fDistance = 0.f;
	My_Vec3			m_vTri1;
	My_Vec3			m_vTri2;
	My_Vec3			m_vTri3;
	My_Vec3			_m_vTri1;
	My_Vec3			_m_vTri2;
	My_Vec3			_m_vTri3;
	My_Int			m_iTargetNum = 0;
	My_Bool			m_bIsOrange = false;
	My_Bool			m_bIsBoss = false;

public:
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void			Render();

private:
	void			Set_RenderState();
	void			Release_RenderState();

public:
	static		CRay_Tri*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*		Clone();

protected:
	virtual		My_uLong			ReleaseMemory();

private:
	void	ChangeWorld();
};

