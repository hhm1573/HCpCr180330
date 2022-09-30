#include "..\Header\Vehicle.h"

const My_Float		g_fGR = 0.70f;
const My_Float		g_fDR = 2.00f;
const My_Float		g_fTransEff = 0.7f;
const My_Float		g_fWheelR = 0.34f;

const My_Float		g_fEffB = 1.5f;

//const My_Float		g_fCd = 0.4257f;
const My_Float		g_fCd = 0.2128f;
const My_Float		g_fCrr = g_fCd * 50.0f;

const My_Float		g_fRotDeg = 45.0f;

const My_Float		g_fMaxDriftMult = 1.5f;



Engine::CVehicle::CVehicle(LPDIRECT3DDEVICE9 pDevice)
	:CComponent(pDevice)
{
}

Engine::CVehicle::CVehicle(const CVehicle & Ref)
	: CComponent(Ref)
{
	//여기해~~
	//함~

	m_eState = Ref.m_eState;
	m_vV = Ref.m_vV;
	m_vFt = Ref.m_vFt;
	m_vFd = Ref.m_vFd;
	m_vFrr = Ref.m_vFrr;
	m_fM = Ref.m_fM;
	m_fFhp = Ref.m_fFhp;
}

HRESULT Engine::CVehicle::Initialize()
{
	m_vV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vFt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vFd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vFrr = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fM = 0.0f;
	m_fFhp = 0.0f;

	return S_OK;
}

HRESULT Engine::CVehicle::Set_Inform(const My_Float & fM, const My_Float & fFhp, const My_Float& fDistW)
{
	m_fM = fM;
	m_fFhp = fFhp;
	m_fDistW = fDistW;

	return S_OK;
}

HRESULT Engine::CVehicle::Accelerate()
{
	if (m_fFhp <= 0)
		return E_FAIL;
	if (m_fM <= 0)
		return E_FAIL;

	m_eState = STATE_ACCELERATE;

	return S_OK;
}

HRESULT Engine::CVehicle::Braking()
{
	if (m_fFhp <= 0)
		return E_FAIL;
	if (m_fM <= 0)
		return E_FAIL;

	m_eState = STATE_BRAKING;

	return S_OK;
}

HRESULT Engine::CVehicle::Accelerate_Reverse()
{
	if (m_fFhp <= 0)
		return E_FAIL;
	if (m_fM <= 0)
		return E_FAIL;

	m_eState = STATE_REVERSE;

	return S_OK;
}

HRESULT Engine::CVehicle::Turn_Right()
{
	if (m_fDistW <= 0)
		return E_FAIL;

	m_eWhState = WH_STATE_RIGHT;

	return S_OK;
}

HRESULT Engine::CVehicle::Turn_Left()
{
	if (m_fDistW <= 0)
		return E_FAIL;

	m_eWhState = WH_STATE_LEFT;

	return S_OK;
}

HRESULT Engine::CVehicle::Update(const My_Float & fTimeDelta, const My_Vec3* vLook, const My_Bool& bIsLanded)
{
	m_vV += My_Vec3(0.0f, -9.8f * 3.0f * fTimeDelta, 0.0f);

	if (bIsLanded == false)
		return S_OK;

	/*My_Vec3 vFt = vLook * m_fFhp * fTimeDelta;
	My_Vec3 vFd = -g_fCd * m_vVelocity * D3DXVec3Length(&m_vVelocity);
	My_Vec3 vFrr = -g_fCrr * m_vVelocity;
	My_Vec3 vFn = vFt + vFd + vFrr;
	My_Vec3 vA =*/

	if (FAILED(Update_Direction(fTimeDelta, vLook)))
		return E_FAIL;

	//if (D3DXVec3Length(&m_vV) > 5.0f) {
	//	My_Vec2		vLook0 = D3DXVECTOR2(vLook->x,vLook->z);
	//	My_Vec2		vDir = D3DXVECTOR2(m_vV.x,m_vV.z);
	//	D3DXVec2Normalize(&vLook0, &vLook0);
	//	D3DXVec2Normalize(&vDir, &vDir);
	//	My_Float	fCosf = D3DXVec2Dot(&vLook0, &vDir);
	//	My_Float	fRad = acosf(fCosf);
	//	My_Float	fDeg = D3DXToDegree(fRad);
	//	//My_Float	fMult = fabsf(fDeg) / 90.0f;
	//	m_vV -= m_vV * sinf(fRad) * 0.02f;
	//}
	if (D3DXVec3Length(&m_vV) > 0.0f) {
		My_Vec2		vLook0 = D3DXVECTOR2(vLook->x, vLook->z);
		My_Vec2		vDir = D3DXVECTOR2(m_vV.x, m_vV.z);
		D3DXVec2Normalize(&vLook0, &vLook0);
		My_Float	fDot = D3DXVec2Dot(&vLook0, &vDir);
		My_Vec2		vProjection = vLook0 * fDot;
		My_Vec2		vVertical = vProjection - vDir;
		My_Float	fSlipMult = (m_bDrift == true) ? 0.02f : 0.05f;
		My_Vec3		vFriction = D3DXVECTOR3(vVertical.x, 0.0f, vVertical.y) * 0.05;
		m_vV += vFriction;
	}
	/*if (fabsf(fDeg) > 30.0f) {
	m_vV *= 0.5f ;
	}*/

	/*if (FAILED(Update_Angular(fTimeDelta, vLook)))
	return E_FAIL;*/

	if (FAILED(Update_Angular_Adv(fTimeDelta, vLook)))
		return E_FAIL;

	/*My_Vec2 TyreDir = D3DXVECTOR2(vLook->x,vLook->z);
	My_Vec2 MoveDir = D3DXVECTOR2(m_vV.x, m_vV.z);
	D3DXVec2Normalize(&TyreDir, &TyreDir);
	D3DXVec2Normalize(&MoveDir, &MoveDir);*/

	if (m_bDrift == false) {
		m_fDriftMult = 0.0f;
	}

	m_eState = STATE_IDLE;
	m_eWhState = WH_STATE_IDLE;
	m_bDrift = false;

	return S_OK;
}

HRESULT Engine::CVehicle::Drift()
{
	m_bDrift = true;
	return S_OK;
}

HRESULT Engine::CVehicle::Update_Direction(const My_Float & fTimeDelta, const My_Vec3 * vLook)
{
	if (m_eState == STATE_BRAKING && D3DXVec3Length(&m_vV) == 0) {
		return S_OK;
	}

	My_Vec3 vFt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	My_Vec3 vDir;
	D3DXVec3Normalize(&vDir, &m_vV);

	switch (m_eState) {
	case STATE_ACCELERATE:
		vFt = (*vLook) * m_fFhp * g_fGR * g_fDR * g_fTransEff / g_fWheelR;
		break;
	case STATE_BRAKING:
		vFt = -vDir * m_fFhp * g_fEffB;
		break;
	case STATE_REVERSE:
		vFt = -(*vLook) * m_fFhp * g_fGR * g_fDR * g_fTransEff / g_fWheelR;
		break;
	case STATE_IDLE:
		break;
	}

	My_Vec3 vFd = -g_fCd * m_vV * D3DXVec3Length(&m_vV);
	My_Vec3 vFrr = -g_fCrr * m_vV;
	My_Vec3 vFn = vFt + vFd + vFrr;
	My_Vec3 vA = vFn / m_fM;
	My_Vec3 vV = m_vV + vA * fTimeDelta;

	/*if (isnan(vV.x) == true || isnan(vV.y) == true || isnan(vV.z) == true)
	return S_OK;*/

	m_vV += vA * fTimeDelta;

	if (isnan(m_vV.x) == true || isnan(m_vV.y) == true || isnan(m_vV.z) == true)
		m_vV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (m_eState == STATE_BRAKING && D3DXVec3Length(&m_vV) <= 1.0f) {
		m_vV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	return S_OK;
}

HRESULT Engine::CVehicle::Update_Angular(const My_Float & fTimeDelta, const My_Vec3 * vLook)
{
	My_Float	fDeg = 0.0f;

	switch (m_eWhState) {
	case WheelState::WH_STATE_IDLE:
		m_fVa = 0.0f;
		if (m_fWheelrad < 0) {
			m_fWheelrad += D3DXToRadian(5.0f) * fTimeDelta;
			if (m_fWheelrad > 0.0f)
				m_fWheelrad = 0.0f;
		}
		else if (m_fWheelrad > 0) {
			m_fWheelrad -= D3DXToRadian(5.0f) * fTimeDelta;
			if (m_fWheelrad < 0.0f)
				m_fWheelrad = 0.0f;
		}
		//m_fWheelrad = 0.0f;
		//return S_OK;
		break;
	case WheelState::WH_STATE_LEFT:
		fDeg = -g_fRotDeg;
		if (m_fWheelrad >= -D3DXToRadian(1.0f))
			m_fWheelrad -= D3DXToRadian(5.0f) * fTimeDelta;
		break;
	case WheelState::WH_STATE_RIGHT:
		fDeg = g_fRotDeg;
		if (m_fWheelrad <= D3DXToRadian(1.0f))
			m_fWheelrad += D3DXToRadian(5.0f) * fTimeDelta;
		break;
	}

	if (m_fWheelrad > D3DXToRadian(5.0f))
		int i = 0;

	if (m_bDrift == true && D3DXVec3Length(&m_vV) > 10.0f)
		fDeg *= 1.5f;

	/*My_Float	fRds = m_fDistW / sinf(D3DXToRadian(fDeg));
	m_fVa = D3DXVec3Length(&m_vV) / fRds * fTimeDelta;*/
	My_Float	fRds = m_fDistW / sinf(m_fWheelrad);
	m_fVa = D3DXVec3Length(&m_vV) / fRds * fTimeDelta;

	m_eWhState = WH_STATE_IDLE;

	return S_OK;
}

HRESULT Engine::CVehicle::Update_Angular_Adv(const My_Float & fTimeDelta, const My_Vec3 * vLook)
{
	m_fVa = 0.0f;
	if (m_eWhState == WH_STATE_IDLE) {
		m_bDrift = false;
		return S_OK;
	}

	My_Vec3				vLook_Dir;
	D3DXVec3Normalize(&vLook_Dir, vLook);
	My_Vec3				vVel = m_vV;
	My_Float			fDot = D3DXVec3Dot(&vLook_Dir, &vVel);
	My_Vec3				vLookVel = vLook_Dir * fDot;

	My_Vec3				vRight;
	D3DXVec3Cross(&vRight, &My_Vec3(0.0f, 1.0f, 0.0f), vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vRight *= 50.0f;

	My_Float			fLookVelLength = D3DXVec3Length(&vLookVel);
	if (fLookVelLength == 0) {
		m_fVa = 0.0f;
		return S_OK;
	}

	/*My_Vec3				vLookVel_Dir;
	D3DXVec3Normalize(&vLookVel_Dir, &vLookVel);
	if (D3DXVec3Dot(&vLookVel_Dir, &vLook_Dir) >= D3DXToRadian(90.0f)) {
	vLookVel *= -1.0f;
	}*/

	/*if (m_eWhState == WH_STATE_LEFT)
	vRight *= -1.0f;*/

	if (m_bDrift == true) {
		My_Vec3				vVel_Dir;
		D3DXVec3Normalize(&vVel_Dir, &vVel);
		My_Float fVelCosf = D3DXVec3Dot(&vVel_Dir, &vLook_Dir);
		My_Float fVelRad = acosf(fVelCosf);
		My_Float fVelDeg = D3DXToDegree(fVelRad);

		if (fVelRad >= D3DXToRadian(90.0f)) {
			m_bDrift = false;
		}
		else if (fLookVelLength <= 15.0f) {
			m_bDrift = false;
		}
		else {
			m_fDriftMult = (m_fDriftMult + 0.05f > g_fMaxDriftMult) ? g_fMaxDriftMult : m_fDriftMult + 0.05f;
		}
	}

	My_Vec3				vAngledVel = vLookVel + vRight;
	My_Vec3				vAngledVel_Dir;
	D3DXVec3Normalize(&vAngledVel_Dir, &vAngledVel);
	My_Float			fCos = D3DXVec3Dot(&vLook_Dir, &vAngledVel_Dir);
	My_Float			fRad = acosf(fCos);
	My_Float			fDeg = D3DXToDegree(fRad);
	if (fabs(fDeg) >= 45.0f)
		fDeg = 45.0f;
	if (m_eWhState == WH_STATE_LEFT)
		fRad *= -1.0f;

	fRad *= 1.0f + m_fDriftMult;

	m_fVa = fRad * fTimeDelta;
	return S_OK;
}

CVehicle * Engine::CVehicle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CVehicle*		pInstance = new CVehicle(pDevice);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CComponent * Engine::CVehicle::Clone(void)
{
	return new CVehicle(*this);
}

My_uLong Engine::CVehicle::ReleaseMemory()
{
	CComponent::ReleaseMemory();
	return My_uLong();
}
