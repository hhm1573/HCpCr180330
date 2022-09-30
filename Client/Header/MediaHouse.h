#pragma once

#include "Model.h"

class CMediaHouse final
	:public CModel
{
private:
	explicit CMediaHouse(LPDIRECT3DDEVICE9 pDevice);
	explicit CMediaHouse(const CMediaHouse& Ref);
	virtual ~CMediaHouse() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CMediaHouse*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

