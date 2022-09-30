#include "stdafx.h"
#include "..\Header\MediaHouse.h"

CMediaHouse::CMediaHouse(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CMediaHouse::CMediaHouse(const CMediaHouse & Ref)
	: CModel(Ref)
{
}

HRESULT CMediaHouse::Initialize()
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CMediaHouse::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Media_House_Client.dat");
}

My_Int CMediaHouse::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CMediaHouse::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CMediaHouse::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CMediaHouse * CMediaHouse::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMediaHouse*		pInstance = new CMediaHouse(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CMediaHouse::Clone(void)
{
	CMediaHouse*		pInstance = new CMediaHouse(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CMediaHouse::ReleaseMemory(void)
{
	CModel::ReleaseMemory();
	return My_uLong();
}
