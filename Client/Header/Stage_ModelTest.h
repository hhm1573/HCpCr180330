#pragma once

#include "Model.h"

class CStage_ModelTest final
	:public Engine::CModel
{
private:
	explicit CStage_ModelTest(LPDIRECT3DDEVICE9 pDevice);
	explicit CStage_ModelTest(const CStage_ModelTest& Ref);
	virtual ~CStage_ModelTest(void) = default;

public:
	HRESULT				Initialize(void);
	HRESULT				Initialize_Prototype(void);
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render(void) override;

public:
	static CStage_ModelTest*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual Engine::CGameObject*	Clone(void) override;
protected:
	virtual My_uLong				ReleaseMemory(void) override;
};

