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

class CGameStartButton final : public CGameObject
{
private:
	explicit CGameStartButton(LPDIRECT3DDEVICE9 pDevice);
	explicit CGameStartButton(const CGameStartButton& Ref);
	virtual ~CGameStartButton() = default;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CViewPort_Buffer*		m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

public:
	My_Bool			GetIsStart() { return mIsStart; }

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Component();
	HRESULT		Initialize_Prototype();

public:
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void		Render();

private:
	My_Int			mNum;
	My_Bool			mIsStart;

private:
	void			Set_RenderState();
	void			Release_RenderState();

public:
	static		CGameStartButton*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*		Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

