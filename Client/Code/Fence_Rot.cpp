#include "stdafx.h"
#include "..\Header\Fence_Rot.h"

CFence_Rot::CFence_Rot(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CFence_Rot::CFence_Rot(const CFence_Rot & Ref)
	: CModel(Ref)
{
}

HRESULT CFence_Rot::Initialize()
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CFence_Rot::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Fence_Rot_Client.dat");
}

My_Int CFence_Rot::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CFence_Rot::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CFence_Rot::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CFence_Rot * CFence_Rot::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFence_Rot*		pInstance = new CFence_Rot(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CFence_Rot::Clone(void)
{
	CFence_Rot*		pInstance = new CFence_Rot(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CFence_Rot::ReleaseMemory(void)
{
	CModel::ReleaseMemory();
	return My_uLong();
}
