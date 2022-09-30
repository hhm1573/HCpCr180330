#include "stdafx.h"
#include "..\Header\Item_Water_Fly.h"
#include "GameObjectMgr.h"

#include "Bazzi.h"
#include "Dao.h"
#include "Dizni.h"

CItem_Water_Fly::CItem_Water_Fly(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_vPosition(0.f, 0.f, 0.f)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CItem_Water_Fly::CItem_Water_Fly(const CItem_Water_Fly & Ref)
	: CGameObject(Ref)
	, m_pTransformCom(Ref.m_pTransformCom)
	, m_pBufferCom(Ref.m_pBufferCom)
	, m_pTextureCom(Ref.m_pTextureCom)
	, m_pRendererCom(Ref.m_pRendererCom)
	, m_tFrame(Ref.m_tFrame)
	, m_pCollisioner(Ref.m_pCollisioner)
	, m_iState(Ref.m_iState)
	, m_pPlayerTrans(Ref.m_pPlayerTrans)
	, m_vPosition(Ref.m_vPosition)
	, m_fAccTime(Ref.m_fAccTime)
	, m_iRand(Ref.m_iRand)
{
}

void CItem_Water_Fly::Set_Transform(CTransform * pTransform)
{
	m_pPlayerTrans = pTransform;
	My_Vec3 vPlayerPos = *m_pPlayerTrans->Get_Information(CTransform::INFO_POSITION);
	m_vPosition = My_Vec3(vPlayerPos.x, vPlayerPos.y + 1.f, vPlayerPos.z);

	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &m_vPosition);
}

void CItem_Water_Fly::SetTarget(My_Int target)
{
	mTarget = target;

	CGameObject*		pTarget = nullptr;

	switch (mTarget)
	{
	case 0:
		pTarget = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0));
		const_cast<CBazzi*>(static_cast<const CBazzi*>(pTarget))->SetWaterFlyTarget();
		break;

	case 1:
		pTarget = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0));
		const_cast<CDao*>(static_cast<const CDao*>(pTarget))->SetWaterFlyTarget();
		break;

	case 2:
		pTarget = const_cast<CGameObject*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0));
		const_cast<CDizni*>(static_cast<const CDizni*>(pTarget))->SetWaterFlyTarget();
		break;
	}
}

HRESULT CItem_Water_Fly::Initialize()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	m_tFrame = FRAME(0, 40, 4);

	m_pCollisioner = (CCollisioner*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Collisioner");
	if (m_pCollisioner == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Collisioner", m_pCollisioner)))
		return E_FAIL;

	srand(unsigned(time(NULL)));

	return NOERROR;
}

HRESULT CItem_Water_Fly::Initialize_Component()
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
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_Item_Water_Fly");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_Item_Water_Fly", m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CItem_Water_Fly::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CItem_Water_Fly::Update(const My_Float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	m_tFrame.fFrame += m_tFrame.fCount * fTimeDelta;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = 0;
	}

	m_fAccTime += fTimeDelta;

	My_Vec3		vPos = *mTargetTrans->Get_Information(CTransform::INFO_POSITION);
	My_Vec3		vPosition = { vPos.x, vPos.y + 1.f, vPos.z };

	My_Vec3 vDir = vPosition - m_vPosition;
	My_Float fDis = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	m_iRand = rand() % 180 - 90;

	My_Matrix	matRot;
	D3DXMatrixRotationY(&matRot, D3DXToRadian(float(m_iRand)));

	D3DXVec3TransformNormal(&vDir, &vDir, &matRot);

	if (fDis < 1.f)
	{
	/*	if (true == m_bIsFirst)
		{
			m_fFirstTime = fTimeDelta;
			m_bIsFirst = false;
		}

		m_fAccTime2 += fTimeDelta;

		if (3.f >(m_fAccTime2 - m_fFirstTime))
		{
			m_pComponent_Transform->Rotation_Y(10.f * fTimeDelta);
		}
		else
			return 1;*/
	}
	else
	{
		m_vPosition = m_vPosition + vDir * 2.f * m_fAccTime;

		m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &m_vPosition);

	}

	return My_Int();
}

My_Int CItem_Water_Fly::LastUpdate(const My_Float & fTimeDelta)
{
	// 다오와 부딪혔을 때 삭제되기
	if (mTarget == 0 && 1 == m_iState) // 배찌
		return 1;

	else if (mTarget == 1 && 5 == m_iState) // 다오
		return 1;

	else if (mTarget == 2 && 8 == m_iState) // 디즈니
		return 1;

	/*빌보드*/
	My_Matrix	matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, nullptr, &matView);

	My_Vec3 vec3[3];

	vec3[0] = *(My_Vec3*)&matView.m[CTransform::INFO_RIGHT][0];
	vec3[1] = *(My_Vec3*)&matView.m[CTransform::INFO_UP][0];
	vec3[2] = *(My_Vec3*)&matView.m[CTransform::INFO_LOOK][0];

	D3DXVec3Normalize(&vec3[0], &vec3[0]);
	D3DXVec3Normalize(&vec3[1], &vec3[1]);
	D3DXVec3Normalize(&vec3[2], &vec3[2]);

	m_pTransformCom->Set_Information(CTransform::INFO_RIGHT, &(vec3[0] * 1.4f));
	m_pTransformCom->Set_Information(CTransform::INFO_UP, &(vec3[1] * 0.7f));
	m_pTransformCom->Set_Information(CTransform::INFO_LOOK, &(vec3[2] * 1.4f));

	// Collisioner: 9
	m_pCollisioner->Add_Collision(*m_pTransformCom->Get_Information(CTransform::INFO_POSITION), 1.0f, 9, &m_iState);

	return My_Int();
}

void CItem_Water_Fly::Render()
{
	if (nullptr == m_pBufferCom)
		return;

	m_pTransformCom->SetUp_OnGraphicDev();

	m_pTextureCom->SetUp_OnGraphicDev((My_uInt)m_tFrame.fFrame);

	Set_RenderState();

	m_pBufferCom->Render();

	Release_RenderState();
}

void CItem_Water_Fly::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x0f);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void CItem_Water_Fly::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CItem_Water_Fly * CItem_Water_Fly::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItem_Water_Fly*	pInstance = new CItem_Water_Fly(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CItem_Water_Fly Prototype Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem_Water_Fly::Clone()
{
	CItem_Water_Fly*	pInstance = new CItem_Water_Fly(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CItem_Water_Fly Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CItem_Water_Fly::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pCollisioner);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
