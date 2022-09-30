#include "..\Header\VIBuffer.h"



CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pDevice)
	:CComponent(pDevice)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & Ref)
	: CComponent(Ref)
{
	m_pVB = Ref.m_pVB;
	m_pVB->AddRef();
	m_pIB = Ref.m_pIB;
	m_pIB->AddRef();

	m_dwVertexNum = Ref.m_dwVertexNum;
	m_dwVertexSize = Ref.m_dwVertexSize;
	m_dwVertexFVF = Ref.m_dwVertexFVF;

	m_dwNumPolygons = Ref.m_dwNumPolygons;
	m_dwPolygonSize = Ref.m_dwPolygonSize;
	m_IndexFormat = Ref.m_IndexFormat;

	m_dwNumX = Ref.m_dwNumX;
	m_dwNumZ = Ref.m_dwNumZ;
	m_fInterval = Ref.m_fInterval;

	m_vecPoints = Ref.m_vecPoints;
	m_vec4Points = Ref.m_vec4Points;
}

HRESULT CVIBuffer::Initialize(void)
{
	if (FAILED(m_pDevice->CreateVertexBuffer(m_dwVertexSize * m_dwVertexNum, 0, m_dwVertexFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateIndexBuffer(m_dwPolygonSize * m_dwNumPolygons, 0, m_IndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CVIBuffer::Set_Transform(D3DTRANSFORMSTATETYPE Type, const My_Matrix * pMat)
{
	if (nullptr == m_pVB
		|| nullptr == pMat)
		return E_FAIL;

	void*		pVertices = nullptr;

	m_pVB->Lock(0, 0, &pVertices, 0);

	// *(pVertices + i)

	if (0 < m_vec4Points.size())
	{
		auto	iter2 = m_vec4Points.begin();

		for (size_t i = 0; i < m_dwVertexNum; i++, ++iter2)
		{
			if (iter2 == m_vec4Points.end())
				return E_FAIL;

			My_Vec4 vPosition2;
			vPosition2 = (*iter2);

			D3DXVec4Transform(&vPosition2, &vPosition2, pMat);
			memcpy(((My_Byte*)pVertices) + m_dwVertexSize * i, &vPosition2, sizeof(My_Vec4));
		}
	}
	else
	{
		auto	iter = m_vecPoints.begin();

		for (size_t i = 0; i < m_dwVertexNum; i++, ++iter)
		{
			if (iter == m_vecPoints.end())
				return E_FAIL;

			My_Vec3	vPosition;

			if (D3DTS_WORLD != Type)
				memcpy(&vPosition, ((My_Byte*)pVertices) + m_dwVertexSize * i, sizeof(My_Vec3));

			else
				vPosition = (*iter);


			D3DXVec3TransformCoord(&vPosition, &vPosition, pMat);
			memcpy(((My_Byte*)pVertices) + m_dwVertexSize * i, &vPosition, sizeof(My_Vec3));
		}
	}

	m_pVB->Unlock();

	return NOERROR;
}

My_uLong CVIBuffer::ReleaseMemory()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	CComponent::ReleaseMemory();

	return My_uLong();
}
