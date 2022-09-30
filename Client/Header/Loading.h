#pragma once

#include "Include.h"
#include "Scene.h"

namespace Engine
{
	class CTransform;
	class CViewPort_Buffer;
	class CRenderer;
	class CTexture;
}

class CLoading final : public CScene
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLoading() = default;

public:
	virtual		HRESULT		Initialize(void);
	virtual		My_Int		Update(const My_Float& fTimeDelta);
	virtual		My_Int		LastUpdate(const My_Float& fTimeDelta);
	virtual		void		Render();

private:
	HRESULT		Initialize_Component_Prototype();
	HRESULT		Initialize_GameObject_Prototype();
	HRESULT		Initialize_Layer_Loading_BackGround(const My_TChar* szKey);
	HRESULT		Initialize_Lighting();

public:
	static unsigned int __stdcall ThreadFunc(void* arg);

private:
	HANDLE				mThread;
	CRITICAL_SECTION	mCSKey;
	bool				mComplete;

public:
	static		CLoading*	Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual		My_uLong	ReleaseMemory();
};

