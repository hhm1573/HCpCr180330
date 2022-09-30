#include "..\Header\ViewPort_Buffer.h"

USING(Engine)

CViewPort_Buffer::CViewPort_Buffer(LPDIRECT3DDEVICE9 pDevice)
	:CVIBuffer(pDevice)
{
}

CViewPort_Buffer::CViewPort_Buffer(const CViewPort_Buffer & Ref)
	: CVIBuffer(Ref)
{
}

HRESULT CViewPort_Buffer::Initialize()
{
	m_dwVertexNum = 4;
	m_dwVertexSize = sizeof(VTXVIEWPORTTEX);
	m_dwVertexFVF = VTXFVF_VIEWPORTTEX;
	m_dwNumX = 2;
	m_dwNumZ = 2;


	m_dwNumPolygons = 2;
	m_dwPolygonSize = sizeof(POLYGON16);
	m_IndexFormat = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Initialize()))
		return E_FAIL;

	VTXVIEWPORTTEX*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	// In.Local Space
	pVertices[0].vPosition = My_Vec4(-0.5f, -0.5f, 0.f, 1.0f);
	pVertices[0].vTexUV = My_Vec2(0.f, 0.0f);
	m_vec4Points.push_back(pVertices[0].vPosition);

	pVertices[1].vPosition = My_Vec4(0.5f, -0.5f, 0.f, 1.0f);
	pVertices[1].vTexUV = My_Vec2(1.f, 0.0f);
	m_vec4Points.push_back(pVertices[1].vPosition);

	pVertices[2].vPosition = My_Vec4(0.5f, 0.5f, 0.f, 1.0f);
	pVertices[2].vTexUV = My_Vec2(1.f, 1.f);
	m_vec4Points.push_back(pVertices[2].vPosition);

	pVertices[3].vPosition = My_Vec4(-0.5f, 0.5f, 0.f, 1.0f);
	pVertices[3].vTexUV = My_Vec2(0.f, 1.f);
	m_vec4Points.push_back(pVertices[3].vPosition);

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

void CViewPort_Buffer::Render()
{
	if (nullptr == m_pDevice
		|| nullptr == m_pVB)
		return;

	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVertexSize);
	m_pDevice->SetFVF(m_dwVertexFVF);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVertexNum, 0, m_dwNumPolygons);
}

CViewPort_Buffer * CViewPort_Buffer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CViewPort_Buffer*		pInstance = new CViewPort_Buffer(pDevice);

	if (FAILED(pInstance->Initialize())) {
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

CComponent * CViewPort_Buffer::Clone()
{
	return new CViewPort_Buffer(*this);
}

My_uLong CViewPort_Buffer::ReleaseMemory()
{
	CVIBuffer::ReleaseMemory();

	return My_uLong();
}
