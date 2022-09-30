#pragma once

#include "Base.h"

class CTetris;

class CTetrisManager
	: public CBase
{
	DECLARE_SINGLETON(CTetrisManager)

private:
	explicit CTetrisManager();
	virtual ~CTetrisManager() = default;

public:
	HRESULT		Initialize();
	My_Int		Update(const My_Float& fTimeDelta);

public:
	void		SetTetris(CTetris* tetris);
	void		SetIsBoss(My_Bool flag) { mIsBoss = flag; }

private:
	vector<CTetris*>	mTetrisVector;
	My_Long				mTargetIndex;
	My_Float			mTime;
	My_Bool				mIsBoss = false;

protected:
	virtual My_uLong	ReleaseMemory(void);
};

