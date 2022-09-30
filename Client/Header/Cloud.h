#pragma once

#include "Model.h"

class CCloud final
	:public CModel
{
private:
	explicit CCloud(LPDIRECT3DDEVICE9 pDevice);
	explicit CCloud(const CCloud& Ref);
	virtual ~CCloud() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CCloud*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

