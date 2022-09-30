#pragma once

#include "Base.h"
#include "ComponentMgr.h"

BEGIN(Engine)


class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject(void) = default;
public:
	const CComponent* Get_ComponentPointer(const My_TChar* pComponentTag) const;
	const My_Float& Get_ViewZ(void) const {
		return m_fViewZ;
	}
public:

	virtual My_Int Update(const My_Float& fTimeDelta) PURE;
	virtual My_Int LastUpdate(const My_Float& fTimeDelta)PURE;
	virtual void Render(void)PURE;
public:
	void Compute_ViewZ(const My_Vec3* pPosition);
protected:
	LPDIRECT3DDEVICE9						m_pDevice = nullptr;
protected:
	CComponentMgr*						m_pComponentMgr = nullptr;
protected:
	My_Float									m_fViewZ = 0.0f; // 카메라와 객체 사이의 거리.
protected:
	map<const My_TChar*, CComponent*>			m_mapComponent;
	typedef map<const My_TChar*, CComponent*>	MAPCOMPONENT;
protected:
	HRESULT Add_Component(const My_TChar* pComponentTag, CComponent* pComponent);
private:
	CComponent* Find_Component(const My_TChar* pComponentTag) const;
public:
	virtual CGameObject* Clone(void) = 0;
	virtual My_uLong ReleaseMemory(void);
};

END