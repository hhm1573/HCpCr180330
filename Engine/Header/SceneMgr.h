#pragma once

#include "Base.h"

BEGIN(Engine)

class CScene;

class ENGINE_DLL CSceneMgr final
	:public CBase
{
	DECLARE_SINGLETON(CSceneMgr)

private:
	explicit CSceneMgr();
	virtual ~CSceneMgr();

private:
	CScene*		m_pCurScene = nullptr;

public:
	HRESULT		SetUp_CurrentScene(CScene* pNewScene);
	My_Int		Update(const My_Float& fTimeDelta);
	void		Render();

protected:
	virtual My_uLong		ReleaseMemory();
};

END