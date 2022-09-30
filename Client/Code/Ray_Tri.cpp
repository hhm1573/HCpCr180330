#include "stdafx.h"
#include "..\Header\Ray_Tri.h"
#include "GameObjectMgr.h"
#include "Camera_Target.h"
#include "Dao.h"
#include "Item_Magnet.h"
#include "Bazzi.h"
#include "Ray_Tri2.h"


CRay_Tri::CRay_Tri(LPDIRECT3DDEVICE9 pDevice)
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
	m_iTargetNum = 0;
	m_bIsOrange = false;
}

CRay_Tri::CRay_Tri(const CRay_Tri & Ref)
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
	m_iTargetNum = 0;
	m_bIsOrange = false;
}

HRESULT CRay_Tri::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_pTransformCom->Scaling(15.f, 8.f, 0.f);

	auto	pVecPos = *m_pBufferCom->Get_PosArray();

	_m_vTri1 = pVecPos[0];
	_m_vTri2 = pVecPos[1];
	_m_vTri3 = pVecPos[2];

	m_pBazzi = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0));


	return NOERROR;
}

HRESULT CRay_Tri::Initialize_Component()
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

HRESULT CRay_Tri::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CRay_Tri::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	My_Vec3 vPosition = *m_pTargetTrans->Get_Information(CTransform::INFO_POSITION);
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(vPosition.x, vPosition.y + 1.f, vPosition.z));

	///*ºôº¸µå*/
	//My_Matrix	matView;
	//m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	//D3DXMatrixInverse(&matView, nullptr, &matView);

	//My_Vec3 vec3[3];

	//vec3[0] = *(My_Vec3*)&matView.m[CTransform::INFO_RIGHT][0];
	//vec3[1] = *(My_Vec3*)&matView.m[CTransform::INFO_UP][0];
	//vec3[2] = *(My_Vec3*)&matView.m[CTransform::INFO_LOOK][0];

	//D3DXVec3Normalize(&vec3[0], &vec3[0]);
	//D3DXVec3Normalize(&vec3[1], &vec3[1]);
	//D3DXVec3Normalize(&vec3[2], &vec3[2]);

	//m_pTransformCom->Set_Information(CTransform::INFO_RIGHT, &(vec3[0] * 15.f));
	//m_pTransformCom->Set_Information(CTransform::INFO_UP, &(vec3[1] * 8.f));
	//m_pTransformCom->Set_Information(CTransform::INFO_LOOK, &(vec3[2] * 0.f));

	//auto	pVecPos = *m_pBufferCom->Get_PosArray();

	//_m_vTri1 = pVecPos[0];
	//_m_vTri2 = pVecPos[1];
	//_m_vTri3 = pVecPos[2];

	/*auto	pVecPos = *m_pBufferCom->Get_PosArray();

	_m_vTri1 = pVecPos[0];
	_m_vTri2 = pVecPos[1];
	_m_vTri3 = pVecPos[2];

	ChangeWorld();

	My_Matrix matWorld = *m_pTargetTrans->Get_WorldMatrix();
	My_Vec3 vRight = *(My_Vec3*)&matWorld.m[CTransform::INFO_RIGHT][0];
	My_Vec3 vUp = *(My_Vec3*)&matWorld.m[CTransform::INFO_UP][0];
	My_Vec3 vLook = *(My_Vec3*)&matWorld.m[CTransform::INFO_LOOK][0];

	m_pTransformCom->Set_Information(CTransform::INFO_RIGHT, &(vRight * 15.f));
	m_pTransformCom->Set_Information(CTransform::INFO_UP, &(vUp * 8.f));
	m_pTransformCom->Set_Information(CTransform::INFO_LOOK, &(vLook * 0.f));*/

	ChangeWorld();

	if (true == m_bIsBoss)
		m_pTransformCom->Scaling(30.f, 16.f, 0.f);

	m_vPosition = *dynamic_cast<CBazzi*>(m_pBazzi)->Get_Transform()->Get_Information(CTransform::INFO_POSITION);
	m_vLook = *dynamic_cast<CBazzi*>(m_pBazzi)->Get_Transform()->Get_Information(CTransform::INFO_LOOK);
	D3DXVec3Normalize(&m_vLook, &m_vLook);

	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &My_Vec3(vPosition.x, vPosition.y + 1.f, vPosition.z));

	return My_Int();
}

My_Int CRay_Tri::LastUpdate(const My_Float & fTimeDelta)
{
	float fU = 0.f, fV = 0.f;

	CGameObject* pObject = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Item_Magnet", 0));

	if (nullptr == pObject)
		return 1;

	if (pObject != nullptr)
	{
		if (dynamic_cast<CItem_Magnet*>(pObject)->Get_Num() == 2)
		{
			return 1;
		}
	}

	CGameObject* pObject2 = nullptr;

	if (true == m_bIsBoss)
	{
		pObject2 = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Ray_Tri2", 0));
	}
	else
	{
		switch (m_iTargetNum)
		{
		case 1:
			pObject2 = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Ray_Tri2", 0));
			break;
		case 2:
			pObject2 = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Ray_Tri2", 1));
			break;
		}
	}

	if (TRUE == D3DXIntersectTri(&m_vTri1, &m_vTri2, &m_vTri3, &m_vPosition, &m_vLook, &fU, &fV, &m_fDistance))
	{
		if (m_fDistance < 100.f)
			if (dynamic_cast<CRay_Tri2*>(pObject2)->Get_IsRay2() == false)
			{
				m_bIsOrange = true;
				dynamic_cast<CItem_Magnet*>(pObject)->Set_Num(1);
			}
	}
	else
	{
		m_bIsOrange = false;
		CGameObject* pRay1 = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Ray_Tri", 0));
		//CGameObject* pRay2 = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Ray_Tri2", 1));

		if (dynamic_cast<CRay_Tri*>(pRay1)->Get_IsOrange() == false && dynamic_cast<CRay_Tri2*>(pObject2)->Get_IsRay2() == false)
		{
			dynamic_cast<CItem_Magnet*>(pObject)->Set_Num(0);
		}
	}

	///*ºôº¸µå*/
	//My_Matrix	matView;
	//m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	//D3DXMatrixInverse(&matView, nullptr, &matView);

	//My_Vec3 vec3[3];

	//vec3[0] = *(My_Vec3*)&matView.m[CTransform::INFO_RIGHT][0];
	//vec3[1] = *(My_Vec3*)&matView.m[CTransform::INFO_UP][0];
	//vec3[2] = *(My_Vec3*)&matView.m[CTransform::INFO_LOOK][0];

	//D3DXVec3Normalize(&vec3[0], &vec3[0]);
	//D3DXVec3Normalize(&vec3[1], &vec3[1]);
	//D3DXVec3Normalize(&vec3[2], &vec3[2]);

	//m_pTransformCom->Set_Information(CTransform::INFO_RIGHT, &(vec3[0] * 15.f));
	//m_pTransformCom->Set_Information(CTransform::INFO_UP, &(vec3[1] * 8.f));
	//m_pTransformCom->Set_Information(CTransform::INFO_LOOK, &(vec3[2] * 0.f));

	return My_Int();
}

void CRay_Tri::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pTransformCom->SetUp_OnGraphicDev();

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CRay_Tri::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CRay_Tri::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CRay_Tri * CRay_Tri::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRay_Tri*	pInstance = new CRay_Tri(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CRay_Tri Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRay_Tri::Clone()
{
	CRay_Tri*		pInstance = new CRay_Tri(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CRay_Tri Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CRay_Tri::ReleaseMemory()
{
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);


	CGameObject::ReleaseMemory();

	return My_uLong();
}

void CRay_Tri::ChangeWorld()
{
	m_vTri1 = _m_vTri1;
	m_vTri2 = _m_vTri2;
	m_vTri3 = _m_vTri3;

	My_Matrix matWorld = *m_pTransformCom->Get_WorldMatrix();

	D3DXVec3TransformCoord(&m_vTri1, &m_vTri1, &matWorld);
	D3DXVec3TransformCoord(&m_vTri2, &m_vTri2, &matWorld);
	D3DXVec3TransformCoord(&m_vTri3, &m_vTri3, &matWorld);

}
