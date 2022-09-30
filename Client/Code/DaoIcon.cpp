#include "stdafx.h"
#include "..\Header\DaoIcon.h"

#include "GameObjectMgr.h"
#include "MiniMap.h"

#include "Dao.h"

CDaoIcon::CDaoIcon(LPDIRECT3DDEVICE9 pDevice)
	: CMiniMapIcon(pDevice)
{
}

CDaoIcon::CDaoIcon(const CDaoIcon & rhs)
	: CMiniMapIcon(rhs)
{
}

HRESULT CDaoIcon::Initialize()
{
	if (FAILED(CMiniMapIcon::Initialize()))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_MiniMap_Dao");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_MiniMap_Dao", m_pTextureCom)))
		return E_FAIL;

	mTarget = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0);

	return NOERROR;
}

HRESULT CDaoIcon::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CDaoIcon::Update(const My_Float & fTimeDelta)
{
	if (!mIsAward)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	const My_Matrix*	matTarget = static_cast<const CDao*>(mTarget)->GetWorld();

	m_pTransformCom->Set_Information(matTarget);
	m_pTransformCom->Rotation_X(D3DXToRadian(90.f));
	m_pTransformCom->Scaling(20.f, 20.f, 20.f);

	return My_Int();
}

My_Int CDaoIcon::LastUpdate(const My_Float & fTimeDelta)
{
	return My_Int();
}

void CDaoIcon::Render()
{
	CMiniMapIcon::Render();
}

CDaoIcon * CDaoIcon::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDaoIcon*	pInstance = new CDaoIcon(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CItemSlot Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDaoIcon::Clone()
{
	CDaoIcon*	pInstance = new CDaoIcon(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CItemSlot Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDaoIcon::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CDaoIcon::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

My_uLong CDaoIcon::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
