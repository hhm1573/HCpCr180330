#include "stdafx.h"
#include "..\Header\Tetris_Bridge_White.h"

CTetris_Bridge_White::CTetris_Bridge_White(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CTetris_Bridge_White::CTetris_Bridge_White(const CTetris_Bridge_White & Ref)
	: CModel(Ref)
{
}

HRESULT CTetris_Bridge_White::Initialize()
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CTetris_Bridge_White::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Tetris_Bridge_White_Client.dat");
}

My_Int CTetris_Bridge_White::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CTetris_Bridge_White::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CTetris_Bridge_White::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CTetris_Bridge_White * CTetris_Bridge_White::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTetris_Bridge_White*		pInstance = new CTetris_Bridge_White(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CTetris_Bridge_White::Clone(void)
{
	CTetris_Bridge_White*		pInstance = new CTetris_Bridge_White(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CTetris_Bridge_White::ReleaseMemory(void)
{
	CModel::ReleaseMemory();
	return My_uLong();
}
