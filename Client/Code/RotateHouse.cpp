#include "stdafx.h"
#include "..\Header\RotateHouse.h"

CRotateHouse::CRotateHouse(LPDIRECT3DDEVICE9 pDevice)
	:CModel(pDevice), 
	mAngleX(0.f), mAngleY(90.f), mAngleZ(0.f), 
	mSpeed(40.f), mWaitTime(0.f),
	mIsMediaHouse(true), mIsWait(false)
{
	mOriPos = { 0.f, 0.f, 0.f };
}

CRotateHouse::CRotateHouse(const CRotateHouse & Ref)
	: CModel(Ref), 
	mAngleX(Ref.mAngleX), mAngleY(Ref.mAngleY), mAngleZ(Ref.mAngleZ), 
	mIsMediaHouse(Ref.mIsMediaHouse), mIsWait(Ref.mIsWait),
	mSpeed(Ref.mSpeed), mWaitTime(Ref.mWaitTime), mOriPos(Ref.mOriPos)
{
}

HRESULT CRotateHouse::Initialize()
{
	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CRotateHouse::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Rotate_House_Client.dat");
}

My_Int CRotateHouse::Update(const My_Float & fTimeDelta)
{
	if (mIsWait)
	{
		mWaitTime += fTimeDelta;

		if (mWaitTime > 3.5f)
		{
			mWaitTime = 0.f;
			mIsWait = false;
		}
	}

	else
	{
		const My_Vec3* matWorld = m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);

		if (mIsMediaHouse)
		{
			mAngleZ += mSpeed * fTimeDelta;
			m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(matWorld->x, matWorld->y, matWorld->z + fTimeDelta));
		}

		else
		{
			mAngleZ -= mSpeed * fTimeDelta;
			m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &My_Vec3(matWorld->x, matWorld->y, matWorld->z - fTimeDelta));
		}

		if (mIsMediaHouse && 180.f <= mAngleZ)
		{
			mAngleZ = 180.f;
			mIsMediaHouse = false;
			mIsWait = true;
			m_pComponent_Transform->Set_Information(CTransform::INFO_POSITION, &mOriPos);
		}

		else if (!mIsMediaHouse && 0.f >= mAngleZ)
		{
			mAngleZ = 0.f;
			mIsMediaHouse = true;
			mIsWait = true;
		}

		m_pComponent_Transform->RotationYawPitchRoll(D3DXToRadian(mAngleX), D3DXToRadian(mAngleY), D3DXToRadian(mAngleZ));
	}

	return My_Int();
}

My_Int CRotateHouse::LastUpdate(const My_Float & fTimeDelta)
{
	m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CRotateHouse::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CRotateHouse * CRotateHouse::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRotateHouse*		pInstance = new CRotateHouse(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CRotateHouse::Clone(void)
{
	CRotateHouse*		pInstance = new CRotateHouse(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CRotateHouse::ReleaseMemory(void)
{
	CModel::ReleaseMemory();
	return My_uLong();
}
