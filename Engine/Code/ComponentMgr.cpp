#include "..\Header\ComponentMgr.h"
#include "Component.h"

USING(Engine)
IMPLEMENT_SINGLETON(CComponentMgr)



CComponentMgr::CComponentMgr()
{
}

HRESULT CComponentMgr::Reserve(const My_uLong & dwNum)
{
	if (nullptr != m_pComponents)
	{
		return E_FAIL;
	}

	m_pComponents = new MAPCOMPONENT[dwNum];
	m_pComponents_Dependent = new MAPCOMPONENT[dwNum];

	m_dwSize = dwNum;

	return NOERROR;
}

HRESULT CComponentMgr::Add_Component(const My_uLong & dwSceneID, const My_TChar * szKey, CComponent * pData)
{
	if (m_dwSize <= dwSceneID
		|| nullptr == m_pComponents)
		return E_FAIL;

	CComponent* pFindComponent = Find_Component(dwSceneID, szKey);
	if (nullptr != pFindComponent)
		return E_FAIL;

	m_pComponents[dwSceneID].insert(MAPCOMPONENT::value_type(szKey, pData));

	return NOERROR;
}

HRESULT CComponentMgr::Add_Dependent_Component(const My_uLong & dwSceneID, const My_TChar * szKey, CComponent * pData)
{
	if (m_dwSize <= dwSceneID
		|| nullptr == m_pComponents)
		return E_FAIL;

	CComponent* pFindComponent = Find_Dependent_Component(dwSceneID, szKey);
	if (nullptr != pFindComponent)
		return E_FAIL;

	m_pComponents[dwSceneID].insert(MAPCOMPONENT::value_type(szKey, pData));

	return NOERROR;
}

CComponent * CComponentMgr::Clone_Component(const My_uLong & dwSceneID, const My_TChar * szKey)
{
	if (m_dwSize <= dwSceneID
		|| nullptr == m_pComponents)
		return nullptr;

	CComponent* pComponent = Find_Component(dwSceneID, szKey);
	if (nullptr == pComponent) {
		pComponent = Find_Dependent_Component(dwSceneID, szKey);
		if (nullptr == pComponent)
			return nullptr;
	}

	return pComponent->Clone();
}

void CComponentMgr::Clear_Component(const My_uLong & dwSceneID)
{
	if (m_dwSize <= dwSceneID
		|| nullptr == m_pComponents)
		return;

	for (auto& Pair : m_pComponents[dwSceneID])
		Safe_Release(Pair.second);

	for (auto& Pair : m_pComponents_Dependent[dwSceneID])
		Safe_Release(Pair.second);

	m_pComponents[dwSceneID].clear();
	m_pComponents_Dependent[dwSceneID].clear();
}

CComponent * CComponentMgr::Find_Component(const My_uLong & dwSceneID, const My_TChar * szKey)
{
	if (m_dwSize <= dwSceneID
		|| nullptr == m_pComponents)
		return nullptr;

	auto iter = find_if(m_pComponents[dwSceneID].begin(), m_pComponents[dwSceneID].end(), CFinder_Tag(szKey));

	if (iter == m_pComponents[dwSceneID].end())
		return nullptr;

	return iter->second;
}

CComponent * CComponentMgr::Find_Dependent_Component(const My_uLong & dwSceneID, const My_TChar * szKey)
{
	if (m_dwSize <= dwSceneID
		|| nullptr == m_pComponents_Dependent)
		return nullptr;

	auto iter = find_if(m_pComponents_Dependent[dwSceneID].begin(), m_pComponents_Dependent[dwSceneID].end(), CFinder_Tag(szKey));

	if (iter == m_pComponents_Dependent[dwSceneID].end())
		return nullptr;

	return iter->second;
}

My_uLong CComponentMgr::ReleaseMemory(void)
{
	My_uLong			dwRefCnt = 0;

	for (size_t i = 0; i < m_dwSize; i++)
	{
		for (auto& Pair : m_pComponents[i])
		{
			if (dwRefCnt = Safe_Release(Pair.second))
				return dwRefCnt;
		}
		m_pComponents[i].clear();
	}

	for (size_t i = 0; i < m_dwSize; i++)
	{
		for (auto& Pair : m_pComponents_Dependent[i])
		{
			if (dwRefCnt = Safe_Release(Pair.second))
				return dwRefCnt;
		}
		m_pComponents_Dependent[i].clear();
	}

	Safe_Delete_Array(m_pComponents);
	Safe_Delete_Array(m_pComponents_Dependent);

	return dwRefCnt;
}