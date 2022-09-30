#include "stdafx.h"
#include "..\Header\Loading.h"
#include "SceneMgr.h"
#include "ComponentMgr.h"
#include "GameObjectMgr.h"
#include "TimerMgr.h"
#include "Loading_BG.h"
#include "Stage.h"
#include "Debugger.h"

#include <process.h> 
#include "SoundManager.h"
#include "Loading_Img.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice), mThread(nullptr), mComplete(false)
{
	ZeroMemory(&mCSKey, sizeof(CRITICAL_SECTION));
}

HRESULT CLoading::Initialize()
{
	if (FAILED(Initialize_Component_Prototype()))
		return E_FAIL;

	if (FAILED(Initialize_GameObject_Prototype()))
		return E_FAIL;

	if (FAILED(Initialize_Layer_Loading_BackGround(L"Layer_Loading_BackGround")))
		return E_FAIL;

	CDebugger::GetInstance()->Set_Camera_Free();

	InitializeCriticalSection(&mCSKey);
	mThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, this, 0, 0);

	CSoundMgr::GetInstance()->PlaySound(L"Intro.MP3", CSoundMgr::Channel_Intro);


	return NOERROR;
}

My_Int CLoading::Update(const My_Float & fTimeDelta)
{
	return CScene::Update(fTimeDelta);
}

My_Int CLoading::LastUpdate(const My_Float & fTimeDelta)
{
	if (mComplete)
	{
		CScene*		pScene = CStage::Create(m_pDevice);
		if (nullptr == pScene)
			return -1;
		if (FAILED(CSceneMgr::GetInstance()->SetUp_CurrentScene(pScene)))
			return -1;

		return 0;
	}

	return CScene::LastUpdate(fTimeDelta);
}

void CLoading::Render()
{

}

HRESULT CLoading::Initialize_Component_Prototype()
{
	if (nullptr == m_pComponentMgr)
		return E_FAIL;

	CComponent*		pComponent = nullptr;

	// For.Component_Buffer_ViewPort
	pComponent = CViewPort_Buffer::Create(m_pDevice);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponentMgr->Add_Component(SCENE_LOADING, L"Component_Buffer_ViewPort", pComponent)))
		return E_FAIL;

	// For.Component_Texture
	pComponent = Engine::CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Loading/%d.png", 119);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponentMgr->Add_Component(SCENE_LOADING, L"Component_Texture_Loading", pComponent)))
		return E_FAIL;

	// For.Component_Texture          
	pComponent = Engine::CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Loading/Loading%d.png", 30);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponentMgr->Add_Component(SCENE_LOADING, L"Component_Texture_Loading_Img", pComponent)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CLoading::Initialize_GameObject_Prototype()
{
	CGameObject*		pGameObject = nullptr;

	// For.GameObject_Loading_BG
	pGameObject = CLoading_BG::Create(m_pDevice);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_LOADING, L"Loading_BackGround", pGameObject)))
		return E_FAIL;

	// For.GameObject_Loading_BG
	pGameObject = CLoading_Img::Create(m_pDevice);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_LOADING, L"Loading_Img", pGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CLoading::Initialize_Layer_Loading_BackGround(const My_TChar * szKey)
{
	CGameObject*		pGameObject = nullptr;

	// For.Object_Loading_BG
	pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_LOADING, L"Loading_BackGround");
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_LOADING, szKey, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CLoading::Initialize_Lighting()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return NOERROR;
}

unsigned int CLoading::ThreadFunc(void * arg)
{
	CLoading* loading = static_cast<CLoading*>(arg);

	//Texture
	//Map
	CComponent* pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Map/KartMap.bmp", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Map", pComponent)))
		return E_FAIL;

	// Boss_HP
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/Heart.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Heart", pComponent)))
		return E_FAIL;

	// Boss_Money
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Item/BossMoney.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_BossMoney", pComponent)))
		return E_FAIL;

	// Logo
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/logoback.png", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_LogoBack", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/GameStart%d.png", 2);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_StartButton", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/Game_Exit_Button.png", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_ExitButton", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/watingroom.png", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_WaitingRoom", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/Real_Game_Start_Button%d.png", 2);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_GameStartButton", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_StartCounting", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/end%d.png", 7);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_EndCounting", pComponent)))
		return E_FAIL;

	// Award_Box
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/Award_Box.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Award_Box", pComponent)))
		return E_FAIL;

	// Bazzi_Ani
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Character/Bazzi/Bazzi%d.png", 16);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Bazzi_Ani", pComponent)))
		return E_FAIL;

	// Dao_Ani
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Character/Dao/Dao%d.png", 16);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Dao_Ani", pComponent)))
		return E_FAIL;

	// Dizni_Ani
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Character/Dizny/Dizny%d.png", 16);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Dizny_Ani", pComponent)))
		return E_FAIL;

	// Color_Paper
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Effect/Color%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Color_Paper", pComponent)))
		return E_FAIL;


	//Character/Bazzi/
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Character/Bazzi/Bazzi_Body.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Bazzi_Body", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Character/Bazzi/Bazzi_Head.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Bazzi_Head", pComponent)))
		return E_FAIL;



	//Character/Dao
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Character/Dao/Dao_Body.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Dao_Body", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Character/Dao/Dao_Head.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Dao_Head", pComponent)))
		return E_FAIL;


	//Character/Dizni
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Character/Dizny/Dizny_Body.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Dizny_Body", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Character/Dizny/Dizny_Head.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Dizny_Head", pComponent)))
		return E_FAIL;



	//Cart
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Cart/CartBox_Back.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_CartBox_Back", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Cart/CartBox_Bottom.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_CartBox_Bottom", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Cart/CartBox_Engine.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_CartBox_Engine", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Cart/CartBox_Front.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_CartBox_Front", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Cart/CartBox_Side.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_CartBox_Side", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Cart/CartBox_Tire.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_CartBox_Tire", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Cart/CartBox_Wing.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_CartBox_Wing", pComponent)))
		return E_FAIL;



	//Item
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Item/Item_Box.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Item_Box", pComponent)))
		return E_FAIL;


	//Track/Fence
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Fence/FenceDirect1.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Fence_Direct1", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Fence/FenceDirect2.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Fence_Direct2", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Fence/FenceRot.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Fence_Rot", pComponent)))
		return E_FAIL;



	//Track/Objects
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Cloud.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Cloud", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Green_leaf.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Green_Leaf", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Tree_Body.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Tree_Body", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Red_Leaf.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Red_Leaf", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Crocodile_Body.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Crocodile_Body", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Crocodile_Body_Top.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Crocodile_Body_Top", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Crocodile_Eye.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Crocodile_Eye", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Crocodile_Face_Bottom.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Crocodile_Face_Bottom", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Crocodile_Face_Top.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Crocodile_Face_Top", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Crocodile_Leg.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Crocodile_Leg", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Crocodile_Tooth.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Crocodile_Tooth", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Crocodile_Tree.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Crocodile_Tree", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/House_Door.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_House_Door", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/House_Red_Tree_1.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_House_Red_Tree_1", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/House_Red_Tree_2.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_House_Red_Tree_2", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/House_Red_Tree_3.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_House_Red_Tree_3", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/House_Roof.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_House_Roof", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/House_Side.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_House_Side", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/House_Top.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_House_Top", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Media_House.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Media_House", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Media_House_Door.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Media_House_Door", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Media_House_Roof.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Media_House_Roof", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Media_House_Door_Side.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Media_House_Door_Side", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Media_House_Tile.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Media_House_Tile", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Media_House_Tile2.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Media_House_Tile2", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Objects/Media_House_Box.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Media_House_Box", pComponent)))
		return E_FAIL;

	//Track/Road
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Road/Dash_Block.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Dash_Block", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Road/KartRider_Sign.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_KartRider_Sign", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Road/Tetris_Blue.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Tetris_Blue", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Road/Tetris_Purple.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Tetris_Purple", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Road/Tetris_Red.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Tetris_Red", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Road/Tetris_Yellow.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Tetris_Yellow", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Road/Tetris_Bridge_Red.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Tetris_Bridge_Red", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Road/Tetris_Bridge_White.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Tetris_Bridge_White", pComponent)))
		return E_FAIL;



	//Track/Zone
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Zone/BoosterBox.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Booster_Box", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Track/Zone/JumpBox.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Jump_Box", pComponent)))
		return E_FAIL;

	// Water
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Map/bk_water.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Water", pComponent)))
		return E_FAIL;

	/* Item */

	// Item_Cloud
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Item/Item_Cloud_Area.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Item_Cloud", pComponent)))
		return E_FAIL;

	// Item_Cloud_Small
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Item/Item_Cloud_1.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Cloud_Small", pComponent)))
		return E_FAIL;

	// Item_Banana
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Item/Item_Banana.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Item_Banana", pComponent)))
		return E_FAIL;

	// Item_Water_Bomb
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Item/Water_Bomb.dds");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Item_Water_Bomb", pComponent)))
		return E_FAIL;

	// Item_First
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Item/Item_First.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Item_First", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Item/Item_First_Bar.dds", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Item_First_Bar", pComponent)))
		return E_FAIL;

	// Item_Booster
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Item/Item_Booster%d.png", 2);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Item_Booster", pComponent)))
		return E_FAIL;

	// Item_Devil
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Texture/Item/Item_Devil.dds");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Item_Devil", pComponent)))
		return E_FAIL;

	// Item_Magnet
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Item/Item_Magnet%d.png", 3);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Item_Magnet", pComponent)))
		return E_FAIL;

	// Item_Water_Fly
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Item/Item_Water_Fly%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Item_Water_Fly", pComponent)))
		return E_FAIL;


	// UI
	// ItemSlot
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/itemSlot.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_ItemSlot", pComponent)))
		return E_FAIL;

	// SpeedUBack
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/tachoBG_%d.png", 2);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_SpeedUIBackGround", pComponent)))
		return E_FAIL;

	// SpeedUIOutLine
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/tacho_gage%d.png", 5);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_SpeedUIOutLine", pComponent)))
		return E_FAIL;

	// SpeedUIOutLine
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/number%d.png", 10);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_SpeedUINumber", pComponent)))
		return E_FAIL;

	// Item_Icon
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/Item_Icon%d.png", 10);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Item_Icon", pComponent)))
		return E_FAIL;

	// Time
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/time_%d.png", 12);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Time", pComponent)))
		return E_FAIL;

	// Laps
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/lapsNum%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Laps", pComponent)))
		return E_FAIL;

	// Ranking
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/ranking%d.png", 7);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Ranking", pComponent)))
		return E_FAIL;



	// Minimap
	// PlayerIcon
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/PlayerMiniMapIcon.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_MiniMap_Player", pComponent)))
		return E_FAIL;

	// DaoIcon
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/DaoMiniMapIcon.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_MiniMap_Dao", pComponent)))
		return E_FAIL;

	// DizniIcon
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/UI/DizniMiniMapIcon.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_MiniMap_Dizni", pComponent)))
		return E_FAIL;

	// Track MiniMap
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Map/MiniMap.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_MiniMap_Track", pComponent)))
		return E_FAIL;



	/* Effect */

	// Booster_Effect
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Effect/Booster_Effect%d.png", 7);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Booster_Effect", pComponent)))
		return E_FAIL;

	// Smoke_Effect
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Effect/Smoke_Effect%d.png", 2);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Smoke_Effect", pComponent)))
		return E_FAIL;

	// Engine_Effect
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Effect/Engine_Effect%d.png", 2);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Engine_Effect", pComponent)))
		return E_FAIL;

	// Missile_Effect
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Effect/Missile_Effect.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Missile_Effect", pComponent)))
		return E_FAIL;

	// Bomb_Effect
	pComponent = CTexture::Create(loading->m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Texture/Effect/Bomb_Effect.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Bomb_Effect", pComponent)))
		return E_FAIL;

	pComponent = Engine::CNavMesh::Create(loading->m_pDevice, L"../Bin/Data/Map/NavMesh.dat");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(loading->m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_NavMesh", pComponent)))
		return E_FAIL;
	
	loading->mComplete = true;

	return 0;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLoading*	pInstance = new CLoading(pDevice);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CLoading Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

My_uLong CLoading::ReleaseMemory()
{
	DeleteCriticalSection(&mCSKey);
	WaitForSingleObject(mThread, INFINITE);
	CloseHandle(mThread);

	m_pObjectMgr->Clear_GameObject(SCENE_LOADING);
	m_pComponentMgr->Clear_Component(SCENE_LOADING);

	CScene::ReleaseMemory();

	return My_uLong();
}
