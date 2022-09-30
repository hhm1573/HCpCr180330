#include "..\Header\MyLocalInfo_CubeCol.h"
#include "CubeCol.h"

CMyLocalInfo_CubeCol::CMyLocalInfo_CubeCol(LPDIRECT3DDEVICE9 pDevice)
	:CMyLocalInfo(pDevice)
{
}

CMyLocalInfo_CubeCol::CMyLocalInfo_CubeCol(const CMyLocalInfo_CubeCol & Ref)
	:CMyLocalInfo(Ref)
{
}

HRESULT CMyLocalInfo_CubeCol::Initialize(CCubeCol * pCubeCol)
{
	if (pCubeCol == nullptr)
		return E_FAIL;

	m_pBuffer = pCubeCol;

	return S_OK;
}

void CMyLocalInfo_CubeCol::Render(const My_Matrix * matWorld)
{
	if (m_pDevice == nullptr || m_pBuffer == nullptr)
		return;

	My_Matrix matLocalWorld = (matWorld == nullptr) ? m_matLocal : m_matLocal * (*matWorld);
	m_pDevice->SetTransform(D3DTS_WORLD, &matLocalWorld);

	((CCubeCol*)m_pBuffer)->SetColor(m_dwColor);
	((CCubeCol*)m_pBuffer)->Render();
}

CMyLocalInfo_CubeCol * CMyLocalInfo_CubeCol::Create(LPDIRECT3DDEVICE9 pDevice, CCubeCol * pCubeCol)
{
	CMyLocalInfo_CubeCol* pInfo = new CMyLocalInfo_CubeCol(pDevice);

	pInfo->Initialize(pCubeCol);

	return pInfo;
}

My_uLong CMyLocalInfo_CubeCol::ReleaseMemory(void)
{
	CMyLocalInfo::ReleaseMemory();
	return My_uLong();
}
