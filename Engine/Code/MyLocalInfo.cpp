#include "..\Header\MyLocalInfo.h"
#include "VIBuffer.h"



CMyLocalInfo::CMyLocalInfo(LPDIRECT3DDEVICE9 pDevice)
{
	m_vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_vRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vTranslation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_matLocal);

	m_pDevice = pDevice;
	m_pDevice->AddRef();
}

CMyLocalInfo::CMyLocalInfo(const CMyLocalInfo & Ref)
{
	m_vScale = Ref.m_vScale;
	m_vRotation = Ref.m_vRotation;
	m_vTranslation = Ref.m_vTranslation;
	m_matLocal = Ref.m_matLocal;

	m_pDevice = Ref.m_pDevice;
	m_pDevice->AddRef();
}

HRESULT CMyLocalInfo::Refresh(void)
{
	//initialize matLocal
	D3DXMatrixIdentity(&m_matLocal);

	//scaling
	m_matLocal._11 = m_vScale.x;
	m_matLocal._22 = m_vScale.y;
	m_matLocal._33 = m_vScale.z;

	//Rotation
	My_Matrix matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, m_vRotation.y, m_vRotation.x, m_vRotation.z);
	m_matLocal *= matRot;

	//Translation
	m_matLocal._41 = m_vTranslation.x;
	m_matLocal._42 = m_vTranslation.y;
	m_matLocal._43 = m_vTranslation.z;

	return S_OK;
}

void CMyLocalInfo::Render(const My_Matrix * matWorld)
{
	int* BoomBoom = nullptr;
	*BoomBoom = 1;
}

My_uLong CMyLocalInfo::ReleaseMemory(void)
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pBuffer);
	return My_uLong();
}

