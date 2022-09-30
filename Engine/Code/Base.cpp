#include "..\Header\Base.h"



Engine::CBase::CBase()
{
	m_dwRefCnt = 0;
}

Engine::My_uLong Engine::CBase::AddRef(void)
{
	return ++m_dwRefCnt;
}

Engine::My_uLong Engine::CBase::Release(void)
{
	if (m_dwRefCnt == 0) {
		ReleaseMemory();
		delete this;
		return 0l;
	}
	else
		return m_dwRefCnt--;
}
