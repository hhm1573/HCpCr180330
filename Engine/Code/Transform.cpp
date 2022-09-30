#include "..\Header\Transform.h"
#include "VIBuffer.h"
#include "Vehicle.h"
#include "MathMgr.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pDevice)
	:CComponent(pDevice)
{
}

CTransform::CTransform(const CTransform & Ref)
	: CComponent(Ref)
{
	m_matWorld = Ref.m_matWorld;
}

HRESULT CTransform::Initialize(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	return NOERROR;
}

void CTransform::SetUp_OnGraphicDev()
{
	if (nullptr == m_pDevice)
		return;

	m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
}

My_Vec3 CTransform::Get_Scale(void)
{
	return D3DXVECTOR3(D3DXVec3Length((My_Vec3*)&m_matWorld.m[INFO_RIGHT][0]),
		D3DXVec3Length((My_Vec3*)&m_matWorld.m[INFO_UP][0]),
		D3DXVec3Length((My_Vec3*)&m_matWorld.m[INFO_LOOK][0]));
}

My_Float CTransform::Get_Height_OnBuffer(const CVIBuffer * pBuffer)
{
	My_Float fReturn = -5.0f;

	const My_Vec3* pPosition = Get_Information(INFO_POSITION);

	My_uLong	dwNumX = pBuffer->Get_NumVertex_X();
	My_uLong	dwNumZ = pBuffer->Get_NumVertex_Z();
	My_Float	fInterval = pBuffer->Get_Interval();

	if (pPosition->x < 0 || pPosition->x >= (dwNumX - 1)*fInterval
		|| pPosition->z < 0 || pPosition->z >= (dwNumZ - 1) * fInterval)
		return fReturn;

	const vector<My_Vec3>* pPosArray = pBuffer->Get_PosArray();

	My_uLong	dwIndex = My_uLong(pPosition->z / fInterval) * dwNumX + My_uLong(pPosition->x / fInterval);

	if (dwIndex < 0 || dwIndex >= dwNumX * dwNumZ)
		return fReturn;

	My_uLong	dwPivotIndex = dwIndex + dwNumX;

	My_Float	fRatioX = (pPosition->x - (*pPosArray)[dwPivotIndex].x) / ((*pPosArray)[dwPivotIndex + 1].x - (*pPosArray)[dwPivotIndex].x);
	My_Float	fRatioZ = ((*pPosArray)[dwPivotIndex].z - pPosition->z) / ((*pPosArray)[dwPivotIndex].z - (*pPosArray)[dwIndex].z);

	My_Float	fHeight[4] = {
		(*pPosArray)[dwIndex + dwNumX].y,
		(*pPosArray)[dwIndex + dwNumX + 1].y,
		(*pPosArray)[dwIndex + 1].y,
		(*pPosArray)[dwIndex].y };

	if (fRatioX >= fRatioZ)
	{
		fReturn = fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;
	}

	else
	{
		fReturn = fHeight[0] + (fHeight[3] - fHeight[0]) * fRatioZ + (fHeight[2] - fHeight[3]) * fRatioX;
	}

	return fReturn;
}

My_Float CTransform::Get_Height_OnBuffer(const CVIBuffer * pBuffer, const My_Vec3 & vPos)
{
	My_Float fReturn = -5.0f;

	My_Vec3 vPosition = vPos;

	My_uLong	dwNumX = pBuffer->Get_NumVertex_X();
	My_uLong	dwNumZ = pBuffer->Get_NumVertex_Z();
	My_Float	fInterval = pBuffer->Get_Interval();

	if (vPosition.x < 0 || vPosition.x >= (dwNumX - 1)*fInterval
		|| vPosition.z < 0 || vPosition.z >= (dwNumZ - 1) * fInterval)
		return fReturn;

	const vector<My_Vec3>* pPosArray = pBuffer->Get_PosArray();

	My_uLong	dwIndex = My_uLong(vPosition.z / fInterval) * dwNumX + My_uLong(vPosition.x / fInterval);

	if (dwIndex < 0 || dwIndex >= dwNumX * dwNumZ)
		return fReturn;

	My_uLong	dwPivotIndex = dwIndex + dwNumX;

	My_Float	fRatioX = (vPosition.x - (*pPosArray)[dwPivotIndex].x) / ((*pPosArray)[dwPivotIndex + 1].x - (*pPosArray)[dwPivotIndex].x);
	My_Float	fRatioZ = ((*pPosArray)[dwPivotIndex].z - vPosition.z) / ((*pPosArray)[dwPivotIndex].z - (*pPosArray)[dwIndex].z);

	My_Float	fHeight[4] = {
		(*pPosArray)[dwIndex + dwNumX].y,
		(*pPosArray)[dwIndex + dwNumX + 1].y,
		(*pPosArray)[dwIndex + 1].y,
		(*pPosArray)[dwIndex].y };

	if (fRatioX >= fRatioZ)
	{
		fReturn = fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;
	}

	else
	{
		fReturn = fHeight[0] + (fHeight[3] - fHeight[0]) * fRatioZ + (fHeight[2] - fHeight[3]) * fRatioX;
	}

	return fReturn;
}

My_Float CTransform::Get_Height_OnNav()
{
	if (m_pNavMesh == nullptr)
		return -10.0f;

	My_Float fReturn = -5.0f;

	const My_Vec3* pPosition = Get_Information(INFO_POSITION);

	My_Float	fRatioX = (pPosition->x - m_pNavMesh->vLT.x) / (m_pNavMesh->vRT.x - m_pNavMesh->vLT.x);
	My_Float	fRatioZ = (m_pNavMesh->vLT.z - pPosition->z) / (m_pNavMesh->vLT.z - m_pNavMesh->vLB.z);

	My_Float	fHeight[4] = {
		m_pNavMesh->vLT.y,
		m_pNavMesh->vRT.y,
		m_pNavMesh->vRB.y,
		m_pNavMesh->vLB.y };

	if (fRatioX >= fRatioZ)
	{
		fReturn = fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;
	}

	else
	{
		fReturn = fHeight[0] + (fHeight[3] - fHeight[0]) * fRatioZ + (fHeight[2] - fHeight[3]) * fRatioX;
	}

	return fReturn;
}


void CTransform::Move_Forward(const My_Float & fSpdPerSec)
{
	My_Vec3		vLook;

	memcpy(&vLook, &m_matWorld.m[INFO_LOOK][0], sizeof(My_Vec3));

	*(My_Vec3*)&m_matWorld.m[INFO_POSITION][0] += *D3DXVec3Normalize(&vLook, &vLook) * fSpdPerSec;
}

void CTransform::Move_Backward(const My_Float & fSpdPerSec)
{
	My_Vec3		vLook;

	memcpy(&vLook, &m_matWorld.m[INFO_LOOK][0], sizeof(My_Vec3));

	*(My_Vec3*)&m_matWorld.m[INFO_POSITION][0] -= *D3DXVec3Normalize(&vLook, &vLook) * fSpdPerSec;
}

void CTransform::Move_Right(const My_Float & fSpdPerSec)
{
	My_Vec3		vRight;

	memcpy(&vRight, &m_matWorld.m[INFO_RIGHT][0], sizeof(My_Vec3));

	*(My_Vec3*)&m_matWorld.m[INFO_POSITION][0] += *D3DXVec3Normalize(&vRight, &vRight) * fSpdPerSec;
}

void CTransform::Move_Left(const My_Float & fSpdPerSec)
{
	My_Vec3		vRight;

	memcpy(&vRight, &m_matWorld.m[INFO_RIGHT][0], sizeof(My_Vec3));

	*(My_Vec3*)&m_matWorld.m[INFO_POSITION][0] -= *D3DXVec3Normalize(&vRight, &vRight) * fSpdPerSec;
}

void CTransform::Move_Up(const My_Float & fSpdPerSec)
{
	My_Vec3		vUp;

	memcpy(&vUp, &m_matWorld.m[INFO_UP][0], sizeof(My_Vec3));

	*(My_Vec3*)&m_matWorld.m[INFO_POSITION][0] += *D3DXVec3Normalize(&vUp, &vUp) * fSpdPerSec;
}

void CTransform::Move_Down(const My_Float & fSpdPerSec)
{
	My_Vec3		vUp;

	memcpy(&vUp, &m_matWorld.m[INFO_UP][0], sizeof(My_Vec3));

	*(My_Vec3*)&m_matWorld.m[INFO_POSITION][0] -= *D3DXVec3Normalize(&vUp, &vUp) * fSpdPerSec;
}

void CTransform::Move_Position(const My_Vec3 & vPos)
{
	memcpy_s(&m_matWorld.m[INFO_POSITION][0], sizeof(My_Vec3), vPos, sizeof(My_Vec3));
}

void CTransform::Move_Free(const My_Vec3 * pvVelocity, const My_Float & fTimeDelta)
{
	*(My_Vec3*)&m_matWorld.m[INFO_POSITION][0] += (*pvVelocity) * fTimeDelta;
}

My_Bool CTransform::Set_NavMesh(const NAVMESH * pNavMesh)
{
	if (pNavMesh == nullptr)
		return false;

	My_Vec3		vLT = My_Vec3(pNavMesh->vLT.x, 0.0f, pNavMesh->vLT.z);
	My_Vec3		vRB = My_Vec3(pNavMesh->vRB.x, 0.0f, pNavMesh->vRB.z);

	My_Vec3		vPos = *(My_Vec3*)&m_matWorld.m[INFO_POSITION][0];

	if (vPos.x >= vLT.x && vPos.x <= vRB.x &&
		vPos.z >= vRB.z && vPos.z <= vLT.z) {
		m_pNavMesh = pNavMesh;
		return true;
	}

	return false;
}

My_Bool CTransform::Move_On_NavMesh(const My_Vec3 * pvVelocity, const CVIBuffer * pBuffer, const My_Float& fTimeDelta, CVehicle* pVehicle, My_Bool* pbIsLanded)
{
	My_Float	fFallSpd = 9.8f * 1.5f;

	My_Vec3	vPos = *Get_Information(INFO_POSITION);
	My_Vec3 vAfter = vPos + (*pvVelocity);

	if (m_pNavMesh == nullptr) {
		if (pBuffer != nullptr) {
			My_Float fHeight = Get_Height_OnBuffer(pBuffer, vAfter);
			if (vAfter.y > fHeight) {
				*pbIsLanded = false;
				//vAfter.y = vPos.y - fFallSpd * fTimeDelta;
			}
			else {
				*pbIsLanded = true;
				pVehicle->Set_Velocity_Y(0.0f);
				vAfter.y = fHeight;
			}
		}
		Set_Information(INFO_POSITION, &vAfter);
		m_iTest = 1;
		return false;
	}

	if (vPos.x < m_pNavMesh->vLT.x || vPos.x > m_pNavMesh->vRT.x ||
		vPos.z < m_pNavMesh->vLB.z || vPos.z > m_pNavMesh->vLT.z) {

		m_pNavMesh = nullptr;

		My_Int iTest = m_iTest;

		if (pBuffer != nullptr) {
			My_Float fHeight = Get_Height_OnBuffer(pBuffer, vAfter);
			if (vAfter.y > fHeight) {
				*pbIsLanded = false;
				//vAfter.y = vPos.y - fFallSpd * fTimeDelta;
			}
			else {
				*pbIsLanded = true;
				pVehicle->Set_Velocity_Y(0.0f);
				vAfter.y = fHeight;
			}
		}
		Set_Information(INFO_POSITION, &vAfter);
		return false;
	}

	POINTFLOAT	ptMy0 = { vPos.x,vPos.z };
	POINTFLOAT	ptMy1 = { vAfter.x,vAfter.z };
	POINTFLOAT	ptMesh0, ptMesh1;

	POINTFLOAT	ptMeshLT = { m_pNavMesh->vLT.x,m_pNavMesh->vLT.z };
	POINTFLOAT	ptMeshRT = { m_pNavMesh->vRT.x,m_pNavMesh->vRT.z };
	POINTFLOAT	ptMeshLB = { m_pNavMesh->vLB.x,m_pNavMesh->vLB.z };
	POINTFLOAT	ptMeshRB = { m_pNavMesh->vRB.x,m_pNavMesh->vRB.z };
	POINTFLOAT	ArrPtMesh[4] = { ptMeshLT,ptMeshRT,ptMeshRB,ptMeshLB };

	POINTFLOAT	ptIntersect;

	My_Bool		bIsIntersect = false;


	for (int i = 0; i < 4; ++i) {
		ptMesh0 = ArrPtMesh[i];
		ptMesh1 = (i >= 3) ? ArrPtMesh[0] : ArrPtMesh[i + 1];
		if (CMathMgr::IntersectLine(ptMy0, ptMy1, ptMesh0, ptMesh1, &ptIntersect) == true) {
			bIsIntersect = true;
			break;
		}
	}

	if ((ptIntersect.x == m_pNavMesh->vLT.x && ptIntersect.y == m_pNavMesh->vLT.z) ||
		(ptIntersect.x == m_pNavMesh->vRT.x && ptIntersect.y == m_pNavMesh->vRT.z) ||
		(ptIntersect.x == m_pNavMesh->vLB.x && ptIntersect.y == m_pNavMesh->vLB.z) ||
		(ptIntersect.x == m_pNavMesh->vRB.x && ptIntersect.y == m_pNavMesh->vRB.z)) {
		My_Vec3 vCenter = My_Vec3((m_pNavMesh->vLT.x + m_pNavMesh->vRB.x) * 0.5f, 0.0f, (m_pNavMesh->vRT.z + m_pNavMesh->vRB.z) * 0.5f);
		My_Vec3 vDirToCenter = vCenter - vPos;
		vDirToCenter.y = 0.0f;
		My_Vec3	vDir_Nor;
		D3DXVec3Normalize(&vDir_Nor, &vDirToCenter);
		My_Int	iRandX = rand();
		My_Int	iRandY = rand();
		My_Float fRandX = My_Float(iRandX % 10) / 100.0f;
		My_Float fRandY = My_Float(iRandY % 10) / 100.0f;
		vDirToCenter.x *= fRandX;
		vDirToCenter.z *= fRandY;
		My_Vec3	vPosFixed = vPos + vDirToCenter;
		Set_Information(INFO_POSITION, &vPosFixed);
		m_iTest = 2;
		return Move_On_NavMesh(pvVelocity, pBuffer, fTimeDelta, pVehicle, pbIsLanded);
	}

	if (bIsIntersect == false) {
		My_Float fHeightBuffer = -25.0f;
		if (pBuffer != nullptr)
			fHeightBuffer = Get_Height_OnBuffer(pBuffer, vAfter);
		My_Float fHeightNav = Get_Height_OnNav();
		My_Float fHigherHeight = (fHeightBuffer > fHeightNav) ? fHeightBuffer : fHeightNav;

		if (vAfter.y > fHigherHeight) {
			*pbIsLanded = false;
			//vAfter.y = vPos.y - fFallSpd * fTimeDelta;
		}
		else {
			*pbIsLanded = true;
			pVehicle->Set_Velocity_Y(0.0f);
			vAfter.y = fHigherHeight;
		}
		//vAfter.y = (vAfter.y - fFallSpd*fTimeDelta > fHigherHeight) ? vAfter.y - fFallSpd * fTimeDelta : fHigherHeight;
		Set_Information(INFO_POSITION, &vAfter);
		m_iTest = 3;
		return true;
	}	//외곽에 충돌안했을때의처리



	My_Vec2	vNavCenter = My_Vec2((m_pNavMesh->vLT.x + m_pNavMesh->vRB.x) * 0.5f, (m_pNavMesh->vLT.z + m_pNavMesh->vRB.z) * 0.5f);
	My_Vec2 vDir = My_Vec2(ptIntersect.x - vNavCenter.x, ptIntersect.y - vNavCenter.y);
	My_Vec2 vLook = My_Vec2(0.0f, 1.0f);
	D3DXVec2Normalize(&vDir, &vDir);
	My_Float fCosf = D3DXVec2Dot(&vDir, &vLook);
	My_Float fRadian = acosf(fCosf);
	My_Float fDegree = D3DXToDegree(fRadian);
	NAVMESH*	pTargetMesh = nullptr;

	My_uShort	wCollisionSide = 0;

	if (fDegree < 45.0f) {
		pTargetMesh = m_pNavMesh->pU;
		wCollisionSide = 0x0008;
	}
	else if (fDegree < 135.0f) {
		if (ptIntersect.x < vNavCenter.x) {
			pTargetMesh = m_pNavMesh->pL;
			wCollisionSide = 0x0002;
		}
		else {
			pTargetMesh = m_pNavMesh->pR;
			wCollisionSide = 0x0001;
		}
	}
	else {
		pTargetMesh = m_pNavMesh->pB;
		wCollisionSide = 0x0004;
	}



	if (pTargetMesh == nullptr) {		//건너편 메쉬가 없을때의 처리
		if (m_pNavMesh->wState & 0x8000 && m_pNavMesh->wState & wCollisionSide) {

			My_Float fHeightBuffer = -25.0f;
			if (pBuffer != nullptr)
				fHeightBuffer = Get_Height_OnBuffer(pBuffer);
			My_Float fHeightNav = Get_Height_OnNav();
			My_Float fHigherHeight = (fHeightBuffer > fHeightNav) ? fHeightBuffer : fHeightNav;

			if (vPos.y - fFallSpd * fTimeDelta > fHigherHeight) {
				*pbIsLanded = false;
				vAfter.y = vPos.y - fFallSpd * fTimeDelta;
			}
			else {
				*pbIsLanded = true;
				vAfter.y = fHigherHeight;
			}
			//vAfter.y = (vAfter.y - fFallSpd*fTimeDelta > fHigherHeight) ? vAfter.y - fFallSpd * fTimeDelta : fHigherHeight;
			Set_Information(INFO_POSITION, &vAfter);
			m_pNavMesh = nullptr;

			m_iTest = 3;

			return false;
		}
		else {
			My_Vec3		vIntersectPoint = My_Vec3(ptIntersect.x, 0.0f, ptIntersect.y);
			My_Vec3		vOverVelocity = vAfter - vIntersectPoint;
			My_Vec3		vApplyVelocity = *pvVelocity;
			vOverVelocity.y = 0.0f;
			if (wCollisionSide & 0x0008 || wCollisionSide & 0x0004) {
				vOverVelocity.z *= -1.0f;
				vApplyVelocity.z *= -1.0f;
			}
			else {
				vOverVelocity.x *= -1.0f;
				vApplyVelocity.x *= -1.0f;
			}
			vApplyVelocity += vOverVelocity;
			if (wCollisionSide & 0x0008 || wCollisionSide & 0x0004)
				vApplyVelocity.x = 0.0f;
			else
				vApplyVelocity.z = 0.0f;

			My_Vec3		vCenter = My_Vec3((m_pNavMesh->vLT.x + m_pNavMesh->vRB.x)*0.5f, 0.0f, (m_pNavMesh->vLT.z + m_pNavMesh->vRB.z) * 0.5f);
			My_Vec3		vCenterDir = vCenter - vIntersectPoint;
			D3DXVec3Normalize(&vCenterDir, &vCenterDir);

			if (wCollisionSide & 0x0008 || wCollisionSide & 0x0004)
				vCenterDir.x = 0.0f;
			else
				vCenterDir.z = 0.0f;

			vIntersectPoint += vCenterDir * 0.01f;
			vIntersectPoint.y = vPos.y;
			Set_Information(INFO_POSITION, &vIntersectPoint);
			pVehicle->Apply_Velocity(vApplyVelocity / fTimeDelta * 1.0f);
			if (vIntersectPoint.x < m_pNavMesh->vLT.x || vIntersectPoint.x > m_pNavMesh->vRT.x ||
				vIntersectPoint.z < m_pNavMesh->vLB.z || vIntersectPoint.z > m_pNavMesh->vLT.z) {
				int i = 0;
			}
			m_iTest = 4;
			return Move_On_NavMesh(&vOverVelocity, pBuffer, fTimeDelta, pVehicle, pbIsLanded);
		}
	}



	//My_Vec3		vIntersectAfter = My_Vec3(ptIntersect.x, vPos.y, ptIntersect.y);
	//My_Vec3		vVelocityDir = *pvVelocity;
	//D3DXVec3Normalize(&vVelocityDir, &vVelocityDir);
	//vIntersectAfter += vVelocityDir * 0.1f;		//vIntersectAfter = 다음호출때의 내위치

	//My_Vec3		vIntersectDir = vIntersectAfter - vPos;
	//My_Vec3		vVelocityAfter = (*pvVelocity) - vIntersectDir;

	//m_pNavMesh = pTargetMesh;
	//Set_Information(INFO_POSITION, &vIntersectAfter);
	My_Vec3		vTargetCenter = My_Vec3((pTargetMesh->vLT.x + pTargetMesh->vRB.x) * 0.5f, 0.0f, (pTargetMesh->vLT.z + pTargetMesh->vRB.z) * 0.5f);
	My_Vec3		vIntersectPoint = My_Vec3(ptIntersect.x, 0.0f, ptIntersect.y);

	My_Vec3		vTargetCenterDir = vTargetCenter - vIntersectPoint;
	D3DXVec3Normalize(&vTargetCenterDir, &vTargetCenterDir);

	My_Vec3		vIntersectAfter = vIntersectPoint + vTargetCenterDir * 0.01f;
	vIntersectAfter.y = vPos.y;
	Set_Information(INFO_POSITION, &vIntersectAfter);

	My_Vec3		vInvVelocityDir = vIntersectPoint - vPos;
	vInvVelocityDir.y = 0.0f;
	My_Vec3		vVelocityAfter = (*pvVelocity) - vInvVelocityDir;

	m_pNavMesh = pTargetMesh;
	m_iTest = 5;
	return Move_On_NavMesh(&vVelocityAfter, pBuffer, fTimeDelta, pVehicle, pbIsLanded);
}


void CTransform::Rotation_X(const My_Float & fRadPerSec)
{
	Engine::My_Vec3		vAxisX, vAxisY, vAxisZ;
	memcpy_s(&vAxisX, sizeof(Engine::My_Vec3), &m_matWorld.m[0][0], sizeof(Engine::My_Vec3));
	memcpy_s(&vAxisY, sizeof(Engine::My_Vec3), &m_matWorld.m[1][0], sizeof(Engine::My_Vec3));
	memcpy_s(&vAxisZ, sizeof(Engine::My_Vec3), &m_matWorld.m[2][0], sizeof(Engine::My_Vec3));

	Engine::My_Matrix	matRot;
	D3DXMatrixRotationAxis(&matRot, &vAxisX, fRadPerSec);

	D3DXVec3TransformNormal(&vAxisY, &vAxisY, &matRot);
	D3DXVec3TransformNormal(&vAxisZ, &vAxisZ, &matRot);

	memcpy_s(&m_matWorld.m[1][0], sizeof(Engine::My_Vec3), &vAxisY, sizeof(Engine::My_Vec3));
	memcpy_s(&m_matWorld.m[2][0], sizeof(Engine::My_Vec3), &vAxisZ, sizeof(Engine::My_Vec3));
}

void CTransform::Rotation_Y(const My_Float & fRadPerSec)
{
	Engine::My_Vec3		vAxisX, vAxisY, vAxisZ;
	memcpy_s(&vAxisX, sizeof(Engine::My_Vec3), &m_matWorld.m[0][0], sizeof(Engine::My_Vec3));
	memcpy_s(&vAxisY, sizeof(Engine::My_Vec3), &m_matWorld.m[1][0], sizeof(Engine::My_Vec3));
	memcpy_s(&vAxisZ, sizeof(Engine::My_Vec3), &m_matWorld.m[2][0], sizeof(Engine::My_Vec3));

	Engine::My_Matrix	matRot;
	D3DXMatrixRotationAxis(&matRot, &vAxisY, fRadPerSec);

	D3DXVec3TransformNormal(&vAxisX, &vAxisX, &matRot);
	D3DXVec3TransformNormal(&vAxisZ, &vAxisZ, &matRot);

	memcpy_s(&m_matWorld.m[0][0], sizeof(Engine::My_Vec3), &vAxisX, sizeof(Engine::My_Vec3));
	memcpy_s(&m_matWorld.m[2][0], sizeof(Engine::My_Vec3), &vAxisZ, sizeof(Engine::My_Vec3));
}

void CTransform::Rotation_Z(const My_Float & fRadPerSec)
{
	Engine::My_Vec3		vAxisX, vAxisY, vAxisZ;
	memcpy_s(&vAxisX, sizeof(Engine::My_Vec3), &m_matWorld.m[0][0], sizeof(Engine::My_Vec3));
	memcpy_s(&vAxisY, sizeof(Engine::My_Vec3), &m_matWorld.m[1][0], sizeof(Engine::My_Vec3));
	memcpy_s(&vAxisZ, sizeof(Engine::My_Vec3), &m_matWorld.m[2][0], sizeof(Engine::My_Vec3));

	Engine::My_Matrix	matRot;
	D3DXMatrixRotationAxis(&matRot, &vAxisZ, fRadPerSec);

	D3DXVec3TransformNormal(&vAxisY, &vAxisY, &matRot);
	D3DXVec3TransformNormal(&vAxisX, &vAxisX, &matRot);

	memcpy_s(&m_matWorld.m[0][0], sizeof(Engine::My_Vec3), &vAxisX, sizeof(Engine::My_Vec3));
	memcpy_s(&m_matWorld.m[1][0], sizeof(Engine::My_Vec3), &vAxisY, sizeof(Engine::My_Vec3));
}

void CTransform::Rotation_Free(const My_Vec3 & vAxis, const My_Float & fRadPerSec)
{
}

void CTransform::RotationYawPitchRoll(const My_Float radianX, const My_Float radianY, const My_Float radianZ)
{
	if (0 == radianX && 0 == radianY && 0 == radianZ)
		return;

	My_Matrix matWorld = m_matWorld;

	D3DXMatrixRotationYawPitchRoll(&m_matWorld, radianY, radianX, radianZ);

	*(My_Vec3*)&m_matWorld.m[INFO_POSITION][0] = *(My_Vec3*)&matWorld.m[INFO_POSITION][0];

	My_Vec3         vRight, vUp, vLook;
	memcpy(&vRight, &matWorld.m[INFO_RIGHT][0], sizeof(My_Vec3));
	memcpy(&vUp, &matWorld.m[INFO_UP][0], sizeof(My_Vec3));
	memcpy(&vLook, &matWorld.m[INFO_LOOK][0], sizeof(My_Vec3));

	memcpy(&m_matWorld.m[INFO_RIGHT][0], &(*(My_Vec3*)&m_matWorld.m[INFO_RIGHT][0] * D3DXVec3Length(&vRight)), sizeof(My_Vec3));
	memcpy(&m_matWorld.m[INFO_UP][0], &(*(My_Vec3*)&m_matWorld.m[INFO_UP][0] * D3DXVec3Length(&vUp)), sizeof(My_Vec3));
	memcpy(&m_matWorld.m[INFO_LOOK][0], &(*(My_Vec3*)&m_matWorld.m[INFO_LOOK][0] * D3DXVec3Length(&vLook)), sizeof(My_Vec3));
}

void CTransform::Scaling(const My_Float & fX, const My_Float & fY, const My_Float & fZ)
{
	My_Vec3		vRight, vUp, vLook;
	memcpy(&vRight, &m_matWorld.m[INFO_RIGHT][0], sizeof(My_Vec3));
	memcpy(&vUp, &m_matWorld.m[INFO_UP][0], sizeof(My_Vec3));
	memcpy(&vLook, &m_matWorld.m[INFO_LOOK][0], sizeof(My_Vec3));

	memcpy(&m_matWorld.m[INFO_RIGHT][0], &(*D3DXVec3Normalize(&vRight, &vRight) * fX), sizeof(My_Vec3));
	memcpy(&m_matWorld.m[INFO_UP][0], &(*D3DXVec3Normalize(&vUp, &vUp) * fY), sizeof(My_Vec3));
	memcpy(&m_matWorld.m[INFO_LOOK][0], &(*D3DXVec3Normalize(&vLook, &vLook) * fZ), sizeof(My_Vec3));
}

void CTransform::Set_HeightOnBuffer(const CVIBuffer * pBuffer)
{
	const My_Vec3* pPosition = Get_Information(INFO_POSITION);

	My_uLong	dwNumX = pBuffer->Get_NumVertex_X();
	My_uLong	dwNumZ = pBuffer->Get_NumVertex_Z();
	My_Float	fInterval = pBuffer->Get_Interval();

	if (pPosition->x < 0 || pPosition->x >= (dwNumX - 1)*fInterval
		|| pPosition->z < 0 || pPosition->z >= (dwNumZ - 1) * fInterval)
		return;

	const vector<My_Vec3>* pPosArray = pBuffer->Get_PosArray();

	My_uLong	dwIndex = My_uLong(pPosition->z / fInterval) * dwNumX + My_uLong(pPosition->x / fInterval);

	if (dwIndex < 0 || dwIndex >= dwNumX * dwNumZ)
		return;

	My_uLong	dwPivotIndex = dwIndex + dwNumX;

	My_Float	fRatioX = (pPosition->x - (*pPosArray)[dwPivotIndex].x) / ((*pPosArray)[dwPivotIndex + 1].x - (*pPosArray)[dwPivotIndex].x);
	My_Float	fRatioZ = ((*pPosArray)[dwPivotIndex].z - pPosition->z) / ((*pPosArray)[dwPivotIndex].z - (*pPosArray)[dwIndex].z);

	My_Float	fHeight[4] = {
		(*pPosArray)[dwIndex + dwNumX].y,
		(*pPosArray)[dwIndex + dwNumX + 1].y,
		(*pPosArray)[dwIndex + 1].y,
		(*pPosArray)[dwIndex].y };

	if (fRatioX >= fRatioZ)
	{
		m_matWorld.m[3][1] = fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;
	}

	else
	{
		m_matWorld.m[3][1] = fHeight[0] + (fHeight[3] - fHeight[0]) * fRatioZ + (fHeight[2] - fHeight[3]) * fRatioX;
	}
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTransform*	pInstance = new CTransform(pDevice);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CTransform Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTransform::Clone()
{
	return new CTransform(*this);
}

My_uLong CTransform::ReleaseMemory()
{
	CComponent::ReleaseMemory();

	return My_uLong();
}
