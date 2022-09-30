#pragma once
#ifndef __ENGINE_BASE_H__
#define __ENGINE_BASE_H__

#include "Engine_Include.h"

namespace Engine {
	class ENGINE_DLL CBase abstract
	{
	protected:
		explicit CBase(void);
		virtual ~CBase(void) = default;

	private:
		My_uLong		m_dwRefCnt = 0l;

	public:
		My_uLong				AddRef(void);

	public:
		My_uLong				Release(void);
	protected:
		virtual My_uLong		ReleaseMemory(void) PURE;
	};
}

/*
AddRef => 값을 올린 후 반환한다.
Release => 반환한 후 값을 반환한다.
RefCnt 0 => 아무 참조도 없는상태 => 릴리즈하면 지워질 상태

*/

#endif // !__ENGINE_BASE_H__