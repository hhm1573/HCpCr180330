#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriCol final
	:public CVIBuffer
{
private:
	explicit CTriCol(LPDIRECT3DDEVICE9 pDevice);
	explicit CTriCol(const CTriCol& Ref);
	virtual ~CTriCol() = default;

public:
	HRESULT		Initialize(void);
	void		Render(void);

public:
	static CTriCol*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent*		Clone();

protected:
	virtual My_uLong		ReleaseMemory();


};

END