#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract
	: public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer(const CVIBuffer& Ref);
	virtual ~CVIBuffer() = default;

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;
protected:
	My_uLong					m_dwVertexNum = 0;
	My_uLong					m_dwVertexSize = 0;
	My_uLong					m_dwVertexFVF = 0;
protected:
	My_uLong					m_dwNumPolygons = 0;
	My_uLong					m_dwPolygonSize = 0;
	D3DFORMAT					m_IndexFormat;
protected:
	My_uLong					m_dwNumX = 0, m_dwNumZ = 0;
	My_Float					m_fInterval = 0.0f;
protected:
	vector<My_Vec3>				m_vecPoints;
	vector<My_Vec4>				m_vec4Points;

public:
	HRESULT						Initialize(void);
	HRESULT						Set_Transform(D3DTRANSFORMSTATETYPE Type, const My_Matrix* pMat);

public: //getter
	My_uLong					Get_NumVertex_X() const {
		return m_dwNumX;
	}
	My_uLong					Get_NumVertex_Z() const {
		return m_dwNumZ;
	}
	My_Float					Get_Interval() const {
		return m_fInterval;
	}
	const vector<My_Vec3>*		Get_PosArray() const {
		return &m_vecPoints;
	}

public:
	virtual CComponent*		Clone() PURE;
protected:
	virtual My_uLong		ReleaseMemory() override;
};

END