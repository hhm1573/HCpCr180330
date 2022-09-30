#pragma once

#include "Model.h"

class CGreenBlock final
	:public CModel
{
private:
	explicit CGreenBlock(LPDIRECT3DDEVICE9 pDevice);
	explicit CGreenBlock(const CGreenBlock& Ref);
	virtual ~CGreenBlock() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CGreenBlock*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

