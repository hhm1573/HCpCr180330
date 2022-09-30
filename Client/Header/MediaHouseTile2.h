#pragma once

#include "Model.h"

class CMediaHouseTile2 final
	:public CModel
{
private:
	explicit CMediaHouseTile2(LPDIRECT3DDEVICE9 pDevice);
	explicit CMediaHouseTile2(const CMediaHouseTile2& Ref);
	virtual ~CMediaHouseTile2() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CMediaHouseTile2*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

