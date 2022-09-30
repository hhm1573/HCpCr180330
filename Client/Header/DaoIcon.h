#pragma once

#include "Include.h"
#include "MiniMapIcon.h"

class CMiniMap;

class CDaoIcon
	: public CMiniMapIcon
{
private:
	explicit CDaoIcon(LPDIRECT3DDEVICE9 pDevice);
	explicit CDaoIcon(const CDaoIcon& rhs);
	virtual ~CDaoIcon() = default;

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
	static		CDaoIcon*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*		Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

