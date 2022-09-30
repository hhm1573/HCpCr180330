#pragma once

#include "GameObject.h"


namespace Engine
{
	class CTransform;
	class CTerrain_Buffer;
	class CRenderer;
	class CTexture;
}

class CTerrain : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;
public:
	virtual HRESULT Initialize_Prototype(void);
	virtual HRESULT Initialize(void);
	virtual My_Int Update(const My_Float& fTimeDelta);
	virtual My_Int LastUpdate(const My_Float& fTimeDelta);
	virtual void Render(void);
public:
	const CTerrain_Buffer*	Get_Buffer(void) const {
		return m_pBufferCom;
	}
private:
	CTransform*			m_pTransformCom = nullptr;
	CTerrain_Buffer*	m_pBufferCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
private:
	HRESULT Initialize_Component(void);
public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void);
protected:
	virtual My_uLong ReleaseMemory(void);

};

