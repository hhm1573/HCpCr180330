#include "stdafx.h"
#include "..\Header\Stage_ModelTest.h"

CStage_ModelTest::CStage_ModelTest(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
{
}

CStage_ModelTest::CStage_ModelTest(const CStage_ModelTest & Ref)
	: CModel(Ref)
{
}

HRESULT CStage_ModelTest::Initialize(void)
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CStage_ModelTest::Initialize_Prototype(void)
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Bazzi_Client.dat");
}

My_Int CStage_ModelTest::Update(const My_Float & fTimeDelta)
{
	if (GetAsyncKeyState('Z') & 0x8000) {
		My_Vec3	vScale = m_pComponent_Transform->Get_Scale();
		vScale.x += 1.0f * fTimeDelta;
		vScale.y += 1.0f * fTimeDelta;
		vScale.z += 1.0f * fTimeDelta;
		m_pComponent_Transform->Scaling(vScale.x, vScale.y, vScale.z);
	}

	return My_Int();
}

My_Int CStage_ModelTest::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CStage_ModelTest::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CStage_ModelTest * CStage_ModelTest::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage_ModelTest*		pInstance = new CStage_ModelTest(pDevice);
	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}
	return pInstance;
}

Engine::CGameObject * CStage_ModelTest::Clone(void)
{
	CStage_ModelTest*		pInstance = new CStage_ModelTest(*this);
	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}
	return pInstance;
}

My_uLong CStage_ModelTest::ReleaseMemory(void)
{
	Engine::My_uLong	dwRefCnt = 0;
	dwRefCnt = CModel::ReleaseMemory();
	return dwRefCnt;
}
