#include "..\Header\Terrain_Buffer.h"

CTerrain_Buffer::CTerrain_Buffer(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
}

CTerrain_Buffer::CTerrain_Buffer(const CTerrain_Buffer & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CTerrain_Buffer::Initialize(const My_TChar * pHeightMapPath, const My_Float & fInterval)
{
	HANDLE		hFile;

	My_uLong		dwByte = 0;

	m_fInterval = fInterval;

	// 현재 경로에 해당하는 파일의 핸들을 생성했다.
	hFile = CreateFile(pHeightMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	ReadFile(hFile, &m_fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);

	ReadFile(hFile, &m_ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	My_uLong*			pPixel = new My_uLong[m_ih.biWidth * m_ih.biHeight];

	ReadFile(hFile, pPixel, sizeof(My_uLong) * m_ih.biWidth * m_ih.biHeight, &dwByte, nullptr);

	CloseHandle(hFile);

	m_dwNumX = m_ih.biWidth;
	m_dwNumZ = m_ih.biHeight;

	m_dwVertexNum = m_dwNumX * m_dwNumZ;
	m_dwVertexSize = sizeof(VTXNORTEX);
	m_dwVertexFVF = VTXFVF_NORTEX;

	m_dwNumPolygons = (m_dwNumX - 1) * (m_dwNumZ - 1) * 2;
	m_dwPolygonSize = sizeof(POLYGON32);
	m_IndexFormat = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Initialize()))
		return E_FAIL;

	VTXNORTEX*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	m_vecPoints.reserve(m_dwNumX * m_dwNumZ);

	// In.Local Space

	for (size_t i = 0; i < m_dwNumZ; i++)
	{
		for (size_t j = 0; j < m_dwNumX; j++)
		{
			My_uLong		iIndex = i * m_dwNumX + j;

			/*
			00101101 00000000 00001011 01111011
			00000000 00000000 00000000 11111111
			00000000 00000000 00000000 01111011*/

			pVertices[iIndex].vPosition = My_Vec3(j * fInterval, (pPixel[iIndex] & 0x000000ff) / 25.f, i * fInterval);
			pVertices[iIndex].vNormal = My_Vec3(0.0f, 0.0f, 0.0f);
			pVertices[iIndex].vUV = My_Vec2(My_Float(j) / (m_dwNumX - 1), 1 - My_Float(i) / (m_dwNumZ - 1));

			m_vecPoints.push_back(pVertices[iIndex].vPosition);
		}
	}

	POLYGON32*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	My_uInt			iNumTriangle = 0;

	for (size_t i = 0; i < m_dwNumZ - 1; i++)
	{
		for (size_t j = 0; j < m_dwNumX - 1; j++)
		{
			My_uLong iIndex = i * m_dwNumX + j;

			pIndices[iNumTriangle]._1 = iIndex + m_dwNumX;
			pIndices[iNumTriangle]._2 = iIndex + m_dwNumX + 1;
			pIndices[iNumTriangle]._3 = iIndex + 1;

			My_Vec3			vSour, vDest, vResult;
			vSour = m_vecPoints[pIndices[iNumTriangle]._2] - m_vecPoints[pIndices[iNumTriangle]._1];
			vDest = m_vecPoints[pIndices[iNumTriangle]._3] - m_vecPoints[pIndices[iNumTriangle]._1];

			D3DXVec3Cross(&vResult, &vSour, &vDest);

			pVertices[pIndices[iNumTriangle]._1].vNormal += vResult;
			pVertices[pIndices[iNumTriangle]._2].vNormal += vResult;
			pVertices[pIndices[iNumTriangle]._3].vNormal += vResult;

			++iNumTriangle;



			pIndices[iNumTriangle]._1 = iIndex + m_dwNumX;
			pIndices[iNumTriangle]._2 = iIndex + 1;
			pIndices[iNumTriangle]._3 = iIndex;

			vSour = m_vecPoints[pIndices[iNumTriangle]._2] - m_vecPoints[pIndices[iNumTriangle]._1];
			vDest = m_vecPoints[pIndices[iNumTriangle]._3] - m_vecPoints[pIndices[iNumTriangle]._1];

			D3DXVec3Cross(&vResult, &vSour, &vDest);

			pVertices[pIndices[iNumTriangle]._1].vNormal += vResult;
			pVertices[pIndices[iNumTriangle]._2].vNormal += vResult;
			pVertices[pIndices[iNumTriangle]._3].vNormal += vResult;

			++iNumTriangle;
		}
	}

	for (size_t i = 0; i < m_dwVertexNum; i++)
	{
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);
	}

	m_pIB->Unlock();
	m_pVB->Unlock();

	return NOERROR;
}

void CTerrain_Buffer::Render(void)
{
	if (nullptr == m_pDevice
		|| nullptr == m_pVB)
		return;

	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVertexSize);
	m_pDevice->SetFVF(m_dwVertexFVF);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVertexNum, 0, m_dwNumPolygons);
}

CTerrain_Buffer * CTerrain_Buffer::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const My_TChar * pHeightMapPath, const My_Float & fInterval)
{
	CTerrain_Buffer*	pInstance = new CTerrain_Buffer(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pHeightMapPath, fInterval)))
	{
		MSG_BOX("CTerrain_Buffer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTerrain_Buffer::Clone()
{
	return new CTerrain_Buffer(*this);
}

My_uLong CTerrain_Buffer::ReleaseMemory(void)
{
	CVIBuffer::ReleaseMemory();

	return My_uLong();
}
