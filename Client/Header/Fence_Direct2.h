#pragma once

#include "Model.h"

class CFence_Direct2 final
	:public CModel
{
private:
	explicit CFence_Direct2(LPDIRECT3DDEVICE9 pDevice);
	explicit CFence_Direct2(const CFence_Direct2& Ref);
	virtual ~CFence_Direct2() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CFence_Direct2*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

