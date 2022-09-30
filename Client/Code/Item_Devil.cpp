#include "stdafx.h"
#include "..\Header\Item_Devil.h"
#include "ComponentMgr.h"
#include "GameObjectMgr.h"
#include "Bazzi.h"


CItem_Devil::CItem_Devil(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
	, m_iNum(0)
	, m_fAccTime(0.f)
{
}

CItem_Devil::CItem_Devil(const CItem_Devil & Ref)
	: CModel(Ref)
	, m_iNum(Ref.m_iNum)
	, m_fAccTime(Ref.m_fAccTime)
	, m_pPlayerTrans(Ref.m_pPlayerTrans)
	, m_fFirstTime(Ref.m_fFirstTime)
	, m_bIsFirst(Ref.m_bIsFirst)
{
}

HRESULT CItem_Devil::Initialize()
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CItem_Devil::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Item/Item_Devil_Client.dat");
}

My_Int CItem_Devil::Update(const My_Float & fTimeDelta)
{	

	return My_Int();
}

My_Int CItem_Devil::LastUpdate(const My_Float & fTimeDelta)
{
	if (true == m_bIsFirst)
	{
		m_fFirstTime = fTimeDelta;
		m_bIsFirst = false;
	}

	m_fAccTime += fTimeDelta;

	if (3.f < m_fAccTime - m_fFirstTime)
	{
		CGameObject* pPlayer = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0));
		dynamic_cast<CBazzi*>(pPlayer)->Set_IsDevil(false);
		return 1;
	}
	
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	My_Matrix	matWorld, matRot, matTrans, matRot2, matParent, matScale;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRot2);
	D3DXMatrixIdentity(&matParent);
	D3DXMatrixIdentity(&matScale);

	D3DXMatrixScaling(&matScale, 3.f, 3.f, 3.f);
	D3DXMatrixRotationY(&matRot, D3DXToRadian(10.f * m_fAccTime));
	D3DXMatrixRotationY(&matRot2, D3DXToRadian(200.f * m_fAccTime));
	matParent = *m_pPlayerTrans->Get_WorldMatrix();


	switch (m_iNum)
	{
	case 1:
		D3DXMatrixTranslation(&matTrans, 5.f, 15.f, 5.f);
	break;
	case 2:
		D3DXMatrixTranslation(&matTrans, -5.f, 15.f, 5.f);
	break;
	case 3:
		D3DXMatrixTranslation(&matTrans, 5.f, 15.f, -5.f);
	break;
	case 4:
		D3DXMatrixTranslation(&matTrans, -5.f, 15.f, -5.f);
	break;
	}

	matWorld = matScale* matRot * matTrans * matRot2 * matParent;

	m_pComponent_Transform->Set_World(matWorld);

	return My_Int();
}

void CItem_Devil::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CItem_Devil * CItem_Devil::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItem_Devil*		pInstance = new CItem_Devil(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CItem_Devil::Clone(void)
{
	CItem_Devil*		pInstance = new CItem_Devil(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CItem_Devil::ReleaseMemory(void)
{
	CModel::ReleaseMemory();

	return My_uLong();
}
