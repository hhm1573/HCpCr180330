#include "stdafx.h"
#include "..\Header\MediaHouseTile2.h"

CMediaHouseTile2::CMediaHouseTile2(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CMediaHouseTile2::CMediaHouseTile2(const CMediaHouseTile2 & Ref)
	: CModel(Ref)
{
}

HRESULT CMediaHouseTile2::Initialize()
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CMediaHouseTile2::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Media_House_Tile2_Client.dat");
}

My_Int CMediaHouseTile2::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CMediaHouseTile2::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CMediaHouseTile2::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CMediaHouseTile2 * CMediaHouseTile2::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMediaHouseTile2*		pInstance = new CMediaHouseTile2(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CMediaHouseTile2::Clone(void)
{
	CMediaHouseTile2*		pInstance = new CMediaHouseTile2(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CMediaHouseTile2::ReleaseMemory(void)
{
	CModel::ReleaseMemory();
	return My_uLong();
}
