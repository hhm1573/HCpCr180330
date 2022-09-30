#pragma once

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CRectTex;
	class CTransform;
	class CTexture;
}

class CWater final : public CGameObject
{
private:
	explicit CWater(LPDIRECT3DDEVICE9 pDevice);
	explicit CWater(const CWater& rhs);
	virtual ~CWater() = default;

private:
	CRenderer* 		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CRectTex*  		m_pBufferCom = nullptr;
	CTexture*  		m_pTextureCom = nullptr;

private:
	HRESULT			Initialize();
	HRESULT			Initialize_Component();
	HRESULT			Initialize_Prototype();

public:
	My_Int			Update(const My_Float& fTimeDelta);
	My_Int			LastUpdate(const My_Float& fTimeDelta);
	void			Render();

public:
	void			SetPositon(const My_Vec3& pos);

private:
	void			Set_RenderState();
	void			Release_RenderState();

public:
	static		CWater*				Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*		Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

