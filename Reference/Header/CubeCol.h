#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeCol
	:public CVIBuffer
{
private:
	explicit CCubeCol(LPDIRECT3DDEVICE9 pDevice);
	explicit CCubeCol(const CCubeCol& Ref);
	virtual ~CCubeCol() = default;

public:
	HRESULT		Initialize(void);
	void		Render(void);
private:
	void		SetIndex(POLYGON16* pIndex, My_uInt wIndex, My_uInt wV0, My_uInt wV1, My_uInt wV2);
	
public:
	void		SetColor(D3DCOLOR color);

public:
	static CCubeCol*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent*		Clone();
protected:
	virtual My_uLong		ReleaseMemory();
};

END