#include "stdafx.h"
#include "..\Header\SkyBox.h"



CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
	m_pCom_CubeTex = nullptr;
	m_pCom_Renderer = nullptr;
	m_pCom_Texture = nullptr;
	m_pCom_Transform = nullptr;
}

CSkyBox::CSkyBox(const CSkyBox & Ref)
	:CGameObject(Ref)
	, m_pCom_CubeTex(Ref.m_pCom_CubeTex)
	, m_pCom_Renderer(Ref.m_pCom_Renderer)
	, m_pCom_Texture(Ref.m_pCom_Texture)
	, m_pCom_Transform(Ref.m_pCom_Transform)
{
}

HRESULT CSkyBox::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkyBox::Initialize_Component()
{
	//Renderer
	m_pCom_Renderer = (CRenderer*)m_pComponentMgr->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (m_pCom_Renderer == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Renderer", m_pCom_Renderer)))
		return E_FAIL;

	//Transform
	m_pCom_Transform = (CTransform*)m_pComponentMgr->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (m_pCom_Transform == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Transform", m_pCom_Transform)))
		return E_FAIL;

	//CubeTex
	m_pCom_CubeTex = (CCubeTex*)m_pComponentMgr->Clone_Component(SCENE_STATIC, L"Component_Buffer_CubeTex");
	if (m_pCom_CubeTex == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Buffer_CubeTex", m_pCom_CubeTex)))
		return E_FAIL;

	//Texture
	m_pCom_Texture = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_SkyBox");
	if (m_pCom_Texture == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture", m_pCom_Texture)))
		return E_FAIL;

	return S_OK;
}

My_Int CSkyBox::Update(const My_Float & fTimeDelta)
{

	
	return My_Int();
}

My_Int CSkyBox::LastUpdate(const My_Float & fTimeDelta)
{
	My_Matrix	matWorld;
	m_pDevice->GetTransform(D3DTS_VIEW, &matWorld);
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);

	m_pCom_Transform->Set_Information(CTransform::INFO_POSITION, (My_Vec3*)&matWorld.m[3][0]);

	m_pCom_Renderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
	return My_Int();
}

void CSkyBox::Render()
{
	m_pCom_Transform->SetUp_OnGraphicDev();
	m_pCom_Texture->SetUp_OnGraphicDev(4);

	Set_RenderState();

	m_pCom_CubeTex->Render();

	Release_RenderState();
}

void CSkyBox::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CSkyBox::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CSkyBox * CSkyBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSkyBox* p = new CSkyBox(pDevice);
	if (FAILED(p->Initialize_Prototype())) {
		delete p;
		p = nullptr;
	}
	return p;
}

CGameObject * CSkyBox::Clone()
{
	CSkyBox* p = new CSkyBox(*this);

	if (FAILED(p->Initialize())) {
		delete p;
		p = nullptr;
	}
	return p;
}

My_uLong CSkyBox::ReleaseMemory()
{
	Safe_Release(m_pCom_Renderer);
	Safe_Release(m_pCom_Transform);
	Safe_Release(m_pCom_CubeTex);
	Safe_Release(m_pCom_Texture);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
