#pragma once

#include "Base.h"
#include "GraphicDev.h"

class CDebugger;

namespace Engine {
	class CSceneMgr;
	class CComponentMgr;
	class CRenderer;
	class CGameObjectMgr;
}

class CMainApp final
	:public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
private:
	CGraphicDev*			m_pGraphic_Interface = nullptr;
	CSceneMgr*				m_pSceneMgr = nullptr;
	CComponentMgr*			m_pComponentMgr = nullptr;
	CGameObjectMgr*			m_pGameObjectMgr = nullptr;
private:
	CRenderer*				m_pRenderer = nullptr;
private:
	CDebugger*				m_pDebugger = nullptr;

public:
	HRESULT		Initialize();
	My_Int		Update(const My_Float& fTimeDelta);
	void		Render();
private:
	HRESULT		Initialize_DefaultSetting(CGraphicDev::WIN_MODE eMode, const My_uShort& wBackCX, const My_uShort& wBackCY);
	HRESULT		Initialize_DefaultRenderState();
	HRESULT		Initialize_StartScene(SCENEID eID);
private:
	HRESULT		Initialize_Component_Prototype();
	HRESULT		Initialize_GameObject_Prototype();
	HRESULT		Initialize_GameObject();

private:
	void		Render_GameObject(SCENEID eID, const My_TChar* RendererTag);

public:
	static CMainApp*		Create();
protected:
	virtual My_uLong		ReleaseMemory();
};

