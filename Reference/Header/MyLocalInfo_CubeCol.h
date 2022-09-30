#pragma once

#include "MyLocalInfo.h"

namespace Engine {
	class CCubeCol;
}

BEGIN(Engine)

class CMyLocalInfo_CubeCol final
	:public CMyLocalInfo
{
private:
	explicit CMyLocalInfo_CubeCol(LPDIRECT3DDEVICE9 pDevice);
	explicit CMyLocalInfo_CubeCol(const CMyLocalInfo_CubeCol& Ref);
	virtual ~CMyLocalInfo_CubeCol(void) = default;

private:
	D3DCOLOR					m_dwColor = 0;

public:
	HRESULT						Initialize(CCubeCol* pCubeCol);
	virtual void				Render(const My_Matrix* matWorld) override;

public:
	D3DCOLOR					Get_Color(void) const {
		return m_dwColor;
	}

public:
	void						Set_Color(D3DCOLOR dwColor) { m_dwColor = dwColor; }

public:
	static CMyLocalInfo_CubeCol*		Create(LPDIRECT3DDEVICE9 pDevice, CCubeCol* pCubeCol);
protected:
	virtual My_uLong			ReleaseMemory(void);
};

END