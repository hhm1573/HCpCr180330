#pragma once

#include "Component.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_BOOSTER, RENDER_CLOUD, RENDER_UI, RENDER_END };
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;

private:
	list<CGameObject*>			m_RenderList[RENDER_END];
	typedef list<CGameObject*>	RENDERLIST;

public:
	HRESULT Ready_Renderer(void);
	HRESULT Add_RenderGroup(RENDERGROUP eGroup, CGameObject* pGameObject);
	void Render_Objects();

private:
	void Render_Priority(void);
	void Render_NonAlpha(void);
	void Render_Alpha(void);
	void Render_Booster(void);
	void Render_Cloud(void);
	void Render_UI(void);

public:
	static CRenderer*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*		Clone(void);
protected:
	virtual My_uLong		ReleaseMemory(void);
};

END