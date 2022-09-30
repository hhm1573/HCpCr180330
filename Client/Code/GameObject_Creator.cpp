#include "stdafx.h"
#include "..\Header\GameObject_Creator.h"
#include "GraphicDev.h"
#include "GameObject.h"
#include "GameObjectMgr.h"

#include "Bazzi.h"
#include "Dao.h"

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

IMPLEMENT_SINGLETON(CGameObject_Creator)

CGameObject_Creator::CGameObject_Creator()
{
	m_pDeviceMgr = CGraphicDev::GetInstance();
	m_pDeviceMgr->AddRef();

	m_pDevice = m_pDeviceMgr->Get_Graphic_Device();
	m_pDevice->AddRef();

	m_pObjectMgr = CGameObjectMgr::GetInstance();
	m_pObjectMgr->AddRef();
}

CGameObject * CGameObject_Creator::CreateObject_FromID(const My_uInt & ID, const My_uInt & Sub_ID, const My_Matrix & matWorld)
{
	CGameObject*	pGameObject = nullptr;

	switch (ID) {
	case 0:
		switch (Sub_ID) {
		case 0:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Bazzi");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 1:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Dao");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;
		}
		break;
		
	case 1:
		switch (Sub_ID) {
		case 0:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tree_Green1");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 1:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tree_Green2");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 2:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tree_Red");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;
		}
		break;

	case 2:
		pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Cloud");
		((CModel*)pGameObject)->Set_Matrix(&matWorld);
		break;

	case 3:
		switch (Sub_ID) {
		case 0:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Fence_Direct1");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 1:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Fence_Direct2");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 2:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Fence_Rot");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;
		}
		break;

	case 4:
		switch (Sub_ID) {
		case 0:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_BoosterBox");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 1:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_JumpBox");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 2:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_ItemBox");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;
		}
		break;

	case 5:
		switch (Sub_ID) {
		case 0:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Crocodile");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 1:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Crocodile_Face");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			
			My_Float angleX;

			if (mFaceCount < 3)
			{
				if (0 == mFaceCount)
					angleX = 0.f;

				else if (1 == mFaceCount)
					angleX = 10.f;

				else if (2 == mFaceCount)
					angleX = 5.f;

				static_cast<CCrocodile_Face*>(pGameObject)->SetAngle(angleX, 180.f);
				++mFaceCount;
			}

			else
			{
				if (3 == mFaceCount)
					angleX = 0.f;

				else if (4 == mFaceCount)
					angleX = 5.f;

				else if (5 == mFaceCount)
					angleX = 0.f;

				static_cast<CCrocodile_Face*>(pGameObject)->SetAngle(angleX, 0.f);
				++mFaceCount;
			}
			break;

		case 2:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Crocodile_Tail");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 3:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Crocodile_Tree");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;
		}
		break;

	case 6:
		pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_GreenBlock");
		((CModel*)pGameObject)->Set_Matrix(&matWorld);
	break;

	case 7:
		switch (Sub_ID) {
		case 0:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_House");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 1:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tree_HouseRed");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 2:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_RotateHouse");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			((CRotateHouse*)pGameObject)->SetOriPos(My_Vec3(matWorld._41, matWorld._42, matWorld._43 + 4.f));
			break;
		}
		break;

	case 8:
		switch (Sub_ID) {
		case 0:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_MediaHouse");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 3:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_MediaHouseTile1");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 4:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_MediaHouseTile2");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;
		}
		break;

	case 9:
		switch (Sub_ID) {
		case 0:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Blue");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 1:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Purple1");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 2:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Purple2");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 3:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Red");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 4:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Yellow1");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 5:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Yellow2");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;
		}
		break;

	case 10:
		switch (Sub_ID) {
		case 0:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Bridge_Red");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 1:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_Tetris_Bridge_White");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;

		case 2:
			pGameObject = m_pObjectMgr->Clone_GameObject(SCENE_STAGE, L"Object_KartRiderSign");
			((CModel*)pGameObject)->Set_Matrix(&matWorld);
			break;
		}
		break;
	}

	return pGameObject;
}

My_uLong CGameObject_Creator::ReleaseMemory(void)
{
	Safe_Release(m_pDeviceMgr);
	Safe_Release(m_pDevice);
	Safe_Release(m_pObjectMgr);
	return My_uLong();
}
