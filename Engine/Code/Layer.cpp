#include "..\Header\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

const CGameObject * CLayer::Get_GameObject(const My_uInt & iIndex) const
{
	auto iter = m_ObjectList.begin();

	if (iter == m_ObjectList.end())
		return nullptr;

	if (iIndex >= m_ObjectList.size())
		return nullptr;

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	return (*iter);
}

HRESULT CLayer::Initialize()
{
	return NOERROR;
}

My_Int CLayer::Update(const My_Float & fTimeDelta)
{
	My_Int		iExitCode = 0;

	auto iter = m_ObjectList.begin();
	auto iter_end = m_ObjectList.end();

	for (; iter != iter_end;)
	{
		auto temp = *iter;
		iExitCode = (*iter)->Update(fTimeDelta);
		if (iExitCode == 1)
		{
			Safe_Release(*iter);
			iter = m_ObjectList.erase(iter);
		}
		else if (iExitCode & 0x80000000)
			break;
		else
			++iter;
	}
	return iExitCode;
}

My_Int CLayer::LastUpdate(const My_Float & fTimeDelta)
{
	My_Int		iExitCode = 0;
	auto iter = m_ObjectList.begin();
	auto iter_end = m_ObjectList.end();

	for (; iter != iter_end;)
	{
		iExitCode = (*iter)->LastUpdate(fTimeDelta);
		if (iExitCode == 1)
		{
			Safe_Release(*iter);
			iter = m_ObjectList.erase(iter);
		}
		else if (iExitCode & 0x80000000)
			break;
		else
			++iter;
	}

	return iExitCode;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_ObjectList.push_back(pGameObject);

	return NOERROR;
}

CLayer * CLayer::Create()
{
	CLayer*	pInstance = new CLayer();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CLayer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

My_uLong CLayer::ReleaseMemory(void)
{
	for (auto& pGameObject : m_ObjectList)
		Safe_Release(pGameObject);

	m_ObjectList.clear();

	return My_uLong();
}
