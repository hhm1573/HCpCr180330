#pragma once

#include "Tetris.h"

class CTetris_Yellow1 final
	:public CTetris
{
private:
	explicit CTetris_Yellow1(LPDIRECT3DDEVICE9 pDevice);
	explicit CTetris_Yellow1(const CTetris_Yellow1& Ref);
	virtual ~CTetris_Yellow1() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CTetris_Yellow1*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

