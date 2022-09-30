#include "stdafx.h"
#include "..\Header\Crocodile_Tree.h"

CCrocodile_Tree::CCrocodile_Tree(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CCrocodile_Tree::CCrocodile_Tree(const CCrocodile_Tree & Ref)
	: CModel(Ref)
{
}

HRESULT CCrocodile_Tree::Initialize()
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CCrocodile_Tree::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Crocodile_Tree_Client.dat");
}

My_Int CCrocodile_Tree::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CCrocodile_Tree::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CCrocodile_Tree::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CCrocodile_Tree * CCrocodile_Tree::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCrocodile_Tree*		pInstance = new CCrocodile_Tree(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CCrocodile_Tree::Clone(void)
{
	CCrocodile_Tree*		pInstance = new CCrocodile_Tree(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CCrocodile_Tree::ReleaseMemory(void)
{
	CModel::ReleaseMemory();
	return My_uLong();
}
