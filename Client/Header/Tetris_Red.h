#pragma once

#include "Tetris.h"

class CTetris_Red final
	:public CTetris
{
private:
	explicit CTetris_Red(LPDIRECT3DDEVICE9 pDevice);
	explicit CTetris_Red(const CTetris_Red& Ref);
	virtual ~CTetris_Red() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CTetris_Red*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

