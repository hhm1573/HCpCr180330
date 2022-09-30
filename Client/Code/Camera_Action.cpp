#include "stdafx.h"
#include "..\Header\Camera_Action.h"

CCamera_Action::CCamera_Action(LPDIRECT3DDEVICE9 pDevice)
	:CCamera(pDevice), mIsStage(false)
{
}

CCamera_Action::CCamera_Action(const CCamera_Action & Ref)
	: CCamera(Ref), mIsStage(Ref.mIsStage), mState(Ref.mState), mAngle(Ref.mAngle)
{
}

HRESULT CCamera_Action::Initialize()
{
	Compute_ViewMat();
	Compute_ProjMat();

	return NOERROR;
}

HRESULT CCamera_Action::Initialize_Prototype(const CAMERA_DESC & pCameraDesc, const PROJ_DESC & pProjDesc)
{
	if (FAILED(CCamera::Initialize(&pCameraDesc, &pProjDesc)))
		return E_FAIL;

	m_CameraDesc.vEye = { 422.f, 20.f, 286.5f };
	m_CameraDesc.vAt = { 422.f, 10.f, 306.5f };

	return NOERROR;
}

My_Int CCamera_Action::Update(const My_Float & fTimeDelta)
{
	if (mIsStage)
		return My_Int();

	Engine::My_Float		fSpd = 25.0f;

	if (START_POINT == mState)
	{
		My_Vec3			vLook;
		My_Vec3			vZ = { 0.f, 0.f, 1.f };

		m_CameraDesc.vEye += *D3DXVec3Normalize(&vLook, &vZ) * fSpd * fTimeDelta;
		m_CameraDesc.vAt += *D3DXVec3Normalize(&vLook, &vZ) * fSpd * fTimeDelta;

		if (m_CameraDesc.vEye.z > 432.f)
		{
			m_CameraDesc.vEye.z = 432.f;
			mState = ROT1;
		}
	}

	else if (ROT1 == mState)
	{
		mAngle -= fTimeDelta * 40.f;

		My_Vec3			vLook = m_CameraDesc.vAt - m_CameraDesc.vEye;

		My_Matrix			matRot;
		D3DXMatrixRotationY(&matRot, D3DXToRadian(fTimeDelta * - 40.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_CameraDesc.vAt = m_CameraDesc.vEye + vLook;

		if (mAngle < -90.f)
		{
			mAngle += 90.f;

			D3DXMatrixRotationY(&matRot, D3DXToRadian(-mAngle));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_CameraDesc.vAt = m_CameraDesc.vEye + vLook;
			mState = ROT_HOUSE;
			mAngle = 0.f;
		}
	}

	else if (ROT_HOUSE == mState)
	{
		My_Vec3			vLook;
		My_Vec3			vX = { -1.f, 0.f, 0.f };

		m_CameraDesc.vEye += *D3DXVec3Normalize(&vLook, &vX) * fSpd * fTimeDelta;
		m_CameraDesc.vAt += *D3DXVec3Normalize(&vLook, &vX) * fSpd * fTimeDelta;

		if (m_CameraDesc.vEye.x < 120.f)
		{
			m_CameraDesc.vEye.x = 120.f;
			mState = ROT2;
		}
	}

	else if (ROT2 == mState)
	{
		mAngle -= fTimeDelta * 40.f;

		My_Vec3			vLook = m_CameraDesc.vAt - m_CameraDesc.vEye;

		My_Matrix			matRot;
		D3DXMatrixRotationY(&matRot, D3DXToRadian(fTimeDelta * - 40.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_CameraDesc.vAt = m_CameraDesc.vEye + vLook;

		if (mAngle < -90.f)
		{
			mAngle += 90.f;

			D3DXMatrixRotationY(&matRot, D3DXToRadian(-mAngle));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_CameraDesc.vAt = m_CameraDesc.vEye + vLook;
			mState = TETRIS;
			mAngle = 0.f;
		}
	}

	else if (TETRIS == mState)
	{
		My_Vec3			vLook;
		My_Vec3			vZ = { 0.f, 0.f, -1.f };

		m_CameraDesc.vEye += *D3DXVec3Normalize(&vLook, &vZ) * fSpd * fTimeDelta;
		m_CameraDesc.vAt += *D3DXVec3Normalize(&vLook, &vZ) * fSpd * fTimeDelta;

		if (m_CameraDesc.vEye.z < 110.f)
		{
			m_CameraDesc.vEye.z = 110.f;
			mState = ROT3;
		}
	}

	else if (ROT3 == mState)
	{
		mAngle -= fTimeDelta * 40.f;

		My_Vec3			vLook = m_CameraDesc.vAt - m_CameraDesc.vEye;

		My_Matrix			matRot;
		D3DXMatrixRotationY(&matRot, D3DXToRadian(fTimeDelta * - 40.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_CameraDesc.vAt = m_CameraDesc.vEye + vLook;

		if (mAngle < -90.f)
		{
			mAngle += 90.f;

			D3DXMatrixRotationY(&matRot, D3DXToRadian(-mAngle));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_CameraDesc.vAt = m_CameraDesc.vEye + vLook;
			mState = CROCDILE;
			mAngle = 0.f;
		}
	}

	else if (CROCDILE == mState)
	{
		My_Vec3			vLook;
		My_Vec3			vX = { 1.f, 0.f, 0.f };

		m_CameraDesc.vEye += *D3DXVec3Normalize(&vLook, &vX) * fSpd * fTimeDelta;
		m_CameraDesc.vAt += *D3DXVec3Normalize(&vLook, &vX) * fSpd * fTimeDelta;

		if (m_CameraDesc.vEye.x > 422.f)
		{
			m_CameraDesc.vEye.x = 422.f;
			mState = ROT4;
		}
	}

	else if (ROT4 == mState)
	{
		mAngle -= fTimeDelta * 40.f;

		My_Vec3			vLook = m_CameraDesc.vAt - m_CameraDesc.vEye;

		My_Matrix			matRot;
		D3DXMatrixRotationY(&matRot, D3DXToRadian(fTimeDelta * - 40.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_CameraDesc.vAt = m_CameraDesc.vEye + vLook;

		if (mAngle < -90.f)
		{
			mAngle += 90.f;

			D3DXMatrixRotationY(&matRot, D3DXToRadian(-mAngle));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_CameraDesc.vAt = m_CameraDesc.vEye + vLook;
			mState = END_POINT;
			mAngle = 0.f;
		}
	}

	else if (END_POINT == mState)
	{
		My_Vec3			vLook;
		My_Vec3			vZ = { 0.f, 0.f, 1.f };

		m_CameraDesc.vEye += *D3DXVec3Normalize(&vLook, &vZ) * fSpd * fTimeDelta;
		m_CameraDesc.vAt += *D3DXVec3Normalize(&vLook, &vZ) * fSpd * fTimeDelta;

		if (m_CameraDesc.vEye.z > 286.5f)
		{
			m_CameraDesc.vEye.z = 286.5f;
			mState = START_POINT;
		}
	}

	return My_Int();
}

My_Int CCamera_Action::LastUpdate(const My_Float & fTimeDelta)
{
	if (mIsStage)
		return My_Int();

	Compute_ViewMat();
	Compute_ProjMat();

	return CCamera::LastUpdate(fTimeDelta);
}

CCamera_Action * CCamera_Action::Create(LPDIRECT3DDEVICE9 pDevice, const CAMERA_DESC & pCameraDesc, const PROJ_DESC &pProjDesc)
{
	CCamera_Action*	pInstance = new CCamera_Action(pDevice);

	if (FAILED(pInstance->Initialize_Prototype(pCameraDesc, pProjDesc)))
	{
		MSG_BOX("CCamera_Action Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCamera_Action::Clone()
{
	CCamera_Action* pInstance = new CCamera_Action(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CCamera_Action Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

My_uLong CCamera_Action::ReleaseMemory()
{
	CCamera::ReleaseMemory();
	return My_uLong();
}
