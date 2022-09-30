#pragma once

#include "GameObject.h"

namespace Engine {
	class CRenderer;
	class CRectCol;
	class CInputDev;
}

class CDebugger;

class CStage_NavTest final
	:public CGameObject
{
private:
	enum NAV_MODE { MODE_SEL, MODE_ADD, MODE_MOVE, MODE_TILT, MODE_STATE, MODE_MENU, MODE_END };
private:
	explicit CStage_NavTest(LPDIRECT3DDEVICE9 pDevice);
	explicit CStage_NavTest(const CStage_NavTest& Ref);
	virtual ~CStage_NavTest() = default;

private:
	LPD3DXSPRITE				m_pSprite = nullptr;
	LPD3DXFONT					m_pFont = nullptr;
private:
	NAV_MODE					m_eMode = MODE_SEL;
	My_Bool						m_bDeleteMode = false;
	My_Bool						m_bDownMode = false;
	list<NAVMESH*>				m_List_Nav;
	NAVMESH*					m_pNav_Sel = nullptr;
	My_Float					m_fTime = 0.0f;
private:
	CRenderer*					m_pRenderer = nullptr;
	CRectCol*					m_pRectCol = nullptr;
	CInputDev*					m_pInputDev = nullptr;
	CDebugger*					m_pDebugger = nullptr;

private:
	/*void(CStage_NavTest::**m_pFuncRender)(void);
	void		CallRender(NAV_MODE eMode) { (this->*m_pFuncRender[eMode])(); }*/

public:
	HRESULT						Initialize();
	HRESULT						Initialize_Prototype();
	virtual My_Int				Update(const My_Float& fTimeDelta) override;
	virtual My_Int				LastUpdate(const My_Float& fTimeDelta) override;
	virtual void				Render() override;

#if true

public:
	void				Render_Sel();
	void				Render_Add();
	void				Render_Move();
	void				Render_Tilt();
	void				Render_State();
	void				Render_Menu();
public:
	void				Command_Sel(My_Int iCommand);
	void				Command_Add(My_Int iCommand);
	void				Command_Move(My_Int iCommand);
	void				Command_Tilt(My_Int iCommand);
	void				Command_State(My_Int iCommand);
	void				Command_Menu(My_Int iCommand);

public:
	void				Sel_Next();
	void				Sel_Prev();
	void				Sel_Front();
	void				Sel_Back();
	void				Sel_Left();
	void				Sel_Right();
public:
	void				Move_Left();
	void				Move_Right();
	void				Move_Forward();
	void				Move_Backward();
	void				Move_Up();
	void				Move_Down();

	void				Add_New();
	void				Add_Left();
	void				Add_Right();
	void				Add_Front();
	void				Add_Back();
public:
	void				Delete_Cur();
	void				Delete_Left();
	void				Delete_Right();
	void				Delete_Front();
	void				Delete_Back();
public:
	void				Tilt_Left(My_Bool bUp);
	void				Tilt_Right(My_Bool bUp);
	void				Tilt_Front(My_Bool bUp);
	void				Tilt_Back(My_Bool bUp);
public:
	void				State_Fall_Left();
	void				State_Fall_Right();
	void				State_Fall_Forward();
	void				State_Fall_Backward();
	void				State_Fall_Toggle();
public:
	void				Save();
	void				Load();
	void				ConnectAll();

#endif

private:
	void				Delete_Nav(NAVMESH* pNav);
public:
	void				Mode_Next();
	void				Mode_Prev();

public:
	static CStage_NavTest*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone() override;
protected:
	virtual My_uLong			ReleaseMemory() override;
};

