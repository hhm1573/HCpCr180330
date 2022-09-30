#pragma once

#include "Base.h"

namespace Engine {
	class CGameObject;
	class CGameObjectMgr;
	class CGraphicDev;
}

class CGameObject_Creator final
	:public CBase
{
	DECLARE_SINGLETON(CGameObject_Creator)

private:
	explicit CGameObject_Creator();
	~CGameObject_Creator() = default;

private:
	CGraphicDev*		m_pDeviceMgr = nullptr;
	LPDIRECT3DDEVICE9	m_pDevice = nullptr;
	CGameObjectMgr*		m_pObjectMgr = nullptr;

private:
	My_Int				mFaceCount = 0;

public:
	CGameObject*				CreateObject_FromID(const My_uInt& ID, const My_uInt& Sub_ID, const My_Matrix& matWorld);

protected:
	virtual My_uLong			ReleaseMemory(void);
};

