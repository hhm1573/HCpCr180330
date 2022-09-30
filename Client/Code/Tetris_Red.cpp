#include "stdafx.h"
#include "..\Header\Tetris_Red.h"

CTetris_Red::CTetris_Red(LPDIRECT3DDEVICE9 pDevice)
	:CTetris(pDevice)
{
}

CTetris_Red::CTetris_Red(const CTetris_Red & Ref)
	: CTetris(Ref)
{
}

HRESULT CTetris_Red::Initialize()
{
	CTetris::Initialize();

	return CModel::Initialize(SCENE_STATIC, L"Component_Buffer_CubeCol",
		SCENE_STATIC, L"Component_Buffer_CubeTex",
		SCENE_STATIC, L"Component_Transform",
		SCENE_STATIC, L"Component_Renderer",
		SCENE_STAGE, L"Component_Texture_");
}

HRESULT CTetris_Red::Initialize_Prototype()
{
	return CModel::Initialize_Prototype(L"../Bin/Data/Track/Tetris_Red1_Client.dat");
}

My_Int CTetris_Red::Update(const My_Float & fTimeDelta)
{
	if (mIsMoving)
		CTetris::Update(fTimeDelta);

	My_Vec3	vPivot = *m_pComponent_Transform->Get_Information(CTransform::INFO_POSITION);
	vPivot.x += 4.5f;
	vPivot.y += 4.5f;
	mCollisioner->Add_CollisionAABB(vPivot - My_Vec3(4.5f, 4.5f, 4.5f), vPivot + My_Vec3(4.5f, 4.5f, 4.5f), 16, &mReceive, nullptr);

	vPivot.x -= 9.f;
	mCollisioner->Add_CollisionAABB(vPivot - My_Vec3(4.5f, 4.5f, 4.5f), vPivot + My_Vec3(4.5f, 4.5f, 4.5f), 17, &mReceive, nullptr);

	vPivot.y -= 9.f;
	mCollisioner->Add_CollisionAABB(vPivot - My_Vec3(4.5f, 4.5f, 4.5f), vPivot + My_Vec3(4.5f, 4.5f, 4.5f), 18, &mReceive, nullptr);

	vPivot.x += 9.f;
	mCollisioner->Add_CollisionAABB(vPivot - My_Vec3(4.5f, 4.5f, 4.5f), vPivot + My_Vec3(4.5f, 4.5f, 4.5f), 19, &mReceive, nullptr);

	return My_Int();
}

My_Int CTetris_Red::LastUpdate(const My_Float & fTimeDelta)
{
	if (!(mIsMoving && (WATING == mState || READY == mState)))
		m_pComponent_Renderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CTetris_Red::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CModel::Render();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CTetris_Red * CTetris_Red::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTetris_Red*		pInstance = new CTetris_Red(pDevice);

	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CGameObject * CTetris_Red::Clone(void)
{
	CTetris_Red*		pInstance = new CTetris_Red(*this);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

My_uLong CTetris_Red::ReleaseMemory(void)
{
	CTetris::ReleaseMemory();
	return My_uLong();
}
