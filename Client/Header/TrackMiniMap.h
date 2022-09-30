#pragma once

#include "Include.h"
#include "MiniMapIcon.h"

namespace Engine
{
	class CRenderer;
	class CRectTex;
	class CTransform;
	class CTexture;
}

class CMiniMap;

class CTrackMiniMap
	: public CMiniMapIcon
{
private:
	explicit CTrackMiniMap(LPDIRECT3DDEVICE9 pDevice);
	explicit CTrackMiniMap(const CTrackMiniMap& rhs);
	virtual ~CTrackMiniMap() = default;

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Prototype();

public:
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void		Render();

private:
	void			Set_RenderState();
	void			Release_RenderState();

public:
	static		CTrackMiniMap*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*			Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

