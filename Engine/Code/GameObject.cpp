#include "..\Header\GameObject.h"
#include "ComponentMgr.h"
#include "Component.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pDevice(pGraphic_Device)
	, m_pComponentMgr(CComponentMgr::GetInstance())
{
	m_pDevice->AddRef();
	m_pComponentMgr->AddRef();
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pComponentMgr(rhs.m_pComponentMgr)
{
	if (nullptr != m_pDevice)
		m_pDevice->AddRef();

	if (nullptr != m_pComponentMgr)
		m_pComponentMgr->AddRef();
}

const CComponent * CGameObject::Get_ComponentPointer(const My_TChar * pComponentTag) const
{
	return Find_Component(pComponentTag);
}

void CGameObject::Compute_ViewZ(const My_Vec3 * pPosition)
{
	My_Matrix			matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, nullptr, &matView);

	My_Vec3 		vCamPosition;
	memcpy(&vCamPosition, &matView.m[3][0], sizeof(My_Vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPosition - *pPosition));
}

HRESULT CGameObject::Add_Component(const My_TChar * pComponentTag, CComponent * pComponent)
{
	CComponent* pFindComponet = Find_Component(pComponentTag);
	if (nullptr != pFindComponet)
		return E_FAIL;

	m_mapComponent.insert(MAPCOMPONENT::value_type(pComponentTag, pComponent));

	pComponent->AddRef();

	return NOERROR;
}

CComponent * CGameObject::Find_Component(const My_TChar * pComponentTag)const
{
	auto iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CFinder_Tag(pComponentTag));
	if (iter == m_mapComponent.end())
		return nullptr;

	return iter->second;
}

My_uLong CGameObject::ReleaseMemory(void)
{
	Safe_Release(m_pComponentMgr);
	Safe_Release(m_pDevice);

	for (auto& Pair : m_mapComponent)
		Safe_Release(Pair.second);

	m_mapComponent.clear();

	return My_uLong();
}
