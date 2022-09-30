#include "..\Header\ViewPort_Tri.h"

USING(Engine)

CViewPort_Tri::CViewPort_Tri(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
}

CViewPort_Tri::CViewPort_Tri(const CViewPort_Tri & Ref)
	: CVIBuffer(Ref)
{
}

HRESULT CViewPort_Tri::Initialize()
{
	m_dwVertexNum = 3;
	m_dwVertexSize = sizeof(VTXVIEWPORTCOL);
	m_dwVertexFVF = VTXFVF_VIEWPORTCOL;

	m_dwNumPolygons = 1;
	m_dwPolygonSize = sizeof(POLYGON16);
	m_IndexFormat = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Initialize()))
		return E_FAIL;

	VTXVIEWPORTCOL*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	// In.Local Space
	pVertices[0].vPosition = My_Vec4(0.f, 0.5f, 0.f, 1.0f);
	pVertices[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	m_vec4Points.push_back(pVertices[0].vPosition);

	pVertices[1].vPosition = My_Vec4(0.5f, -0.5f, 0.f, 1.0f);
	pVertices[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	m_vec4Points.push_back(pVertices[1].vPosition);

	pVertices[2].vPosition = My_Vec4(-0.5f, -0.5f, 0.f, 1.0f);
	pVertices[2].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	m_vec4Points.push_back(pVertices[2].vPosition);

	m_pVB->Unlock();

	return NOERROR;
}

void CViewPort_Tri::Render()
{
	if (nullptr == m_pDevice
		|| nullptr == m_pVB)
		return;

	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVertexSize);
	m_pDevice->SetFVF(m_dwVertexFVF);
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
}

CViewPort_Tri * CViewPort_Tri::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CViewPort_Tri*	pInstance = new CViewPort_Tri(pDevice);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CViewPort_Tri Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CViewPort_Tri::Clone()
{
	return new CViewPort_Tri(*this);

}

My_uLong CViewPort_Tri::ReleaseMemory()
{
	CVIBuffer::ReleaseMemory();

	return My_uLong();
}
