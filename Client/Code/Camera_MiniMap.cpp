#include "stdafx.h"
#include "..\Header\Camera_MiniMap.h"
#include "Debugger.h"

CCamera_MiniMap::CCamera_MiniMap(LPDIRECT3DDEVICE9 pDevice)
	:CCamera(pDevice)
	, m_bIsRotate(false)
	, m_vPrePos(0.f, 0.f, 0.f)
{
}

CCamera_MiniMap::CCamera_MiniMap(const CCamera_MiniMap & Ref)
	: CCamera(Ref)
	, m_bIsRotate(Ref.m_bIsRotate)
	, m_vPrePos(Ref.m_vPrePos)
{
	m_pTarget = Ref.m_pTarget;
	if (m_pTarget != nullptr)
		m_pTarget->AddRef();
}

HRESULT CCamera_MiniMap::Initialize()
{
	return S_OK;
}

HRESULT CCamera_MiniMap::Initialize_Prototype(const CAMERA_DESC & tCameraDesc, const PROJ_DESC & tProjDesc)
{
	return Engine::CCamera::Initialize(&tCameraDesc, &tProjDesc);
}

My_Int CCamera_MiniMap::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CCamera_MiniMap::LastUpdate(const My_Float & fTimeDelta)
{
	if (CDebugger::GetInstance()->Get_IsFreeCamera() == true)
		return 0;
	/*if (m_pTarget == nullptr)
	return -1;*/

	const Engine::My_Matrix* pMatWorld_Target = m_pTarget->Get_WorldMatrix();

	Engine::My_Vec3		vRight = *(Engine::My_Vec3*)&pMatWorld_Target->m[0][0];
	Engine::My_Vec3		vLook = *(Engine::My_Vec3*)&pMatWorld_Target->m[2][0];
	Engine::My_Vec3		vPos = *(Engine::My_Vec3*)&pMatWorld_Target->m[3][0];

	Engine::My_Matrix	matRot;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(30.0f));

	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

	Engine::My_Vec3		vCamPos;

	vCamPos = vPos - vLook * 40.f;

	// 캐릭터가 회전 중이지 않을 때 vLook값만 저장해 놓기
	if (false == m_bIsRotate)
		m_vPrePos = -vLook * 40.f;

	// 캐릭터가 회전 중일 때 이전의 vLook값과 현재 vPos값을 더해주기
	if (true == m_bIsRotate)
		vCamPos = m_vPrePos + vPos;

	m_CameraDesc.vEye = vCamPos;
	m_CameraDesc.vAt = vPos;

	if (FAILED(Engine::CCamera::Compute_ViewMat()))
		return -1;
	if (FAILED(Engine::CCamera::Compute_ProjMat()))
		return -1;

	/*m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);*/

	return Engine::My_Int();
}

void CCamera_MiniMap::Render()
{
}

HRESULT CCamera_MiniMap::SetTarget(CTransform * pTarget)
{
	if (pTarget == nullptr)
		return E_FAIL;

	if (m_pTarget != nullptr)
		m_pTarget->Release();

	m_pTarget = pTarget;

	return S_OK;
}

CCamera_MiniMap * CCamera_MiniMap::Create(LPDIRECT3DDEVICE9 pDevice, const CAMERA_DESC& tCameraDesc, const PROJ_DESC& tProjDesc)
{
	CCamera_MiniMap* pInstance = nullptr;
	pInstance = new CCamera_MiniMap(pDevice);

	if (FAILED(pInstance->Initialize_Prototype(tCameraDesc, tProjDesc)))
		return nullptr;

	return pInstance;
}

CGameObject * CCamera_MiniMap::Clone()
{
	CCamera_MiniMap* pInstance = nullptr;
	pInstance = new CCamera_MiniMap(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CCamera_MiniMap::ReleaseMemory()
{
	if (m_pTarget != nullptr)
		m_pTarget->Release();

	return Engine::CCamera::ReleaseMemory();
}
