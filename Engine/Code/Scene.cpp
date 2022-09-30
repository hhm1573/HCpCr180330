#include "..\Header\Scene.h"
#include "ComponentMgr.h"
#include "GameObjectMgr.h"

CScene::CScene(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	m_pDevice->AddRef();
	m_pComponentMgr = CComponentMgr::GetInstance();
	m_pComponentMgr->AddRef();
	m_pObjectMgr = CGameObjectMgr::GetInstance();
	m_pObjectMgr->AddRef();
}

HRESULT CScene::Initialize()
{
	return S_OK;
}

My_Int CScene::Update(const My_Float & fTimeDelta)
{
	if (m_pObjectMgr == nullptr)
		return -1;

	return m_pObjectMgr->Update(fTimeDelta);
}

My_Int CScene::LastUpdate(const My_Float & fTimeDelta)
{
	if (m_pObjectMgr == nullptr)
		return -1;

	return m_pObjectMgr->LastUpdate(fTimeDelta);
}

void CScene::Render()
{
}

My_uLong CScene::ReleaseMemory()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pComponentMgr);
	Safe_Release(m_pObjectMgr);

	return 0;
}
