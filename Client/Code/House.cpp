#include "stdafx.h"
#include "..\Header\House.h"

CHouse::CHouse(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CHouse::CHouse(const CHouse & Ref)
	: CModel(Ref)
{
}

HRESULT CHouse::Initialize()
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CHouse::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/House_Client.dat");
}

My_Int CHouse::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CHouse::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CHouse::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CHouse * CHouse::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CHouse*		pInstance = new CHouse(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CHouse::Clone(void)
{
	CHouse*		pInstance = new CHouse(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CHouse::ReleaseMemory(void)
{
	CModel::ReleaseMemory();
	return My_uLong();
}
