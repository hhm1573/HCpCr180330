#pragma once

#include "Model.h"

class CHouse final
	:public CModel
{
private:
	explicit CHouse(LPDIRECT3DDEVICE9 pDevice);
	explicit CHouse(const CHouse& Ref);
	virtual ~CHouse() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CHouse*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

