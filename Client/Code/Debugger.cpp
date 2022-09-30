#include "stdafx.h"
#include "..\Header\Debugger.h"
#include "InputDev.h"
#include "GraphicDev.h"

IMPLEMENT_SINGLETON(CDebugger)



CDebugger::CDebugger()
{
	m_pDevice = CGraphicDev::GetInstance()->Get_Graphic_Device();
	if (m_pDevice != nullptr)
		m_pDevice->AddRef();

	m_pSprite = CGraphicDev::GetInstance()->Get_Sprite();
	if (m_pSprite != nullptr)
		m_pSprite->AddRef();

	m_pFont = CGraphicDev::GetInstance()->Get_Font();
	if (m_pFont != nullptr)
		m_pFont->AddRef();

	m_dwConfig |= 0x20000000;
}

void CDebugger::Update(void)
{
	CInputDev*	pInput = CInputDev::GetInstance();

	/*if (pInput->Get_DIKState(DIK_NUMPADPLUS) & 0x80) {
	Set_Camera_Free();
	}

	if (pInput->Get_DIKState(DIK_NUMPADMINUS) & 0x80) {
	Set_Camera_Target();
	}*/

	if (pInput->Get_DIK_IsPressed(DIK_F12) == true) {
		if (m_dwConfig & 0x80000000)
			DebugMenu_Hide();
		else
			DebugMenu_Show();
	}

	/*if (!(m_dwConfig & 0x8000000))
	return;
	*/


	if (pInput->Get_DIK_IsPressed(DIK_F1) == true) {
		if (Get_IsNavMode() == true)
			Set_NavMode_Off();
		else
			Set_NavMode_On();
	}

	if (pInput->Get_DIK_IsPressed(DIK_F2) == true) {
		if (Get_IsFreeCamera() == true)
			Set_Camera_Target();
		else
			Set_Camera_Free();
	}

	if (pInput->Get_DIK_IsPressed(DIK_F3) == true) {
		if (Get_IsWireFrame() == true)
			Set_WireFrame_Off();
		else
			Set_WireFrame_On();
	}

	if (pInput->Get_DIK_IsPressed(DIK_F10) == true) {
		Toggle_Render_Terrain();
	}

	if (pInput->Get_DIK_IsPressed(DIK_F11) == true) {
		Toggle_Render_Object();
	}


}

void CDebugger::Render(void)
{
	if (m_pSprite == nullptr) {
		m_pSprite = CGraphicDev::GetInstance()->Get_Sprite();
		if (m_pSprite != nullptr)
			m_pSprite->AddRef();
	}
	if (m_pFont == nullptr) {
		m_pFont = CGraphicDev::GetInstance()->Get_Font();
		if (m_pFont != nullptr)
			m_pFont->AddRef();
	}

	if ((m_dwConfig & 0x80000000) == false)
		return;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	My_Long		lFontHeight = 12;

	RECT rt = { 0,0,0,0 };
	m_pFont->DrawTextW(m_pSprite, L"Debug Menu (F12)", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 1;
	if (Get_IsNavMode() == true)
		m_pFont->DrawTextW(m_pSprite, L"NavMesh Mode (F1) On", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		m_pFont->DrawTextW(m_pSprite, L"NavMesh Mode (F1) Off", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 2;
	if (Get_IsFreeCamera() == true)
		m_pFont->DrawTextW(m_pSprite, L"FreeCamera (F2) On", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		m_pFont->DrawTextW(m_pSprite, L"FreeCamera (F2) Off", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 3;
	if (Get_IsWireFrame() == true)
		m_pFont->DrawTextW(m_pSprite, L"WireFrame (F3) On", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		m_pFont->DrawTextW(m_pSprite, L"WireFrame (F3) Off", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 4;
	if (Get_IsRender_Terrain() == true)
		m_pFont->DrawTextW(m_pSprite, L"Render_Terrain (F10) On", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		m_pFont->DrawTextW(m_pSprite, L"Render_Terrain (F10) Off", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	rt.top = (lFontHeight / 3 + lFontHeight) * 5;
	if (Get_IsRender_Object() == true)
		m_pFont->DrawTextW(m_pSprite, L"Render_Object (F11) On", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		m_pFont->DrawTextW(m_pSprite, L"Render_Object (F11) Off", -1, &rt, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pSprite->End();
}

My_Bool CDebugger::Get_IsFreeCamera()
{
	if (m_dwConfig & 0x00000001)
		return true;
	return false;
}

My_Bool CDebugger::Get_IsNavMode()
{
	return (m_dwConfig & 0x00000002) ? true : false;
}

My_Bool CDebugger::Get_IsWireFrame()
{
	return (m_dwConfig & 0x00000004) ? true : false;
}

My_Bool CDebugger::Get_IsRender_Terrain()
{
	return (m_dwConfig & 0x20000000) ? true : false;
}

My_Bool CDebugger::Get_IsRender_Object()
{
	return (m_dwConfig & 0x40000000) ? true : false;
}

void CDebugger::DebugMenu_Show()
{
	m_dwConfig |= 0x80000000;
}

void CDebugger::DebugMenu_Hide()
{
	m_dwConfig &= ~0x80000000;
}

void CDebugger::Set_Camera_Free()
{
	m_dwConfig |= 0x00000001;
}

void CDebugger::Set_Camera_Target()
{
	m_dwConfig &= ~0x00000001;
}

void CDebugger::Set_NavMode_On()
{
	m_dwConfig |= 0x00000002;
}

void CDebugger::Set_NavMode_Off()
{
	m_dwConfig &= ~0x00000002;
}

void CDebugger::Set_WireFrame_On()
{
	m_dwConfig |= 0x00000004;

	if (m_pDevice == nullptr) {
		m_pDevice = CGraphicDev::GetInstance()->Get_Graphic_Device();
		if (m_pDevice == nullptr)
			return;
		m_pDevice->AddRef();
	}

	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

void CDebugger::Set_WireFrame_Off()
{
	m_dwConfig &= ~0x00000004;

	if (m_pDevice == nullptr) {
		m_pDevice = CGraphicDev::GetInstance()->Get_Graphic_Device();
		if (m_pDevice == nullptr)
			return;
		m_pDevice->AddRef();
	}

	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CDebugger::Toggle_Render_Terrain()
{
	if (Get_IsRender_Terrain() == true)
		m_dwConfig &= ~0x20000000;
	else
		m_dwConfig |= 0x20000000;
}

void CDebugger::Toggle_Render_Object()
{
	if (Get_IsRender_Object() == true)
		m_dwConfig &= ~0x40000000;
	else
		m_dwConfig |= 0x40000000;
}

My_uLong CDebugger::ReleaseMemory()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pSprite);
	Safe_Release(m_pFont);
	return My_uLong();
}
