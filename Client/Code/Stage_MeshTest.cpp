#include "stdafx.h"
#include "..\Header\Stage_MeshTest.h"
#include "MyMesh.h"
#include "InputDev.h"

CStage_MeshTest::CStage_MeshTest(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CStage_MeshTest::CStage_MeshTest(const CStage_MeshTest & Ref)
	: CGameObject(Ref)
	, m_pCom_Mesh(Ref.m_pCom_Mesh)
	, m_iCubeCnt(Ref.m_iCubeCnt)
	, m_pCom_Transform(Ref.m_pCom_Transform)
	, m_pCom_Renderer(Ref.m_pCom_Renderer)
{
}

HRESULT CStage_MeshTest::Initialize(void)
{
	CComponent*		pComponent = m_pComponentMgr->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(Add_Component(L"Component_Transform", pComponent)))
		return E_FAIL;
	m_pCom_Transform = (CTransform*)pComponent;

	pComponent = m_pComponentMgr->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(Add_Component(L"Component_Renderer", pComponent)))
		return E_FAIL;
	m_pCom_Renderer = (CRenderer*)pComponent;

	pComponent = m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Buffer_Mesh");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(Add_Component(L"Component_Buffer_Mesh", pComponent)))
		return E_FAIL;
	m_pCom_Mesh = (CMyMesh*)pComponent;

	m_pCom_Mesh->Add_CubeCol(L"Init", D3DCOLOR_ARGB(255, 255, 200, 200));

	return S_OK;
}

HRESULT CStage_MeshTest::Initialize_Prototype(void)
{
	return S_OK;
}

My_Int CStage_MeshTest::Update(const My_Float & fTimeDelta)
{
	CInputDev*	pInput = CInputDev::GetInstance();

	if (pInput->Get_DIKState(DIK_NUMPAD0) & 0x80) {
		My_TChar*	sz = new My_TChar[16];
		wsprintf(sz, L"Cube%d", m_iCubeCnt);
		m_pCom_Mesh->Add_CubeCol(sz, rand());
		++m_iCubeCnt;
	}



	if (pInput->Get_DIKState(DIK_NUMPAD1) & 0x80) {
		m_pCom_Mesh->Move_Local_Right_All(1.0f, fTimeDelta);
	}

	if (pInput->Get_DIKState(DIK_NUMPAD2) & 0x80) {
		m_pCom_Mesh->Rotation_Local_Z_All(D3DXToRadian(30.0f), fTimeDelta);
	}

	if (pInput->Get_DIKState(DIK_NUMPAD3) & 0x80) {
		m_pCom_Mesh->Scale_Local_Y_All(1.0f, fTimeDelta);
	}



	if (pInput->Get_DIKState(DIK_NUMPAD4) & 0x80) {
		m_pCom_Transform->Move_Forward(10.0f * fTimeDelta);
	}

	if (pInput->Get_DIKState(DIK_NUMPAD5) & 0x80) {
		m_pCom_Transform->Rotation_X(D3DXToRadian(30.0f)*fTimeDelta);
	}

	if (pInput->Get_DIKState(DIK_NUMPAD6) & 0x80) {
	}

	return My_Int();
}

My_Int CStage_MeshTest::LastUpdate(const My_Float & fTimeDelta)
{
	m_pCom_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CStage_MeshTest::Render(void)
{
	m_pDevice->SetTexture(0, nullptr);
	m_pCom_Mesh->Render(m_pCom_Transform->Get_WorldMatrix());
}

CStage_MeshTest * CStage_MeshTest::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage_MeshTest* p = new CStage_MeshTest(pDevice);

	if (FAILED(p->Initialize_Prototype())) {
		delete p;
		p = nullptr;
	}

	return p;
}

CGameObject * CStage_MeshTest::Clone(void)
{
	CStage_MeshTest* p = new CStage_MeshTest(*this);

	if (FAILED(p->Initialize())) {
		delete p;
		p = nullptr;
	}

	return p;
}

My_uLong CStage_MeshTest::ReleaseMemory()
{
	Safe_Release(m_pCom_Transform);
	Safe_Release(m_pCom_Renderer);
	Safe_Release(m_pCom_Mesh);
	CGameObject::ReleaseMemory();
	return 0;
}
