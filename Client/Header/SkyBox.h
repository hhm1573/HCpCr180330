#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CCubeTex;
class CTexture;
END

class CSkyBox final
	:public CGameObject
{
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pDevice);
	explicit CSkyBox(const CSkyBox& Ref);
	virtual ~CSkyBox() = default;

private:
	CRenderer*			m_pCom_Renderer = nullptr;
	CTransform*			m_pCom_Transform = nullptr;
	CCubeTex*			m_pCom_CubeTex = nullptr;
	CTexture*			m_pCom_Texture = nullptr;

private:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
private:
	HRESULT				Initialize_Component();
public:
	virtual My_Int		Update(const My_Float& fTimeDelta);
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta);
	virtual void		Render();
private:
	void				Set_RenderState();
	void				Release_RenderState();

public:
	static CSkyBox*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*	Clone();
protected:
	virtual My_uLong		ReleaseMemory();
};