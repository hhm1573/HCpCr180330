#pragma once

#include "Base.h"

BEGIN(Engine)

class CFrame;

class ENGINE_DLL CFrameMgr
	:public CBase
{
	DECLARE_SINGLETON(CFrameMgr)

private:
	explicit CFrameMgr();
	virtual ~CFrameMgr() = default;

private:
	map<const My_TChar*, CFrame*>			m_mapFrame;
	typedef map<const My_TChar*, CFrame*>	MAPFRAME;

public:
	HRESULT Add_Frame(const My_TChar* pFrameTag, const My_Float& fCallCnt);
	My_Bool Permit_Call(const My_TChar* pFrameTag, const My_Float& fTimeDelta);
private:
	CFrame* Find_Frame(const My_TChar* pFrameTag);

public:
	virtual My_uLong ReleaseMemory(void);
};

END