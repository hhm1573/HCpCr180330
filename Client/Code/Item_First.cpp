#include "stdafx.h"
#include "..\Header\Item_First.h"
#include "ComponentMgr.h"
#include "GameObjectMgr.h"


CItem_First::CItem_First(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
	, m_vPosition(0.f, 0.f, 0.f)
	, m_vRight(0.f, 0.f, 0.f)
	, m_vUp(0.f, 0.f, 0.f)
	, m_vLook(0.f, 0.f, 0.f)
	, m_vCenter(0.f, 0.f, 0.f)
	, m_fAccTime(0.f)
	, m_iNum(0)
{
}

CItem_First::CItem_First(const CItem_First & Ref)
	: CModel(Ref)
	, m_vPosition(Ref.m_vPosition)
	, m_vRight(Ref.m_vRight)
	, m_vUp(Ref.m_vUp)
	, m_vLook(Ref.m_vLook)
	, m_vCenter(Ref.m_vCenter)
	, m_fAccTime(Ref.m_fAccTime)
	, m_pCollisioner(Ref.m_pCollisioner)
	, m_iNum(Ref.m_iNum)
	, m_iState(Ref.m_iState)
{
}

void CItem_First::Set_Player_Transform(const My_Matrix * pPlayerMatrix)
{
	// Player의 월드 행렬을 인자로 얻어와서 멤버 변수에 할당한다.
	m_vRight = *(My_Vec3*)&(*pPlayerMatrix).m[0][0];
	m_vUp = *(My_Vec3*)&(*pPlayerMatrix).m[1][0];
	m_vLook = *(My_Vec3*)&(*pPlayerMatrix).m[2][0];
	m_vPosition = *(My_Vec3*)&(*pPlayerMatrix).m[3][0];

	// 스케일 값이 있으므로 벡터를 길이 1로 만들어준다.
	D3DXVec3Normalize(&m_vRight, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Normalize(&m_vLook, &m_vLook);

	// Player의 회전값 * 스케일값
	m_pComponent_Transform->Set_Information(CTransform::INFO_RIGHT, &(m_vRight * 1.2f));
	m_pComponent_Transform->Set_Information(CTransform::INFO_UP, &(m_vUp * 0.5f));
	m_pComponent_Transform->Set_Information(CTransform::INFO_LOOK, &(m_vLook * 0.7f));
}

void CItem_First::Set_Num(const int & iNum)
{
	m_iNum = iNum;

	switch (m_iNum)
	{
	case 0:
		// 플레이어의 Look 벡터 기반으로 위치 잡기
		m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &(m_vPosition + m_vLook * 40.f));
		m_pComponent_Transform->Rotation_Y(D3DXToRadian(180.f));
		m_vCenter = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);	// 센터 위치값 저장하기
		break;
	case 1:
	{
		My_Matrix	matRot;
		My_Vec3		vRight;
		D3DXMatrixRotationY(&matRot, D3DXToRadian(90.f));	// y축 90도 회전 행렬 구하기
		D3DXVec3TransformNormal(&vRight, &m_vLook, &matRot);	// 플레이어의 Look 벡터를 90도 회전 시키기

																// 원래 센터 위치에서 90도 회전 시킨 방향 벡터로 이동하기
		m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &(m_vCenter + vRight * 15.f));
		m_pComponent_Transform->Rotation_Y(D3DXToRadian(180.f));
	}
	break;
	case 2:
	{
		My_Matrix	matRot;
		My_Vec3		vLeft;
		D3DXMatrixRotationY(&matRot, D3DXToRadian(-90.f));		// y축 -90도 회전 행렬 구하기
		D3DXVec3TransformNormal(&vLeft, &m_vLook, &matRot);		// 플레이어의 Look 벡터를 -90도 회전 시키기

																// 원래 센터 위치에서 -90도 회전 시킨 방향 벡터로 이동하기
		m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &(m_vCenter + vLeft * 15.f));
		m_pComponent_Transform->Rotation_Y(D3DXToRadian(180.f));
	}
	break;
	}
}

HRESULT CItem_First::Initialize()
{
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

HRESULT CItem_First::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Item/Item_First_Client.dat");
}

My_Int CItem_First::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CItem_First::LastUpdate(const My_Float & fTimeDelta)
{
	if (1 == m_iState || 5 == m_iState || 8 == m_iState || 12 == m_iState)
	{
		return 1;
	}

	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	m_pCollisioner->Add_Collision(*m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION), 4.5f, 11, &m_iState);

	return My_Int();
}

void CItem_First::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CItem_First * CItem_First::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItem_First*		pInstance = new CItem_First(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CItem_First::Clone(void)
{
	CItem_First*		pInstance = new CItem_First(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CItem_First::ReleaseMemory(void)
{
	Safe_Release(m_pCollisioner);

	CModel::ReleaseMemory();

	return My_uLong();
}
