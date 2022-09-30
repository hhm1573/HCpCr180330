#include "..\Header\Sphere_Buffer.h"

USING(Engine)

CSphere_Buffer::CSphere_Buffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CSphere_Buffer::CSphere_Buffer(const CSphere_Buffer & rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CSphere_Buffer::Ready_Sphere_Buffer(const My_uLong & dwCnt, const float & fRadius)
{
	m_dwVertexNum = dwCnt * dwCnt;
	m_dwVertexSize = sizeof(VTXNORTEX);
	m_dwVertexFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE4(0);

	m_dwNumPolygons = (dwCnt - 1) * (dwCnt - 1) * 2;
	m_dwPolygonSize = sizeof(POLYGON32);
	m_IndexFormat = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Initialize()))
		return E_FAIL;

	VTXNORTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < dwCnt; ++i)
	{
		for (size_t j = 0; j < dwCnt; ++j)
		{
			My_uInt	iIndex = i * dwCnt + j;

			float al = 2.f * D3DX_PI * ((float)j / (dwCnt - 1.f));
			float th = 2.f * D3DX_PI * ((float)i / (dwCnt - 1.f));
			// ³ë¸» ÁÂÇ¥
			pVertices[iIndex].vNormal = My_Vec3(sinf(th) * sinf(al), cosf(th), sinf(th) * cosf(al));
			// ¹öÅØ½º ÁÂÇ¥
			pVertices[iIndex].vPosition = My_Vec3(
				pVertices[iIndex].vNormal.x * fRadius,
				pVertices[iIndex].vNormal.y * fRadius,
				pVertices[iIndex].vNormal.z * fRadius);
			// ÅØ½ºÃÄ ÁÂÇ¥
			pVertices[iIndex].vUV.x = 1.f - (al / D3DX_PI * 0.5f);
			pVertices[iIndex].vUV.y = 1.f - (th / D3DX_PI);
		}
	}

	m_pVB->Unlock();

	POLYGON32*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	My_uInt		iNumTriangle = 0;

	for (size_t i = 0; i < dwCnt - 1; ++i)
	{
		for (size_t j = 0; j < dwCnt - 1; ++j)
		{
			My_uInt	iIndex = i * dwCnt + j;

			pIndices[iNumTriangle]._1 = My_uShort(iIndex + dwCnt);
			pIndices[iNumTriangle]._2 = My_uShort(iIndex + dwCnt + 1);
			pIndices[iNumTriangle]._3 = My_uShort(iIndex + 1);
			++iNumTriangle;

			pIndices[iNumTriangle]._1 = My_uShort(iIndex + dwCnt);
			pIndices[iNumTriangle]._2 = My_uShort(iIndex + 1);
			pIndices[iNumTriangle]._3 = My_uShort(iIndex);
			++iNumTriangle;
		}
	}

	m_pIB->Unlock();

	return NOERROR;
}

void CSphere_Buffer::Render_Buffer()
{
	if (nullptr == m_pDevice
		|| nullptr == m_pVB)
		return;

	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVertexSize);
	m_pDevice->SetFVF(m_dwVertexFVF);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVertexNum, 0, m_dwNumPolygons);

}

CSphere_Buffer * CSphere_Buffer::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const My_uLong & dwCnt, const float & fRadius)
{
	CSphere_Buffer*		pInstance = new CSphere_Buffer(pGraphic_Device);

	if (FAILED(pInstance->Ready_Sphere_Buffer(dwCnt, fRadius)))
	{
		//_MSG_BOX("CSphere_Buffer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CSphere_Buffer::Clone()
{
	return new CSphere_Buffer(*this);
}

My_uLong CSphere_Buffer::ReleaseMemory()
{
	CVIBuffer::ReleaseMemory();

	return My_uLong();
}
