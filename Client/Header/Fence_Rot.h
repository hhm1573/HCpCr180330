#pragma once

#include "Model.h"

class CFence_Rot final
	:public CModel
{
private:
	explicit CFence_Rot(LPDIRECT3DDEVICE9 pDevice);
	explicit CFence_Rot(const CFence_Rot& Ref);
	virtual ~CFence_Rot() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CFence_Rot*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

