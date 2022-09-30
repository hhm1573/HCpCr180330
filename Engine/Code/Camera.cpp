#include "..\Header\Camera.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
	m_pInputDev = CInputDev::GetInstance();
	m_pInputDev->AddRef();
}

CCamera::CCamera(const CCamera & Ref)
	:CGameObject(Ref)
{
	m_CameraDesc = Ref.m_CameraDesc;
	m_ProjDesc = Ref.m_ProjDesc;

	m_matWorld = Ref.m_matWorld;
	m_matView = Ref.m_matView;
	m_matProj = Ref.m_matProj;

	m_pInputDev = Ref.m_pInputDev;
	m_pInputDev->AddRef();
}

HRESULT CCamera::Initialize(const CAMERA_DESC * pCameraDesc, const PROJ_DESC * pProjDesc)
{
	D3DXMatrixIdentity(&m_matWorld);

	m_CameraDesc = *pCameraDesc;

	m_ProjDesc = *pProjDesc;

	return NOERROR;
}

My_Int CCamera::Update(const My_Float & fTimeDelta)
{
	return My_Int();
}

My_Int CCamera::LastUpdate(const My_Float & fTimeDelta)
{
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return My_Int();
}

HRESULT CCamera::Compute_ViewMat()
{
	D3DXMatrixIdentity(&m_matWorld);

	My_Vec3		vPosition = m_CameraDesc.vEye;

	memcpy(&m_matWorld.m[3][0], &vPosition, sizeof(My_Vec3));

	My_Vec3		vLook = m_CameraDesc.vAt - m_CameraDesc.vEye;

	memcpy(&m_matWorld.m[2][0], D3DXVec3Normalize(&vLook, &vLook), sizeof(My_Vec3));

	My_Vec3		vRight = *D3DXVec3Cross(&vRight, &m_CameraDesc.vAxisY, &vLook);

	memcpy(&m_matWorld.m[0][0], D3DXVec3Normalize(&vRight, &vRight), sizeof(My_Vec3));

	My_Vec3		vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);

	memcpy(&m_matWorld.m[1][0], D3DXVec3Normalize(&vUp, &vUp), sizeof(My_Vec3));

	D3DXMatrixInverse(&m_matView, nullptr, &m_matWorld);

	return NOERROR;
}

HRESULT CCamera::Compute_ProjMat()
{
	D3DXMatrixIdentity(&m_matProj);

	m_matProj._11 = (1.f / tanf(m_ProjDesc.fFovy * 0.5f)) / m_ProjDesc.fAspect;
	m_matProj._22 = 1.f / tanf(m_ProjDesc.fFovy * 0.5f);
	m_matProj._33 = m_ProjDesc.fFar / (m_ProjDesc.fFar - m_ProjDesc.fNear);
	m_matProj._43 = -(m_ProjDesc.fFar * m_ProjDesc.fNear) / (m_ProjDesc.fFar - m_ProjDesc.fNear);
	m_matProj._34 = 1.f;
	m_matProj._44 = 0.f;

	return NOERROR;
}

My_uLong CCamera::ReleaseMemory()
{
	Safe_Release(m_pInputDev);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
