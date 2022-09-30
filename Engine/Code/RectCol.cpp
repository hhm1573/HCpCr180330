#include "..\Header\RectCol.h"


CRectCol::CRectCol(LPDIRECT3DDEVICE9 pDevice)
	:CVIBuffer(pDevice)
{
}

CRectCol::CRectCol(const CRectCol & Ref)
	: CVIBuffer(Ref)
{
}

HRESULT CRectCol::Initialize(void)
{
	m_dwVertexNum = 4;
	m_dwVertexSize = sizeof(VTXCOL);
	m_dwVertexFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	m_dwNumX = 2;
	m_dwNumZ = 2;


	m_dwNumPolygons = 2;
	m_dwPolygonSize = sizeof(POLYGON16);
	m_IndexFormat = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Initialize()))
		return E_FAIL;

	VTXCOL*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	// In.Local Space
	pVertices[0].vPosition = My_Vec3(-0.5f, 0.5f, 0.f);
	pVertices[0].dwColor = D3DCOLOR_ARGB(255, 255, 200, 200);
	m_vecPoints.push_back(pVertices[0].vPosition);

	pVertices[1].vPosition = My_Vec3(0.5f, 0.5f, 0.f);
	pVertices[1].dwColor = D3DCOLOR_ARGB(255, 255, 200, 200);
	m_vecPoints.push_back(pVertices[1].vPosition);

	pVertices[2].vPosition = My_Vec3(0.5f, -0.5f, 0.f);
	pVertices[2].dwColor = D3DCOLOR_ARGB(255, 255, 200, 200);
	m_vecPoints.push_back(pVertices[2].vPosition);

	pVertices[3].vPosition = My_Vec3(-0.5f, -0.5f, 0.f);
	pVertices[3].dwColor = D3DCOLOR_ARGB(255, 255, 200, 200);
	m_vecPoints.push_back(pVertices[3].vPosition);

	m_pVB->Unlock();

	POLYGON16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	m_pIB->Unlock();

	return NOERROR;
}

void CRectCol::Render(void)
{
	if (nullptr == m_pDevice
		|| nullptr == m_pVB)
		return;

	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVertexSize);
	m_pDevice->SetFVF(m_dwVertexFVF);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVertexNum, 0, m_dwNumPolygons);
}

CRectCol * CRectCol::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRectCol*	pInstance = new CRectCol(pDevice);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CRectCol Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CRectCol::Clone()
{
	return new CRectCol(*this);
}

void CRectCol::Set_Verticies(const My_Vec3 & vLT, const My_Vec3 & vRT, const My_Vec3 & vLB, const My_Vec3 & vRB)
{
	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = vLT;
	pVertex[1].vPosition = vRT;
	pVertex[2].vPosition = vRB;
	pVertex[3].vPosition = vLB;

	m_pVB->Unlock();
}

void CRectCol::Reset_Verticies()
{
	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = m_vecPoints[0];
	pVertex[1].vPosition = m_vecPoints[1];
	pVertex[2].vPosition = m_vecPoints[2];
	pVertex[3].vPosition = m_vecPoints[3];

	m_pVB->Unlock();
}

void CRectCol::SetColor(D3DCOLOR color)
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

My_uLong CRectCol::ReleaseMemory(void)
{
	CVIBuffer::ReleaseMemory();

	return My_uLong();
}
