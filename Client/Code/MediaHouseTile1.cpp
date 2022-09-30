#include "stdafx.h"
#include "..\Header\MediaHouseTile1.h"

CMediaHouseTile1::CMediaHouseTile1(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CMediaHouseTile1::CMediaHouseTile1(const CMediaHouseTile1 & Ref)
	: CModel(Ref)
{
}

HRESULT CMediaHouseTile1::Initialize()
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CMediaHouseTile1::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Media_House_Tile1_Client.dat");
}

My_Int CMediaHouseTile1::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CMediaHouseTile1::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CMediaHouseTile1::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CMediaHouseTile1 * CMediaHouseTile1::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMediaHouseTile1*		pInstance = new CMediaHouseTile1(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CMediaHouseTile1::Clone(void)
{
	CMediaHouseTile1*		pInstance = new CMediaHouseTile1(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CMediaHouseTile1::ReleaseMemory(void)
{
	CModel::ReleaseMemory();
	return My_uLong();
}
