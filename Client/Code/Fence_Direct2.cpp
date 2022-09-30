#include "stdafx.h"
#include "..\Header\Fence_Direct2.h"

CFence_Direct2::CFence_Direct2(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CFence_Direct2::CFence_Direct2(const CFence_Direct2 & Ref)
	: CModel(Ref)
{
}

HRESULT CFence_Direct2::Initialize()
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CFence_Direct2::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Fence_Direct2_Client.dat");
}

My_Int CFence_Direct2::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CFence_Direct2::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CFence_Direct2::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CFence_Direct2 * CFence_Direct2::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFence_Direct2*		pInstance = new CFence_Direct2(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CFence_Direct2::Clone(void)
{
	CFence_Direct2*		pInstance = new CFence_Direct2(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CFence_Direct2::ReleaseMemory(void)
{
	CModel::ReleaseMemory();
	return My_uLong();
}
