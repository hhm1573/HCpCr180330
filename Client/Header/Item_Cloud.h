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

class CItem_Cloud final : public CGameObject
{
private:
	explicit CItem_Cloud(LPDIRECT3DDEVICE9 pDevice);
	explicit CItem_Cloud(const CItem_Cloud& rhs);
	virtual ~CItem_Cloud() = default;

public:
	CTransform*		Get_TransformCom() const {
		return m_pTransformCom;
	}

public:
	void		Set_Player_Transform(const My_Matrix* pPlayerMatrix);

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRectTex*				m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CCollisioner*			m_pCollisioner = nullptr;

private:
	My_Vec3			m_vPosition;
	My_Vec3			m_vRight;
	My_Vec3			m_vUp;
	My_Vec3			m_vLook;
	float			m_fAccTime;
	My_Vec3			m_vLine1;
	My_Vec3			m_vLine2;

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Component();
	HRESULT		Initialize_Prototype();

public:
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void			Render();

private:

private:
	void			Set_RenderState();
	void			Release_RenderState();

public:
	static		CItem_Cloud*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*		Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

