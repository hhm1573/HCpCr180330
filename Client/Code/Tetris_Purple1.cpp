#include "stdafx.h"
#include "..\Header\Tetris_Purple1.h"

CTetris_Purple1::CTetris_Purple1(LPDIRECT3DDEVICE9 pDevice)
	:CTetris(pDevice)
{
}

CTetris_Purple1::CTetris_Purple1(const CTetris_Purple1 & Ref)
	: CTetris(Ref)
{
}

HRESULT CTetris_Purple1::Initialize()
{
	CTetris::Initialize();

	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CTetris_Purple1::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Tetris_Purple1_Client.dat");
}

My_Int CTetris_Purple1::Update(const My_Float & fTimeDelta)
{
	if (mIsMoving)
		CTetris::Update(fTimeDelta);

	My_Vec3	vPivot = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
	vPivot.y -= 4.5f;
	mCollisioner->Add_CollisionAABB(vPivot - My_Vec3(4.5f, 4.5f, 4.5f), vPivot + My_Vec3(4.5f, 4.5f, 4.5f), 9, &mReceive, nullptr);

	vPivot.y += 9.f;
	mCollisioner->Add_CollisionAABB(vPivot - My_Vec3(4.5f, 4.5f, 4.5f), vPivot + My_Vec3(4.5f, 4.5f, 4.5f), 10, &mReceive, nullptr);

	vPivot.x += 9.f;
	vPivot.y -= 9.f;
	mCollisioner->Add_CollisionAABB(vPivot - My_Vec3(4.5f, 4.5f, 4.5f), vPivot + My_Vec3(4.5f, 4.5f, 4.5f), 11, &mReceive, nullptr);

	vPivot.x -= 18.f;
	mCollisioner->Add_CollisionAABB(vPivot - My_Vec3(4.5f, 4.5f, 4.5f), vPivot + My_Vec3(4.5f, 4.5f, 4.5f), 12, &mReceive, nullptr);

	return My_Int();
}

My_Int CTetris_Purple1::LastUpdate(const My_Float & fTimeDelta)
{
	if (!(mIsMoving && (WATING == mState || READY == mState)))
		m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return My_Int();
}

void CTetris_Purple1::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CTetris_Purple1 * CTetris_Purple1::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTetris_Purple1*		pInstance = new CTetris_Purple1(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CTetris_Purple1::Clone(void)
{
	CTetris_Purple1*		pInstance = new CTetris_Purple1(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CTetris_Purple1::ReleaseMemory(void)
{
	CTetris::ReleaseMemory();
	return My_uLong();
}
