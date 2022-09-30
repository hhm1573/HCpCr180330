#include "stdafx.h"
#include "..\Header\Item_Missile.h"
#include "ComponentMgr.h"
#include "GameObjectMgr.h"
#include "Bazzi.h"
#include "Dao.h"
#include "Dizni.h"
#include "Item_Magnet.h"
#include "Missile_Effect.h"
#include "Bomb_Effect.h"
#include "Crocodile.h"


CItem_Missile::CItem_Missile(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
	, m_vPosition(0.f, 0.f, 0.f)
	, m_vPivot(0.f, 0.f, 0.f)
	, m_vLook(0.f, 0.f, 0.f)
	, m_fAccTime(0.f)
{
}

CItem_Missile::CItem_Missile(const CItem_Missile & Ref)
	: CModel(Ref)
	, m_pPlayerTrans(Ref.m_pPlayerTrans)
	, m_vPosition(Ref.m_vPosition)
	, m_vPivot(Ref.m_vPivot)
	, m_vLook(Ref.m_vLook)
	, m_fAccTime(Ref.m_fAccTime)
	, m_fZ(Ref.m_fZ)
	, m_pCollisioner(Ref.m_pCollisioner)
	, m_iState(Ref.m_iState)
	, mTargetTrans(Ref.mTargetTrans)
	, m_iTargetNum(Ref.m_iTargetNum)
{
}

void CItem_Missile::Set_Player_Tramsform(CTransform * pPlayerTrans)
{
	m_pPlayerTrans = pPlayerTrans;
	My_Vec3 vPlayerPos = *m_pPlayerTrans->Get_Information(CTransform::INFO_POSITION);
	m_vPivot = My_Vec3(vPlayerPos.x, vPlayerPos.y + 1.f, vPlayerPos.z);

	m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &vPlayerPos);
	m_pComponent_Transform->Rotation_X(135.f);
	m_pComponent_Transform->Scaling(0.1f, 0.1f, 0.1f);

	m_vPosition = m_vPivot;
}

void CItem_Missile::Set_Target_Num(int iTargetNum)
{
	m_iTargetNum = iTargetNum;
	CGameObject*		pTarget = nullptr;

	switch (m_iTargetNum)
	{
	case 0:
		pTarget = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0));
		const_cast<CBazzi*>(static_cast<const CBazzi*>(pTarget))->SetMissileTarget();
		break;

	case 1:
		pTarget = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0));
		const_cast<CDao*>(static_cast<const CDao*>(pTarget))->SetMissileTarget();
		break;

	case 2:
		pTarget = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0));
		const_cast<CDizni*>(static_cast<const CDizni*>(pTarget))->SetMissileTarget();
		break;
	}
}

HRESULT CItem_Missile::Initialize()
{
	if (FAILED(CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_")))
		return E_FAIL;

	m_pCollisioner = (CCollisioner*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Collisioner");
	if (m_pCollisioner == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Collisioner", m_pCollisioner)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CItem_Missile::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Item/Item_Missile_Client.dat");
}

My_Int CItem_Missile::Update(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	m_fAccTime += fTimeDelta;

	My_Matrix	matWorld, matRot, matTrans, matRot2, matParent, matScale;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRot2);
	D3DXMatrixIdentity(&matParent);
	D3DXMatrixIdentity(&matScale);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(100.f * m_fAccTime));
	D3DXMatrixRotationZ(&matRot2, D3DXToRadian(1000.f * m_fAccTime));
	*(My_Vec3*)&matParent.m[3][0] = m_vPivot;

	//m_fZ += 0.01f;

	D3DXMatrixTranslation(&matTrans, 1500.f, 0.f, 0.f);

	matWorld = matScale* matRot * matTrans * matRot2 * *m_pPlayerTrans->Get_WorldMatrix();

	m_pComponent_Transform->Set_World(matWorld);


	return My_Int();
}

My_Int CItem_Missile::LastUpdate(const My_Float & fTimeDelta)
{
	const My_Vec3*		vPosition = nullptr;
	CGameObject*		pTarget = nullptr;

	if (true == m_bIsBoss)
	{
		pTarget = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Crocodile", 0));
		vPosition = dynamic_cast<CCrocodile*>(pTarget)->Get_Component()->Get_Information(CTransform::INFO_POSITION);
	}

	else
	{
		switch (m_iTargetNum)
		{
		case 0:
			pTarget = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0));
			vPosition = dynamic_cast<CBazzi*>(pTarget)->Get_Transform()->Get_Information(CTransform::INFO_POSITION);
			break;

		case 1:
			pTarget = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0));
			vPosition = dynamic_cast<CDao*>(pTarget)->Get_Component()->Get_Information(CTransform::INFO_POSITION);
			break;

		case 2:
			pTarget = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0));
			vPosition = dynamic_cast<CDizni*>(pTarget)->Get_Component()->Get_Information(CTransform::INFO_POSITION);
			break;
		}

	}

	My_Vec3 vDir = *vPosition - m_vPosition;
	My_Float fDis = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	if (fDis < 1.f)
	{
		// Smoke_Effect
		CGameObject*  pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Bomb_Effect");
		if (pObject == nullptr)
			return E_FAIL;
		if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Bomb_Effect", pObject)))
			return E_FAIL;

		if (true == m_bIsBoss)
			dynamic_cast<CBomb_Effect*>(pObject)->Set_IsBoss(true);

		dynamic_cast<CBomb_Effect*>(pObject)->Set_Pos(m_vPosition);

		return 1;
	}

	m_vPosition = m_vPosition + vDir * 1.f * m_fAccTime;

	m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &m_vPosition);

	//switch (m_iTargetNum)
	//{
	//case 1:
	//	m_pComponent_Transform->Set_Information(CTransform::INFO_RIGHT, dynamic_cast<CDao*>(pTarget)->Get_Component()->Get_Information(CTransform::INFO_RIGHT));
	//	m_pComponent_Transform->Set_Information(CTransform::INFO_UP, dynamic_cast<CDao*>(pTarget)->Get_Component()->Get_Information(CTransform::INFO_UP));
	//	m_pComponent_Transform->Set_Information(CTransform::INFO_LOOK, dynamic_cast<CDao*>(pTarget)->Get_Component()->Get_Information(CTransform::INFO_LOOK));
	//	break;
	//case 2:
	//	m_pComponent_Transform->Set_Information(CTransform::INFO_RIGHT, dynamic_cast<CDizni*>(pTarget)->Get_Component()->Get_Information(CTransform::INFO_RIGHT));
	//	m_pComponent_Transform->Set_Information(CTransform::INFO_UP, dynamic_cast<CDizni*>(pTarget)->Get_Component()->Get_Information(CTransform::INFO_UP));
	//	m_pComponent_Transform->Set_Information(CTransform::INFO_LOOK, dynamic_cast<CDizni*>(pTarget)->Get_Component()->Get_Information(CTransform::INFO_LOOK));
	//	break;
	//}



	m_pCollisioner->Add_Collision(*m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION), 1.0f, 6, &m_iState);

	/*My_Vec3		vPosition;

	My_Matrix		matRot;
	D3DXMatrixRotationAxis(&matRot, &m_vLook, D3DXToRadian(50.f) * m_fAccTime);
	D3DXVec3TransformCoord(&vPosition, &m_vPosition, &matRot);

	m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &vPosition);
	*/

	// Missile_Effect
	CGameObject*  pObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_Missile_Effect");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_Missile_Effect", pObject)))
		return E_FAIL;
	dynamic_cast<CMissile_Effect*>(pObject)->Set_Name(L"Missile_1");

	return My_Int();
}

void CItem_Missile::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CItem_Missile * CItem_Missile::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItem_Missile*		pInstance = new CItem_Missile(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CItem_Missile::Clone(void)
{
	CItem_Missile*		pInstance = new CItem_Missile(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CItem_Missile::ReleaseMemory(void)
{
	Safe_Release(m_pCollisioner);

	CModel::ReleaseMemory();

	return My_uLong();
}
