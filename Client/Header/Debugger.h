#pragma once

#include "Base.h"

class CDebugger final
	:public CBase
{
	DECLARE_SINGLETON(CDebugger)
private:
	explicit CDebugger();
	virtual ~CDebugger() = default;

private:
	My_uLong			m_dwConfig = 0;
	LPDIRECT3DDEVICE9	m_pDevice = nullptr;
	LPD3DXSPRITE		m_pSprite = nullptr;
	LPD3DXFONT			m_pFont = nullptr;

public:
	void		Update(void);
	void		Render(void);

public:
	My_Bool		Get_IsFreeCamera();
	My_Bool		Get_IsNavMode();
	My_Bool		Get_IsWireFrame();
	My_Bool		Get_IsRender_Terrain();
	My_Bool		Get_IsRender_Object();

public:
	void		DebugMenu_Show();
	void		DebugMenu_Hide();
public:
	void		Set_Camera_Free();
	void		Set_Camera_Target();

	void		Set_NavMode_On();
	void		Set_NavMode_Off();

	void		Set_WireFrame_On();
	void		Set_WireFrame_Off();

	void		Toggle_Render_Terrain();
	void		Toggle_Render_Object();


protected:
	virtual		My_uLong		ReleaseMemory()override;
};

