#include "..\Header\Renderer.h"
#include "GameObject.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent(pGraphic_Device)
{
}

HRESULT CRenderer::Ready_Renderer(void)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (RENDER_END <= eGroup
		|| nullptr == pGameObject)
		return E_FAIL;

	m_RenderList[eGroup].push_back(pGameObject);

	pGameObject->AddRef();

	return NOERROR;
}

void CRenderer::Render_Objects()
{
	Render_Priority();
	Render_NonAlpha();
	Render_Alpha();
	Render_Booster();
	Render_Cloud();
	Render_UI();

	for (size_t i = 0; i < RENDER_END; i++)
	{
		for (auto& pGameObject : m_RenderList[i])
		{
			Safe_Release(pGameObject);
		}
		m_RenderList[i].clear();
	}
}

void CRenderer::Render_Priority(void)
{
	for (auto& pGameObject : m_RenderList[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();
	}
}

void CRenderer::Render_NonAlpha(void)
{
	for (auto& pGameObject : m_RenderList[RENDER_NONALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();
	}
}

My_Bool Compare(CGameObject* pSour, CGameObject* pDest)
{
	return pSour->Get_ViewZ() > pDest->Get_ViewZ();
}

void CRenderer::Render_Alpha(void)
{
	m_RenderList[RENDER_ALPHA].sort(Compare);

	for (auto& pGameObject : m_RenderList[RENDER_ALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();
	}
}

void CRenderer::Render_Booster(void)
{
	for (auto& pGameObject : m_RenderList[RENDER_BOOSTER])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();
	}
}

void CRenderer::Render_Cloud(void)
{
	for (auto& pGameObject : m_RenderList[RENDER_CLOUD])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();
	}
}

void CRenderer::Render_UI(void)
{

	for (auto& pGameObject : m_RenderList[RENDER_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();
	}
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer*	pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->Ready_Renderer()))
	{
		MSG_BOX("CRenderer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CRenderer::Clone(void)
{
	AddRef();

	return this;
}

My_uLong CRenderer::ReleaseMemory(void)
{
	for (size_t i = 0; i < RENDER_END; i++)
	{
		for (auto& pGameObject : m_RenderList[i])
		{
			Safe_Release(pGameObject);
		}
		m_RenderList[i].clear();
	}

	CComponent::ReleaseMemory();

	return My_uLong();
}
