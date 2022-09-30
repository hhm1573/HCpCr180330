#include "stdafx.h"
#include "..\Header\Tetris_Bridge_Red.h"

CTetris_Bridge_Red::CTetris_Bridge_Red(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CTetris_Bridge_Red::CTetris_Bridge_Red(const CTetris_Bridge_Red & Ref)
	: CModel(Ref)
{
}

HRESULT CTetris_Bridge_Red::Initialize()
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CTetris_Bridge_Red::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Tetris_Bridge_Red_Client.dat");
}

My_Int CTetris_Bridge_Red::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CTetris_Bridge_Red::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CTetris_Bridge_Red::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CTetris_Bridge_Red * CTetris_Bridge_Red::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTetris_Bridge_Red*		pInstance = new CTetris_Bridge_Red(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CTetris_Bridge_Red::Clone(void)
{
	CTetris_Bridge_Red*		pInstance = new CTetris_Bridge_Red(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CTetris_Bridge_Red::ReleaseMemory(void)
{
	CModel::ReleaseMemory();
	return My_uLong();
}
