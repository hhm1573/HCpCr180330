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

class CItem_Cloud_Small final : public CGameObject
{
private:
	explicit CItem_Cloud_Small(LPDIRECT3DDEVICE9 pDevice);
	explicit CItem_Cloud_Small(const CItem_Cloud_Small& Ref);
	virtual ~CItem_Cloud_Small() = default;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CViewPort_Buffer*		m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

public:
	CTransform*		Get_TransformCom() const {
		return m_pTransformCom;
	}

public:
	void		Set_CloudNum(const int& iCloudNum);

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Component();
	HRESULT		Initialize_Prototype();

private:
	My_Vec3			m_vPosition;
	int				m_iCloudNum;
	float			m_fTime;

public:
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void			Render();

private:
	void			Set_RenderState();
	void			Release_RenderState();

public:
	static		CItem_Cloud_Small*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*			Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

