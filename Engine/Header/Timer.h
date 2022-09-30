#pragma once
#ifndef __ENGINE_TIMER_H__
#define __ENGINE_TIMER_H__

#include "Base.h"

namespace Engine {
	class CTimer final
		:public Engine::CBase
	{
	private:
		explicit CTimer(void);
		virtual ~CTimer(void) = default;

	private:
		LARGE_INTEGER		m_CPUTick;
		LARGE_INTEGER		m_FixTime;
		LARGE_INTEGER		m_PreTime;
		LARGE_INTEGER		m_CurTime;
	private:
		Engine::My_Float	m_fTime = 0.0f;

	public:
		HRESULT				Initialize(void);
		Engine::My_Float	Compute_Time(void);

	public:
		static Engine::CTimer*		Create(void);
	protected:
		virtual Engine::My_uLong	ReleaseMemory(void) override;
	};
}

#endif // !__ENGINE_TIMER_H__