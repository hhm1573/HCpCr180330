#include "..\Header\Component.h"



CComponent::CComponent(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	m_pDevice->AddRef();
}

CComponent::CComponent(const CComponent & Ref)
{
	m_pDevice = Ref.m_pDevice;
	m_pDevice->AddRef();
}

My_uLong CComponent::ReleaseMemory(void)
{
	Safe_Release(m_pDevice);

	return My_uLong();
}
