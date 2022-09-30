#include "stdafx.h"
#include "..\Header\ItemBox.h"

#include "GameObjectMgr.h"
#include "ItemBox_Effect.h"

CItemBox::CItemBox(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice), mAngleX(45.f), mAngleY(45.f), mAngleZ(0.f), mSpeed(100.f)
{
}

CItemBox::CItemBox(const CItemBox & Ref)
	: CModel(Ref), mAngleX(Ref.mAngleX), mAngleY(Ref.mAngleY), mAngleZ(Ref.mAngleZ), mSpeed(Ref.mSpeed), m_iState(Ref.m_iState)
{
}

HRESULT CItemBox::Initialize()
{
	m_pCollisioner = (CCollisioner*)m_pComponentMgr->Clone_Component(SCENE_STAGE, L"Component_Collisioner");
	if (m_pCollisioner == nullptr)
		return E_FAIL;
	if (FAILED(Add_Component(L"Component_Collisioner", m_pCollisioner)))
		return E_FAIL;


	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CItemBox::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Item/ItemBox_Client.dat");
}

My_Int CItemBox::Update(const My_Float & fTimeDelta)
{
	mAngleY += fTimeDelta * mSpeed;
	m_pComponent_Transform->RotationYawPitchRoll(D3DXToRadian(mAngleX), D3DXToRadian(mAngleY), D3DXToRadian(mAngleZ));

	return My_Int();
}

My_Int CItemBox::LastUpdate(const My_Float & fTimeDelta)
{
	if (1 == m_iState || 8 == m_iState || 5 == m_iState)
	{
		if (1 == m_iState)
		{
			for (int i = 0; i < 30; ++i)
			{
				My_Int	iRand = rand() % 30 + 1;
				My_Int	iRandX = rand() % 900 + 200;
				My_Int	iRandY = rand() % 300;
				CGameObject* pGameObject = CGameObjectMgr::GetInstance()->Clone_GameObject(SCENE_STAGE, L"Object_ItemBox_Effect");
				if (pGameObject == nullptr)
					return E_FAIL;
				if (FAILED(CGameObjectMgr::GetInstance()->Add_Object(SCENE_STAGE, L"Object_ItemBox_Effect", pGameObject)))
					return E_FAIL;

				dynamic_cast<CItemBox_Effect*>(pGameObject)->Set_JumpPow(iRand);
				dynamic_cast<CItemBox_Effect*>(pGameObject)->Set_Position(My_Vec3((float)iRandX, (float)iRandY, 0.f));
			}
		}

		mIsCol = true;
		m_iState = 0;
	}

	if (mIsCol)
	{
		mRespawnTime += fTimeDelta;

		if (mRespawnTime > 3.f)
		{
			mIsCol = false;
			mRespawnTime = 0.f;
		}
	}

	else
	{
		m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
		m_pCollisioner->Add_Collision(*m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION), 1.0f, 2, &m_iState);
	}

	return My_Int();
}

void CItemBox::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CItemBox * CItemBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItemBox*		pInstance = new CItemBox(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CItemBox::Clone(void)
{
	CItemBox*		pInstance = new CItemBox(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CItemBox::ReleaseMemory(void)
{
	Safe_Release(m_pCollisioner);

	CModel::ReleaseMemory();
	return My_uLong();
}
