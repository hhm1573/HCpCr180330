#include "stdafx.h"
#include "..\Header\Stage.h"

#include "Component.h"
#include "ComponentMgr.h"
#include "GameObject.h"
#include "GameObjectMgr.h"

#include "MyMesh.h"

#include "SkyBox.h"
#include "Terrain.h"
//#include "Stage_ModelTest.h"
#include "Bazzi.h"
#include "Dao.h"
#include "Dizni.h"

#include "Fence_Direct1.h"
#include "Fence_Direct2.h"
#include "Fence_Rot.h"

#include "Tree_Green1.h"
#include "Tree_Green2.h"
#include "Tree_Red.h"
#include "Tree_HouseRed.h"

#include "Crocodile.h"
#include "Crocodile_Face.h"
#include "Crocodile_Tail.h"
#include "Crocodile_Tree.h"

#include "House.h"
#include "MediaHouse.h"
#include "RotateHouse.h"
#include "MediaHouseTile1.h"
#include "MediaHouseTile2.h"

#include "Tetris_Blue.h"
#include "Tetris_Purple1.h"
#include "Tetris_Purple2.h"
#include "Tetris_Red.h"
#include "Tetris_Yellow1.h"
#include "Tetris_Yellow2.h"
#include "Tetris_Bridge_Red.h"
#include "Tetris_Bridge_White.h"

#include "Cloud.h"
#include "GreenBlock.h"
#include "KartRiderSign.h"

#include "ItemBox.h"
#include "BoosterBox.h"
#include "JumpBox.h"

#include "Item_Cloud.h"
#include "Item_Cloud_Small.h"
#include "Item_Banana.h"
#include "Item_Water_Bomb.h"
#include "Item_First.h"
#include "Item_UFO.h"
#include "Item_Booster.h"
#include "Item_Devil.h"
#include "Item_Magnet.h"
#include "Item_Missile.h"
#include "Item_UFO.h"
#include "Item_Water_Fly.h"

#include "Booster_Effect.h"
#include "Smoke_Effect.h"
#include "Engine_Effect.h"
#include "Missile_Effect.h"
#include "Bomb_Effect.h"

#include "Ray_Tri.h"
#include "Ray_Tri2.h"

#include "GameObject_Creator.h"
#include "Camera_Target.h"

#include "Tetris.h"
#include "TetrisManager.h"

#include "ItemSlot.h"
#include "SpeedUIBackGround.h"
#include "SpeedUIOutLine.h"
#include "SpeedUINumber.h"
#include "TimeUI.h"
#include "LapsUI.h"
#include "RankingUI.h"

#include "Item_Icon.h"

#include "Water.h"

#include "MiniMap.h"
#include "PlayerIcon.h"
#include "DaoIcon.h"
#include "DizniIcon.h"
#include "TrackMiniMap.h"

#include "Camera_MiniMap.h"
#include "Camera_Action.h"

#include "Logo_BG.h"
#include "StartButton.h"
#include "EndButton.h"
#include "WaitingRoom_BG.h"
#include "GameStartButton.h"
#include "StartCountingUI.h"
#include "EndCountUI.h"

#include "Debugger.h"
#include "Stage_NavTest.h"
#include "Collisioner.h"

#include "RecordingManager.h"
#include "RankingManager.h"

#include "Award_Box.h"
#include "Color_Paper.h"
#include "Bazzi_Ani.h"
#include "Dao_Ani.h"
#include "Dizny_Ani.h"

#include "ItemBox_Effect.h"
#include "SkidMark.h"
#include "BossMoney.h"

#include "Boss_Skill.h"
#include "Devil_Skill.h"
#include "Boss_HP.h"
#include "Water_Effect.h"
#include "SoundManager.h"

CStage::CStage(LPDIRECT3DDEVICE9 pDevice)
	:CScene(pDevice)
{
}

CStage::CStage(const CStage & Ref)
	: CScene(Ref)
	, mTetrisManager(Ref.mTetrisManager)
	, mRankingManager(Ref.mRankingManager)
	, m_pCollisioner(Ref.m_pCollisioner)
	, mStartButton(nullptr)
	, mGameStartButton(nullptr)
{
}

HRESULT CStage::Initialize()
{
	if (FAILED(Initialize_Lighting()))
		return E_FAIL;

	if (FAILED(Initialize_Component_Prototype()))
		return E_FAIL;

	if (FAILED(Initialize_Component_Texture()))
		return E_FAIL;

	if (FAILED(Initialize_GameObject_Prototype()))
		return E_FAIL;

	if (FAILED(Initialize_LoadObject()))
		return E_FAIL;

	if (FAILED(Initialize_Tetris()))
		return E_FAIL;
	
	if (FAILED(SetLogo()))
		return E_FAIL;

	CSoundMgr::GetInstance()->PlayBGM(L"Main.MP3");

	return S_OK;
}

HRESULT CStage::Initialize_Component_Prototype()
{
	CComponent* pComponent = nullptr;
	pComponent = CTexture::Create(m_pDevice, CTexture::TYPE_CUBE, L"../Bin/Resources/Textures/SkyBox/Burger%d.dds", 5);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_SkyBox", pComponent)))
		return E_FAIL;

	pComponent = CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Terrain/Grass_%d.tga", 2);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_Terrain", pComponent)))
		return E_FAIL;

	//Terrain
	pComponent = Engine::CTerrain_Buffer::Create(m_pDevice, L"../Bin/Texture/Map/HeightMap513.bmp", 1.0f);
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Buffer_Terrain", pComponent)))
		return E_FAIL;

	//Vehicle
	pComponent = Engine::CVehicle::Create(m_pDevice);
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Vehicle", pComponent)))
		return E_FAIL;

	pComponent = CCollisioner::Create(m_pDevice);
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Collisioner", pComponent)))
		return E_FAIL;
	m_pCollisioner = (CCollisioner*)pComponent;

	/* Item */

	// Item_Cloud
	// For.Component_RectTex
	pComponent = CRectTex::Create(m_pDevice);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Buffer_RectTex", pComponent)))
		return E_FAIL;

	// Item_Cloud_Small
	// For.Component_ViewPort
	pComponent = CViewPort_Buffer::Create(m_pDevice);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Buffer_ViewPort", pComponent)))
		return E_FAIL;

	// Item_Water_Bomb
	// For.Component_Sphere
	pComponent = CSphere_Buffer::Create(m_pDevice, 100, 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Sphere_Buffer", pComponent)))
		return E_FAIL;

	/*pComponent = CTexture::Create(m_pDevice, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Terrain/Height.bmp", 1);
	if (pComponent == nullptr)
	return E_FAIL;
	if (FAILED(m_pComponentMgr->Add_Component(SCENE_STAGE, L"Component_Texture_HeightMap", pComponent)))
	return E_FAIL;*/

	return S_OK;
}

HRESULT CStage::Initialize_Component_Texture()
{
	return S_OK;
}

HRESULT CStage::Initialize_GameObject_Prototype()
{
	CGameObject* pGameObject = nullptr;

	// Obj.Terrain
	pGameObject = CTerrain::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Terrain", pGameObject)))
		return E_FAIL;

	// Obj.Bazzi
	pGameObject = CBazzi::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Bazzi", pGameObject)))
		return E_FAIL;

	// Obj.Dao
	pGameObject = CDao::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Dao", pGameObject)))
		return E_FAIL;

	// Obj.Dizni
	pGameObject = CDizni::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Dizni", pGameObject)))
		return E_FAIL;

	{
		pGameObject = CFence_Direct1::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Fence_Direct1", pGameObject)))
			return E_FAIL;

		pGameObject = CFence_Direct2::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Fence_Direct2", pGameObject)))
			return E_FAIL;

		pGameObject = CFence_Rot::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Fence_Rot", pGameObject)))
			return E_FAIL;

		pGameObject = CTree_Green1::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Tree_Green1", pGameObject)))
			return E_FAIL;

		pGameObject = CTree_Green2::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Tree_Green2", pGameObject)))
			return E_FAIL;

		pGameObject = CTree_Red::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Tree_Red", pGameObject)))
			return E_FAIL;

		pGameObject = CTree_HouseRed::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Tree_HouseRed", pGameObject)))
			return E_FAIL;



		//
		pGameObject = CCrocodile::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Crocodile", pGameObject)))
			return E_FAIL;

		pGameObject = CCrocodile_Face::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Crocodile_Face", pGameObject)))
			return E_FAIL;

		pGameObject = CCrocodile_Tail::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Crocodile_Tail", pGameObject)))
			return E_FAIL;

		pGameObject = CCrocodile_Tree::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Crocodile_Tree", pGameObject)))
			return E_FAIL;

		pGameObject = CHouse::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_House", pGameObject)))
			return E_FAIL;

		pGameObject = CMediaHouse::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_MediaHouse", pGameObject)))
			return E_FAIL;

		pGameObject = CRotateHouse::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_RotateHouse", pGameObject)))
			return E_FAIL;

		pGameObject = CMediaHouseTile1::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_MediaHouseTile1", pGameObject)))
			return E_FAIL;

		pGameObject = CMediaHouseTile2::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_MediaHouseTile2", pGameObject)))
			return E_FAIL;

		pGameObject = CTetris_Blue::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Tetris_Blue", pGameObject)))
			return E_FAIL;

		pGameObject = CTetris_Purple1::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Tetris_Purple1", pGameObject)))
			return E_FAIL;

		pGameObject = CTetris_Purple2::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Tetris_Purple2", pGameObject)))
			return E_FAIL;

		pGameObject = CTetris_Red::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Tetris_Red", pGameObject)))
			return E_FAIL;

		pGameObject = CTetris_Yellow1::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Tetris_Yellow1", pGameObject)))
			return E_FAIL;

		pGameObject = CTetris_Yellow2::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Tetris_Yellow2", pGameObject)))
			return E_FAIL;

		pGameObject = CTetris_Bridge_Red::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Tetris_Bridge_Red", pGameObject)))
			return E_FAIL;

		pGameObject = CTetris_Bridge_White::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Tetris_Bridge_White", pGameObject)))
			return E_FAIL;

		pGameObject = CGreenBlock::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_GreenBlock", pGameObject)))
			return E_FAIL;

		pGameObject = CKartRiderSign::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_KartRiderSign", pGameObject)))
			return E_FAIL;

		pGameObject = CCloud::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Cloud", pGameObject)))
			return E_FAIL;

		pGameObject = CItemBox::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_ItemBox", pGameObject)))
			return E_FAIL;

		pGameObject = CBoosterBox::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_BoosterBox", pGameObject)))
			return E_FAIL;

		pGameObject = CJumpBox::Create(m_pDevice);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_JumpBox", pGameObject)))
			return E_FAIL;
	}

	/* Item */

	// Item_Cloud
	pGameObject = CItem_Cloud::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Item_Cloud", pGameObject)))
		return E_FAIL;

	// Item_Cloud_Small
	pGameObject = CItem_Cloud_Small::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Item_Cloud_Small", pGameObject)))
		return E_FAIL;

	// Item_Banana
	pGameObject = CItem_Banana::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Item_Banana", pGameObject)))
		return E_FAIL;

	// Item_Water_Bomb
	pGameObject = CItem_Water_Bomb::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Item_Water_Bomb", pGameObject)))
		return E_FAIL;

	// Item_First
	pGameObject = CItem_First::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Item_First", pGameObject)))
		return E_FAIL;

	// Item_Booster
	pGameObject = CItem_Booster::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Item_Booster", pGameObject)))
		return E_FAIL;

	// Item_Devil
	pGameObject = CItem_Devil::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Item_Devil", pGameObject)))
		return E_FAIL;

	// Item_Magnet
	pGameObject = CItem_Magnet::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Item_Magnet", pGameObject)))
		return E_FAIL;

	// Item_Missile
	pGameObject = CItem_Missile::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Item_Missile", pGameObject)))
		return E_FAIL;

	// Item_UFO
	pGameObject = CItem_UFO::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Item_UFO", pGameObject)))
		return E_FAIL;

	// Item_Water_Fly
	pGameObject = CItem_Water_Fly::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Item_Water_Fly", pGameObject)))
		return E_FAIL;


	/* UI */
	pGameObject = CItemSlot::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_ItemSlot", pGameObject)))
		return E_FAIL;

	pGameObject = CSpeedUIBackGround::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_SpeedUI_BackGround", pGameObject)))
		return E_FAIL;

	pGameObject = CSpeedUIOutLine::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_SpeedUI_OutLine", pGameObject)))
		return E_FAIL;

	pGameObject = CSpeedUINumber::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_SpeedUI_Number", pGameObject)))
		return E_FAIL;

	// Item_Icon
	pGameObject = CItem_Icon::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Item_Icon", pGameObject)))
		return E_FAIL;

	// Time
	pGameObject = CTimeUI::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Time", pGameObject)))
		return E_FAIL;

	// Laps
	pGameObject = CLapsUI::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Laps", pGameObject)))
		return E_FAIL;

	// Ranking
	pGameObject = CRankingUI::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Ranking", pGameObject)))
		return E_FAIL;

	// MiniMap
	pGameObject = CMiniMap::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_MiniMap", pGameObject)))
		return E_FAIL;

	pGameObject = CPlayerIcon::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_MiniMap_PlayerIcon", pGameObject)))
		return E_FAIL;

	pGameObject = CDaoIcon::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_MiniMap_DaoIcon", pGameObject)))
		return E_FAIL;

	pGameObject = CDizniIcon::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_MiniMap_DizniIcon", pGameObject)))
		return E_FAIL;

	pGameObject = CTrackMiniMap::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_MiniMap_Track", pGameObject)))
		return E_FAIL;

	// Water
	pGameObject = CWater::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Water", pGameObject)))
		return E_FAIL;


	/* Effect */

	// Booster_Effect
	pGameObject = CBooster_Effect::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Booster_Effect", pGameObject)))
		return E_FAIL;

	// Smoke_Effect
	pGameObject = CSmoke_Effect::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Smoke_Effect", pGameObject)))
		return E_FAIL;

	// Engine_Effect
	pGameObject = CEngine_Effect::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Engine_Effect", pGameObject)))
		return E_FAIL;

	// Missile_Effect
	pGameObject = CMissile_Effect::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Missile_Effect", pGameObject)))
		return E_FAIL;

	// Bomb_Effect
	pGameObject = CBomb_Effect::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Bomb_Effect", pGameObject)))
		return E_FAIL;

	// ItemBox_Effect
	pGameObject = CItemBox_Effect::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_ItemBox_Effect", pGameObject)))
		return E_FAIL;

	// SkidMark
	pGameObject = CSkidMark::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_SkidMark", pGameObject)))
		return E_FAIL;

	/* Ray */

	// RayTri
	pGameObject = CRay_Tri::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Ray_Tri", pGameObject)))
		return E_FAIL;

	// RayTri2
	pGameObject = CRay_Tri2::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Ray_Tri2", pGameObject)))
		return E_FAIL;

	// Boss_Skill
	pGameObject = CBoss_Skill::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Boss_Skill", pGameObject)))
		return E_FAIL;

	pGameObject = CDevil_Skill::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Devil_Skill", pGameObject)))
		return E_FAIL;

	pGameObject = CBoss_HP::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Boss_HP", pGameObject)))
		return E_FAIL;

	pGameObject = CBossMoney::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_BossMoney", pGameObject)))
		return E_FAIL;


	// LogoBack
	pGameObject = CLogo_BG::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_LogoBack", pGameObject)))
		return E_FAIL;

	pGameObject = CStartButton::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_StartButton", pGameObject)))
		return E_FAIL;

	pGameObject = CEndButton::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_EndButton", pGameObject)))
		return E_FAIL;

	pGameObject = CWaitingRoom_BG::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_WaitingRoom", pGameObject)))
		return E_FAIL;

	pGameObject = CGameStartButton::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_GameStartButton", pGameObject)))
		return E_FAIL;

	pGameObject = CStartCountingUI::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_StartingCountUI", pGameObject)))
		return E_FAIL;

	pGameObject = CEndCountingUI::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_EndCountingUI", pGameObject)))
		return E_FAIL;


	CAMERA_DESC tCameraDesc;
	tCameraDesc.vEye = D3DXVECTOR3(0.0f, 10.0f, -20.0f);
	tCameraDesc.vAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	tCameraDesc.vAxisY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	PROJ_DESC tProjDesc;
	tProjDesc.fFovy = D3DXToRadian(85.0f);
	tProjDesc.fAspect = My_Float(g_iBackCX) / g_iBackCY;
	tProjDesc.fFar = 1000.0f;
	tProjDesc.fNear = 0.1f;

	pGameObject = CCamera_Target::Create(m_pDevice, tCameraDesc, tProjDesc);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Camera_Target", pGameObject)))
		return E_FAIL;

	pGameObject = CCamera_MiniMap::Create(m_pDevice, tCameraDesc, tProjDesc);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Camera_MiniMap", pGameObject)))
		return E_FAIL;

	pGameObject = CCamera_Action::Create(m_pDevice, tCameraDesc, tProjDesc);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Camera_Action", pGameObject)))
		return E_FAIL;

	pGameObject = CStage_NavTest::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Test_NavMesh", pGameObject)))
		return E_FAIL;

	pGameObject = CSkyBox::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_SkyBox", pGameObject)))
		return E_FAIL;

	// Award_Box
	pGameObject = CAward_Box::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Award_Box", pGameObject)))
		return E_FAIL;

	// Bazzi_Ani
	pGameObject = CBazzi_Ani::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Bazzi_Ani", pGameObject)))
		return E_FAIL;

	// Dao_Ani
	pGameObject = CDao_Ani::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Dao_Ani", pGameObject)))
		return E_FAIL;

	// Dizni_Ani
	pGameObject = CDizny_Ani::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Dizny_Ani", pGameObject)))
		return E_FAIL;

	// Color_Paper
	pGameObject = CColor_Paper::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object_Prototype(SCENE_STAGE, L"Object_Color_Paper", pGameObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage::Initialize_GameObject()
{
	CGameObject* pObject = nullptr;

	// Player
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Bazzi");
	if (pObject == nullptr)
		return E_FAIL; 
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Bazzi", pObject)))
		return E_FAIL;
	
	CGameObject* pCharacter = pObject;

	// Dao
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Dao");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Dao", pObject)))
		return E_FAIL;

	// Dizni
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Dizni");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Dizni", pObject)))
		return E_FAIL;

	// RankingManager
	mRankingManager = CRankingManager::GetInstance();
	mRankingManager->Initialize();

	mRankingManager->SetTransform(const_cast<CTransform*>(static_cast<const CTransform*>(m_pObjectMgr->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0)->Get_ComponentPointer(L"Component_Transform"))),
		const_cast<CTransform*>(static_cast<const CTransform*>(m_pObjectMgr->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0)->Get_ComponentPointer(L"Component_Transform"))),
		const_cast<CTransform*>(static_cast<const CTransform*>(m_pObjectMgr->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0)->Get_ComponentPointer(L"Component_Transform"))));


	// Camera
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Camera_Target");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Camera_Target", pObject)))
		return E_FAIL;
	((CCamera_Target*)pObject)->SetTarget((CTransform*)pCharacter->Get_ComponentPointer(L"Component_Transform"));


	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Test_NavMesh");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Test_NavMesh", pObject)))
		return E_FAIL;

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Camera_MiniMap");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Camera_MiniMap", pObject)))
		return E_FAIL;
	((CCamera_MiniMap*)pObject)->SetTarget((CTransform*)pCharacter->Get_ComponentPointer(L"Component_Transform"));

	// MiniMap
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_MiniMap");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_MiniMap", pObject)))
		return E_FAIL;

	// MiniMapIcon
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_MiniMap_Track");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_MiniMapIcon", pObject)))
		return E_FAIL;

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_MiniMap_DizniIcon");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_MiniMapIcon", pObject)))
		return E_FAIL;

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_MiniMap_DaoIcon");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_MiniMapIcon", pObject)))
		return E_FAIL;

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_MiniMap_PlayerIcon");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_MiniMapIcon", pObject)))
		return E_FAIL;

	// Time
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Time");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Time", pObject)))
		return E_FAIL;

	// Laps
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Laps");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Laps", pObject)))
		return E_FAIL;

	// Ranking
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Ranking");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Ranking", pObject)))
		return E_FAIL;

	// StartCounting
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_StartingCountUI");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_StartingCountUI", pObject)))
		return E_FAIL;

	// EndCounting
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_EndCountingUI");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_EndCountingUI", pObject)))
		return E_FAIL;

	///* Effect */

	//// Smoke_Effect
	//pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Smoke_Effect");
	//if (pObject == nullptr)
	//	return E_FAIL;
	//if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Smoke_Effect", pObject)))
	//	return E_FAIL;
	//dynamic_cast<CSmoke_Effect*>(pObject)->Set_Name(L"CartBox_Engine_Left");

	//pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Smoke_Effect");
	//if (pObject == nullptr)
	//	return E_FAIL;
	//if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Smoke_Effect", pObject)))
	//	return E_FAIL;
	//dynamic_cast<CSmoke_Effect*>(pObject)->Set_Name(L"CartBox_Engine_Right");

	return S_OK;
}

HRESULT CStage::Initialize_LoadObject()
{
	HANDLE				fFile = CreateFile(L"../Bin/Data/Map/map17.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	My_uLong			dwByte = 0;

	INFO_MAP			Info;

	CGameObject_Creator*	pCreator = CGameObject_Creator::GetInstance();

	while (true) {
		ReadFile(fFile, &Info, sizeof(INFO_MAP), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		CGameObject*	pObject = nullptr;
		pObject = pCreator->CreateObject_FromID(Info.ID, Info.SubID, Info.matWorld);
		if (pObject == nullptr) {
			continue;
			return E_FAIL;
		}

		if (7 == Info.ID && 2 == Info.SubID)
			m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_RotateHouse", pObject);

		else
			m_pObjectMgr->Add_Object(SCENE_STAGE, L"Map_Object", pObject);
		/*if (pObject != nullptr)
		m_pObjectMgr->Add_Object(SCENE_STAGE, L"Map_Object", pObject);*/
	}

	Engine::Safe_Release(pCreator);

	return S_OK;
}

HRESULT CStage::Initialize_Lighting()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	D3DLIGHT9	Light;
	memset(&Light, 0, sizeof(D3DLIGHT9));
	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Direction = D3DXVECTOR3(0.0f, -1.0f, 1.0f);
	Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_pDevice->SetLight(0, &Light);
	m_pDevice->LightEnable(0, TRUE);

	return S_OK;
}

HRESULT CStage::Initialize_Tetris()
{
	mTetrisManager = CTetrisManager::GetInstance();
	mTetrisManager->Initialize();

	// 테트리스 생성 및 tetrismanger에 pushBack
	CGameObject* pObject = nullptr;
	CTetris* tetris = nullptr;

	// 0번
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Blue");
	if (pObject == nullptr)
		return E_FAIL;

	tetris = static_cast<CTetris*>(pObject);

	tetris->SetDownCount(2);
	tetris->SetRespawnPos(My_Vec3(129.5f, 52.f, 255.f));
	tetris->SetStandingHeight(7.f);
	tetris->SetAngle(180.f, 0.f);
	tetris->SetIsMoving(true);
	tetris->SetScale();

	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Tetris", pObject)))
		return E_FAIL;

	mTetrisManager->SetTetris(tetris);


	// 1번
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Purple1");
	if (pObject == nullptr)
		return E_FAIL;

	tetris = static_cast<CTetris*>(pObject);

	tetris->SetDownCount(2);
	tetris->SetRespawnPos(My_Vec3(102.5f, 52.f, 255.f));
	tetris->SetStandingHeight(7.f);
	tetris->SetAngle(0.f, 0.f);
	tetris->SetIsMoving(true);
	tetris->SetScale();

	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Tetris", pObject)))
		return E_FAIL;

	mTetrisManager->SetTetris(tetris);


	// 2번
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Red");
	if (pObject == nullptr)
		return E_FAIL;

	tetris = static_cast<CTetris*>(pObject);

	tetris->SetDownCount(2);
	tetris->SetRespawnPos(My_Vec3(75.5f, 52.f, 255.f));
	tetris->SetStandingHeight(7.f);
	tetris->SetAngle(0.f, 0.f);
	tetris->SetIsMoving(true);
	tetris->SetScale();

	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Tetris", pObject)))
		return E_FAIL;

	mTetrisManager->SetTetris(tetris);


	// 3번
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Yellow2");
	if (pObject == nullptr)
		return E_FAIL;

	tetris = static_cast<CTetris*>(pObject);

	tetris->SetDownCount(1);
	tetris->SetRespawnPos(My_Vec3(66.5f, 52.f, 255.f));
	tetris->SetStandingHeight(7.f);
	tetris->SetAngle(0.f, 0.f);
	tetris->SetIsMoving(true);
	tetris->SetScale();

	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Tetris", pObject)))
		return E_FAIL;

	mTetrisManager->SetTetris(tetris);


	// 4번
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Purple2");
	if (pObject == nullptr)
		return E_FAIL;

	tetris = static_cast<CTetris*>(pObject);

	tetris->SetDownCount(2);
	tetris->SetRespawnPos(My_Vec3(57.5f, 68.5f, 255.f));
	tetris->SetStandingHeight(23.9f);
	tetris->SetAngle(0.f, 180.f);
	tetris->SetIsMoving(true);
	tetris->SetScale();

	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Tetris", pObject)))
		return E_FAIL;

	mTetrisManager->SetTetris(tetris);


	// 5번
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Yellow1");
	if (pObject == nullptr)
		return E_FAIL;

	tetris = static_cast<CTetris*>(pObject);

	tetris->SetDownCount(1);
	tetris->SetRespawnPos(My_Vec3(120.5f, 52.f, 255.f));
	tetris->SetStandingHeight(7.f);
	tetris->SetAngle(0.f, 0.f);
	tetris->SetIsMoving(true);
	tetris->SetScale();

	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Tetris", pObject)))
		return E_FAIL;

	mTetrisManager->SetTetris(tetris);


	// 6번
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Yellow2");
	if (pObject == nullptr)
		return E_FAIL;

	tetris = static_cast<CTetris*>(pObject);

	tetris->SetDownCount(2);
	tetris->SetRespawnPos(My_Vec3(97.5f, 65.f, 255.f));
	tetris->SetStandingHeight(20.f);
	tetris->SetAngle(0.f, 90.f);
	tetris->SetIsMoving(true);
	tetris->SetScale();
	tetris->SetNumber(1);

	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Tetris", pObject)))
		return E_FAIL;

	mTetrisManager->SetTetris(tetris);


	// 7번
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Red");
	if (pObject == nullptr)
		return E_FAIL;

	tetris = static_cast<CTetris*>(pObject);

	tetris->SetDownCount(2);
	tetris->SetRespawnPos(My_Vec3(129.5f, 52.f, 255.f));
	tetris->SetStandingHeight(7.f);
	tetris->SetAngle(0.f, 0.f);
	tetris->SetIsMoving(true);
	tetris->SetScale();

	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Tetris", pObject)))
		return E_FAIL;

	mTetrisManager->SetTetris(tetris);


	// 8번
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Blue");
	if (pObject == nullptr)
		return E_FAIL;

	tetris = static_cast<CTetris*>(pObject);

	tetris->SetDownCount(2);
	tetris->SetRespawnPos(My_Vec3(75.5f, 52.f, 255.f));
	tetris->SetStandingHeight(7.f);
	tetris->SetAngle(180.f, 0.f);
	tetris->SetIsMoving(true);
	tetris->SetScale();

	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Tetris", pObject)))
		return E_FAIL;

	mTetrisManager->SetTetris(tetris);


	// 9번
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Purple2");
	if (pObject == nullptr)
		return E_FAIL;

	tetris = static_cast<CTetris*>(pObject);

	tetris->SetDownCount(2);
	tetris->SetRespawnPos(My_Vec3(102.5f, 52.f, 255.f));
	tetris->SetStandingHeight(7.f);
	tetris->SetAngle(0.f, 0.f);
	tetris->SetIsMoving(true);
	tetris->SetScale();
	tetris->SetNumber(1);

	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Tetris", pObject)))
		return E_FAIL;

	mTetrisManager->SetTetris(tetris);


	// 10번
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Yellow2");
	if (pObject == nullptr)
		return E_FAIL;

	tetris = static_cast<CTetris*>(pObject);

	tetris->SetDownCount(1);
	tetris->SetRespawnPos(My_Vec3(120.5f, 52.f, 255.f));
	tetris->SetStandingHeight(7.f);
	tetris->SetAngle(0.f, 0.f);
	tetris->SetIsMoving(true);
	tetris->SetScale();

	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Tetris", pObject)))
		return E_FAIL;

	mTetrisManager->SetTetris(tetris);


	// 11번
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Yellow2");
	if (pObject == nullptr)
		return E_FAIL;

	tetris = static_cast<CTetris*>(pObject);

	tetris->SetDownCount(1);
	tetris->SetRespawnPos(My_Vec3(120.5f, 52.f, 255.f));
	tetris->SetStandingHeight(7.f);
	tetris->SetAngle(0.f, 0.f);
	tetris->SetIsMoving(true);
	tetris->SetScale();

	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Tetris", pObject)))
		return E_FAIL;

	mTetrisManager->SetTetris(tetris);

	// 12번
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Yellow1");
	if (pObject == nullptr)
		return E_FAIL;

	tetris = static_cast<CTetris*>(pObject);

	tetris->SetDownCount(1);
	tetris->SetRespawnPos(My_Vec3(66.5f, 52.f, 255.f));
	tetris->SetStandingHeight(7.f);
	tetris->SetAngle(0.f, 0.f);
	tetris->SetIsMoving(true);
	tetris->SetScale();

	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Tetris", pObject)))
		return E_FAIL;

	mTetrisManager->SetTetris(tetris);

	return S_OK;
}

HRESULT CStage::Initialize_UI()
{
	CGameObject* pObject = nullptr;

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_ItemSlot");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_UI", pObject)))
		return E_FAIL;

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_SpeedUI_BackGround");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_UI", pObject)))
		return E_FAIL;

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_SpeedUI_OutLine");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_UI", pObject)))
		return E_FAIL;

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_SpeedUI_Number");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_UI", pObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage::Initialize_Box()
{
	CGameObject* pObject = nullptr;

	// BoosterBox
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_BoosterBox");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_BoosterBox", pObject)))
		return E_FAIL;

	static_cast<CBoosterBox*>(pObject)->SetIsMoving(true);
	static_cast<CBoosterBox*>(pObject)->SetPos(My_Vec3(-1.0f, -3.2f, 1.5f));

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_BoosterBox");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_BoosterBox", pObject)))
		return E_FAIL;

	static_cast<CBoosterBox*>(pObject)->SetIsMoving(true);
	static_cast<CBoosterBox*>(pObject)->SetPos(My_Vec3(-5.0f, -3.2f, 1.5f));

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_BoosterBox");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_BoosterBox", pObject)))
		return E_FAIL;

	static_cast<CBoosterBox*>(pObject)->SetIsMoving(true);
	static_cast<CBoosterBox*>(pObject)->SetPos(My_Vec3(3.0f, -3.2f, 1.5f));


	// JumpBox
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_JumpBox");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_JumpBox", pObject)))
		return E_FAIL;

	static_cast<CJumpBox*>(pObject)->SetIsMoving(true);
	static_cast<CJumpBox*>(pObject)->SetPos(My_Vec3(-1.0f, 1.f, 1.5f));

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_JumpBox");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_JumpBox", pObject)))
		return E_FAIL;

	static_cast<CJumpBox*>(pObject)->SetIsMoving(true);
	static_cast<CJumpBox*>(pObject)->SetPos(My_Vec3(-5.0f, 1.f, 1.5f));

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_JumpBox");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_JumpBox", pObject)))
		return E_FAIL;

	static_cast<CJumpBox*>(pObject)->SetIsMoving(true);
	static_cast<CJumpBox*>(pObject)->SetPos(My_Vec3(3.0f, 1.f, 1.5f));

	return S_OK;
}

HRESULT CStage::Initialize_Award()
{
	CSoundMgr::GetInstance()->PlayBGM(L"FireWork.ogg");

	CGameObject* pObject = nullptr;

	// Award_Box
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Award_Box");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Award", pObject)))
		return E_FAIL;

	My_Int bazziRank, DaoRank, DizniRank;

	bazziRank = CRankingManager::GetInstance()->GetRank(CRankingManager::BAZZI);
	DaoRank = CRankingManager::GetInstance()->GetRank(CRankingManager::DAO);
	DizniRank = CRankingManager::GetInstance()->GetRank(CRankingManager::DIZNI);

	// Bazzi_Ani
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Bazzi_Ani");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Award", pObject)))
		return E_FAIL;
	const_cast<CBazzi_Ani*>(static_cast<const CBazzi_Ani*>(pObject))->SetRanking(bazziRank);

	// Dao_Ani
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Dao_Ani");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Award", pObject)))
		return E_FAIL;
	const_cast<CDao_Ani*>(static_cast<const CDao_Ani*>(pObject))->SetRanking(DaoRank);

	// Dizni_Ani
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Dizny_Ani");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Award", pObject)))
		return E_FAIL;
	const_cast<CDizny_Ani*>(static_cast<const CDizny_Ani*>(pObject))->SetRanking(DizniRank);

	const_cast<CCamera_Target*>(static_cast<const CCamera_Target*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Camera_Target", 0)))->SetState(CCamera_Target::AWARD);
	
	return S_OK;
}

My_Int CStage::Update(const My_Float& fTimeDelta)
{
	mTetrisManager->Update(fTimeDelta);
	
	if (mIsState)
	{
		mRankingManager->Update(fTimeDelta);

		if (GetAsyncKeyState('Q') & 0x8000)
		{
			CSoundMgr::GetInstance()->PlaySound(L"Count321.MP3", CSoundMgr::Channel_Effect);

			const_cast<CStartCountingUI*>(static_cast<const CStartCountingUI*>(m_pObjectMgr->Get_GameObject(SCENE_STAGE, L"Object_StartingCountUI", 0)))->SetIsStart();
			const_cast<CBazzi*>(static_cast<const CBazzi*>(m_pObjectMgr->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0)))->SetStartPos();
			const_cast<CBazzi*>(static_cast<const CBazzi*>(m_pObjectMgr->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0)))->SetIsCounting(true);
			const_cast<CDao*>(static_cast<const CDao*>(m_pObjectMgr->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0)))->SetStartPos();
			const_cast<CDizni*>(static_cast<const CDizni*>(m_pObjectMgr->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0)))->SetStartPos();
		}

		if (mIsAward)
		{
			if (true == m_bIsFirst)
			{
				m_bIsFirst = false;
				m_fFirstTime = fTimeDelta;

				CGameObject* pObject = nullptr;

				// 악어
				pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Crocodile");
				if (pObject == nullptr)
					return E_FAIL;
				if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Award_Crocodile", pObject)))
					return E_FAIL;
				dynamic_cast<CCrocodile*>(pObject)->SetIsAward(true);

				CGameObject* pBody = pObject;

				pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Crocodile_Face");
				if (pObject == nullptr)
					return E_FAIL;
				if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Award_Crocodile", pObject)))
					return E_FAIL;
				dynamic_cast<CCrocodile_Face*>(pObject)->Set_IsBoss(true);
				dynamic_cast<CCrocodile_Face*>(pObject)->SetBody(static_cast<CCrocodile*>(pBody));

				pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Crocodile_Tail");
				if (pObject == nullptr)
					return E_FAIL;
				if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Award_Crocodile", pObject)))
					return E_FAIL;
				dynamic_cast<CCrocodile_Tail*>(pObject)->Set_IsBoss(true);
				dynamic_cast<CCrocodile_Tail*>(pObject)->SetBody(static_cast<CCrocodile*>(pBody));
			}

			if (const_cast<CCrocodile*>(static_cast<const CCrocodile*>(m_pObjectMgr->Get_GameObject(SCENE_STAGE, L"Object_Award_Crocodile", 0)))->GetIsDead())
				SetBossStage();

			m_fAccTime += fTimeDelta;

			if (0.5f < (m_fAccTime - m_fFirstTime))
			{
				My_Int		iRandX = rand() % g_iBackCX;
				My_Int		iRandY = rand() % g_iBackCY;

				for (int i = 0; i < 18; ++i)
				{
					// Color_Paper
					CGameObject* pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Color_Paper");
					if (pObject == nullptr)
						return E_FAIL;
					if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Color_Paper", pObject)))
						return E_FAIL;

					My_Vec3		vPosition = { (My_Float)iRandX, (My_Float)iRandY, 0.f };
					My_Vec3		vLook = { 1.f, 0.f, 0.f };
					My_Matrix	matRot;
					D3DXMatrixIdentity(&matRot);
					D3DXMatrixRotationZ(&matRot, D3DXToRadian(20.f * i));
					D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
					dynamic_cast<CColor_Paper*>(pObject)->Set_Position(vPosition);
					dynamic_cast<CColor_Paper*>(pObject)->Set_Look(vLook);

				}
				m_fFirstTime = m_fAccTime;
			}
		}
	}

	return CScene::Update(fTimeDelta);
}

My_Int CStage::LastUpdate(const My_Float& fTimeDelta)
{
	My_Int iExit = CScene::LastUpdate(fTimeDelta);

	m_pCollisioner->Update();

	if (!mIsState)
	{
		if (nullptr != mStartButton && mStartButton->GetIsStart())
		{
			SetWatingRoom();
			mStartButton = nullptr;
		}

		if (nullptr != mGameStartButton && mGameStartButton->GetIsStart())
		{
			SetStage();
			mGameStartButton = nullptr;
		}
	}

	else
	{
		const CGameObject* object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_EndCountingUI", 0);

		if (GetAsyncKeyState('W') & 0x8000)
		{
			object = nullptr;
			mRankingManager->SetCheet();
		}

		if (nullptr == object && mIsAwardFirst)
		{
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::Channel_BGM);

			mIsAwardFirst = false;
			Initialize_Award();
			mIsAward = true;

			const_cast<CItemSlot*>(static_cast<const CItemSlot*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_UI", 0)))->SetIsAward(true);
			const_cast<CSpeedUIBackGround*>(static_cast<const CSpeedUIBackGround*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_UI", 1)))->SetIsAward(true);
			const_cast<CSpeedUINumber*>(static_cast<const CSpeedUINumber*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_UI", 3)))->SetIsAward(true);
			const_cast<CSpeedUIOutLine*>(static_cast<const CSpeedUIOutLine*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_UI", 2)))->SetIsAward(true);

			const int iIconSize = CGameObjectMgr::GetInstance()->Get_LayerSize(SCENE_STAGE, L"Object_Icon");

			for (int i = 0; i < iIconSize; ++i)
				const_cast<CItem_Icon*>(static_cast<const CItem_Icon*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Icon", i)))->SetIsAward(true);

			const_cast<CPlayerIcon*>(static_cast<const CPlayerIcon*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_MiniMapIcon", 0)))->SetIsAward(true);
			const_cast<CDaoIcon*>(static_cast<const CDaoIcon*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_MiniMapIcon", 0)))->SetIsAward(true);
			const_cast<CDizniIcon*>(static_cast<const CDizniIcon*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_MiniMapIcon", 0)))->SetIsAward(true);
			const_cast<CTrackMiniMap*>(static_cast<const CTrackMiniMap*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_MiniMapIcon", 0)))->SetIsAward(true);
			const_cast<CMiniMap*>(static_cast<const CMiniMap*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_MiniMap", 0)))->SetIsAward(true);
			const_cast<CTimeUI*>(static_cast<const CTimeUI*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Time", 0)))->SetIsAward(true);
			const_cast<CLapsUI*>(static_cast<const CLapsUI*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Laps", 0)))->SetIsAward(true);
			const_cast<CRankingUI*>(static_cast<const CRankingUI*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Ranking", 0)))->SetIsAward(true);
		}
	}
	
	return iExit;
}

void CStage::Render()
{
	CScene::Render();
}

HRESULT CStage::SetLogo()
{
	CGameObject* pObject = nullptr;

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Camera_Action");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Camera_Action", pObject)))
		return E_FAIL;

	// LogoBack
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_LogoBack");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Logo", pObject)))
		return E_FAIL;

	// StartButton
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_StartButton");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Logo", pObject)))
		return E_FAIL;
	mStartButton = static_cast<CStartButton*>(pObject);

	// EndButton
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_EndButton");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Logo", pObject)))
		return E_FAIL;

	// Terrain
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Terrain");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Terrain", pObject)))
		return E_FAIL;

	// Skybox
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_SkyBox");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_SkyBox", pObject)))
		return E_FAIL;

	// Water
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Water");
			if (pObject == nullptr)
				return E_FAIL;
			static_cast<CWater*>(pObject)->SetPositon(My_Vec3(195.f + 20.f * j, 5.75f, 100.f + 20.f * i));
			if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Water", pObject)))
				return E_FAIL;
		}
	}

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_SkidMark");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_SkidMark", pObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage::SetWatingRoom()
{
	m_pObjectMgr->Clear_Layer(SCENE_STAGE, L"Object_Logo");

	CGameObject* pObject = nullptr;

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_WaitingRoom");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_WaitingRoom", pObject)))
		return E_FAIL;

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_GameStartButton");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_WaitingRoom", pObject)))
		return E_FAIL;
	mGameStartButton = static_cast<CGameStartButton*>(pObject);

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Bazzi_Ani");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_WaitingRoom", pObject)))
		return E_FAIL;
	dynamic_cast<CBazzi_Ani*>(pObject)->Set_WaitingRoomPos();

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Dao_Ani");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_WaitingRoom", pObject)))
		return E_FAIL;
	dynamic_cast<CDao_Ani*>(pObject)->Set_WaitingRoomPos();

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Dizny_Ani");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_WaitingRoom", pObject)))
		return E_FAIL;
	dynamic_cast<CDizny_Ani*>(pObject)->Set_WaitingRoomPos();

	return S_OK;
}

HRESULT CStage::SetStage()
{
	m_pObjectMgr->Clear_Layer(SCENE_STAGE, L"Object_WaitingRoom");

	mIsState = true;
	CGameObject* object = const_cast<CGameObject*>(m_pObjectMgr->Get_GameObject(SCENE_STAGE, L"Object_Camera_Action", 0));
	static_cast<CCamera_Action*>(object)->SetIsStage(true);

	if (FAILED(Initialize_GameObject()))
		return E_FAIL;

	if (FAILED(Initialize_UI()))
		return E_FAIL;

	if (FAILED(Initialize_Box()))
		return E_FAIL;

	CDebugger::GetInstance()->Set_Camera_Target();

	return S_OK;
}

HRESULT CStage::SetBossStage()
{
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::Channel_BGM);
	CSoundMgr::GetInstance()->PlayBGM(L"Crocodile.ogg");
	CSoundMgr::GetInstance()->PlayBGM(L"Boss_Bgm.MP3");

	m_pObjectMgr->Clear_Layer(SCENE_STAGE, L"Object_Award");
	m_pObjectMgr->Clear_Layer(SCENE_STAGE, L"Object_Award_Crocodile");

	mIsAward = false;

	mTetrisManager->SetIsBoss(true);

	CGameObject* pObject = nullptr;

	// 악어
	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Crocodile");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Crocodile", pObject)))
		return E_FAIL;
	dynamic_cast<CCrocodile*>(pObject)->Set_IsBoss(true);

	CGameObject* pBody = pObject;

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Crocodile_Face");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Crocodile", pObject)))
		return E_FAIL;
	dynamic_cast<CCrocodile_Face*>(pObject)->Set_IsBoss(true);
	dynamic_cast<CCrocodile_Face*>(pObject)->SetBody(static_cast<CCrocodile*>(pBody));

	pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Crocodile_Tail");
	if (pObject == nullptr)
		return E_FAIL;
	if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Crocodile", pObject)))
		return E_FAIL;
	dynamic_cast<CCrocodile_Tail*>(pObject)->Set_IsBoss(true);
	dynamic_cast<CCrocodile_Tail*>(pObject)->SetBody(static_cast<CCrocodile*>(pBody));


	for (int i = 0; i < 10; ++i)
	{
		pObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Boss_HP");
		if (pObject == nullptr)
			return E_FAIL;
		if (FAILED(m_pObjectMgr->Add_Object(SCENE_STAGE, L"Object_Boss_HP", pObject)))
			return E_FAIL;
		dynamic_cast<CBoss_HP*>(pObject)->Get_TransformCom()->Set_Information(CTransform::INFO_POSITION, &My_Vec3(360.f + 60.f * i, 60.f, 0.f));
	}

	// UI
	const_cast<CItemSlot*>(static_cast<const CItemSlot*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_UI", 0)))->SetIsAward(false);
	const_cast<CSpeedUIBackGround*>(static_cast<const CSpeedUIBackGround*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_UI", 1)))->SetIsAward(false);
	const_cast<CSpeedUINumber*>(static_cast<const CSpeedUINumber*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_UI", 3)))->SetIsAward(false);
	const_cast<CSpeedUIOutLine*>(static_cast<const CSpeedUIOutLine*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_UI", 2)))->SetIsAward(false);

	const int iIconSize = CGameObjectMgr::GetInstance()->Get_LayerSize(SCENE_STAGE, L"Object_Icon");

	for (int i = 0; i < iIconSize; ++i)
		const_cast<CItem_Icon*>(static_cast<const CItem_Icon*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Icon", i)))->SetIsAward(false);

	const_cast<CPlayerIcon*>(static_cast<const CPlayerIcon*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_MiniMapIcon", 0)))->SetIsAward(false);
	const_cast<CTrackMiniMap*>(static_cast<const CTrackMiniMap*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_MiniMapIcon", 0)))->SetIsAward(false);
	const_cast<CMiniMap*>(static_cast<const CMiniMap*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_MiniMap", 0)))->SetIsAward(false);

	const_cast<CCamera_Target*>(static_cast<const CCamera_Target*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Camera_Target", 0)))->SetState(CCamera_Target::NORMAL);

	const_cast<CBazzi*>(static_cast<const CBazzi*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0)))->SetIsBoss(true);
	const_cast<CBazzi*>(static_cast<const CBazzi*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0)))->SetPos(&My_Vec3(120.f, 20.f, 200.f));

	return S_OK;
}

CStage * CStage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage* p = new CStage(pDevice);
	if (FAILED(p->Initialize())) {
		delete p;
		p = nullptr;
	}
	return p;
}

My_uLong CStage::ReleaseMemory()
{
	CRecordingManager::GetInstance()->Save();

	CRecordingManager::GetInstance()->DestroyInstance();
	m_pObjectMgr->Clear_GameObject(SCENE_STAGE);
	m_pComponentMgr->Clear_Component(SCENE_STAGE);
	mTetrisManager->DestroyInstance();
	mRankingManager->DestroyInstance();
	CScene::ReleaseMemory();
	return My_uLong();
}
