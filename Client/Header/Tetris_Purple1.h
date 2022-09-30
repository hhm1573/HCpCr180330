#pragma once

#include "Tetris.h"

class CTetris_Purple1 final
	:public CTetris
{
private:
	explicit CTetris_Purple1(LPDIRECT3DDEVICE9 pDevice);
	explicit CTetris_Purple1(const CTetris_Purple1& Ref);
	virtual ~CTetris_Purple1() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CTetris_Purple1*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

