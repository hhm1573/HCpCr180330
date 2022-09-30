#pragma once

#include "Scene.h"

class CLogo
	:public CScene
{
public:
	explicit CLogo(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLogo() = default;

public:
	virtual HRESULT		Initialize(void);
	virtual My_Int		Update(const My_Float& fTimeDelta);
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta);
	virtual void		Render();
private:
	HRESULT		Initialize_Component_Prototype();
	HRESULT		Initialize_GameObject_Prototype();
	HRESULT		Initialize_Layer_BackGround(const My_TChar* szKey);
	HRESULT		Initialize_Lighting();

public:
	static CLogo*		Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	virtual My_uLong	ReleaseMemory();
};

