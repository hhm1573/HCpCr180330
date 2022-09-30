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

class CItem_Water_Fly final : public CGameObject
{
private:
	explicit CItem_Water_Fly(LPDIRECT3DDEVICE9 pDevice);
	explicit CItem_Water_Fly(const CItem_Water_Fly& Ref);
	virtual ~CItem_Water_Fly() = default;

public:
	void			Set_Transform(CTransform* pTransform);
	void			SetTargetTrans(CTransform* pTransform) { mTargetTrans = pTransform; }
	void			SetTarget(My_Int target);

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRectTex*				m_pBufferCom = nullptr;
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
	FRAME					m_tFrame;
	CCollisioner*			m_pCollisioner = nullptr;
	My_Int					m_iState = 0;
	CTransform*				m_pPlayerTrans = nullptr;
	My_Vec3					m_vPosition;
	My_Float				m_fAccTime = 0.f;
	My_Int					m_iRand = 0;
	CTransform*				mTargetTrans = nullptr;
	My_Int					mTarget = 0;

public:
	static		CItem_Water_Fly*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*			Clone();

protected:
	virtual		My_uLong			ReleaseMemory();

};

