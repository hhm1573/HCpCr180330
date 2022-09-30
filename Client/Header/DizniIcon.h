#pragma once

#include "Include.h"
#include "MiniMapIcon.h"

class CMiniMap;

class CDizniIcon
	: public CMiniMapIcon
{
private:
	explicit CDizniIcon(LPDIRECT3DDEVICE9 pDevice);
	explicit CDizniIcon(const CDizniIcon& rhs);
	virtual ~CDizniIcon() = default;

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
	static		CDizniIcon*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*		Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

