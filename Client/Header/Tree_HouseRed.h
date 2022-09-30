#pragma once

#include "Model.h"

class CTree_HouseRed final
	:public CModel
{
private:
	explicit CTree_HouseRed(LPDIRECT3DDEVICE9 pDevice);
	explicit CTree_HouseRed(const CTree_HouseRed& Ref);
	virtual ~CTree_HouseRed() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CTree_HouseRed*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

