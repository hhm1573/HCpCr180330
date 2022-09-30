#include "stdafx.h"
#include "..\Header\KartRiderSign.h"

CKartRiderSign::CKartRiderSign(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CKartRiderSign::CKartRiderSign(const CKartRiderSign & Ref)
	: CModel(Ref)
{
}

HRESULT CKartRiderSign::Initialize()
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CKartRiderSign::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/KartRider_Sign_Client.dat");
}

My_Int CKartRiderSign::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CKartRiderSign::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CKartRiderSign::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CKartRiderSign * CKartRiderSign::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CKartRiderSign*		pInstance = new CKartRiderSign(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CKartRiderSign::Clone(void)
{
	CKartRiderSign*		pInstance = new CKartRiderSign(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CKartRiderSign::ReleaseMemory(void)
{
	CModel::ReleaseMemory();
	return My_uLong();
}
