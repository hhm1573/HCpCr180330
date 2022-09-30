#include "stdafx.h"
#include "..\Header\Crocodile_Tail.h"
#include "ComponentMgr.h"
#include "GameObjectMgr.h"
#include "Terrain.h"
#include "Crocodile.h"


CCrocodile_Tail::CCrocodile_Tail(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
	, m_pComponent_Vehicle(nullptr)
	, m_pCollisioner(nullptr)
{
}

CCrocodile_Tail::CCrocodile_Tail(const CCrocodile_Tail & Ref)
	: CModel(Ref)
	, m_pComponent_Vehicle(Ref.m_pComponent_Vehicle)
	, m_pCollisioner(Ref.m_pCollisioner)
{
	if (nullptr != m_pComponent_Vehicle)
		m_pComponent_Vehicle->AddRef();
}

HRESULT CCrocodile_Tail::Initialize()
{
	m_pComponent_Vehicle = (CVehicle*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Vehicle");
	if (m_pComponent_Vehicle == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Vehicle", m_pComponent_Vehicle)))
		return E_FAIL;
	m_pComponent_Vehicle->Set_Inform(50.0f, 1000.0f, 1.0f);

	m_pComponent_NavMesh = (CNavMesh*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_NavMesh");
	if (m_pComponent_NavMesh == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_NavMesh", m_pComponent_NavMesh)))
		return E_FAIL;

	m_pCollisioner = (CCollisioner*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Collisioner");
	if (m_pCollisioner == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Collisioner", m_pCollisioner)))
		return E_FAIL;

	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CCrocodile_Tail::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Crocodile_Tail_Client.dat");
}

My_Int CCrocodile_Tail::Update(const My_Float & fTimeDelta)
{
	if (nullptr != mBody && mBody->GetDead())
		return My_Int();

	if (true == m_bIsBoss)
	{
		My_Matrix	matScale, matParent, matMyWorld;
		D3DXMatrixIdentity(&matScale);
		D3DXMatrixIdentity(&matParent);
		D3DXMatrixIdentity(&matMyWorld);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

		CCrocodile* pGameObject = mBody;
		matParent = *pGameObject->Get_Component()->Get_WorldMatrix();

		matMyWorld = matScale * matParent;

		m_pComponent_Transform->Set_World(matMyWorld);
	}

	if (true == m_bIsBoss)
	{
		if (GetAsyncKeyState('W') & 0x8000)
		{
			m_pComponent_Vehicle->Accelerate();
		}

		if (GetAsyncKeyState('S') & 0x8000) {
			m_pComponent_Vehicle->Accelerate_Reverse();
		}

		if (GetAsyncKeyState('A') & 0x8000) {
			m_pComponent_Vehicle->Turn_Left();
		}

		if (GetAsyncKeyState('D') & 0x8000) {
			m_pComponent_Vehicle->Turn_Right();
		}

		const My_Vec3*	pos = m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
		const My_Vec3*	vLook = m_pComponent_Transform->Get_Information(CTransform::INFO_LOOK);

		m_pComponent_Vehicle->Update(fTimeDelta, vLook, m_bLanded);

		const CTerrain*		pTerrain = (CTerrain*)CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Terrain", 0);

		//// 물폭탄에 맞으면 NavMesh 타지 말기
		//if (false == m_bIsWater)
		//{
		if (m_pComponent_Transform->Move_On_NavMesh(&(m_pComponent_Vehicle->Get_Velocity() * fTimeDelta), pTerrain->Get_Buffer(), fTimeDelta, m_pComponent_Vehicle, &m_bLanded) == false) {
			m_pComponent_Transform->Set_NavMesh(m_pComponent_NavMesh->Get_FromPos(*m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION)));
		}
		m_pComponent_Transform->Rotation_Y(m_pComponent_Vehicle->Get_AngularVelocity());
		//}

		m_pComponent_Transform->Rotation_Y(m_pComponent_Vehicle->Get_AngularVelocity());
	}

	return My_Int();
}

My_Int CCrocodile_Tail::LastUpdate(const My_Float & fTimeDelta)
{
	if (nullptr != mBody && mBody->GetDead())
	{
	}

	else
		m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return My_Int();
}

void CCrocodile_Tail::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CCrocodile_Tail * CCrocodile_Tail::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCrocodile_Tail*		pInstance = new CCrocodile_Tail(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CCrocodile_Tail::Clone(void)
{
	CCrocodile_Tail*		pInstance = new CCrocodile_Tail(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CCrocodile_Tail::ReleaseMemory(void)
{
	Safe_Release(m_pComponent_Vehicle);
	Safe_Release(m_pCollisioner);
	Safe_Release(m_pComponent_NavMesh);
	CModel::ReleaseMemory();
	return My_uLong();
}
