#include "stdafx.h"
#include "..\Header\Boss_Skill.h"


CBoss_Skill::CBoss_Skill(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
{
	srand(unsigned(time(NULL)));
}

CBoss_Skill::CBoss_Skill(const CBoss_Skill & rhs)
	: CGameObject(rhs)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_pRendererCom(rhs.m_pRendererCom)
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

void CBoss_Skill::Set_Boss_Matrix(const My_Matrix * pPlayerMatrix)
{
	m_iRand = rand() % 7 - 5;

	// Player의 월드 행렬을 인자로 얻어와서 멤버 변수에 할당한다.
	My_Vec3	 vRight = *(My_Vec3*)&(*pPlayerMatrix).m[0][0];
	My_Vec3  vUp = *(My_Vec3*)&(*pPlayerMatrix).m[1][0];
	m_vLook = *(My_Vec3*)&(*pPlayerMatrix).m[2][0];
	My_Vec3 vPosition = *(My_Vec3*)&(*pPlayerMatrix).m[3][0];
	vPosition = vPosition + m_vLook * 20.f;
	m_vPosition = My_Vec3(vPosition.x + m_iRand, vPosition.y + 2.f, vPosition.z + m_iRand);

	// 스케일 값이 있으므로 벡터를 길이 1로 만들어준다.
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&m_vLook, &m_vLook);

	// Player의 위치 + Player의 Look * 거리 = 현재 객체 위치 (공전)

	// Player의 회전값 * 스케일값
	m_pTransformCom->Set_Information(CTransform::INFO_RIGHT, &vRight);
	m_pTransformCom->Set_Information(CTransform::INFO_UP, &vUp);
	m_pTransformCom->Set_Information(CTransform::INFO_LOOK, &m_vLook);

	//m_pTransformCom->Scaling(2.f, 2.f, 2.f);
}

HRESULT CBoss_Skill::Initialize()
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

HRESULT CBoss_Skill::Initialize_Component()
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

HRESULT CBoss_Skill::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CBoss_Skill::Update(const My_Float & fTimeDelta)
{
	if (true == m_bIsFirst)
	{
		m_bIsFirst = false;
		m_fFirstTime = fTimeDelta;
	}

	m_fAccTime += fTimeDelta;

	if (2.f < (m_fAccTime - m_fFirstTime))
		return 1;

	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &(m_vPosition + m_vLook * 50.f * m_fAccTime));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return My_Int();
}

My_Int CBoss_Skill::LastUpdate(const My_Float & fTimeDelta)
{
	m_pCollisioner->Add_Collision(*m_pTransformCom->Get_Information(CTransform::INFO_POSITION), 2.f, 13, &m_iState);

	return My_Int();
}

void CBoss_Skill::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pTransformCom->SetUp_OnGraphicDev();

	m_pTextureCom->SetUp_OnGraphicDev(0);

	Set_RenderState();

	m_pBufferCom->Render_Buffer();

	Release_RenderState();
}

void CBoss_Skill::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CBoss_Skill::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CBoss_Skill * CBoss_Skill::Create(LPDIRECT3DDEVICE9 pDevice)
{
	// 원형객체를 생성하는 작업.
	CBoss_Skill*	pInstance = new CBoss_Skill(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CBoss_Skill  Prototype Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBoss_Skill::Clone()
{
	CBoss_Skill* pInstance = new CBoss_Skill(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CBoss_Skill Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

My_uLong CBoss_Skill::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pCollisioner);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
