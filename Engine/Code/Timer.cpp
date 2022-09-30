#include "..\Header\Timer.h"



Engine::CTimer::CTimer()
{
}

HRESULT Engine::CTimer::Initialize(void)
{
	QueryPerformanceFrequency(&m_CPUTick);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_PreTime);
	QueryPerformanceCounter(&m_CurTime);

	return S_OK;
}

Engine::My_Float Engine::CTimer::Compute_Time(void)
{
	QueryPerformanceCounter(&m_CurTime);

	m_fTime = float(m_CurTime.QuadPart - m_PreTime.QuadPart) / m_CPUTick.QuadPart;
	
	if ((m_CurTime.QuadPart - m_FixTime.QuadPart) / m_CPUTick.QuadPart > 1.0f) {
		QueryPerformanceFrequency(&m_CPUTick);
		m_FixTime = m_CurTime;
	}

	m_PreTime = m_CurTime;

	return m_fTime;
}

Engine::CTimer * Engine::CTimer::Create(void)
{
	Engine::CTimer*		pInstance = nullptr;

	pInstance = new Engine::CTimer();

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

Engine::My_uLong Engine::CTimer::ReleaseMemory(void)
{
	return Engine::My_uLong();
}