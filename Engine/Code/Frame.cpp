#include "..\Header\Frame.h"

CFrame::CFrame()
{
}

HRESULT CFrame::Ready_Frame(const My_Float & fCallCnt)
{
	m_fCallPerSec = 1.f / fCallCnt;

	return NOERROR;
}

My_Bool CFrame::Permit_Call(const My_Float & fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fCallPerSec <= m_fTimeAcc)
	{
		m_fTimeAcc = 0.f;
		return true;
	}

	return false;
}

CFrame * CFrame::Create(const My_Float & fCallCnt)
{
	CFrame*	pInstance = new CFrame();

	if (FAILED(pInstance->Ready_Frame(fCallCnt)))
	{
		MSG_BOX("CFrame Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

My_uLong CFrame::ReleaseMemory(void)
{
	return My_uLong();
}
