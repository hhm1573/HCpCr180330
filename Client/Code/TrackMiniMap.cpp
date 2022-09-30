#include "stdafx.h"
#include "..\Header\TrackMiniMap.h"

#include "GameObjectMgr.h"
#include "MiniMap.h"

CTrackMiniMap::CTrackMiniMap(LPDIRECT3DDEVICE9 pDevice)
	: CMiniMapIcon(pDevice)
{
}

CTrackMiniMap::CTrackMiniMap(const CTrackMiniMap & rhs)
	: CMiniMapIcon(rhs)
{
}

HRESULT CTrackMiniMap::Initialize()
{
	if (FAILED(CMiniMapIcon::Initialize()))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Texture_MiniMap_Track");
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Texture_MiniMap_Track", m_pTextureCom)))
		return E_FAIL;

	const My_Vec3 vecPos = { 256.5f , 0.f, 256.5f };

	m_pTransformCom->Scaling(513.f, 513.f, 0.f);
	m_pTransformCom->Rotation_X(D3DXToRadian(90.f));
	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &vecPos);

	return NOERROR;
}

HRESULT CTrackMiniMap::Initialize_Prototype()
{
	return NOERROR;
}

My_Int CTrackMiniMap::Update(const My_Float & fTimeDelta)
{
	if (!mIsAward)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	/*m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	const My_Matrix*	matTarget = static_cast<const CBazzi*>(mTarget)->GetPos();
	const My_Vec3		vecPos = { matTarget->_41, matTarget->_42, matTarget->_43 };

	m_pTransformCom->Set_Information(CTransform::INFO_POSITION, &vecPos);
	// m_pTransformCom->Rotation_X(D3DXToRadian(30.f));*/

	return My_Int();
}

My_Int CTrackMiniMap::LastUpdate(const My_Float & fTimeDelta)
{
	return My_Int();
}

void CTrackMiniMap::Render()
{
	/*m_pTransformCom->SetUp_OnGraphicDev();

	m_pTextureCom->SetUp_OnGraphicDev(0);

	m_pBufferCom->Render();
*/
	CMiniMapIcon::Render();
}

CTrackMiniMap * CTrackMiniMap::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTrackMiniMap*	pInstance = new CTrackMiniMap(pDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CItemSlot Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrackMiniMap::Clone()
{
	CTrackMiniMap*	pInstance = new CTrackMiniMap(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CItemSlot Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrackMiniMap::Set_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CTrackMiniMap::Release_RenderState()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

My_uLong CTrackMiniMap::ReleaseMemory()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
