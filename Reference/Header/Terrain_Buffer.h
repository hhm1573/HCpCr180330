#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrain_Buffer final : public CVIBuffer
{
private:
	explicit CTerrain_Buffer(LPDIRECT3DDEVICE9 pDevice);
	explicit CTerrain_Buffer(const CTerrain_Buffer& rhs);
	virtual ~CTerrain_Buffer() = default;
public:
	HRESULT Initialize(const My_TChar* pHeightMapPath, const My_Float& fInterval);
	void Render(void);
private:
	// ���� BMP���Ͽ� ���� ������ ������(����ũ��)
	BITMAPFILEHEADER						m_fh;

	// ���� �̹����� ���� ������ ������.(����, ���� �ȼ���)
	BITMAPINFOHEADER						m_ih;
public:
	static CTerrain_Buffer* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const My_TChar* pHeightMapPath, const My_Float& fInterval);
	virtual CComponent* Clone();
	virtual My_uLong ReleaseMemory(void);
};

END