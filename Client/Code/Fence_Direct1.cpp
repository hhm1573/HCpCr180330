#include "stdafx.h"
#include "..\Header\Fence_Direct1.h"

CFence_Direct1::CFence_Direct1(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CFence_Direct1::CFence_Direct1(const CFence_Direct1 & Ref)
	:CModel(Ref)
{
}

HRESULT CFence_Direct1::Initialize()
{
	return CModel::Initialize(SCENE_STATIC,L"Component_Buffer_CubeCol",
		SCENE_STATIC,L"Component_Buffer_CubeTex",
		SCENE_STATIC,L"Component_Transform",
		SCENE_STATIC,L"Component_Renderer",
		SCENE_STAGE,L"Component_Texture_");
}

HRESULT CFence_Direct1::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Fence_Direct1_Client.dat");
}

My_Int CFence_Direct1::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CFence_Direct1::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CFence_Direct1::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CFence_Direct1 * CFence_Direct1::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFence_Direct1*		pInstance = new CFence_Direct1(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CFence_Direct1::Clone(void)
{
	CFence_Direct1*		pInstance = new CFence_Direct1(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CFence_Direct1::ReleaseMemory(void)
{
	CModel::ReleaseMemory();
	return My_uLong();
}
