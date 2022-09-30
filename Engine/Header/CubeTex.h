#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex final
	:public CVIBuffer
{
public:
	explicit CCubeTex(LPDIRECT3DDEVICE9 pDevice);
	explicit CCubeTex(const CCubeTex& Ref);
	virtual ~CCubeTex() = default;

public:
	HRESULT		Initialize();
	void		Render();

public:
	static CCubeTex*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent*		Clone();
protected:
	virtual My_uLong		ReleaseMemory();
};

END