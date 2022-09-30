#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract
	:public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pDevice);
	explicit CComponent(const CComponent& Ref);
	virtual ~CComponent() = default;

protected:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;

public:
	virtual CComponent*		Clone(void) PURE;
	virtual My_uLong		ReleaseMemory(void);
};

END