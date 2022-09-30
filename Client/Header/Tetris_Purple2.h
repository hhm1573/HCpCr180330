#pragma once

#include "Tetris.h"

class CTetris_Purple2 final
	:public CTetris
{
private:
	explicit CTetris_Purple2(LPDIRECT3DDEVICE9 pDevice);
	explicit CTetris_Purple2(const CTetris_Purple2& Ref);
	virtual ~CTetris_Purple2() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CTetris_Purple2*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

