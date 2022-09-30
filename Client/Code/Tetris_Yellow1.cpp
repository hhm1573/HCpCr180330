#include "stdafx.h"
#include "..\Header\Tetris_Yellow1.h"

CTetris_Yellow1::CTetris_Yellow1(LPDIRECT3DDEVICE9 pDevice)
	:CTetris(pDevice)
{
}

CTetris_Yellow1::CTetris_Yellow1(const CTetris_Yellow1 & Ref)
	: CTetris(Ref)
{
}

HRESULT CTetris_Yellow1::Initialize()
{
	CTetris::Initialize();

	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CTetris_Yellow1::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Tetris_Yellow1_Client.dat");
}

My_Int CTetris_Yellow1::Update(const My_Float & fTimeDelta)
{
	if (mIsMoving)
		CTetris::Update(fTimeDelta);

	My_Vec3	vPivot = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
	vPivot.x += 9.f;
	mCollisioner->Add_CollisionAABB(vPivot - My_Vec3(4.5f, 4.5f, 4.5f), vPivot + My_Vec3(4.5f, 4.5f, 4.5f), 20, &mReceive, nullptr);

	vPivot.x -= 9.f;
	mCollisioner->Add_CollisionAABB(vPivot - My_Vec3(4.5f, 4.5f, 4.5f), vPivot + My_Vec3(4.5f, 4.5f, 4.5f), 21, &mReceive, nullptr);

	vPivot.x -= 9.f;
	mCollisioner->Add_CollisionAABB(vPivot - My_Vec3(4.5f, 4.5f, 4.5f), vPivot + My_Vec3(4.5f, 4.5f, 4.5f), 22, &mReceive, nullptr);

	return My_Int();
}

My_Int CTetris_Yellow1::LastUpdate(const My_Float & fTimeDelta)
{
	if (!(mIsMoving && (WATING == mState || READY == mState)))
		m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CTetris_Yellow1::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CTetris_Yellow1 * CTetris_Yellow1::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTetris_Yellow1*		pInstance = new CTetris_Yellow1(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CTetris_Yellow1::Clone(void)
{
	CTetris_Yellow1*		pInstance = new CTetris_Yellow1(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CTetris_Yellow1::ReleaseMemory(void)
{
	CTetris::ReleaseMemory();
	return My_uLong();
}
