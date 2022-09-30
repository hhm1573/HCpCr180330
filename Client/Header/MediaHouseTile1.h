#pragma once

#include "Model.h"

class CMediaHouseTile1 final
	:public CModel
{
private:
	explicit CMediaHouseTile1(LPDIRECT3DDEVICE9 pDevice);
	explicit CMediaHouseTile1(const CMediaHouseTile1& Ref);
	virtual ~CMediaHouseTile1() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CMediaHouseTile1*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

