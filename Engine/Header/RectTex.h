#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRectTex final
	:public CVIBuffer
{
private:
	explicit CRectTex(LPDIRECT3DDEVICE9 pDevice);
	explicit CRectTex(const CRectTex& Ref);
	virtual ~CRectTex() = default;

public:
	HRESULT		Initialize(void);
	void		Render(void);

public:
	static CRectTex*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent*		Clone();
protected:
	virtual My_uLong		ReleaseMemory(void);
};

END