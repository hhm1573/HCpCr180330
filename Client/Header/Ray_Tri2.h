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

class CRay_Tri2	 final : public CGameObject
{
private:
	explicit CRay_Tri2(LPDIRECT3DDEVICE9 pDevice);
	explicit CRay_Tri2(const CRay_Tri2& Ref);
	virtual ~CRay_Tri2() = default;

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
	My_Bool	Get_IsRay2() {
		return m_bIsRay2;
	}
	My_Vec3		Get_Position() {
		return		*m_pTransformCom->Get_Information(CTransform::INFO_POSITION);
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
	My_Bool			m_bIsRay2 = false;
	My_Int			m_iTargetNum = 0;
	My_Bool			m_bIsBoss = false;

public:
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void			Render();

private:
	void			Set_RenderState();
	void			Release_RenderState();

public:
	static		CRay_Tri2*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*		Clone();

protected:
	virtual		My_uLong			ReleaseMemory();

private:
	void	ChangeWorld();
};
