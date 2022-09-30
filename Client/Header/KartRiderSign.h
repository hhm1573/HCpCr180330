#pragma once

#include "Model.h"

class CKartRiderSign final
	:public CModel
{
private:
	explicit CKartRiderSign(LPDIRECT3DDEVICE9 pDevice);
	explicit CKartRiderSign(const CKartRiderSign& Ref);
	virtual ~CKartRiderSign() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CKartRiderSign*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

