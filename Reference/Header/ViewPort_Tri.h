#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CViewPort_Tri :	public CVIBuffer
{
private:
	explicit CViewPort_Tri(LPDIRECT3DDEVICE9 pDevice);
	explicit CViewPort_Tri(const CViewPort_Tri& Ref);
	virtual ~CViewPort_Tri() = default;

public:
	HRESULT				Initialize();
	void					Render();

public:
	static CViewPort_Tri*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent*			Clone();

protected:
	virtual My_uLong			ReleaseMemory();
};

END