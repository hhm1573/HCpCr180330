#include "..\Header\RectTex.h"


CRectTex::CRectTex(LPDIRECT3DDEVICE9 pDevice)
	:CVIBuffer(pDevice)
{
}

CRectTex::CRectTex(const CRectTex & Ref)
	: CVIBuffer(Ref)
{
}

HRESULT CRectTex::Initialize(void)
{
	m_dwVertexNum = 4;
	m_dwVertexSize = sizeof(VTXTEX);
	m_dwVertexFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_dwNumX = 2;
	m_dwNumZ = 2;


	m_dwNumPolygons = 2;
	m_dwPolygonSize = sizeof(POLYGON16);
	m_IndexFormat = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Initialize()))
		return E_FAIL;

	VTXTEX*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	// In.Local Space
	pVertices[0].vPosition = My_Vec3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexUV = My_Vec2(0.f, 0.0f);
	m_vecPoints.push_back(pVertices[0].vPosition);

	pVertices[1].vPosition = My_Vec3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexUV = My_Vec2(1.f, 0.0f);
	m_vecPoints.push_back(pVertices[1].vPosition);

	pVertices[2].vPosition = My_Vec3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexUV = My_Vec2(1.f, 1.f);
	m_vecPoints.push_back(pVertices[2].vPosition);

	pVertices[3].vPosition = My_Vec3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexUV = My_Vec2(0.f, 1.f);
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

void CRectTex::Render(void)
{
	if (nullptr == m_pDevice
		|| nullptr == m_pVB)
		return;

	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVertexSize);
	m_pDevice->SetFVF(m_dwVertexFVF);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVertexNum, 0, m_dwNumPolygons);
}

CRectTex * CRectTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRectTex*	pInstance = new CRectTex(pDevice);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CRectTex Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CRectTex::Clone()
{
	return new CRectTex(*this);
}

My_uLong CRectTex::ReleaseMemory(void)
{
	CVIBuffer::ReleaseMemory();

	return My_uLong();
}
