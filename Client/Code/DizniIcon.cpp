#include "stdafx.h"
#include "..\Header\DizniIcon.h"

#include "GameObjectMgr.h"
#include "MiniMap.h"

#include "Dizni.h"

CDizniIcon::CDizniIcon(LPDIRECT3DDEVICE9 pDevice)
	: CMiniMapIcon(pDevice)
{
}

CDizniIcon::CDizniIcon(const CDizniIcon & rhs)
	: CMiniMapIcon(rhs)
{
}

HRESULT CDizniIcon::Initialize()
{
	if (FAILED(CMiniMapIcon::Initialize()))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_MiniMap_Dizni");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_MiniMap_Dizni", m_pTextureCom)))
		return E_FAIL;

	mTarget = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0);

	return NOERROR;
}

HRESULT CDizniIcon::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CDizniIcon::Update(const My_Float & fTimeDelta)
{
	if (!mIsAward)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	const My_Matrix*	matTarget = static_cast<const CDizni*>(mTarget)->GetWorld();

	m_pTransformCom->Set_Information(matTarget);
	m_pTransformCom->Rotation_X(D3DXToRadian(90.f));
	m_pTransformCom->Scaling(20.f, 20.f, 20.f);

	return My_Int();
}

My_Int CDizniIcon::LastUpdate(const My_Float & fTimeDelta)
{
	return My_Int();
}

void CDizniIcon::Render()
{
	CMiniMapIcon::Render();
}

CDizniIcon * CDizniIcon::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDizniIcon*	pInstance = new CDizniIcon(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CItemSlot Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDizniIcon::Clone()
{
	CDizniIcon*	pInstance = new CDizniIcon(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CItemSlot Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDizniIcon::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CDizniIcon::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

My_uLong CDizniIcon::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
