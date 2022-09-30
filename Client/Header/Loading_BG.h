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
class CLoading_BG final : public CGameObject
{
public:
	explicit CLoading_BG(LPDIRECT3DDEVICE9 pDevice);
	explicit CLoading_BG(const CLoading_BG& Ref);
	virtual ~CLoading_BG() = default;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CViewPort_Buffer*		m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

private:
	LPDIRECT3DTEXTURE9		m_pTexture = nullptr;

private:
	FRAME		m_tFrame;
	My_Bool		m_bIsLoading = false;

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Component();
	HRESULT		Initialize_Prototype();

public:
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void			Render();

public:
	static		CLoading_BG*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*		Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

