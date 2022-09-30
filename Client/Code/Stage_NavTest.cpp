#include "stdafx.h"
#include "..\Header\Stage_NavTest.h"
#include "InputDev.h"
#include "Debugger.h"
#include "GraphicDev.h"

const My_Float	g_fSize = 4.0f;

CStage_NavTest::CStage_NavTest(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
	/*m_pFuncRender = new (void(CStage_NavTest::*[MODE_END])(void));
	m_pFuncRender[MODE_SEL] = &Render_Sel;
	m_pFuncRender[MODE_ADD] = &Render_Add;
	m_pFuncRender[MODE_MOVE] = &Render_Move;
	m_pFuncRender[MODE_TILT] = &Render_Tilt;*/
}

CStage_NavTest::CStage_NavTest(const CStage_NavTest & Ref)
	: CGameObject(Ref)
{
}

HRESULT CStage_NavTest::Initialize()
{
	m_pInputDev = CInputDev::GetInstance();

	m_pSprite = CGraphicDev::GetInstance()->Get_Sprite();
	if (m_pSprite != nullptr)
		m_pSprite->AddRef();
	m_pFont = CGraphicDev::GetInstance()->Get_Font();
	if (m_pFont != nullptr)
		m_pFont->AddRef();

	m_pRenderer = (CRenderer*)m_pComponentMgr->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (m_pRenderer == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Renderer", m_pRenderer)))
		return E_FAIL;

	m_pRectCol = (CRectCol*)m_pComponentMgr->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectCol");
	if (m_pRectCol == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(L"Component_Buffer_RectCol", m_pRectCol)))
		return E_FAIL;

	m_pDebugger = CDebugger::GetInstance();
	m_pDebugger->AddRef();

	return S_OK;
}

HRESULT CStage_NavTest::Initialize_Prototype()
{
	return S_OK;
}

My_Int CStage_NavTest::Update(const My_Float & fTimeDelta)
{
	if (m_pDebugger->Get_IsNavMode() == false)
		return 0;

	void(CStage_NavTest::*pFunc[MODE_END])(My_Int) = { &CStage_NavTest::Command_Sel, &CStage_NavTest::Command_Add,&CStage_NavTest::Command_Move,&CStage_NavTest::Command_Tilt,&CStage_NavTest::Command_State, &CStage_NavTest::Command_Menu };

	/*if (m_pInputDev->Get_DIK_IsPressed(DIK_NUMPAD7) == true) {
	Move_Left();
	}
	if (m_pInputDev->Get_DIK_IsPressed(DIK_NUMPAD8) == true) {
	Sel_Next();
	}
	if (m_pInputDev->Get_DIK_IsPressed(DIK_NUMPAD9) == true) {
	Add_New();
	}*/

	if (m_pInputDev->Get_DIKState(DIK_NUMPADSTAR) & 0x80) {
		if (m_pInputDev->Get_DIKState(DIK_NUMPAD7) & 0x80)
			(this->*pFunc[m_eMode])(7);
		if (m_pInputDev->Get_DIKState(DIK_NUMPAD8) & 0x80)
			(this->*pFunc[m_eMode])(8);
		if (m_pInputDev->Get_DIKState(DIK_NUMPAD9) & 0x80)
			(this->*pFunc[m_eMode])(9);

		if (m_pInputDev->Get_DIKState(DIK_NUMPAD4) & 0x80)
			(this->*pFunc[m_eMode])(4);
		if (m_pInputDev->Get_DIKState(DIK_NUMPAD5) & 0x80)
			(this->*pFunc[m_eMode])(5);
		if (m_pInputDev->Get_DIKState(DIK_NUMPAD6) & 0x80)
			(this->*pFunc[m_eMode])(6);

		if (m_pInputDev->Get_DIKState(DIK_NUMPAD1) & 0x80) {
			Mode_Prev();
		}
		if (m_pInputDev->Get_DIKState(DIK_NUMPAD2) & 0x80)
			(this->*pFunc[m_eMode])(2);
		if (m_pInputDev->Get_DIKState(DIK_NUMPAD3) & 0x80) {
			Mode_Next();
		}
	}

	if (m_pInputDev->Get_DIK_IsPressed(DIK_NUMPAD7) == true)
		(this->*pFunc[m_eMode])(7);
	if (m_pInputDev->Get_DIK_IsPressed(DIK_NUMPAD8) == true)
		(this->*pFunc[m_eMode])(8);
	if (m_pInputDev->Get_DIK_IsPressed(DIK_NUMPAD9) == true)
		(this->*pFunc[m_eMode])(9);

	if (m_pInputDev->Get_DIK_IsPressed(DIK_NUMPAD4) == true)
		(this->*pFunc[m_eMode])(4);
	if (m_pInputDev->Get_DIK_IsPressed(DIK_NUMPAD5) == true)
		(this->*pFunc[m_eMode])(5);
	if (m_pInputDev->Get_DIK_IsPressed(DIK_NUMPAD6) == true)
		(this->*pFunc[m_eMode])(6);

	if (m_pInputDev->Get_DIK_IsPressed(DIK_NUMPAD1) == true) {
		Mode_Prev();
	}
	if (m_pInputDev->Get_DIK_IsPressed(DIK_NUMPAD2) == true)
		(this->*pFunc[m_eMode])(2);
	if (m_pInputDev->Get_DIK_IsPressed(DIK_NUMPAD3) == true) {
		Mode_Next();
	}
	return My_Int();
}

My_Int CStage_NavTest::LastUpdate(const My_Float & fTimeDelta)
{
	if (m_pDebugger->Get_IsNavMode() == false)
		return 0;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	return My_Int();
}

void CStage_NavTest::Render()
{
	My_Matrix	matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	for (auto& pNav : m_List_Nav) {
		m_pRectCol->Set_Verticies(pNav->vLT, pNav->vRT, pNav->vLB, pNav->vRB);
		m_pRectCol->SetColor(D3DCOLOR_ARGB(255, 0, 100, 255));
		if (pNav->wState & 0x8000)
			m_pRectCol->SetColor(D3DCOLOR_ARGB(255, 255, 0, 0));
		if (pNav == m_pNav_Sel)
			m_pRectCol->SetColor(D3DCOLOR_ARGB(255, 0, 200, 255));
		m_pRectCol->Render();
	}
	m_pRectCol->Reset_Verticies();

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	if (m_pDebugger->Get_IsWireFrame() == true)
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	/*void(CStage_NavTest::*pFunc[MODE_END])(void) = { Render_Sel,Render_Add,Render_Move,Render_Tilt };
	(this->*pFunc[m_eMode])();

	CallRender(m_eMode);*/
	/*void(CStage_NavTest::*pFunc)(void) = &CStage_NavTest::Render_Sel;
	(this->*pFunc)();*/

	switch (m_eMode) {
	case MODE_SEL:
		Render_Sel();
		break;
	case MODE_ADD:
		Render_Add();
		break;
	case MODE_MOVE:
		Render_Move();
		break;
	case MODE_TILT:
		Render_Tilt();
		break;
	case MODE_STATE:
		Render_State();
		break;
	case MODE_MENU:
		Render_Menu();
		break;
	}
}



//Render
#if true

void CStage_NavTest::Render_Sel()
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	My_Long		lFontHeight = 12;

	RECT		rt = { 200,0,0,0 };
	m_pFont->DrawTextW(m_pSprite, L"NavMode : Select", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 1;
	m_pFont->DrawTextW(m_pSprite, L"7 :SelPrev    8 :Front         9:SelNext", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 2;
	m_pFont->DrawTextW(m_pSprite, L"4 :Left          5 :Back         6:Right", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 3;
	m_pFont->DrawTextW(m_pSprite, L"1 :PrevMode 2 :                3 :NextMode", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pSprite->End();
}

void CStage_NavTest::Render_Add()
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	My_Long		lFontHeight = 12;

	RECT		rt = { 200,0,0,0 };
	m_pFont->DrawTextW(m_pSprite, L"NavMode : Add/Delete", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 1;
	m_pFont->DrawTextW(m_pSprite, L"7 :M_Add     8 :Front        9:M_Delete", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 2;
	m_pFont->DrawTextW(m_pSprite, L"4 :Left          5 :Back         6:Right", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 3;
	if (m_bDeleteMode == false)
		m_pFont->DrawTextW(m_pSprite, L"1 :PrevMode 2 :Zero          3 :NextMode", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		m_pFont->DrawTextW(m_pSprite, L"1 :PrevMode 2 :Cur         3 :NextMode", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 4;
	if (m_bDeleteMode == false)
		m_pFont->DrawTextW(m_pSprite, L"Mode : Add", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		m_pFont->DrawTextW(m_pSprite, L"Mode : Delete", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pSprite->End();
}

void CStage_NavTest::Render_Move()
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	My_Long		lFontHeight = 12;

	RECT		rt = { 200,0,0,0 };
	m_pFont->DrawTextW(m_pSprite, L"NavMode : Move", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 1;
	m_pFont->DrawTextW(m_pSprite, L"7 :Down       8 :Front        9:Up", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 2;
	m_pFont->DrawTextW(m_pSprite, L"4 :Left          5 :Back         6:Right", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 3;
	m_pFont->DrawTextW(m_pSprite, L"1 :PrevMode 2 :                3 :NextMode", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pSprite->End();
}

void CStage_NavTest::Render_Tilt()
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	My_Long		lFontHeight = 12;

	RECT		rt = { 200,0,0,0 };
	m_pFont->DrawTextW(m_pSprite, L"NavMode : Tilt", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 1;
	m_pFont->DrawTextW(m_pSprite, L"7 :M_Down   8 :Front        9:M_Up", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 2;
	m_pFont->DrawTextW(m_pSprite, L"4 :Left          5 :Back         6:Right", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 3;
	m_pFont->DrawTextW(m_pSprite, L"1 :PrevMode 2 :                3 :NextMode", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 4;
	if (m_bDownMode == false)
		m_pFont->DrawTextW(m_pSprite, L"Mode : Up", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		m_pFont->DrawTextW(m_pSprite, L"Mode : Down", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pSprite->End();
}

void CStage_NavTest::Render_State()
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	My_Long		lFontHeight = 12;

	RECT		rt = { 200,0,0,0 };
	m_pFont->DrawTextW(m_pSprite, L"NavMode : Tilt", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 1;
	m_pFont->DrawTextW(m_pSprite, L"7 :                8 :Front        9:", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 2;
	m_pFont->DrawTextW(m_pSprite, L"4 :Left          5 :Back         6:Right", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 3;
	m_pFont->DrawTextW(m_pSprite, L"1 :PrevMode 2 :Toggle        3 :NextMode", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pSprite->End();
}

void CStage_NavTest::Render_Menu()
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	My_Long		lFontHeight = 12;

	RECT		rt = { 200,0,0,0 };
	m_pFont->DrawTextW(m_pSprite, L"NavMode : Menu", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 1;
	m_pFont->DrawTextW(m_pSprite, L"7 :Save    8 :Connect       9:Load", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 2;
	m_pFont->DrawTextW(m_pSprite, L"4 :                5 :                6:", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 3;
	m_pFont->DrawTextW(m_pSprite, L"1 :PrevMode 2 :                3 :NextMode", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pSprite->End();
}

#endif



//Command
#if true

void CStage_NavTest::Command_Sel(My_Int iCommand)
{
	switch (iCommand) {
	case 7:
		Sel_Prev();
		break;
	case 8:
		Sel_Front();
		break;
	case 9:
		Sel_Next();
		break;

	case 4:
		Sel_Left();
		break;
	case 5:
		Sel_Back();
		break;
	case 6:
		Sel_Right();
		break;

	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}
}

void CStage_NavTest::Command_Add(My_Int iCommand)
{
	switch (iCommand) {
	case 7:
		m_bDeleteMode = false;
		break;
	case 8:
		if (m_bDeleteMode == true)
			Delete_Front();
		else
			Add_Front();
		break;
	case 9:
		m_bDeleteMode = true;
		break;

	case 4:
		(m_bDeleteMode == true) ? Delete_Left() : Add_Left();
		break;
	case 5:
		(m_bDeleteMode == true) ? Delete_Back() : Add_Back();
		break;
	case 6:
		(m_bDeleteMode == true) ? Delete_Right() : Add_Right();
		break;

	case 1:
		break;
	case 2:
		(m_bDeleteMode == true) ? Delete_Cur() : Add_New();
		break;
	case 3:
		break;
	}
}

void CStage_NavTest::Command_Move(My_Int iCommand)
{
	switch (iCommand) {
	case 7:
		Move_Down();
		break;
	case 8:
		Move_Forward();
		break;
	case 9:
		Move_Up();
		break;

	case 4:
		Move_Left();
		break;
	case 5:
		Move_Backward();
		break;
	case 6:
		Move_Right();
		break;

	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}
}

void CStage_NavTest::Command_Tilt(My_Int iCommand)
{
	switch (iCommand) {
	case 7:
		m_bDownMode = true;
		break;
	case 8:
		(m_bDownMode == true) ? Tilt_Front(false) : Tilt_Front(true);
		break;
	case 9:
		m_bDownMode = false;
		break;

	case 4:
		(m_bDownMode == true) ? Tilt_Left(false) : Tilt_Left(true);
		break;
	case 5:
		(m_bDownMode == true) ? Tilt_Back(false) : Tilt_Back(true);
		break;
	case 6:
		(m_bDownMode == true) ? Tilt_Right(false) : Tilt_Right(true);
		break;

	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}
}

void CStage_NavTest::Command_State(My_Int iCommand)
{
	switch (iCommand) {
	case 7:
		break;
	case 8:
		State_Fall_Forward();
		break;
	case 9:
		break;

	case 4:
		State_Fall_Left();
		break;
	case 5:
		State_Fall_Backward();
		break;
	case 6:
		State_Fall_Right();
		break;

	case 1:
		break;
	case 2:
		State_Fall_Toggle();
		break;
	case 3:
		break;
	}
}

void CStage_NavTest::Command_Menu(My_Int iCommand)
{
	switch (iCommand) {
	case 7:
		Save();
		break;
	case 8:
		ConnectAll();
		break;
	case 9:
		Load();
		break;

	case 4:
		break;
	case 5:
		break;
	case 6:
		break;

	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}
}

#endif



//Sel
#if true

void CStage_NavTest::Sel_Next()
{
	if (m_List_Nav.size() == 0)
		return;

	if (m_pNav_Sel == nullptr) {
		m_pNav_Sel = m_List_Nav.front();
		return;
	}

	auto	iterBegin = m_List_Nav.begin();
	auto	iterEnd = m_List_Nav.end();
	for (; iterBegin != iterEnd; ++iterBegin) {
		if (m_pNav_Sel == (*iterBegin) == true) {
			m_pNav_Sel = (++iterBegin == iterEnd) ? m_List_Nav.front() : *(iterBegin);
			return;
		}
	}

	m_pNav_Sel = m_List_Nav.front();
}

void CStage_NavTest::Sel_Prev()
{
	if (m_List_Nav.size() == 0)
		return;

	if (m_pNav_Sel == nullptr) {
		m_pNav_Sel = m_List_Nav.front();
		return;
	}

	auto	iterBegin = m_List_Nav.begin();
	auto	iterEnd = m_List_Nav.end();
	for (; iterBegin != iterEnd; ++iterBegin) {
		if (m_pNav_Sel == (*iterBegin)) {
			m_pNav_Sel = (iterBegin == m_List_Nav.begin()) ? m_List_Nav.back() : *(--iterBegin);
			return;
		}
	}

	m_pNav_Sel = m_List_Nav.front();
}

void CStage_NavTest::Sel_Front()
{
	if (m_List_Nav.size() == 0)
		return;

	if (m_pNav_Sel == nullptr) {
		if (m_List_Nav.size() != 0)
			m_pNav_Sel = m_List_Nav.front();
		return;
	}

	if (m_pNav_Sel->pU != nullptr)
		m_pNav_Sel = m_pNav_Sel->pU;
}

void CStage_NavTest::Sel_Back()
{
	if (m_List_Nav.size() == 0)
		return;

	if (m_pNav_Sel == nullptr) {
		if (m_List_Nav.size() != 0)
			m_pNav_Sel = m_List_Nav.front();
		return;
	}

	if (m_pNav_Sel->pB != nullptr)
		m_pNav_Sel = m_pNav_Sel->pB;
}

void CStage_NavTest::Sel_Left()
{
	if (m_List_Nav.size() == 0)
		return;

	if (m_pNav_Sel == nullptr) {
		if (m_List_Nav.size() != 0)
			m_pNav_Sel = m_List_Nav.front();
		return;
	}

	if (m_pNav_Sel->pL != nullptr)
		m_pNav_Sel = m_pNav_Sel->pL;
}

void CStage_NavTest::Sel_Right()
{
	if (m_List_Nav.size() == 0)
		return;

	if (m_pNav_Sel == nullptr) {
		if (m_List_Nav.size() != 0)
			m_pNav_Sel = m_List_Nav.front();
		return;
	}

	if (m_pNav_Sel->pR != nullptr)
		m_pNav_Sel = m_pNav_Sel->pR;
}

#endif



//Move
#if true

void CStage_NavTest::Move_Left()
{
	if (m_List_Nav.size() == 0)
		return;
	if (m_pNav_Sel == nullptr)
		m_pNav_Sel = m_List_Nav.front();

	My_Float fMove = g_fSize;
	if (m_pInputDev->Get_DIKState(DIK_NUMPAD0) & 0x80)
		fMove = 0.5f;

	m_pNav_Sel->vLT.x -= fMove;
	m_pNav_Sel->vRT.x -= fMove;
	m_pNav_Sel->vLB.x -= fMove;
	m_pNav_Sel->vRB.x -= fMove;
}

void CStage_NavTest::Move_Right()
{
	if (m_List_Nav.size() == 0)
		return;
	if (m_pNav_Sel == nullptr)
		m_pNav_Sel = m_List_Nav.front();

	My_Float fMove = g_fSize;
	if (m_pInputDev->Get_DIKState(DIK_NUMPAD0) & 0x80)
		fMove = 0.5f;

	m_pNav_Sel->vLT.x += fMove;
	m_pNav_Sel->vRT.x += fMove;
	m_pNav_Sel->vLB.x += fMove;
	m_pNav_Sel->vRB.x += fMove;
}

void CStage_NavTest::Move_Forward()
{
	if (m_List_Nav.size() == 0)
		return;
	if (m_pNav_Sel == nullptr)
		m_pNav_Sel = m_List_Nav.front();

	My_Float fMove = g_fSize;
	if (m_pInputDev->Get_DIKState(DIK_NUMPAD0) & 0x80)
		fMove = 0.5f;

	m_pNav_Sel->vLT.z += fMove;
	m_pNav_Sel->vRT.z += fMove;
	m_pNav_Sel->vLB.z += fMove;
	m_pNav_Sel->vRB.z += fMove;
}

void CStage_NavTest::Move_Backward()
{
	if (m_List_Nav.size() == 0)
		return;
	if (m_pNav_Sel == nullptr)
		m_pNav_Sel = m_List_Nav.front();

	My_Float fMove = g_fSize;
	if (m_pInputDev->Get_DIKState(DIK_NUMPAD0) & 0x80)
		fMove = 0.5f;

	m_pNav_Sel->vLT.z -= fMove;
	m_pNav_Sel->vRT.z -= fMove;
	m_pNav_Sel->vLB.z -= fMove;
	m_pNav_Sel->vRB.z -= fMove;
}

void CStage_NavTest::Move_Up()
{
	if (m_List_Nav.size() == 0)
		return;
	if (m_pNav_Sel == nullptr)
		m_pNav_Sel = m_List_Nav.front();

	My_Float fMove = g_fSize;
	if (m_pInputDev->Get_DIKState(DIK_NUMPAD0) & 0x80)
		fMove = 0.5f;

	m_pNav_Sel->vLT.y += fMove;
	m_pNav_Sel->vRT.y += fMove;
	m_pNav_Sel->vLB.y += fMove;
	m_pNav_Sel->vRB.y += fMove;
}

void CStage_NavTest::Move_Down()
{
	if (m_List_Nav.size() == 0)
		return;
	if (m_pNav_Sel == nullptr)
		m_pNav_Sel = m_List_Nav.front();

	My_Float fMove = g_fSize;
	if (m_pInputDev->Get_DIKState(DIK_NUMPAD0) & 0x80)
		fMove = 0.5f;

	m_pNav_Sel->vLT.y -= fMove;
	m_pNav_Sel->vRT.y -= fMove;
	m_pNav_Sel->vLB.y -= fMove;
	m_pNav_Sel->vRB.y -= fMove;
}

#endif



//Add
#if true

void CStage_NavTest::Add_New()
{

	NAVMESH*		pNav = new NAVMESH;

	if (m_pNav_Sel != nullptr) {
		pNav->vLT = m_pNav_Sel->vLT;
		pNav->vRT = pNav->vLT + D3DXVECTOR3(g_fSize, 0.0f, 0.0f);
		pNav->vLB = pNav->vLT + D3DXVECTOR3(0.0f, 0.0f, -g_fSize);
		pNav->vRB = pNav->vLT + D3DXVECTOR3(g_fSize, 0.0f, -g_fSize);;
	}
	else {
		pNav->vLT = D3DXVECTOR3(0.0f, 0.0f, g_fSize);
		pNav->vRT = D3DXVECTOR3(g_fSize, 0.0f, g_fSize);
		pNav->vLB = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pNav->vRB = D3DXVECTOR3(g_fSize, 0.0f, 0.0f);
	}
	pNav->pU = nullptr;
	pNav->pB = nullptr;
	pNav->pL = nullptr;
	pNav->pR = nullptr;
	pNav->wState = 0;

	m_pNav_Sel = pNav;

	m_List_Nav.push_back(pNav);
}

void CStage_NavTest::Add_Left()
{
	if (m_pNav_Sel == nullptr)
		return;

	My_Vec3 vOffSet = My_Vec3(-g_fSize, 0.0f, 0.0f);

	NAVMESH* pNav = new NAVMESH;
	pNav->vLT = m_pNav_Sel->vLT + vOffSet;
	pNav->vRT = m_pNav_Sel->vRT + vOffSet;
	pNav->vLB = m_pNav_Sel->vLB + vOffSet;
	pNav->vRB = m_pNav_Sel->vRB + vOffSet;
	pNav->pU = nullptr;
	pNav->pB = nullptr;
	pNav->pL = nullptr;
	pNav->pR = nullptr;
	pNav->wState = 0;

	m_pNav_Sel = pNav;

	m_List_Nav.push_back(pNav);
}

void CStage_NavTest::Add_Right()
{
	if (m_pNav_Sel == nullptr)
		return;

	My_Vec3 vOffSet = My_Vec3(g_fSize, 0.0f, 0.0f);

	NAVMESH* pNav = new NAVMESH;
	pNav->vLT = m_pNav_Sel->vLT + vOffSet;
	pNav->vRT = m_pNav_Sel->vRT + vOffSet;
	pNav->vLB = m_pNav_Sel->vLB + vOffSet;
	pNav->vRB = m_pNav_Sel->vRB + vOffSet;
	pNav->pU = nullptr;
	pNav->pB = nullptr;
	pNav->pL = nullptr;
	pNav->pR = nullptr;
	pNav->wState = 0;

	m_pNav_Sel = pNav;

	m_List_Nav.push_back(pNav);
}

void CStage_NavTest::Add_Front()
{
	if (m_pNav_Sel == nullptr)
		return;

	My_Vec3 vOffSet = My_Vec3(0.0f, 0.0f, g_fSize);

	NAVMESH* pNav = new NAVMESH;
	pNav->vLT = m_pNav_Sel->vLT + vOffSet;
	pNav->vRT = m_pNav_Sel->vRT + vOffSet;
	pNav->vLB = m_pNav_Sel->vLB + vOffSet;
	pNav->vRB = m_pNav_Sel->vRB + vOffSet;
	pNav->pU = nullptr;
	pNav->pB = nullptr;
	pNav->pL = nullptr;
	pNav->pR = nullptr;
	pNav->wState = 0;

	m_pNav_Sel = pNav;

	m_List_Nav.push_back(pNav);
}

void CStage_NavTest::Add_Back()
{
	if (m_pNav_Sel == nullptr)
		return;

	My_Vec3 vOffSet = My_Vec3(0.0f, 0.0f, -g_fSize);

	NAVMESH* pNav = new NAVMESH;
	pNav->vLT = m_pNav_Sel->vLT + vOffSet;
	pNav->vRT = m_pNav_Sel->vRT + vOffSet;
	pNav->vLB = m_pNav_Sel->vLB + vOffSet;
	pNav->vRB = m_pNav_Sel->vRB + vOffSet;
	pNav->pU = nullptr;
	pNav->pB = nullptr;
	pNav->pL = nullptr;
	pNav->pR = nullptr;
	pNav->wState = 0;

	m_pNav_Sel = pNav;

	m_List_Nav.push_back(pNav);
}

#endif



//Delete
#if true

void CStage_NavTest::Delete_Cur()
{
	if (m_pNav_Sel == nullptr)
		return;

	Delete_Nav(m_pNav_Sel);
	if (m_List_Nav.size() != 0)
		m_pNav_Sel = m_List_Nav.back();
}

void CStage_NavTest::Delete_Left()
{
	if (m_pNav_Sel == nullptr)
		return;

	if (m_pNav_Sel->pL == nullptr)
		return;

	NAVMESH*	pTemp = m_pNav_Sel->pL;
	Delete_Nav(m_pNav_Sel);
	m_pNav_Sel = pTemp;
}

void CStage_NavTest::Delete_Right()
{
	if (m_pNav_Sel == nullptr)
		return;

	if (m_pNav_Sel->pR == nullptr)
		return;

	NAVMESH*	pTemp = m_pNav_Sel->pR;
	Delete_Nav(m_pNav_Sel);
	m_pNav_Sel = pTemp;
}

void CStage_NavTest::Delete_Front()
{
	if (m_pNav_Sel == nullptr)
		return;

	if (m_pNav_Sel->pU == nullptr)
		return;

	NAVMESH*	pTemp = m_pNav_Sel->pU;
	Delete_Nav(m_pNav_Sel);
	m_pNav_Sel = pTemp;
}

void CStage_NavTest::Delete_Back()
{
	if (m_pNav_Sel == nullptr)
		return;

	if (m_pNav_Sel->pB == nullptr)
		return;

	NAVMESH*	pTemp = m_pNav_Sel->pB;
	Delete_Nav(m_pNav_Sel);
	m_pNav_Sel = pTemp;
}

void CStage_NavTest::Delete_Nav(NAVMESH * pNav)
{
	if (pNav == nullptr)
		return;

	list<NAVMESH*>::iterator iterBegin = m_List_Nav.begin();
	list<NAVMESH*>::iterator iterEnd = m_List_Nav.end();

	for (; iterBegin != iterEnd; ++iterBegin) {
		if (pNav == *iterBegin) {
			delete pNav;
			pNav = nullptr;
			m_List_Nav.erase(iterBegin);
			return;
		}
	}
}

#endif



//Tilt
#if true

void CStage_NavTest::Tilt_Left(My_Bool bUp)
{
	if (m_pNav_Sel == nullptr) {
		if (m_List_Nav.size() != 0)
			m_pNav_Sel = m_List_Nav.front();
		return;
	}

	My_Float	fMult = (bUp == true) ? 1.0f : -1.0f;
	if (m_pInputDev->Get_DIKState(DIK_NUMPAD0) & 0x80)
		fMult *= 0.5f;

	m_pNav_Sel->vLT += D3DXVECTOR3(0.0f, 1.0f, 0.0f) * fMult;
	m_pNav_Sel->vLB += D3DXVECTOR3(0.0f, 1.0f, 0.0f) * fMult;
}

void CStage_NavTest::Tilt_Right(My_Bool bUp)
{
	if (m_pNav_Sel == nullptr) {
		if (m_List_Nav.size() != 0)
			m_pNav_Sel = m_List_Nav.front();
		return;
	}

	My_Float	fMult = (bUp == true) ? 1.0f : -1.0f;
	if (m_pInputDev->Get_DIKState(DIK_NUMPAD0) & 0x80)
		fMult *= 0.5f;

	m_pNav_Sel->vRT += D3DXVECTOR3(0.0f, 1.0f, 0.0f) * fMult;
	m_pNav_Sel->vRB += D3DXVECTOR3(0.0f, 1.0f, 0.0f) * fMult;
}

void CStage_NavTest::Tilt_Front(My_Bool bUp)
{
	if (m_pNav_Sel == nullptr) {
		if (m_List_Nav.size() != 0)
			m_pNav_Sel = m_List_Nav.front();
		return;
	}

	My_Float	fMult = (bUp == true) ? 1.0f : -1.0f;
	if (m_pInputDev->Get_DIKState(DIK_NUMPAD0) & 0x80)
		fMult *= 0.5f;

	m_pNav_Sel->vLT += D3DXVECTOR3(0.0f, 1.0f, 0.0f) * fMult;
	m_pNav_Sel->vRT += D3DXVECTOR3(0.0f, 1.0f, 0.0f) * fMult;
}

void CStage_NavTest::Tilt_Back(My_Bool bUp)
{
	if (m_pNav_Sel == nullptr) {
		if (m_List_Nav.size() != 0)
			m_pNav_Sel = m_List_Nav.front();
		return;
	}

	My_Float	fMult = (bUp == true) ? 1.0f : -1.0f;
	if (m_pInputDev->Get_DIKState(DIK_NUMPAD0) & 0x80)
		fMult *= 0.5f;

	m_pNav_Sel->vLB += D3DXVECTOR3(0.0f, 1.0f, 0.0f) * fMult;
	m_pNav_Sel->vRB += D3DXVECTOR3(0.0f, 1.0f, 0.0f) * fMult;
}

#endif



//State
#if true

void CStage_NavTest::State_Fall_Left()
{
	if (m_pNav_Sel == nullptr)
		return;

	if (m_pNav_Sel->wState & 0x0002) {
		MSG_BOX("Turned Off");
		m_pNav_Sel->wState &= ~0x0002;
	}
	else {
		MSG_BOX("Turned On");
		m_pNav_Sel->wState |= 0x0002;
	}
}

void CStage_NavTest::State_Fall_Right()
{
	if (m_pNav_Sel == nullptr)
		return;

	if (m_pNav_Sel->wState & 0x0001) {
		MSG_BOX("Turned Off");
		m_pNav_Sel->wState &= ~0x0001;
	}
	else {
		MSG_BOX("Turned On");
		m_pNav_Sel->wState |= 0x0001;
	}
}

void CStage_NavTest::State_Fall_Forward()
{
	if (m_pNav_Sel == nullptr)
		return;

	if (m_pNav_Sel->wState & 0x0008) {
		MSG_BOX("Turned Off");
		m_pNav_Sel->wState &= ~0x0008;
	}
	else {
		MSG_BOX("Turned On");
		m_pNav_Sel->wState |= 0x0008;
	}
}

void CStage_NavTest::State_Fall_Backward()
{
	if (m_pNav_Sel == nullptr)
		return;

	if (m_pNav_Sel->wState & 0x0004) {
		MSG_BOX("Turned Off");
		m_pNav_Sel->wState &= ~0x0004;
	}
	else {
		MSG_BOX("Turned On");
		m_pNav_Sel->wState |= 0x0004;
	}
}

void CStage_NavTest::State_Fall_Toggle()
{
	if (m_pNav_Sel == nullptr)
		return;

	if (m_pNav_Sel->wState & 0x8000) {
		m_pNav_Sel->wState &= ~0x8000;
		m_pNav_Sel->wState &= ~0x0008;
		m_pNav_Sel->wState &= ~0x0004;
		m_pNav_Sel->wState &= ~0x0002;
		m_pNav_Sel->wState &= ~0x0001;
	}
	else
		m_pNav_Sel->wState |= 0x8000;

	My_TChar	szResult[256] = L"";
}


#endif



//Menu
#if true

void CStage_NavTest::Save()
{
	MSG_BOX("Click OK To Continue Save Process");

	HANDLE		hFile = CreateFile(L"../Bin/Data/Map/NavMesh.dat", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, NULL);

	if (hFile == NULL)
		MSG_BOX("SaveFailed");

	My_uLong		dwByte = 0;

	for (auto& pNav : m_List_Nav) {
		WriteFile(hFile, &pNav->vLT, sizeof(My_Vec3), &dwByte, nullptr);
		WriteFile(hFile, &pNav->vRT, sizeof(My_Vec3), &dwByte, nullptr);
		WriteFile(hFile, &pNav->vLB, sizeof(My_Vec3), &dwByte, nullptr);
		WriteFile(hFile, &pNav->vRB, sizeof(My_Vec3), &dwByte, nullptr);
		WriteFile(hFile, &pNav->wState, sizeof(My_uShort), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MSG_BOX("Save process completed");
}

void CStage_NavTest::Load()
{
	MSG_BOX("Click OK To Continue Load Process");

	if (m_List_Nav.size() != 0) {
		for (auto& pNav : m_List_Nav) {
			if (pNav != nullptr) {
				delete pNav;
				pNav = nullptr;
			}
		}
		m_List_Nav.clear();
		m_List_Nav.swap(list<NAVMESH*>());
	}

	HANDLE		hFile = CreateFile(L"../Bin/Data/Map/NavMesh.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, NULL);

	if (hFile == NULL)
		MSG_BOX("LoadFailed");

	My_uLong		dwByte = 0;

	My_Vec3		vLT, vRT, vLB, vRB;
	My_uShort	wState;
	NAVMESH*	pNav = nullptr;

	while (true) {
		ReadFile(hFile, &vLT, sizeof(My_Vec3), &dwByte, nullptr);
		ReadFile(hFile, &vRT, sizeof(My_Vec3), &dwByte, nullptr);
		ReadFile(hFile, &vLB, sizeof(My_Vec3), &dwByte, nullptr);
		ReadFile(hFile, &vRB, sizeof(My_Vec3), &dwByte, nullptr);
		ReadFile(hFile, &wState, sizeof(My_uShort), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		pNav = new NAVMESH;
		pNav->vLT = vLT;
		pNav->vRT = vRT;
		pNav->vLB = vLB;
		pNav->vRB = vRB;
		pNav->pU = nullptr;
		pNav->pB = nullptr;
		pNav->pL = nullptr;
		pNav->pR = nullptr;
		//pNav->wState = 0;
		pNav->wState = wState;

		m_List_Nav.push_back(pNav);
	}

	CloseHandle(hFile);

	MSG_BOX("Load process completed");
}

void CStage_NavTest::ConnectAll()
{
	MSG_BOX("Connecting process started");

	for (auto& pNav0 : m_List_Nav) {
		if (pNav0 == NULL)
			continue;

		for (auto& pNav1 : m_List_Nav) {
			if (pNav0 == pNav1)
				continue;

			if (pNav1 == NULL)
				continue;

			if (pNav0->vLT == pNav1->vLT &&
				pNav0->vRT == pNav1->vRT &&
				pNav0->vLB == pNav1->vLB &&
				pNav0->vRB == pNav1->vRB) {
				delete pNav1;
				pNav1 = nullptr;
			}
		}
	}

	auto iterBegin = m_List_Nav.begin();
	auto iterEnd = m_List_Nav.end();
	for (; iterBegin != iterEnd;) {
		if (*iterBegin == nullptr)
			iterBegin = m_List_Nav.erase(iterBegin);
		else
			++iterBegin;
	}

	for (auto& pNav0 : m_List_Nav) {
		if (pNav0->pU != nullptr && pNav0->pB != nullptr && pNav0->pL != nullptr && pNav0->pR)
			continue;

		for (auto& pNav1 : m_List_Nav) {
			if (pNav0->pU == nullptr) {
				if (pNav1->vLB == pNav0->vLT && pNav1->vRB == pNav0->vRT) {
					pNav0->pU = pNav1;
					pNav1->pB = pNav0;
					continue;
				}
			}

			if (pNav0->pB == nullptr) {
				if (pNav0->vLB == pNav1->vLT && pNav0->vRB == pNav1->vRT) {
					pNav0->pB = pNav1;
					pNav1->pU = pNav0;
					continue;
				}
			}

			if (pNav0->pL == nullptr) {
				if (pNav0->vLT == pNav1->vRT && pNav0->vLB == pNav1->vRB) {
					pNav0->pL = pNav1;
					pNav1->pR = pNav0;
					continue;
				}
			}

			if (pNav0->pR == nullptr) {
				if (pNav0->vRT == pNav1->vLT && pNav0->vRB == pNav1->vLB) {
					pNav0->pR = pNav1;
					pNav1->pL = pNav0;
					continue;
				}
			}
		}
	}

	MSG_BOX("Connecting process completed");
}

#endif

void CStage_NavTest::Mode_Next()
{
	My_Int	ienum = m_eMode;
	ienum = (++ienum == MODE_END) ? 0 : ienum;
	m_eMode = NAV_MODE(ienum);
}

void CStage_NavTest::Mode_Prev()
{
	My_Int	ienum = m_eMode;
	ienum = (ienum == 0) ? MODE_END - 1 : --ienum;
	m_eMode = NAV_MODE(ienum);
}

CStage_NavTest * CStage_NavTest::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage_NavTest*		pInstance = new CStage_NavTest(pDevice);
	if (FAILED(pInstance->Initialize_Prototype())) {
		delete pInstance;
		pInstance = nullptr;
	}
	return pInstance;
}

CGameObject * CStage_NavTest::Clone()
{
	CStage_NavTest*		pInstance = new CStage_NavTest(*this);
	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}
	return pInstance;
}

My_uLong CStage_NavTest::ReleaseMemory()
{
	for (auto& pNav : m_List_Nav) {
		if (pNav != nullptr) {
			delete pNav;
			pNav = nullptr;
		}
	}
	m_List_Nav.clear();

	//Safe_Release(m_pSprite);
	//Safe_Release(m_pFont);
	if (m_pSprite != nullptr)
		m_pSprite->Release();
	if (m_pFont != nullptr)
		m_pFont->Release();

	Safe_Release(m_pRectCol);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDebugger);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
