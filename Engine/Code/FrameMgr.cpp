#include "..\Header\FrameMgr.h"
#include "Frame.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrameMgr)

CFrameMgr::CFrameMgr()
{
}

HRESULT CFrameMgr::Add_Frame(const My_TChar * pFrameTag, const My_Float & fCallCnt)
{
	CFrame*		pFrame = Find_Frame(pFrameTag);

	if (nullptr != pFrame)
		return E_FAIL;

	pFrame = CFrame::Create(fCallCnt);
	if (nullptr == pFrame)
		return E_FAIL;

	m_mapFrame.insert(MAPFRAME::value_type(pFrameTag, pFrame));

	return NOERROR;
}

My_Bool CFrameMgr::Permit_Call(const My_TChar * pFrameTag, const My_Float & fTimeDelta)
{
	CFrame*		pFrame = Find_Frame(pFrameTag);

	if (nullptr == pFrame)
		return false;

	return pFrame->Permit_Call(fTimeDelta);
}

CFrame * CFrameMgr::Find_Frame(const My_TChar * pFrameTag)
{
	auto iter = find_if(m_mapFrame.begin(), m_mapFrame.end(), CFinder_Tag(pFrameTag));

	if (iter == m_mapFrame.end())
		return nullptr;

	return iter->second;
	//find_if(m_mapFrame.begin(), m_mapFrame.end(), [&](MAPFrame::value_type Pair) {return !lstrcmp(pFrameTag, Pair.first); });

	return nullptr;
}

My_uLong CFrameMgr::ReleaseMemory(void)
{
	for (auto& Pair : m_mapFrame)
	{
		Safe_Release(Pair.second);
	}
	m_mapFrame.clear();

	return 0;
}
