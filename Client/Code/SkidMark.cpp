#include "stdafx.h"
#include "..\Header\SkidMark.h"



CSkidMark::CSkidMark(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CSkidMark::CSkidMark(const CSkidMark & Ref)
	:CGameObject(Ref)
{
	m_pComponent_Renderer = Ref.m_pComponent_Renderer;
	m_pComponent_RectTex = Ref.m_pComponent_RectTex;
	m_pComponent_Texture = Ref.m_pComponent_Texture;
}

HRESULT CSkidMark::Initialize()
{
	m_pComponent_Renderer = (CRenderer*)m_pComponentMgr->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (m_pComponent_Renderer == nullptr)
		return E_FAIL;
	if (FAILED(Add_Component(L"Component_Renderer", m_pComponent_Renderer)))
		return E_FAIL;

	m_pComponent_RectTex = (CRectTex*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Buffer_RectTex");
	if (m_pComponent_RectTex == nullptr)
		return E_FAIL;
	if (FAILED(Add_Component(L"Component_Buffer_RectTex", m_pComponent_RectTex)))
		return E_FAIL;

	m_pComponent_Texture = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Water");
	if (m_pComponent_Texture == nullptr)
		return E_FAIL;
	if (FAILED(Add_Component(L"Component_Texture_Water", m_pComponent_Texture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkidMark::Initialize_Prototype()
{
	return S_OK;
}

My_Int CSkidMark::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CSkidMark::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CSkidMark::Render()
{
	m_pComponent_Texture->SetUp_OnGraphicDev(0);
	for (auto& matWorld : m_List_Matrix) {
		m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_pComponent_RectTex->Render();
	}
}

HRESULT CSkidMark::Add_SkidMark(const My_Vec3 & vStart, const My_Vec3 & vEnd)
{
	My_Vec3		vDir = vEnd - vStart;
	My_Float	fDistance = D3DXVec3Length(&vDir);
	My_Vec3		vCenter = (vStart + vEnd) * 0.5f;
	
	My_Vec3		vLook = My_Vec3(0.0f, 0.0f, 1.0f);
	My_Vec3		vDir_Nor = *D3DXVec3Normalize(&vDir_Nor, &vDir);
	My_Float	fCosf = D3DXVec3Dot(&vDir_Nor, &vLook);
	My_Float	fRad = acosf(fCosf);
	My_Float	fDeg = D3DXToDegree(fRad);
	if (vStart.x >= vEnd.x)
		fRad *= -1.0f;

	//스자이공부
	//케전동전모
	//일
	My_Matrix	MatWorld, MatScale, MatRotX, MatRotY,MatRotYPR, MatTrans;
	D3DXMatrixIdentity(&MatWorld);
	D3DXMatrixIdentity(&MatScale);
	D3DXMatrixIdentity(&MatRotX);
	D3DXMatrixIdentity(&MatRotY);
	D3DXMatrixIdentity(&MatRotYPR);
	D3DXMatrixIdentity(&MatTrans);
	
	D3DXMatrixScaling(&MatScale, 0.125f, fDistance, 1.0f);
	D3DXMatrixRotationX(&MatRotX, D3DXToRadian(90.0f));
	D3DXMatrixRotationY(&MatRotY, fRad);
	D3DXMatrixTranslation(&MatTrans, vCenter.x, vCenter.y + 0.01f, vCenter.z);

	MatWorld = MatScale * MatRotX * MatRotY * MatTrans;

	m_List_Matrix.push_back(MatWorld);

	return S_OK;
}

CSkidMark * CSkidMark::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSkidMark*		pInstance = new CSkidMark(pDevice);
	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}
	return pInstance;
}

CGameObject * CSkidMark::Clone()
{
	CSkidMark*		pInstance = new CSkidMark(*this);
	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}
	return pInstance;
}

My_uLong CSkidMark::ReleaseMemory()
{
	Safe_Release(m_pComponent_Renderer);
	Safe_Release(m_pComponent_RectTex);
	Safe_Release(m_pComponent_Texture);
	CGameObject::ReleaseMemory();
	return My_uLong();
}
