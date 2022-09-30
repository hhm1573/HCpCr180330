#pragma once

#include "Tetris.h"

class CTetris_Blue final
	:public CTetris
{
private:
	explicit CTetris_Blue(LPDIRECT3DDEVICE9 pDevice);
	explicit CTetris_Blue(const CTetris_Blue& Ref);
	virtual ~CTetris_Blue() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CTetris_Blue*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

