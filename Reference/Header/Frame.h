#pragma once

#include "Base.h"

BEGIN(Engine)

class CFrame
	:public CBase
{
private:
	explicit CFrame();
	virtual ~CFrame() = default;

private:
	My_Float		m_fCallPerSec = 0.0f;
	My_Float		m_fTimeAcc = 0.0f;

public:
	HRESULT Ready_Frame(const My_Float& fCallCnt);
	My_Bool Permit_Call(const My_Float& fTimeDelta);

public:
	static CFrame* Create(const My_Float& fCallCnt);
protected:
	virtual My_uLong ReleaseMemory(void);
};

END