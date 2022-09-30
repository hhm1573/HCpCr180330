#pragma once

#include "Model.h"

class CTree_Green2 final
	:public CModel
{
private:
	explicit CTree_Green2(LPDIRECT3DDEVICE9 pDevice);
	explicit CTree_Green2(const CTree_Green2& Ref);
	virtual ~CTree_Green2() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CTree_Green2*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

