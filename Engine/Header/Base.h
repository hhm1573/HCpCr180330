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
AddRef => ���� �ø� �� ��ȯ�Ѵ�.
Release => ��ȯ�� �� ���� ��ȯ�Ѵ�.
RefCnt 0 => �ƹ� ������ ���»��� => �������ϸ� ������ ����

*/

#endif // !__ENGINE_BASE_H__