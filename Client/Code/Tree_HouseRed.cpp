#include "stdafx.h"
#include "..\Header\Tree_HouseRed.h"

CTree_HouseRed::CTree_HouseRed(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CTree_HouseRed::CTree_HouseRed(const CTree_HouseRed & Ref)
	: CModel(Ref)
{
}

HRESULT CTree_HouseRed::Initialize()
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CTree_HouseRed::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/House_Red_Tree_Client.dat");
}

My_Int CTree_HouseRed::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CTree_HouseRed::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CTree_HouseRed::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CTree_HouseRed * CTree_HouseRed::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTree_HouseRed*		pInstance = new CTree_HouseRed(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CTree_HouseRed::Clone(void)
{
	CTree_HouseRed*		pInstance = new CTree_HouseRed(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CTree_HouseRed::ReleaseMemory(void)
{
	CModel::ReleaseMemory();
	return My_uLong();
}
