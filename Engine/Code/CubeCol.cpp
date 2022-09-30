#include "..\Header\CubeCol.h"



CCubeCol::CCubeCol(LPDIRECT3DDEVICE9 pDevice)
	:CVIBuffer(pDevice)
{
}

CCubeCol::CCubeCol(const CCubeCol & Ref)
	:CVIBuffer(Ref)
{
}

HRESULT CCubeCol::Initialize(void)
{
	m_dwVertexNum = 8;
	m_dwVertexSize = sizeof(VTXCOL);
	m_dwVertexFVF = VTXFVF_COL;

	m_dwPolygonSize = sizeof(POLYGON16);
	m_dwNumPolygons = 12;
	m_IndexFormat = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Initialize()))
		return E_FAIL;

	VTXCOL*		pVertex = nullptr;

	if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertex, 0)))
		return E_FAIL;

	/*
	/4----5
	/ |  / |
	0----1  |
	|  6-|-/7
	| /  |/
	2----3
	*/
	{
		pVertex[0].vPosition = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
		pVertex[0].dwColor = D3DCOLOR_ARGB(255, 0, 255, 200);

		pVertex[1].vPosition = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
		pVertex[1].dwColor = D3DCOLOR_ARGB(255, 0, 255, 200);

		pVertex[2].vPosition = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
		pVertex[2].dwColor = D3DCOLOR_ARGB(255, 0, 255, 200);

		pVertex[3].vPosition = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
		pVertex[3].dwColor = D3DCOLOR_ARGB(255, 0, 255, 200);

		pVertex[4].vPosition = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
		pVertex[4].dwColor = D3DCOLOR_ARGB(255, 255, 80, 200);

		pVertex[5].vPosition = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		pVertex[5].dwColor = D3DCOLOR_ARGB(255, 255, 80, 200);

		pVertex[6].vPosition = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
		pVertex[6].dwColor = D3DCOLOR_ARGB(255, 255, 80, 200);

		pVertex[7].vPosition = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
		pVertex[7].dwColor = D3DCOLOR_ARGB(255, 255, 80, 200);

		/*pVertex[0].vNormal = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
		pVertex[1].vNormal = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
		pVertex[2].vNormal = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
		pVertex[3].vNormal = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
		pVertex[4].vNormal = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
		pVertex[5].vNormal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVertex[6].vNormal = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
		pVertex[7].vNormal = D3DXVECTOR3(1.0f, -1.0f, 1.0f);*/

		/*for (int i = 0; i < m_dwVertexNum; ++i) {
			pVertex[i].vNormal = My_Vec3(0.0f, 1.0f, 0.0f);
		}*/

		for (Engine::My_uLong i = 0; i < m_dwVertexNum; ++i) {
			m_vecPoints.push_back(pVertex[i].vPosition);
			//D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);
		}
	}

	if (FAILED(m_pVB->Unlock()))
		return E_FAIL;



	POLYGON16*	pIndex = nullptr;

	if (FAILED(m_pIB->Lock(0, 0, (void**)&pIndex, 0)))
		return E_FAIL;

	{
		SetIndex(pIndex, 0, 0, 1, 3);
		SetIndex(pIndex, 1, 0, 3, 2);

		SetIndex(pIndex, 2, 1, 5, 7);
		SetIndex(pIndex, 3, 1, 7, 3);

		SetIndex(pIndex, 4, 5, 4, 6);
		SetIndex(pIndex, 5, 5, 6, 7);

		SetIndex(pIndex, 6, 4, 0, 2);
		SetIndex(pIndex, 7, 4, 2, 6);

		SetIndex(pIndex, 8, 4, 5, 1);
		SetIndex(pIndex, 9, 4, 1, 0);

		SetIndex(pIndex, 10, 7, 6, 3);
		SetIndex(pIndex, 11, 6, 2, 3);
	}

	if (FAILED(m_pIB->Unlock()))
		return E_FAIL;

	return S_OK;
}

void CCubeCol::Render(void)
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVertexSize);
	m_pDevice->SetFVF(m_dwVertexFVF);

	m_pDevice->SetIndices(m_pIB);

	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVertexNum, 0, m_dwNumPolygons);
}

void CCubeCol::SetIndex(POLYGON16 * pIndex, My_uInt wIndex, My_uInt wV0, My_uInt wV1, My_uInt wV2)
{
	pIndex[wIndex]._1 = wV0;
	pIndex[wIndex]._2 = wV1;
	pIndex[wIndex]._3 = wV2;
}

void CCubeCol::SetColor(D3DCOLOR color)
{
	VTXCOL*		pVertex = nullptr;
	
	if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertex, 0)))
		return;

	for (My_uLong i = 0; i < m_dwVertexNum; ++i) {
		pVertex[i].dwColor = color;
	}

	if (FAILED(m_pVB->Unlock()))
		return;
}

CCubeCol * CCubeCol::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCubeCol* pInstance = nullptr;
	pInstance = new CCubeCol(pDevice);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CComponent * CCubeCol::Clone()
{
	CCubeCol*	pInstance = nullptr;
	pInstance = new CCubeCol(*this);

	return pInstance;
}

My_uLong CCubeCol::ReleaseMemory()
{
	CVIBuffer::ReleaseMemory();

	return My_uLong();
}
