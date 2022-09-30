#include "stdafx.h"
#include "..\Header\Item_Cloud.h"
#include "ComponentMgr.h"
#include "GameObjectMgr.h"
#include "Bazzi.h"
#include "Item_Cloud_Small.h"
#include "MathMgr.h"
#include "Dao.h"
#include "Dizni.h"


CItem_Cloud::CItem_Cloud(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_vPosition(0.f, 0.f, 0.f)
	, m_vRight(0.f, 0.f, 0.f)
	, m_vUp(0.f, 0.f, 0.f)
	, m_vLook(0.f, 0.f, 0.f)
	, m_fAccTime(0.f)
	, m_vLine1(0.f, 0.f, 0.f)
	, m_vLine2(0.f, 0.f, 0.f)
{
}

CItem_Cloud::CItem_Cloud(const CItem_Cloud & rhs)
	: CGameObject(rhs)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_pRendererCom(rhs.m_pRendererCom)
	, m_vPosition(rhs.m_vPosition)
	, m_vRight(rhs.m_vRight)
	, m_vUp(rhs.m_vUp)
	, m_vLook(rhs.m_vLook)
	, m_fAccTime(rhs.m_fAccTime)
	, m_vLine1(rhs.m_vLine1)
	, m_vLine2(rhs.m_vLine2)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	if (nullptr != m_pTextureCom)
		m_pTextureCom->AddRef();
}

void CItem_Cloud::Set_Player_Transform(const My_Matrix * pPlayerMatrix)
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

	// Player의 위치 + Player의 Look * 거리 = 현재 객체 위치 (공전)
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &(m_vPosition - m_vLook * 30.f));

	// Player의 회전값 * 스케일값
	m_pTransformCom->Set_Information(CTransform::INFO_RIGHT, &(m_vRight * 80.f));
	m_pTransformCom->Set_Information(CTransform::INFO_UP, &(m_vUp * 30.f));
	m_pTransformCom->Set_Information(CTransform::INFO_LOOK, &m_vLook);

	auto	pVecPos = *m_pBufferCom->Get_PosArray();

	m_vLine1 = pVecPos[0];
	m_vLine2 = pVecPos[1];

	My_Matrix matWorld = *m_pTransformCom->Get_WorldMatrix();

	D3DXVec3TransformCoord(&m_vLine1, &m_vLine1, &matWorld);
	D3DXVec3TransformCoord(&m_vLine2, &m_vLine2, &matWorld);
}

HRESULT CItem_Cloud::Initialize()
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

HRESULT CItem_Cloud::Initialize_Component()
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

	// For.Com_Buffer_RectTex
	m_pBufferCom = (CRectTex*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Buffer_RectTex");
	if (nullptr == m_pBufferCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Buffer_RectTex", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Item_Cloud");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Item_Cloud", m_pTextureCom)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CItem_Cloud::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CItem_Cloud::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return My_Int();
}

My_Int CItem_Cloud::LastUpdate(const My_Float & fTimeDelta)
{
	CGameObject* pBazzi = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0));
	My_Vec3 vBazziPos = *dynamic_cast<CBazzi*>(pBazzi)->Get_Transform()->Get_Information(CTransform::INFO_POSITION);

	CGameObject* pDao = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0));
	My_Vec3 vDaoPos = *dynamic_cast<CDao*>(pDao)->Get_Component()->Get_Information(CTransform::INFO_POSITION);

	CGameObject* pDizni = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0));
	My_Vec3 vDizniPos = *dynamic_cast<CDizni*>(pDizni)->Get_Component()->Get_Information(CTransform::INFO_POSITION);

	POINT Point1 = { (LONG)m_vLine1.x, (LONG)m_vLine1.z };
	POINT Point2 = { (LONG)m_vLine2.x, (LONG)m_vLine2.z };
	POINT Point3 = { (LONG)vBazziPos.x, (LONG)vBazziPos.z + 5 };
	POINT Point4 = { (LONG)vBazziPos.x, (LONG)vBazziPos.z - 5 };
	POINT Point5;
	POINT Point6 = { (LONG)vDaoPos.x, (LONG)vDaoPos.z + 5 };
	POINT Point7 = { (LONG)vDaoPos.x, (LONG)vDaoPos.z - 5 };
	POINT Point8;
	POINT Point9 = { (LONG)vDizniPos.x, (LONG)vDizniPos.z + 5 };
	POINT Point10 = { (LONG)vDizniPos.x, (LONG)vDizniPos.z - 5 };
	POINT Point11;

	My_Bool bIsCol = CMathMgr::IntersectLine(Point1, Point2, Point3, Point4, &Point5);
	My_Bool bIsCol2 = CMathMgr::IntersectLine(Point1, Point2, Point6, Point7, &Point8);
	My_Bool bIsCol3 = CMathMgr::IntersectLine(Point1, Point2, Point9, Point10, &Point11);

	// 현재까지의 시간을 누적시킴
	m_fAccTime += fTimeDelta;

	// 2초 뒤에 작은 구름들 생성 -> for문으로 정리
	/*if (2.f < m_fAccTime)
	{*/
	if (true == bIsCol)
	{
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Item_Cloud_Small");
			if (pObject == nullptr)
				return E_FAIL;
			if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Item_Cloud_Small", pObject)))
				return E_FAIL;

			dynamic_cast<CItem_Cloud_Small*>(pObject)->Set_CloudNum(i + 1);
		}
		return 1;
	}

	if (true == bIsCol2 || true == bIsCol3)
	{
		return 1;
	}

	return My_Int();
}

void CItem_Cloud::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pTransformCom->SetUp_OnGraphicDev();

	m_pTextureCom->SetUp_OnGraphicDev(0);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();

}

void CItem_Cloud::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CItem_Cloud::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CItem_Cloud * CItem_Cloud::Create(LPDIRECT3DDEVICE9 pDevice)
{
	// 원형객체를 생성하는 작업.
	CItem_Cloud*	pInstance = new CItem_Cloud(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CItem_Cloud  Prototype Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CItem_Cloud::Clone()
{
	CItem_Cloud* pInstance = new CItem_Cloud(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CItem_Cloud Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

My_uLong CItem_Cloud::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pCollisioner);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
