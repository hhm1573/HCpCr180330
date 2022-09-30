#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final
	:public CComponent
{
public:
	enum TEXTURETYPE { TYPE_GENERAL, TYPE_CUBE, TYPE_VOLUME };

private:
	explicit CTexture(LPDIRECT3DDEVICE9 pDevice);
	explicit CTexture(const CTexture& Ref);
	virtual ~CTexture() = default;

private:
	vector<IDirect3DBaseTexture9*>		m_VecTexture;
	typedef vector<IDirect3DBaseTexture9*>		VECTEXTURE;

public:
	HRESULT		Initialize(TEXTURETYPE eType, const My_TChar* pTextureFilePath, const My_uInt& iNumTexture);
	HRESULT		SetUp_OnGraphicDev(const My_uInt& iIndex);

public:
	static CTexture*		Create(LPDIRECT3DDEVICE9 pDevice, TEXTURETYPE eType, const My_TChar* pTextureFilePath, const My_uInt& iNumTexture = 1);
	virtual CComponent*		Clone();
protected:
	virtual My_uLong		ReleaseMemory();
};

END