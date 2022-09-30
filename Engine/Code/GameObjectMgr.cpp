#include "..\Header\GameObjectMgr.h"
#include "GameObject.h"
#include "Layer.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGameObjectMgr)

const int & CGameObjectMgr::Get_LayerSize(const My_uLong & dwSceneID, const My_TChar * szKey)
{
	const CLayer* pLayer = Find_Layer_Const(dwSceneID, szKey);

	if (pLayer == nullptr)
		return 0;

	return pLayer->Get_ObjectSize();
}

HRESULT CGameObjectMgr::Reserve(const My_uLong & dwNum)
{
	if (nullptr != m_pPrototypeObject)
		return E_FAIL;
	m_pPrototypeObject = new MAPPROTOTYPEOBJECT[dwNum];
	m_pLayers = new MAPLAYER[dwNum];

	m_dwNumMaxScene = dwNum;
	return NOERROR;
}

HRESULT CGameObjectMgr::Add_Object_Prototype(const My_uLong & dwSceneID, const My_TChar * szKey, CGameObject * pObject)
{
	if (nullptr == m_pPrototypeObject
		|| m_dwNumMaxScene <= dwSceneID)
		return E_FAIL;

	CGameObject*		pFindObject = Find_Object_Prototype(dwSceneID, szKey);
	if (nullptr != pFindObject)
		return E_FAIL;

	m_pPrototypeObject[dwSceneID].insert(MAPPROTOTYPEOBJECT::value_type(szKey, pObject));

	return NOERROR;
}

HRESULT CGameObjectMgr::Add_Object(const My_uLong & dwSceneID, const My_TChar * szKey, CGameObject * pObject)
{
	if (nullptr == m_pLayers
		|| m_dwNumMaxScene <= dwSceneID)
		return E_FAIL;

	CLayer*		pLayer = Find_Layer(dwSceneID, szKey);
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(pObject)))
			return E_FAIL;

		m_pLayers[dwSceneID].insert(MAPLAYER::value_type(szKey, pLayer));
	}
	else
	{
		if (FAILED(pLayer->Add_GameObject(pObject)))
			return E_FAIL;
	}

	return NOERROR;
}

CGameObject * CGameObjectMgr::Clone_GameObject(const My_uLong & dwSceneID, const My_TChar * szKey)
{
	if (nullptr == m_pPrototypeObject
		|| m_dwNumMaxScene <= dwSceneID)
		return nullptr;

	CGameObject*		pFindObject = Find_Object_Prototype(dwSceneID, szKey);
	if (nullptr == pFindObject)
		return nullptr;



	return pFindObject->Clone();
}

void CGameObjectMgr::Clear_GameObject(const My_uLong & dwSceneID)
{
	if (m_dwNumMaxScene <= dwSceneID
		|| nullptr == m_pLayers
		|| nullptr == m_pPrototypeObject)
		return;

	for (auto& Pair : m_pLayers[dwSceneID])
		Safe_Release(Pair.second);
	m_pLayers[dwSceneID].clear();

	for (auto& Pair : m_pPrototypeObject[dwSceneID])
		Safe_Release(Pair.second);
	m_pPrototypeObject[dwSceneID].clear();
}

void CGameObjectMgr::Clear_Layer(const My_uLong & dwSceneID, const My_TChar * szLayerKey)
{
	CLayer*		pLayer_Find = Find_Layer(dwSceneID, szLayerKey);

	if (pLayer_Find == nullptr)
		return;

	pLayer_Find->ReleaseMemory();
}

const CGameObject * CGameObjectMgr::Get_GameObject(const My_uLong & dwSceneID, const My_TChar * szLayerKey, const My_uLong & dwIndex)const
{
	const CLayer*		pLayer_Find = Find_Layer_Const(dwSceneID, szLayerKey);

	if (pLayer_Find == nullptr)
		return nullptr;

	const CGameObject*	pObject_Find = pLayer_Find->Get_GameObject(dwIndex);

	return pObject_Find;
}

My_Int CGameObjectMgr::Update(const My_Float & fTimeDelta)
{
	My_Int iExitCode = 0;

	for (size_t i = 0; i < m_dwNumMaxScene; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			iExitCode = Pair.second->Update(fTimeDelta);
			if (iExitCode & 0x80000000)
				break;
		}
	}

	return iExitCode;
}

My_Int CGameObjectMgr::LastUpdate(const My_Float & fTimeDelta)
{
	My_Int iExitCode = 0;

	for (size_t i = 0; i < m_dwNumMaxScene; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			iExitCode = Pair.second->LastUpdate(fTimeDelta);
			if (iExitCode & 0x80000000)
				break;
		}
	}

	return iExitCode;
}

CGameObject * CGameObjectMgr::Find_Object_Prototype(const My_uLong & dwSceneID, const My_TChar * szKey)
{
	if (m_dwNumMaxScene <= dwSceneID ||
		nullptr == m_pPrototypeObject)
		return nullptr;

	auto iter = find_if(m_pPrototypeObject[dwSceneID].begin(), m_pPrototypeObject[dwSceneID].end(), CFinder_Tag(szKey));

	if (m_pPrototypeObject[dwSceneID].end() == iter)
		return nullptr;

	return iter->second;
}

CLayer * CGameObjectMgr::Find_Layer(const My_uLong & dwSceneID, const My_TChar * szKey)
{
	if (m_dwNumMaxScene <= dwSceneID ||
		nullptr == m_pLayers)
		return nullptr;

	auto iter = find_if(m_pLayers[dwSceneID].begin(), m_pLayers[dwSceneID].end(), CFinder_Tag(szKey));

	if (m_pLayers[dwSceneID].end() == iter)
		return nullptr;

	return iter->second;
}

const CLayer * CGameObjectMgr::Find_Layer_Const(const My_uLong & dwSceneID, const My_TChar * szLayerKey) const
{
	if (m_dwNumMaxScene <= dwSceneID ||
		nullptr == m_pLayers)
		return nullptr;

	auto iter = find_if(m_pLayers[dwSceneID].begin(), m_pLayers[dwSceneID].end(), CFinder_Tag(szLayerKey));

	if (m_pLayers[dwSceneID].end() == iter)
		return nullptr;

	return iter->second;
}

My_uLong CGameObjectMgr::ReleaseMemory()
{
	My_uLong dwRefCnt = 0;

	for (size_t i = 0; i < m_dwNumMaxScene; i++)
	{
		for (auto& Pair : m_pPrototypeObject[i])
		{
			if (dwRefCnt = Safe_Release(Pair.second))
				return dwRefCnt;
		}
		m_pPrototypeObject[i].clear();

		for (auto& Pair : m_pLayers[i])
		{
			if (dwRefCnt = Safe_Release(Pair.second))
				return dwRefCnt;
		}
		m_pLayers[i].clear();
	}
	Safe_Delete_Array(m_pPrototypeObject);
	Safe_Delete_Array(m_pLayers);


	return dwRefCnt;
}
