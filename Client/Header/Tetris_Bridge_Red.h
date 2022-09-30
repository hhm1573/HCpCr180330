#pragma once

#include "Model.h"

class CTetris_Bridge_Red final
	:public CModel
{
private:
	explicit CTetris_Bridge_Red(LPDIRECT3DDEVICE9 pDevice);
	explicit CTetris_Bridge_Red(const CTetris_Bridge_Red& Ref);
	virtual ~CTetris_Bridge_Red() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CTetris_Bridge_Red*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

