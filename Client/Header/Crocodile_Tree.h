#pragma once

#include "Model.h"

class CCrocodile_Tree final
	:public CModel
{
private:
	explicit CCrocodile_Tree(LPDIRECT3DDEVICE9 pDevice);
	explicit CCrocodile_Tree(const CCrocodile_Tree& Ref);
	virtual ~CCrocodile_Tree() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	static CCrocodile_Tree*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*		Clone(void);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

