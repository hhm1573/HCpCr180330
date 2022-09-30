#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRectCol final
	:public CVIBuffer
{
private:
	explicit CRectCol(LPDIRECT3DDEVICE9 pDevice);
	explicit CRectCol(const CRectCol& Ref);
	virtual ~CRectCol() = default;

public:
	HRESULT		Initialize(void);
	void		Render(void);

public:
	static CRectCol*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent*		Clone();
public:
	void					Set_Verticies(const My_Vec3& vLT, const My_Vec3& vRT, const My_Vec3& vLB, const My_Vec3& vRB);
	void					Reset_Verticies();
	void					SetColor(D3DCOLOR color);

protected:
	virtual My_uLong		ReleaseMemory(void);
};

END