#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer;

class ENGINE_DLL CTimerMgr
	:public CBase
{
	DECLARE_SINGLETON(CTimerMgr)

private:
	explicit CTimerMgr();
	virtual ~CTimerMgr() = default;

private:
	map<const My_TChar*, CTimer*>		m_mapTimer;
	typedef map<const My_TChar*, CTimer*>	MAPTIMER;

public:
	HRESULT		Add_Timer(const My_TChar* pTimerTag);
	My_Float	Compute_Time(const My_TChar* pTimerTag);
private:
	CTimer*		Find_Timer(const My_TChar* pTimerTag);

protected:
	virtual My_uLong	ReleaseMemory(void);
};

END