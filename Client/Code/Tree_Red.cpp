#include "stdafx.h"
#include "..\Header\Tree_Red.h"

CTree_Red::CTree_Red(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CTree_Red::CTree_Red(const CTree_Red & Ref)
	: CModel(Ref)
{
}

HRESULT CTree_Red::Initialize()
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CTree_Red::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Red_Tree_Client.dat");
}

My_Int CTree_Red::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CTree_Red::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CTree_Red::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CTree_Red * CTree_Red::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTree_Red*		pInstance = new CTree_Red(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CTree_Red::Clone(void)
{
	CTree_Red*		pInstance = new CTree_Red(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CTree_Red::ReleaseMemory(void)
{
	CModel::ReleaseMemory();
	return My_uLong();
}
