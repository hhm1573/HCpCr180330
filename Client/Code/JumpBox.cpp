#include "stdafx.h"
#include "..\Header\JumpBox.h"

#include "GameObjectMgr.h"
#include "RotateHouse.h"

CJumpBox::CJumpBox(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice), mSpeed(-3.f), mState(0), mIsMoving(false), mAngle(0.f)
{
}

CJumpBox::CJumpBox(const CJumpBox & Ref)
	: CModel(Ref), mSpeed(Ref.mSpeed), mState(Ref.mState), mIsMoving(Ref.mIsMoving), mAngle(Ref.mAngle)
{
}

void CJumpBox::SetIsMoving(My_Bool flag)
{
	mIsMoving = flag;
	mMatParent = static_cast<const CRotateHouse*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_RotateHouse", 0))->GetWorldMatrix();
}

HRESULT CJumpBox::Initialize()
{
	m_pComponent_Collisioner = (CCollisioner*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Collisioner");
	if (m_pComponent_Collisioner == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Collisioner", m_pComponent_Collisioner)))
		return E_FAIL;

	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CJumpBox::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/JumpBox_Client.dat");
}

My_Int CJumpBox::Update(const My_Float & fTimeDelta)
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

My_Int CJumpBox::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	m_pComponent_Collisioner->Add_Collision(*m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION), 1.0f, 14, &mState);
	return My_Int();
}

void CJumpBox::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CJumpBox * CJumpBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CJumpBox*		pInstance = new CJumpBox(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CJumpBox::Clone(void)
{
	CJumpBox*		pInstance = new CJumpBox(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CJumpBox::ReleaseMemory(void)
{
	Safe_Release(m_pComponent_Collisioner);
	CModel::ReleaseMemory();
	return My_uLong();
}
