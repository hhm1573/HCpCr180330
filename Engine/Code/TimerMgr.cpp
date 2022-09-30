#include "..\Header\TimerMgr.h"
#include "Timer.h"


USING(Engine)
IMPLEMENT_SINGLETON(CTimerMgr)

CTimerMgr::CTimerMgr()
{
}

HRESULT CTimerMgr::Add_Timer(const My_TChar * pTimerTag)
{
	CTimer*		pTimer = Find_Timer(pTimerTag);

	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();
	if (nullptr == pTimer)
		return E_FAIL;

	m_mapTimer.insert(MAPTIMER::value_type(pTimerTag, pTimer));

	return NOERROR;
}

My_Float CTimerMgr::Compute_Time(const My_TChar * pTimerTag)
{
	CTimer*		pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Compute_Time();
}

CTimer * CTimerMgr::Find_Timer(const My_TChar * pTimerTag)
{
	auto iter = find_if(m_mapTimer.begin(), m_mapTimer.end(), CFinder_Tag(pTimerTag));

	if (iter == m_mapTimer.end())
		return nullptr;

	return iter->second;

	return nullptr;
}

My_uLong CTimerMgr::ReleaseMemory(void)
{
	for (auto& Pair : m_mapTimer)
	{
		Safe_Release(Pair.second);
	}
	m_mapTimer.clear();

	return 0;
}