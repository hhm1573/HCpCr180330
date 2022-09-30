#pragma once

#include "Include.h"
#include "Base.h"

#include <functional>

namespace Engine
{
	class CCollisioner;
	class CTransform;
}

class CRankingManager
	: public CBase
{
	DECLARE_SINGLETON(CRankingManager)

public:
	enum Racer { BAZZI, DAO, DIZNI, RACER_END };

private:
	explicit CRankingManager();
	virtual ~CRankingManager() = default;

public:
	HRESULT			Initialize();
	My_Int			Update(const My_Float& fTimeDelta);

public:
	My_Int			GetLaps();
	vector<Racer>*	GetRankingVector() { return &mRanking; }
	Racer			GetFrontRacer(Racer racer);
	Racer			GetFrist() { return mRanking[0]; }
	My_Int			GetRank(Racer racer);

public:
	void			SetTransform(CTransform* bazzi, CTransform* dao, CTransform* dizni);
	void			SetIsStart() { mIsStart = true; }
	void			SetCheet();

private:
	vector<LINE_DATA*>		mLineVector;
	vector<Racer>			mRanking;
	vector<Racer>			mFinalRanking;

	// Bazzi, Dao, Dizni ¼ø¼­
	multimap<My_Int, Racer, greater<My_Int>>	mLaps;
	vector<My_Bool>								mIsLapsCol;
	vector<My_Float>							mColTime;
	vector<CTransform*>							mTransformVec;

	My_Vec3					mStartLineMinPos;
	My_Vec3					mStartLineMaxPos;

	My_Int					mState = 0;

	My_Bool					mIsStart = false;

	typedef		multimap<My_Int, Racer, greater<My_Int>>	MAP_LAPS;

private:
	CCollisioner*			mCollisioner = nullptr;

private:
	void				ComputeLaps(const My_Float & fTimeDelta);
	void				ComputeRanking(const My_Float & fTimeDelta);

private:
	My_Int				ComputeNearestLineIndex(Racer racer);

protected:
	virtual My_uLong	ReleaseMemory(void);
};

