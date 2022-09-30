#include "stdafx.h"
#include "..\Header\Item_Water_Bomb.h"
#include "ComponentMgr.h"
#include "GameObjectMgr.h"


CItem_Water_Bomb::CItem_Water_Bomb(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_fAccTime(0.f)
	, m_fXZ(0.f)
	, m_fY(0.f)
	, m_bIsFull(false)
	, m_pCollisioner(nullptr)
	, m_pPlayerTrans(nullptr)
{
}

CItem_Water_Bomb::CItem_Water_Bomb(const CItem_Water_Bomb & rhs)
	: CGameObject(rhs)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_pRendererCom(rhs.m_pRendererCom)
	, m_fAccTime(rhs.m_fAccTime)
	, m_fXZ(rhs.m_fXZ)
	, m_fY(rhs.m_fY)
	, m_bIsFull(rhs.m_bIsFull)
	, m_pCollisioner(rhs.m_pCollisioner)
	, m_pPlayerTrans(rhs.m_pPlayerTrans)
	, m_fFirstTime(rhs.m_fFirstTime)
	, m_bIsFirst(rhs.m_bIsFirst)
	, m_iNumber(rhs.m_iNumber)
	, m_iState(rhs.m_iState)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();

	if (nullptr != m_pPlayerTrans)
		m_pPlayerTrans->AddRef();
}

void CItem_Water_Bomb::Set_Player_Matrix(const My_Matrix * pPlayerMatrix)
{
	// Player의 월드 행렬을 인자로 얻어와서 멤버 변수에 할당한다.
	My_Vec3	 m_vRight = *(My_Vec3*)&(*pPlayerMatrix).m[0][0];
	My_Vec3  m_vUp = *(My_Vec3*)&(*pPlayerMatrix).m[1][0];
	My_Vec3  m_vLook = *(My_Vec3*)&(*pPlayerMatrix).m[2][0];
	My_Vec3  m_vPosition = *(My_Vec3*)&(*pPlayerMatrix).m[3][0];

	// 스케일 값이 있으므로 벡터를 길이 1로 만들어준다.
	D3DXVec3Normalize(&m_vRight, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Normalize(&m_vLook, &m_vLook);

	// Player의 위치 + Player의 Look * 거리 = 현재 객체 위치 (공전)
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &(m_vPosition + m_vLook * 100.f));

	// Player의 회전값 * 스케일값
	m_pTransformCom->Set_Information(CTransform::INFO_RIGHT, &m_vRight);
	m_pTransformCom->Set_Information(CTransform::INFO_UP, &m_vUp);
	m_pTransformCom->Set_Information(CTransform::INFO_LOOK, &m_vLook);
}

HRESULT CItem_Water_Bomb::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_pCollisioner = (CCollisioner*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Collisioner");
	if (m_pCollisioner == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Collisioner", m_pCollisioner)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CItem_Water_Bomb::Initialize_Component()
{
	if (nullptr == m_pComponentMgr)
		return E_FAIL;

	// For.Com_Renderer
	m_pRendererCom = (CRenderer*)m_pComponentMgr->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (nullptr == m_pRendererCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Renderer", m_pRendererCom)))
		return E_FAIL;

	// For.Com_Transform
	m_pTransformCom = (CTransform*)m_pComponentMgr->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == m_pTransformCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Transform", m_pTransformCom)))
		return E_FAIL;

	// For.Com_Buffer_Sphere_Buffer
	m_pBufferCom = (CSphere_Buffer*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Sphere_Buffer");
	if (nullptr == m_pBufferCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Sphere_Buffer", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Item_Water_Bomb");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Item_Water_Bomb", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CItem_Water_Bomb::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CItem_Water_Bomb::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return My_Int();
}

My_Int CItem_Water_Bomb::LastUpdate(const My_Float & fTimeDelta)
{
	switch (m_iNumber)
	{
	case 1:
	{
		// 길이가 10 이하면 계속 늘리게 만들기
		if (10.f > m_fXZ && false == m_bIsFull)
		{
			m_fXZ += 1.f;
			m_fY += 0.7f;
		}
		else
		{
			// 계속 줄이게 만들기
			m_bIsFull = true;

			if (true == m_bIsFirst)
			{
				m_fFirstTime = fTimeDelta;
				m_bIsFirst = true;
			}

			m_fAccTime += fTimeDelta;

			if (1.f < m_fAccTime - m_fFirstTime)
			{
				m_fXZ -= 1.f;
				m_fY -= 0.7f;

				// 0 이하면 삭제
				if (m_fXZ < 0.f || m_fY < 0.f)
				{
					m_fAccTime = m_fFirstTime;
					return 1;
				}
			}
		}

		m_pTransformCom->Scaling(m_fXZ, m_fY, m_fXZ);
		m_pTransformCom->Rotation_Y(10 * fTimeDelta);

		m_pCollisioner->Add_Collision(*m_pTransformCom->Get_Information(CTransform::INFO_POSITION), m_fXZ, 4, &m_iState);

	}
	break;
	case 2:		// 플에이어가 물방울에 갇히는 형태
	{
		if (true == m_bIsFirst)
		{
			m_fFirstTime = fTimeDelta;
			m_bIsFirst = true;
		}

		m_fAccTime += fTimeDelta;		// 구가 생성하고 난 뒤의 흘러간 시간을 구함

		// 내 위치에 플레이어의 위치를 대입함
		My_Vec3 pPlayerPosition = *m_pPlayerTrans->Get_Information(CTransform::INFO_POSITION);
		m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &pPlayerPosition);

		m_pTransformCom->Rotation_Y(3 * fTimeDelta);

		// 2초가 지나면 삭제하기
		if (2.f < m_fAccTime - m_fFirstTime)
			return 1;
	}
	break;
	}



	return My_Int();
}

void CItem_Water_Bomb::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pTransformCom->SetUp_OnGraphicDev();

	m_pTextureCom->SetUp_OnGraphicDev(0);

	Set_RenderState();

	m_pBufferCom->Render_Buffer();

	Release_RenderState();
}

void CItem_Water_Bomb::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CItem_Water_Bomb::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CItem_Water_Bomb * CItem_Water_Bomb::Create(LPDIRECT3DDEVICE9 pDevice)
{
	// 원형객체를 생성하는 작업.
	CItem_Water_Bomb*	pInstance = new CItem_Water_Bomb(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CItem_Water_Bomb  Prototype Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CItem_Water_Bomb::Clone()
{
	CItem_Water_Bomb* pInstance = new CItem_Water_Bomb(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CItem_Water_Bomb Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

My_uLong CItem_Water_Bomb::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pCollisioner);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
