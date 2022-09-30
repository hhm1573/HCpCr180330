#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CViewPort_Buffer final
	:public CVIBuffer
{
private:
	explicit CViewPort_Buffer(LPDIRECT3DDEVICE9 pDevice);
	explicit CViewPort_Buffer(const CViewPort_Buffer& Ref);
	virtual ~CViewPort_Buffer() = default;

public:
	HRESULT				Initialize();
	void				Render();

public:
	static CViewPort_Buffer*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent*			Clone();
protected:
	virtual My_uLong			ReleaseMemory();
};

END