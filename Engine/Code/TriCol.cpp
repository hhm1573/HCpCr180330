#include "..\Header\TriCol.h"



CTriCol::CTriCol(LPDIRECT3DDEVICE9 pDevice)
	:CVIBuffer(pDevice)
{
}

CTriCol::CTriCol(const CTriCol & Ref)
	:CVIBuffer(Ref)
{
}

HRESULT CTriCol::Initialize(void)
{
	m_dwVertexNum = 3;
	m_dwVertexSize = sizeof(VTXCOL);
	m_dwVertexFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	m_dwNumPolygons = 1;
	m_dwPolygonSize = sizeof(POLYGON16);
	m_IndexFormat = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Initialize()))
		return E_FAIL;

	VTXCOL*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	// In.Local Space
	pVertices[0].vPosition = My_Vec3(0.f, 0.5f, 0.f);
	pVertices[0].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_vecPoints.push_back(pVertices[0].vPosition);

	pVertices[1].vPosition = My_Vec3(0.5f, -0.5f, 0.f);
	pVertices[1].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_vecPoints.push_back(pVertices[1].vPosition);

	pVertices[2].vPosition = My_Vec3(-0.5f, -0.5f, 0.f);
	pVertices[2].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_vecPoints.push_back(pVertices[2].vPosition);

	m_pVB->Unlock();

	return NOERROR;
}

void CTriCol::Render(void)
{
	if (nullptr == m_pDevice
		|| nullptr == m_pVB)
		return;

	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVertexSize);
	m_pDevice->SetFVF(m_dwVertexFVF);
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
}

CTriCol * CTriCol::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTriCol*	pInstance = new CTriCol(pDevice);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CTriangle_Color Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTriCol::Clone()
{
	return new CTriCol(*this);
}

My_uLong CTriCol::ReleaseMemory()
{
	CVIBuffer::ReleaseMemory();

	return My_uLong();
}
