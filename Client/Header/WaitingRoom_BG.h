#pragma once

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CViewPort_Buffer;
	class CTransform;
	class CTexture;
}

class CWaitingRoom_BG final : public CGameObject
{
private:
	explicit CWaitingRoom_BG(LPDIRECT3DDEVICE9 pDevice);
	explicit CWaitingRoom_BG(const CWaitingRoom_BG& Ref);
	virtual ~CWaitingRoom_BG() = default;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CViewPort_Buffer*		m_pBufferCom = nullptr;
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

public:
	static		CWaitingRoom_BG*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*	Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

