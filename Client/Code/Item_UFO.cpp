#include "stdafx.h"
#include "..\Header\Item_UFO.h"
#include "GameObjectMgr.h"

#include "Bazzi.h"
#include "Dao.h"
#include "Dizni.h"

#include "Crocodile.h"

CItem_UFO::CItem_UFO(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice)
	, m_vPosition(0.f, 0.f, 0.f)
	, m_vDir(0.f, 0.f, 0.f)
{
}

CItem_UFO::CItem_UFO(const CItem_UFO & Ref)
	: CModel(Ref)
	, m_pPlayerTrans(Ref.m_pPlayerTrans)
	, m_pCollisioner(Ref.m_pCollisioner)
	, m_vPosition(Ref.m_vPosition)
	, m_fAccTime(Ref.m_fAccTime)
	, m_fAccTime2(Ref.m_fAccTime2)
	, m_fFirstTime(Ref.m_fFirstTime)
	, m_fDis(Ref.m_fDis)
	, m_vDir(Ref.m_vDir)
	, m_bIsFirst(Ref.m_bIsFirst)
	, m_iState(Ref.m_iState)
{
}

void CItem_UFO::Set_Transform(CTransform * pTransform)
{
	m_pPlayerTrans = pTransform;
	My_Vec3 vPlayerPos = *m_pPlayerTrans->Get_Information(CTransform::INFO_POSITION);
	m_vPosition = My_Vec3(vPlayerPos.x, vPlayerPos.y + 1.f, vPlayerPos.z);

	m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &m_vPosition);

	if (true == m_bIsBoss)
		m_pComponent_Transform->Scaling(1.f, 1.f, 1.f);
	else
		m_pComponent_Transform->Scaling(0.2f, 0.2f, 0.2f);
}

HRESULT CItem_UFO::Initialize()
{
	m_pCollisioner = (CCollisioner*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Collisioner");
	if (m_pCollisioner == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Collisioner", m_pCollisioner)))
		return E_FAIL;

	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CItem_UFO::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Item/Item_UFO_Client.dat");
}

My_Int CItem_UFO::Update(const My_Float & fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	CGameObject* pGameObject = nullptr;
	const My_Vec3*	vPos = const_cast<CTransform*>(static_cast<const CTransform*>(mTarget->Get_ComponentPointer(L"Component_Transform")))->Get_Information(CTransform::INFO_POSITION);
	My_Vec3			vPosition = { vPos->x, vPos->y + 2.f, vPos->z };

	if (true == m_bIsBoss)
	{
		pGameObject = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Crocodile", 0));
		vPos = static_cast<CCrocodile*>(pGameObject)->Get_Component()->Get_Information(CTransform::INFO_POSITION);
		vPosition = { vPos->x, vPos->y + 10.f, vPos->z };
	}

	m_vDir = vPosition - m_vPosition;
	m_fDis = D3DXVec3Length(&m_vDir);
	D3DXVec3Normalize(&m_vDir, &m_vDir);


	return My_Int();
}

My_Int CItem_UFO::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	CGameObject* pGameObject = nullptr;

	if (true == m_bIsBoss)
		pGameObject = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Crocodile", 0));

	if ((1 == m_iState && 0 == mTargetNum) || (5 == m_iState && 1 == mTargetNum) || (8 == m_iState && 2 == mTargetNum))
	{
		if (true == m_bIsFirst)
		{
			m_fFirstTime = fTimeDelta;
			m_bIsFirst = false;

			if (0 == mTargetNum)
				dynamic_cast<CBazzi*>(mTarget)->SetSpeed(50.f, 500.f, 1.f);

			else if (1 == mTargetNum)
				dynamic_cast<CDao*>(mTarget)->SetSpeed(50.f, 500.f, 1.f);

			else if (2 == mTargetNum)
				dynamic_cast<CDizni*>(mTarget)->SetSpeed(50.f, 500.f, 1.f);
		}

		m_fAccTime2 += fTimeDelta;

		if (3.f > (m_fAccTime2 - m_fFirstTime))
		{
			const My_Vec3*	vPos = const_cast<CTransform*>(static_cast<const CTransform*>(mTarget->Get_ComponentPointer(L"Component_Transform")))->Get_Information(CTransform::INFO_POSITION);
			My_Vec3		vPosition2 = { vPos->x, vPos->y + 2.f, vPos->z };
			m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &vPosition2);
			m_pComponent_Transform->Rotation_Y(10.f * fTimeDelta);
			
			if (0 == mTargetNum)
				dynamic_cast<CBazzi*>(mTarget)->SetSpeed(50.f, 500.f, 1.f);

			else if (1 == mTargetNum)
				dynamic_cast<CDao*>(mTarget)->SetSpeed(50.f, 500.f, 1.f);

			else if (2 == mTargetNum)
				dynamic_cast<CDizni*>(mTarget)->SetSpeed(50.f, 500.f, 1.f);
		}
		else
		{
			if (0 == mTargetNum)
				dynamic_cast<CBazzi*>(mTarget)->SetSpeed(50.f, 2500.f, 1.f);

			else if (1 == mTargetNum)
				dynamic_cast<CDao*>(mTarget)->SetSpeed(50.f, 2500.f, 1.f);

			else if (2 == mTargetNum)
				dynamic_cast<CDizni*>(mTarget)->SetSpeed(50.f, 2500.f, 1.f);

			return 1;
		}
	}
	else if (12 == m_iState)
	{
		if (true == m_bIsBoss)
		{
			if (true == m_bIsFirst)
			{
				m_fFirstTime = fTimeDelta;
				m_bIsFirst = false;
			}

			m_fAccTime2 += fTimeDelta;

			if (3.f > (m_fAccTime2 - m_fFirstTime))
			{
				My_Vec3 vPos2 = *dynamic_cast<CCrocodile*>(pGameObject)->Get_Component()->Get_Information(CTransform::INFO_POSITION);
				My_Vec3 vPosition2 = { vPos2.x, vPos2.y + 13.f, vPos2.z };
				m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &vPosition2);
				m_pComponent_Transform->Rotation_Y(10.f * fTimeDelta);
				// 보스 스피드를 낮추기
				dynamic_cast<CCrocodile*>(pGameObject)->Set_Speed(50.f, 100.f, 1.f);
			}
			else
			{
				dynamic_cast<CCrocodile*>(pGameObject)->Set_Speed(50.f, 1000.f, 1.f);
				return 1;
			}
		}
	}
	else
	{
		m_vPosition = m_vPosition + m_vDir * 1.f * m_fAccTime;

		m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &m_vPosition);
	}

	// Collisioner: 9
	if (true == m_bIsBoss)
		m_pCollisioner->Add_Collision(*m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION), 15.0f, 10, &m_iState);
	else
		m_pCollisioner->Add_Collision(*m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION), 5.0f, 10, &m_iState);

	return My_Int();
}

void CItem_UFO::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CItem_UFO * CItem_UFO::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItem_UFO*		pInstance = new CItem_UFO(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CItem_UFO::Clone(void)
{
	CItem_UFO*		pInstance = new CItem_UFO(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CItem_UFO::ReleaseMemory(void)
{
	Safe_Release(m_pCollisioner);

	CModel::ReleaseMemory();
	return My_uLong();
}
