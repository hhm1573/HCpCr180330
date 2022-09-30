#include "stdafx.h"
#include "..\Header\Tree_Green2.h"

CTree_Green2::CTree_Green2(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CTree_Green2::CTree_Green2(const CTree_Green2 & Ref)
	: CModel(Ref)
{
}

HRESULT CTree_Green2::Initialize()
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CTree_Green2::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Green_Tree2_Client.dat");
}

My_Int CTree_Green2::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CTree_Green2::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CTree_Green2::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CTree_Green2 * CTree_Green2::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTree_Green2*		pInstance = new CTree_Green2(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CTree_Green2::Clone(void)
{
	CTree_Green2*		pInstance = new CTree_Green2(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CTree_Green2::ReleaseMemory(void)
{
	CModel::ReleaseMemory();
	return My_uLong();
}
