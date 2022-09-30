#include "stdafx.h"
#include "..\Header\TetrisManager.h"

#include "Tetris.h"

IMPLEMENT_SINGLETON(CTetrisManager)

CTetrisManager::CTetrisManager()
{
}

HRESULT CTetrisManager::Initialize()
{
	mTime = 0.f;
	mTargetIndex = 0;
	mTetrisVector.reserve(13);
	return S_OK;
}

My_Int CTetrisManager::Update(const My_Float & fTimeDelta)
{
	if (0 == mTargetIndex && mTetrisVector[mTargetIndex]->GetState() == CTetris::READY && mIsBoss)
		return My_Int();

	int prevIndex = 0;

	if (0 == mTargetIndex)
		prevIndex = 12;
	else
		prevIndex = mTargetIndex - 1;

	CTetris::TetrisState state = mTetrisVector[mTargetIndex]->GetState();

	if (CTetris::READY == state)
	{
		if (mTetrisVector[prevIndex]->GetStandingTime() > 1.5f)
			mTetrisVector[mTargetIndex]->SetState(CTetris::GO_DOWN);
	}

	else if (CTetris::STANDING == state)
	{
		switch (mTargetIndex)
		{
		case 3:
			mTetrisVector[0]->SetState(CTetris::DISAPPEAR);
			mTetrisVector[1]->SetState(CTetris::DISAPPEAR);
			mTetrisVector[2]->SetState(CTetris::DISAPPEAR);
			mTetrisVector[3]->SetState(CTetris::DISAPPEAR);
			++mTargetIndex;
			break;

		case 6:
			mTetrisVector[0]->SetState(CTetris::DISAPPEAR);
			mTetrisVector[1]->SetState(CTetris::DISAPPEAR);
			mTetrisVector[2]->SetState(CTetris::DISAPPEAR);
			mTetrisVector[4]->SetState(CTetris::DISAPPEAR);
			mTetrisVector[5]->SetState(CTetris::DISAPPEAR);
			mTetrisVector[6]->SetState(CTetris::DISAPPEAR);
			++mTargetIndex;
			break;

		case 10:
			mTetrisVector[4]->SetState(CTetris::DISAPPEAR);
			mTetrisVector[6]->SetState(CTetris::DISAPPEAR);
			mTetrisVector[7]->SetState(CTetris::DISAPPEAR);
			mTetrisVector[8]->SetState(CTetris::DISAPPEAR);
			mTetrisVector[9]->SetState(CTetris::DISAPPEAR);
			mTetrisVector[10]->SetState(CTetris::DISAPPEAR);
			++mTargetIndex;
			break;

		case 12:
			mTetrisVector[7]->SetState(CTetris::DISAPPEAR);
			mTetrisVector[8]->SetState(CTetris::DISAPPEAR);
			mTetrisVector[9]->SetState(CTetris::DISAPPEAR);
			mTetrisVector[11]->SetState(CTetris::DISAPPEAR);
			mTetrisVector[12]->SetState(CTetris::DISAPPEAR);
			++mTargetIndex;
			break;

		default:
			++mTargetIndex;
			break;
		}
	}

	if (13 == mTargetIndex)
		mTargetIndex = 0;

	return My_Int();
}

void CTetrisManager::SetTetris(CTetris * tetris)
{
	tetris->AddRef();
	mTetrisVector.push_back(tetris);
}

My_uLong CTetrisManager::ReleaseMemory(void)
{
	for (auto& tetris : mTetrisVector)
		Safe_Release(tetris);

	mTetrisVector.clear();
	return My_uLong();
}
