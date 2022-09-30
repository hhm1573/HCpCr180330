#include "stdafx.h"
#include "..\Header\Ray_Tri2.h"
#include "GameObjectMgr.h"
#include "Camera_Target.h"
#include "Dao.h"
#include "Item_Magnet.h"
#include "Bazzi.h"


CRay_Tri2::CRay_Tri2(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
{
	m_pTargetTrans = nullptr;
	m_pBazzi = nullptr;
	m_vPosition = { 0.f, 0.f, 0.f };
	m_vLook = { 0.f, 0.f, 0.f };
	m_fDistance = 0.f;
	m_vTri1 = { 0.f, 0.f, 0.f };
	m_vTri2 = { 0.f, 0.f, 0.f };
	m_vTri3 = { 0.f, 0.f, 0.f };
	_m_vTri1 = { 0.f, 0.f, 0.f };
	_m_vTri2 = { 0.f, 0.f, 0.f };
	_m_vTri3 = { 0.f, 0.f, 0.f };
	m_bIsRay2 = false;
	m_iTargetNum = 0;
}

CRay_Tri2::CRay_Tri2(const CRay_Tri2 & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pRendererCom(Ref.m_pRendererCom)
{
	if (nullptr != m_pTransformCom)
		m_pTransformCom->AddRef();

	if (nullptr != m_pBufferCom)
		m_pBufferCom->AddRef();

	m_pTargetTrans = nullptr;
	m_pBazzi = nullptr;
	m_vPosition = { 0.f, 0.f, 0.f };
	m_vLook = { 0.f, 0.f, 0.f };
	m_fDistance = 0.f;
	m_vTri1 = { 0.f, 0.f, 0.f };
	m_vTri2 = { 0.f, 0.f, 0.f };
	m_vTri3 = { 0.f, 0.f, 0.f };
	_m_vTri1 = { 0.f, 0.f, 0.f };
	_m_vTri2 = { 0.f, 0.f, 0.f };
	_m_vTri3 = { 0.f, 0.f, 0.f };
	m_bIsRay2 = false;
	m_iTargetNum = 0;
}

HRESULT CRay_Tri2::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_pTransformCom->Scaling(5.f, 2.f, 2.f);

	auto	pVecPos = *m_pBufferCom->Get_PosArray();

	_m_vTri1 = pVecPos[0];
	_m_vTri2 = pVecPos[1];
	_m_vTri3 = pVecPos[2];

	m_pBazzi = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0));


	return NOERROR;
}

HRESULT CRay_Tri2::Initialize_Component()
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

	// For.Com_TriCol
	m_pBufferCom = (CTriCol*)m_pComponentMgr->Clone_Component(SCENE_STATIC, L"Component_Buffer_TriCol");
	if (nullptr == m_pBufferCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Buffer_TriCol", m_pBufferCom)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CRay_Tri2::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CRay_Tri2::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	My_Vec3 vPosition = *m_pTargetTrans->Get_Information(CTransform::INFO_POSITION);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(vPosition.x, vPosition.y + 1.f, vPosition.z));


	//My_Matrix matWorld = *m_pTargetTrans->Get_WorldMatrix();
	//My_Vec3 vRight = *(My_Vec3*)&matWorld.m[CTransform::INFO_RIGHT][0];
	//My_Vec3 vUp = *(My_Vec3*)&matWorld.m[CTransform::INFO_UP][0];
	//My_Vec3 vLook = *(My_Vec3*)&matWorld.m[CTransform::INFO_LOOK][0];

	//m_pTransformCom->Set_Information(CTransform::INFO_RIGHT, &(vRight * 5.f));
	//m_pTransformCom->Set_Information(CTransform::INFO_UP, &(vUp * 2.f));
	//m_pTransformCom->Set_Information(CTransform::INFO_LOOK, &(vLook * 2.f));

	ChangeWorld();

	if (true == m_bIsBoss)
		m_pTransformCom->Scaling(10.f, 4.f, 0.f);

	m_vPosition = *dynamic_cast<CBazzi*>(m_pBazzi)->Get_Transform()->Get_Information(CTransform::INFO_POSITION);
	m_vLook = *dynamic_cast<CBazzi*>(m_pBazzi)->Get_Transform()->Get_Information(CTransform::INFO_LOOK);
	D3DXVec3Normalize(&m_vLook, &m_vLook);

	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(vPosition.x, vPosition.y + 1.f, vPosition.z));

	return My_Int();
}

My_Int CRay_Tri2::LastUpdate(const My_Float & fTimeDelta)
{
	CGameObject* pObject = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Item_Magnet", 0));

	if (nullptr == pObject)
		return 1;

	if (nullptr != pObject)
	{
		if (dynamic_cast<CItem_Magnet*>(pObject)->Get_Num() == 2)
		{
			return 1;
		}
	}

	float fU = 0.f, fV = 0.f;

	if (TRUE == D3DXIntersectTri(&m_vTri1, &m_vTri2, &m_vTri3, &m_vPosition, &m_vLook, &fU, &fV, &m_fDistance))
	{
		if (m_fDistance < 100.f)
		{
			m_bIsRay2 = true;
			dynamic_cast<CItem_Magnet*>(pObject)->Set_Num(2);

			if (true == m_bIsBoss)
			{
				dynamic_cast<CItem_Magnet*>(pObject)->Set_IsBoss(true);
			}

			else
			{
				switch (m_iTargetNum)
				{
				case 1:
					dynamic_cast<CItem_Magnet*>(pObject)->Set_Target_Num(1);
					break;
				case 2:
					dynamic_cast<CItem_Magnet*>(pObject)->Set_Target_Num(2);
					break;
				}
			}
		}
	}
	else
		m_bIsRay2 = false;

	return My_Int();
}

void CRay_Tri2::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pTransformCom->SetUp_OnGraphicDev();

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CRay_Tri2::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CRay_Tri2::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CRay_Tri2 * CRay_Tri2::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRay_Tri2*	pInstance = new CRay_Tri2(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CRay_Tri2 Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRay_Tri2::Clone()
{
	CRay_Tri2*		pInstance = new CRay_Tri2(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CRay_Tri2 Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CRay_Tri2::ReleaseMemory()
{
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);


	CGameObject::ReleaseMemory();

	return My_uLong();
}

void CRay_Tri2::ChangeWorld()
{
	m_vTri1 = _m_vTri1;
	m_vTri2 = _m_vTri2;
	m_vTri3 = _m_vTri3;

	My_Matrix matWorld = *m_pTransformCom->Get_WorldMatrix();

	D3DXVec3TransformCoord(&m_vTri1, &m_vTri1, &matWorld);
	D3DXVec3TransformCoord(&m_vTri2, &m_vTri2, &matWorld);
	D3DXVec3TransformCoord(&m_vTri3, &m_vTri3, &matWorld);
}
