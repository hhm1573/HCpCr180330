#include "stdafx.h"
#include "..\Header\Camera_Target.h"
#include "Debugger.h"

CCamera_Target::CCamera_Target(LPDIRECT3DDEVICE9 pDevice)
	:CCamera(pDevice)
	, m_bIsRotate(false)
	, m_iBooster(0)
	, m_vPrePos(0.f, 0.f, 0.f)
	, m_fDistance(3.8f)
	, m_fFirstTime(0.f)
	, m_fAccTime(0.f)
	, m_bIsFirst(true)
	, m_fSpeed(0.f)
	, m_vMyPos(0.f, 0.f, 0.f)
	, mState(NORMAL)
{
}

CCamera_Target::CCamera_Target(const CCamera_Target & Ref)
	: CCamera(Ref)
	, m_bIsRotate(Ref.m_bIsRotate)
	, m_iBooster(Ref.m_iBooster)
	, m_vPrePos(Ref.m_vPrePos)
	, m_fDistance(Ref.m_fDistance)
	, m_fAccTime(Ref.m_fAccTime)
	, m_fFirstTime(Ref.m_fFirstTime)
	, m_bIsFirst(Ref.m_bIsFirst)
	, m_fSpeed(Ref.m_fSpeed)
	, m_vMyPos(Ref.m_vMyPos)
	, mState(Ref.mState)
{
	m_pTarget = Ref.m_pTarget;
	if (m_pTarget != nullptr)
		m_pTarget->AddRef();
}

HRESULT CCamera_Target::Initialize()
{

	return S_OK;
}

HRESULT CCamera_Target::Initialize_Prototype(const CAMERA_DESC & tCameraDesc, const PROJ_DESC & tProjDesc)
{
	return Engine::CCamera::Initialize(&tCameraDesc, &tProjDesc);
}

My_Int CCamera_Target::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CCamera_Target::LastUpdate(const My_Float & fTimeDelta)
{
	if (NORMAL == mState)
	{
		// 처음 시간을 기록함
		if (true == m_bIsFirst)
		{
			m_fFirstTime = fTimeDelta;
			m_bIsFirst = false;
		}

		// 시간을 누적함
		m_fAccTime += fTimeDelta;

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
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(32.0f));

		D3DXVec3Normalize(&vLook, &vLook);
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		Engine::My_Vec3		vCamPos;

		// 부스터 사용시 카메라 멀어짐
		if (1 == m_iBooster)
		{
			if (m_fDistance > 5.f)
			{
				// 5초 지나면 감소하게 만들기
				if (m_fAccTime - m_fFirstTime > 4.f)
				{
					m_iBooster = 2;
					m_fFirstTime = m_fAccTime;
				}
			}
			else
				m_fDistance += 0.01f;
		}
		else if (2 == m_iBooster)
		{
			m_fDistance -= 0.01f;

			// 다시 처음 거리로 돌아오면 부스터 삭제
			if (m_fDistance < 3.8f)
				m_iBooster = 3;
		}

		vCamPos = vPos - vLook * m_fDistance;

		// 캐릭터가 회전 중이지 않을 때 vLook값만 저장해 놓기
		if (false == m_bIsRotate)
			m_vPrePos = -vLook * m_fDistance;

		// 캐릭터가 회전 중일 때 이전의 vLook값과 현재 vPos값을 더해주기
		if (true == m_bIsRotate)
			vCamPos = m_vPrePos + vPos;

		m_CameraDesc.vEye = vCamPos;
		m_CameraDesc.vAt = vPos;

	}

	else if (GOAL == mState)
	{
		mAngle += 30.f * fTimeDelta;
		const Engine::My_Matrix* matWorld = m_pTarget->Get_WorldMatrix();

		My_Vec3 vecPos, vecLook, vecRight, vecDir;

		memcpy(&vecRight, &matWorld->m[0][0], sizeof(My_Vec3));
		memcpy(&vecLook, &matWorld->m[2][0], sizeof(My_Vec3));
		memcpy(&vecPos, &matWorld->m[3][0], sizeof(My_Vec3));

		My_Matrix matRot;
		D3DXMatrixIdentity(&matRot);

		D3DXMatrixRotationY(&matRot, D3DXToRadian(mAngle));
		D3DXVec3TransformNormal(&vecLook, &vecLook, &matRot);
		D3DXVec3TransformNormal(&vecRight, &vecRight, &matRot);

		vecDir = -vecLook * (m_fDistance * 10.f);

		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationAxis(&matRot, &vecRight, D3DXToRadian(32.0f));
		D3DXVec3TransformNormal(&vecDir, &vecDir, &matRot);

		m_CameraDesc.vEye = vecPos + vecDir;
		m_CameraDesc.vAt = vecPos;
	}

	else if (AWARD == mState)
	{
		m_CameraDesc.vEye = { 113.079201f, 15.1408396f, 116.489845f };
		m_CameraDesc.vAt = { 134.878662f,10.1635780f, 116.500206f };
		m_CameraDesc.vAxisY = { 0.f, 1.f, 0.f };
	}

	if (FAILED(Engine::CCamera::Compute_ViewMat()))
		return -1;
	if (FAILED(Engine::CCamera::Compute_ProjMat()))
		return -1;

	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return Engine::My_Int();
}

void CCamera_Target::Render()
{
}

HRESULT CCamera_Target::SetTarget(CTransform * pTarget)
{
	if (pTarget == nullptr)
		return E_FAIL;

	if (m_pTarget != nullptr)
		m_pTarget->Release();

	m_pTarget = pTarget;

	return S_OK;
}

CCamera_Target * CCamera_Target::Create(LPDIRECT3DDEVICE9 pDevice, const CAMERA_DESC& tCameraDesc, const PROJ_DESC& tProjDesc)
{
	CCamera_Target* pInstance = nullptr;
	pInstance = new CCamera_Target(pDevice);

	if (FAILED(pInstance->Initialize_Prototype(tCameraDesc, tProjDesc)))
		return nullptr;

	return pInstance;
}

CGameObject * CCamera_Target::Clone()
{
	CCamera_Target* pInstance = nullptr;
	pInstance = new CCamera_Target(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CCamera_Target::ReleaseMemory()
{
	if (m_pTarget != nullptr)
		m_pTarget->Release();

	return Engine::CCamera::ReleaseMemory();
}
