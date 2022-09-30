#include "stdafx.h"
#include "..\Header\GreenBlock.h"

CGreenBlock::CGreenBlock(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CGreenBlock::CGreenBlock(const CGreenBlock & Ref)
	: CModel(Ref)
{
}

HRESULT CGreenBlock::Initialize()
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CGreenBlock::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Dash_Block_Client.dat");
}

My_Int CGreenBlock::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CGreenBlock::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CGreenBlock::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CGreenBlock * CGreenBlock::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CGreenBlock*		pInstance = new CGreenBlock(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CGreenBlock::Clone(void)
{
	CGreenBlock*		pInstance = new CGreenBlock(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CGreenBlock::ReleaseMemory(void)
{
	CModel::ReleaseMemory();
	return My_uLong();
}
