#include "stdafx.h"
#include "..\Header\Camera_Free.h"
#include "Debugger.h"

CCamera_Free::CCamera_Free(LPDIRECT3DDEVICE9 pDevice)
	:CCamera(pDevice)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free & Ref)
	:CCamera(Ref)
{
}

HRESULT CCamera_Free::Initialize()
{
	// 뷰스페이스 변환 행렬을 만든다.
	Compute_ViewMat();

	// 투영 변환 행렬을 만든다.
	Compute_ProjMat();

	return NOERROR;
}

HRESULT CCamera_Free::Initialize_Prototype(const CAMERA_DESC * pCameraDesc, const PROJ_DESC * pProjDesc)
{
	if (FAILED(CCamera::Initialize(pCameraDesc, pProjDesc)))
		return E_FAIL;

	return NOERROR;
}

My_Int CCamera_Free::Update(const My_Float & fTimeDelta)
{
	if (CDebugger::GetInstance()->Get_IsFreeCamera() == false)
		return 0;

	Engine::My_Float		fSpd = 10.0f;
	if (m_pInputDev->Get_DIKState(DIK_LSHIFT) & 0x80)
		fSpd *= 10.0f;

	if (GetKeyState('W') & 0x8000)
	{
		My_Vec3			vLook;

		m_CameraDesc.vEye += *D3DXVec3Normalize(&vLook, (My_Vec3*)&m_matWorld.m[2][0]) * fSpd * fTimeDelta;
		m_CameraDesc.vAt += *D3DXVec3Normalize(&vLook, (My_Vec3*)&m_matWorld.m[2][0]) * fSpd * fTimeDelta;
	}

	if (GetKeyState('S') & 0x8000)
	{
		My_Vec3			vLook;

		m_CameraDesc.vEye -= *D3DXVec3Normalize(&vLook, (My_Vec3*)&m_matWorld.m[2][0]) * fSpd * fTimeDelta;
		m_CameraDesc.vAt -= *D3DXVec3Normalize(&vLook, (My_Vec3*)&m_matWorld.m[2][0]) * fSpd * fTimeDelta;
	}

	if (GetKeyState('D') & 0x8000)
	{
		My_Vec3			vRight;

		m_CameraDesc.vEye += *D3DXVec3Normalize(&vRight, (My_Vec3*)&m_matWorld.m[0][0]) * fSpd * fTimeDelta;
		m_CameraDesc.vAt += *D3DXVec3Normalize(&vRight, (My_Vec3*)&m_matWorld.m[0][0]) * fSpd * fTimeDelta;
	}

	if (GetKeyState('A') & 0x8000)
	{
		My_Vec3			vRight;

		m_CameraDesc.vEye -= *D3DXVec3Normalize(&vRight, (My_Vec3*)&m_matWorld.m[0][0]) * fSpd * fTimeDelta;
		m_CameraDesc.vAt -= *D3DXVec3Normalize(&vRight, (My_Vec3*)&m_matWorld.m[0][0]) * fSpd * fTimeDelta;
	}

	if (m_pInputDev->Get_DIKState(DIK_C) & 0x80) {
		m_CameraDesc.vEye -= D3DXVECTOR3(0.0f, 1.0f, 0.0f) * fSpd * fTimeDelta;
		m_CameraDesc.vAt -= D3DXVECTOR3(0.0f, 1.0f, 0.0f)*fSpd*fTimeDelta;
	}

	if (m_pInputDev->Get_DIKState(DIK_SPACE) & 0x80) {
		m_CameraDesc.vEye += D3DXVECTOR3(0.0f, 1.0f, 0.0f) * fSpd * fTimeDelta;
		m_CameraDesc.vAt += D3DXVECTOR3(0.0f, 1.0f, 0.0f)*fSpd*fTimeDelta;
	}

	My_Long	MouseMove = 0;

	if (MouseMove = m_pInputDev->Get_DIMMoveState(CInputDev::DIMM_X))
	{
		My_Vec3			vLook = m_CameraDesc.vAt - m_CameraDesc.vEye;

		My_Matrix			matRot;
		D3DXMatrixRotationY(&matRot, D3DXToRadian(MouseMove * 10.f * fTimeDelta));

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_CameraDesc.vAt = m_CameraDesc.vEye + vLook;
	}

	if (MouseMove = m_pInputDev->Get_DIMMoveState(CInputDev::DIMM_Y))
	{
		My_Vec3			vLook = m_CameraDesc.vAt - m_CameraDesc.vEye;

		My_Matrix			matRot;
		D3DXMatrixRotationAxis(&matRot, (My_Vec3*)&m_matWorld.m[0][0], D3DXToRadian(MouseMove * 10.f * fTimeDelta));

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_CameraDesc.vAt = m_CameraDesc.vEye + vLook;
	}


	return My_Int();
}

My_Int CCamera_Free::LastUpdate(const My_Float & fTimeDelta)
{
	if (CDebugger::GetInstance()->Get_IsFreeCamera() == false)
		return 0;

	// 뷰스페이스 변환 행렬을 만든다.
	Compute_ViewMat();

	// 투영 변환 행렬을 만든다.
	Compute_ProjMat();

	return CCamera::LastUpdate(fTimeDelta);
}

CCamera_Free * CCamera_Free::Create(LPDIRECT3DDEVICE9 pDevice, const CAMERA_DESC * pCameraDesc, const PROJ_DESC * pProjDesc)
{
	CCamera_Free*	pInstance = new CCamera_Free(pDevice);

	if (FAILED(pInstance->Initialize_Prototype(pCameraDesc, pProjDesc)))
	{
		MSG_BOX("CCamera_Free Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCamera_Free::Clone()
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CCamera_Free Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

My_uLong CCamera_Free::ReleaseMemory()
{
	CCamera::ReleaseMemory();
	return My_uLong();
}
