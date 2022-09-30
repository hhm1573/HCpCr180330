#include "..\Header\CubeTex.h"

CCubeTex::CCubeTex(LPDIRECT3DDEVICE9 pDevice)
	:CVIBuffer(pDevice)
{
}

CCubeTex::CCubeTex(const CCubeTex & Ref)
	:CVIBuffer(Ref)
{
}

HRESULT CCubeTex::Initialize()
{
	m_dwVertexNum = 8;
	m_dwVertexSize = sizeof(VTXCUBETEX);
	m_dwVertexFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	m_dwNumX = 2;
	m_dwNumZ = 2;


	m_dwNumPolygons = 12;
	m_dwPolygonSize = sizeof(POLYGON16);
	m_IndexFormat = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Initialize()))
		return E_FAIL;

	VTXCUBETEX*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	// In.Local Space
	pVertices[0].vPosition = My_Vec3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vTexUV = pVertices[0].vPosition;
	m_vecPoints.push_back(pVertices[0].vPosition);

	pVertices[1].vPosition = My_Vec3(0.5f, 0.5f, -0.5f);
	pVertices[1].vTexUV = pVertices[1].vPosition;
	m_vecPoints.push_back(pVertices[1].vPosition);

	pVertices[2].vPosition = My_Vec3(0.5f, -0.5f, -0.5f);
	pVertices[2].vTexUV = pVertices[2].vPosition;
	m_vecPoints.push_back(pVertices[2].vPosition);

	pVertices[3].vPosition = My_Vec3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vTexUV = pVertices[3].vPosition;
	m_vecPoints.push_back(pVertices[3].vPosition);

	pVertices[4].vPosition = My_Vec3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vTexUV = pVertices[4].vPosition;
	m_vecPoints.push_back(pVertices[4].vPosition);

	pVertices[5].vPosition = My_Vec3(0.5f, 0.5f, 0.5f);
	pVertices[5].vTexUV = pVertices[5].vPosition;
	m_vecPoints.push_back(pVertices[5].vPosition);

	pVertices[6].vPosition = My_Vec3(0.5f, -0.5f, 0.5f);
	pVertices[6].vTexUV = pVertices[6].vPosition;
	m_vecPoints.push_back(pVertices[6].vPosition);

	pVertices[7].vPosition = My_Vec3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vTexUV = pVertices[7].vPosition;
	m_vecPoints.push_back(pVertices[7].vPosition);


	m_pVB->Unlock();

	POLYGON16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	// +X
	pIndices[0]._1 = 1; pIndices[0]._2 = 5; pIndices[0]._3 = 6;
	pIndices[1]._1 = 1; pIndices[1]._2 = 6; pIndices[1]._3 = 2;
	// -X
	pIndices[2]._1 = 4; pIndices[2]._2 = 0; pIndices[2]._3 = 3;
	pIndices[3]._1 = 4; pIndices[3]._2 = 3; pIndices[3]._3 = 7;

	// +Y
	pIndices[4]._1 = 4; pIndices[4]._2 = 5; pIndices[4]._3 = 1;
	pIndices[5]._1 = 4; pIndices[5]._2 = 1; pIndices[5]._3 = 0;
	// -Y
	pIndices[6]._1 = 3; pIndices[6]._2 = 2; pIndices[6]._3 = 6;
	pIndices[7]._1 = 3; pIndices[7]._2 = 6; pIndices[7]._3 = 7;

	// +Z
	pIndices[8]._1 = 7; pIndices[8]._2 = 6; pIndices[8]._3 = 5;
	pIndices[9]._1 = 7; pIndices[9]._2 = 5; pIndices[9]._3 = 4;
	// -Z
	pIndices[10]._1 = 0; pIndices[10]._2 = 1; pIndices[10]._3 = 2;
	pIndices[11]._1 = 0; pIndices[11]._2 = 2; pIndices[11]._3 = 3;


	m_pIB->Unlock();

	return NOERROR;
}

void CCubeTex::Render()
{
	if (nullptr == m_pDevice
		|| nullptr == m_pVB)
		return;

	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVertexSize);
	m_pDevice->SetFVF(m_dwVertexFVF);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVertexNum, 0, m_dwNumPolygons);
}

CCubeTex * CCubeTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCubeTex*	pInstance = new CCubeTex(pDevice);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CCubeTex Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CCubeTex::Clone()
{
	return new CCubeTex(*this);
}

My_uLong CCubeTex::ReleaseMemory()
{
	CVIBuffer::ReleaseMemory();

	return My_uLong();
}
