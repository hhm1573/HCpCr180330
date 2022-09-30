#pragma once

#include "Include.h"
#include "SpeedUI.h"

namespace Engine
{
	class CRenderer;
	class CViewPort_Buffer;
	class CTransform;
	class CTexture;
}

class CSpeedUINumber final
	: public CSpeedUI
{
private:
	explicit CSpeedUINumber(LPDIRECT3DDEVICE9 pDevice);
	explicit CSpeedUINumber(const CSpeedUINumber& Ref);
	virtual ~CSpeedUINumber() = default;

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Prototype();
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void		Render();

private:
	My_Int		mHundred;
	My_Int		mTen;
	My_Int		mOne;

public:
	static		CSpeedUINumber*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*		Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

