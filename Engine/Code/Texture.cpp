#include "..\Header\Texture.h"



CTexture::CTexture(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
{
}

CTexture::CTexture(const CTexture & Ref)
	: CComponent(Ref)
{
	m_VecTexture = Ref.m_VecTexture;
	for (auto& pTexture : m_VecTexture)
		pTexture->AddRef();
}

HRESULT CTexture::Initialize(TEXTURETYPE eType, const My_TChar * pTextureFilePath, const My_uInt & iNumTexture)
{
	My_TChar			szFullPath[MAX_PATH] = L"";

	m_VecTexture.reserve(iNumTexture);

	for (size_t i = 0; i < iNumTexture; i++)
	{
		IDirect3DBaseTexture9*			pTexture = nullptr;

		wsprintf(szFullPath, pTextureFilePath, i);

		switch (eType)
		{
		case TYPE_GENERAL:
			if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		case TYPE_CUBE:
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		case TYPE_VOLUME:
			if (FAILED(D3DXCreateVolumeTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DVOLUMETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		}

		m_VecTexture.push_back(pTexture);
	}

	return NOERROR;
}

HRESULT CTexture::SetUp_OnGraphicDev(const My_uInt & iIndex)
{
	if (m_VecTexture.size() <= iIndex)
		return E_FAIL;

	if (nullptr != m_VecTexture[iIndex])
		m_pDevice->SetTexture(0, m_VecTexture[iIndex]);

	return NOERROR;
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pDevice, TEXTURETYPE eType, const My_TChar * pTextureFilePath, const My_uInt & iNumTexture)
{
	CTexture*	pInstance = new CTexture(pDevice);

	if (FAILED(pInstance->Initialize(eType, pTextureFilePath, iNumTexture)))
	{
		MSG_BOX("CTexture Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTexture::Clone()
{
	return new CTexture(*this);
}

My_uLong CTexture::ReleaseMemory()
{
	for (auto& pTexture : m_VecTexture)
	{
		Safe_Release(pTexture);
	}
	m_VecTexture.clear();

	CComponent::ReleaseMemory();

	return 0;
}
