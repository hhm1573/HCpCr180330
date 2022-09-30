#include "stdafx.h"
#include "..\Header\Cloud.h"

CCloud::CCloud(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CCloud::CCloud(const CCloud & Ref)
	: CModel(Ref)
{
}

HRESULT CCloud::Initialize()
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CCloud::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Cloud_Client.dat");
}

My_Int CCloud::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CCloud::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CCloud::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CCloud * CCloud::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCloud*		pInstance = new CCloud(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CCloud::Clone(void)
{
	CCloud*		pInstance = new CCloud(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CCloud::ReleaseMemory(void)
{
	CModel::ReleaseMemory();
	return My_uLong();
}
