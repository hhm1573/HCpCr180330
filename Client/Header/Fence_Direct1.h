#pragma once

#include "Model.h"

class CFence_Direct1 final
	:public CModel
{
private:
	explicit CFence_Direct1(LPDIRECT3DDEVICE9 pDevice);
	explicit CFence_Direct1(const CFence_Direct1& Ref);
	virtual ~CFence_Direct1() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CFence_Direct1*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

