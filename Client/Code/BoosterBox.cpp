#include "stdafx.h"
#include "..\Header\BoosterBox.h"

#include "GameObjectMgr.h"
#include "RotateHouse.h"

CBoosterBox::CBoosterBox(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice), mSpeed(-3.f), mState(0), mIsMoving(false), mAngle(0.f)
{
}

CBoosterBox::CBoosterBox(const CBoosterBox & Ref)
	: CModel(Ref), mSpeed(Ref.mSpeed), mState(Ref.mState), mIsMoving(Ref.mIsMoving), mAngle(Ref.mAngle)
{
}

void CBoosterBox::SetIsMoving(My_Bool flag)
{
	mIsMoving = flag;
	mMatParent = static_cast<const CRotateHouse*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_RotateHouse", 0))->GetWorldMatrix();
}

HRESULT CBoosterBox::Initialize()
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

HRESULT CBoosterBox::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/BoosterBox_Client.dat");
}

My_Int CBoosterBox::Update(const My_Float & fTimeDelta)
{
	if (mIsMoving)
	{
		mAngle += mSpeed * fTimeDelta;
		My_Matrix matWorld, matRot, matTrans, matScale; // scale = 0.3f

		D3DXMatrixScaling(&matScale, 0.1f, 0.15f, 0.1f);
		D3DXMatrixRotationY(&matRot, mAngle);
		D3DXMatrixTranslation(&matTrans, mVecPos.x, mVecPos.y, mVecPos.z);

		matWorld = matScale * matRot * matTrans * *mMatParent;

		m_pComponent_Transform->Set_Information(&matWorld);
	}

	else
		m_pComponent_Transform->Rotation_Y(mSpeed * fTimeDelta);

	return My_Int();
}

My_Int CBoosterBox::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	m_pCollisioner->Add_Collision(*m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION), 1.0f, 7, &mState);

	return My_Int();
}

void CBoosterBox::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CBoosterBox * CBoosterBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBoosterBox*		pInstance = new CBoosterBox(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CBoosterBox::Clone(void)
{
	CBoosterBox*		pInstance = new CBoosterBox(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CBoosterBox::ReleaseMemory(void)
{
	Safe_Release(m_pCollisioner);

	CModel::ReleaseMemory();
	return My_uLong();
}
