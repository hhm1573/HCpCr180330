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

class CSpeedUIBackGround final
	: public CSpeedUI
{
private:
	explicit CSpeedUIBackGround(LPDIRECT3DDEVICE9 pDevice);
	explicit CSpeedUIBackGround(const CSpeedUIBackGround& Ref);
	virtual ~CSpeedUIBackGround() = default;

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Prototype();
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void		Render();

public:
	static		CSpeedUIBackGround*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*			Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

