#pragma once

#include "Model.h"

class CTree_Red final
	:public CModel
{
private:
	explicit CTree_Red(LPDIRECT3DDEVICE9 pDevice);
	explicit CTree_Red(const CTree_Red& Ref);
	virtual ~CTree_Red() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CTree_Red*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

