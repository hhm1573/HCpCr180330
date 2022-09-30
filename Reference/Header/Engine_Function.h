#pragma once
#ifndef __ENGINE_FUNCTION_H__
#define __ENGINE_FUNCTION_H__

namespace Engine {
	template<typename Type>
	unsigned long Safe_Release(Type& pInstance) {

		unsigned long dwRefCnt = 0;

		if (pInstance != nullptr) {	
			dwRefCnt = pInstance->Release();
			if (dwRefCnt == 0)
				pInstance = nullptr;
		}

		return dwRefCnt;
	}

	template <typename T>
	void Safe_Delete(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete pPointer;
			pPointer = nullptr;
		}
	}

	template <typename T>
	void Safe_Delete_Array(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete[] pPointer;
			pPointer = nullptr;
		}
	}
}

#endif // !__ENGINE_FUNCTION_H__
