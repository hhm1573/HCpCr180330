#include "stdafx.h"
#include "..\Header\RankingManager.h"

#include "ComponentMgr.h"
#include "GameObjectMgr.h"

#include "Bazzi.h"
#include "Dao.h"
#include "Dizni.h"

#include "EndCountUI.h"
#include "Camera_Target.h"
#include "SoundManager.h"

IMPLEMENT_SINGLETON(CRankingManager)

const	My_uInt	MAX_LINE = 21;
const	My_uInt	MAX_RACER = 3;

CRankingManager::CRankingManager()
{
}

HRESULT CRankingManager::Initialize()
{
	// 예약
	mLineVector.reserve(MAX_LINE);
	mRanking.reserve(MAX_RACER);
	mIsLapsCol.reserve(MAX_RACER);
	mColTime.reserve(MAX_RACER);
	mTransformVec.reserve(MAX_RACER);
	mFinalRanking.reserve(MAX_RACER);

	// 초기화
	// Line 생성
	mLineVector.push_back(new LINE_DATA(My_Vec3(397.f, 7.f, 300.f), My_Vec3(440.f, 7.f, 300.f)));
	mLineVector.push_back(new LINE_DATA(My_Vec3(397.f, 7.f, 356.5f), My_Vec3(440.f, 7.f, 356.5f)));
	mLineVector.push_back(new LINE_DATA(My_Vec3(397.f, 7.f, 420.f), My_Vec3(440.f, 7.f, 420.f)));

	mLineVector.push_back(new LINE_DATA(My_Vec3(396.f, 7.f, 420.f), My_Vec3(396.f, 7.f, 480.f)));
	mLineVector.push_back(new LINE_DATA(My_Vec3(325.f, 7.f, 350.f), My_Vec3(325.f, 7.f, 480.f)));
	mLineVector.push_back(new LINE_DATA(My_Vec3(250.f, 7.f, 350.f), My_Vec3(250.f, 7.f, 480.f)));
	mLineVector.push_back(new LINE_DATA(My_Vec3(200.f, 7.f, 350.f), My_Vec3(200.f, 7.f, 480.f)));
	mLineVector.push_back(new LINE_DATA(My_Vec3(160.f, 7.f, 390.f), My_Vec3(160.f, 7.f, 480.f)));

	mLineVector.push_back(new LINE_DATA(My_Vec3(80.f, 7.f, 400.f), My_Vec3(160.f, 7.f, 400.f)));
	mLineVector.push_back(new LINE_DATA(My_Vec3(80.f, 7.f, 340.f), My_Vec3(160.f, 7.f, 340.f)));
	mLineVector.push_back(new LINE_DATA(My_Vec3(80.f, 7.f, 275.f), My_Vec3(160.f, 7.f, 275.f)));
	mLineVector.push_back(new LINE_DATA(My_Vec3(80.f, 7.f, 210.f), My_Vec3(160.f, 7.f, 210.f)));
	mLineVector.push_back(new LINE_DATA(My_Vec3(60.f, 7.f, 150.f), My_Vec3(160.f, 7.f, 150.f)));

	mLineVector.push_back(new LINE_DATA(My_Vec3(150.f, 7.f, 80.f), My_Vec3(150.f, 7.f, 150.f)));
	mLineVector.push_back(new LINE_DATA(My_Vec3(210.f, 7.f, 80.f), My_Vec3(210.f, 7.f, 150.f)));
	mLineVector.push_back(new LINE_DATA(My_Vec3(270.f, 7.f, 80.f), My_Vec3(270.f, 7.f, 150.f)));
	mLineVector.push_back(new LINE_DATA(My_Vec3(330.f, 7.f, 80.f), My_Vec3(330.f, 7.f, 150.f)));
	mLineVector.push_back(new LINE_DATA(My_Vec3(390.f, 7.f, 80.f), My_Vec3(390.f, 7.f, 150.f)));

	mLineVector.push_back(new LINE_DATA(My_Vec3(397.f, 7.f, 150.f), My_Vec3(440.f, 7.f, 150.f)));
	mLineVector.push_back(new LINE_DATA(My_Vec3(397.f, 7.f, 240.f), My_Vec3(440.f, 7.f, 240.f)));
	mLineVector.push_back(new LINE_DATA(My_Vec3(397.f, 7.f, 285.f), My_Vec3(440.f, 7.f, 285.f)));


	mRanking.push_back(BAZZI);
	mRanking.push_back(DAO);
	mRanking.push_back(DIZNI);

	mLaps.emplace(1, BAZZI);
	mLaps.emplace(1, DAO);
	mLaps.emplace(1, DIZNI);

	for (My_uInt i = 0; i < MAX_RACER; ++i)
		mIsLapsCol.push_back(true);

	for (My_uInt i = 0; i < MAX_RACER; ++i)
		mColTime.push_back(-10.f);

	mStartLineMinPos = { 397.f, 0.f, 293.f };
	mStartLineMaxPos = { 440.f, 50.f, 295.f };

	// 컴포넌트 추가
	mCollisioner = (CCollisioner*)CComponentMgr::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Collisioner");
	if (mCollisioner == nullptr)
		return E_FAIL;

	return S_OK;
}

My_Int CRankingManager::Update(const My_Float & fTimeDelta)
{
	if (mIsStart)
		ComputeLaps(fTimeDelta);
	
	ComputeRanking(fTimeDelta);
	return My_Int();
}

My_Int CRankingManager::GetLaps()
{
	MAP_LAPS::iterator begin = mLaps.begin();
	MAP_LAPS::iterator end = mLaps.end();

	for (; begin != end; ++begin)
	{
		if (begin->second == BAZZI)
			return begin->first;
	}

	return My_Int();
}

CRankingManager::Racer CRankingManager::GetFrontRacer(Racer racer)
{
	My_Int frontIndex = -1;

	for (My_Int i = 0; i < MAX_RACER - 1; ++i)
	{
		if (racer == mRanking[i])
			break;

		frontIndex = i;
	}

	if (-1 == frontIndex)
		return RACER_END;

	return mRanking[frontIndex];
}

My_Int CRankingManager::GetRank(Racer racer)
{
	for (int i = 0; i < MAX_RACER; ++i)
	{
		if (mFinalRanking[i] == racer)
			return i;
	}

	return Racer();
}

void CRankingManager::SetTransform(CTransform * bazzi, CTransform * dao, CTransform * dizni)
{
	mTransformVec.push_back(bazzi);
	mTransformVec.push_back(dao);
	mTransformVec.push_back(dizni);
}

void CRankingManager::SetCheet()
{
	mFinalRanking.push_back(BAZZI);
	mFinalRanking.push_back(DAO);
	mFinalRanking.push_back(DIZNI);
}

void CRankingManager::ComputeLaps(const My_Float & fTimeDelta)
{
	if (MAX_RACER == mFinalRanking.size())
		return;

	const CGameObject* object = CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_EndCountingUI", 0);
	
	if (nullptr == object || const_cast<CEndCountingUI*>(static_cast<const CEndCountingUI*>(object))->GetIsEnd())
	{
		My_Int extra = mRanking.size() - mFinalRanking.size();
		My_Int bazzi = 0, dao = 0, dizni = 0;

		for (My_Int i = 0; i < mFinalRanking.size(); ++i)
		{
			if (mFinalRanking[i] == BAZZI)
				++bazzi;

			else if (mFinalRanking[i] == DAO)
				++dao;

			else if (mFinalRanking[i] == DIZNI)
				++dizni;
		}

		if (1 == extra)
		{
			if (0 == bazzi)
				mFinalRanking.push_back(BAZZI);

			else if (0 == dao)
				mFinalRanking.push_back(DAO);

			else if (0 == dizni)
				mFinalRanking.push_back(DIZNI);
		}

		else if (2 == extra)
		{
			if (1 == bazzi)
			{
				mFinalRanking.push_back(DAO);
				mFinalRanking.push_back(DIZNI);
			}

			else if (1 == dao)
			{
				mFinalRanking.push_back(BAZZI);
				mFinalRanking.push_back(DIZNI);
			}

			else if (1 == dizni)
			{
				mFinalRanking.push_back(BAZZI);
				mFinalRanking.push_back(DAO);
			}
		}

		return;
	}

	if (1 == mState || 2 == mState || 3 == mState)
	{
		My_uInt index = mState - 1;

		if (!mIsLapsCol[index])
		{
			MAP_LAPS::iterator begin = mLaps.begin();
			MAP_LAPS::iterator end = mLaps.end();

			for (; begin != end; ++begin)
			{
				if (begin->second == Racer(index))
				{
					My_uInt labs = begin->first;

					if (4 == labs + 1)
					{
						if (0 == mFinalRanking.size())
							const_cast<CEndCountingUI*>(static_cast<const CEndCountingUI*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_EndCountingUI", 0)))->SetIsStart();

						mFinalRanking.push_back(begin->second);

						if (BAZZI == begin->second)
						{
							const_cast<CBazzi*>(static_cast<const CBazzi*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Bazzi", 0)))->SetIsGoal();
							const_cast<CEndCountingUI*>(static_cast<const CEndCountingUI*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_EndCountingUI", 0)))->SetIsGoal();
							const_cast<CCamera_Target*>(static_cast<const CCamera_Target*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Camera_Target", 0)))->SetState(CCamera_Target::GOAL);
							CSoundMgr::GetInstance()->PlayBGM(L"Goal.MP3");
						}

						else if (DIZNI == begin->second)
							const_cast<CDizni*>(static_cast<const CDizni*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dizni", 0)))->SetIsGoal();

						else if (DAO == begin->second)
							const_cast<CDao*>(static_cast<const CDao*>(CGameObjectMgr::GetInstance()->Get_GameObject(SCENE_STAGE, L"Object_Dao", 0)))->SetIsGoal();


						if (3 == mFinalRanking.size())
						{
							for (My_Int i = 0; i < MAX_RACER; ++i)
								mRanking[i] = mFinalRanking[i];
						}
					}

					else
					{
						Racer racer = begin->second;
						mLaps.erase(begin);
						mLaps.emplace(++labs, racer);
					}
					break;
				}
			}

			mIsLapsCol[index] = true;
		}

		mState = 0;
	}

	for (My_uInt i = 0; i < MAX_RACER; ++i)
	{
		if (mIsLapsCol[i])
			mColTime[i] += fTimeDelta;

		if (mColTime[i] > 15.f)
		{
			mIsLapsCol[i] = false;
			mColTime[i] = 0.f;
		}
	}

	mCollisioner->Add_CollisionAABB(mStartLineMinPos, mStartLineMaxPos, 4, &mState, nullptr);
}

void CRankingManager::ComputeRanking(const My_Float & fTimeDelta)
{
	if (3 == mFinalRanking.size())
		return;

	// Laps 수 비교
	MAP_LAPS::iterator begin = mLaps.begin();
	MAP_LAPS::iterator end = mLaps.end();

	for (My_uInt i = 0; begin != end; ++begin, ++i)
		mRanking[i] = begin->second;

	// Line 비교
	My_Bool		isSameFirstSecond = false, isSameSecondThird = false;
	begin = mLaps.begin();
	Racer		first, second, third;
	My_Int		firstLabs, secondLabs, thirdLabs;

	firstLabs = begin->first;
	first = begin->second;
	++begin;
	secondLabs = begin->first;
	second = begin->second;
	++begin;
	thirdLabs = begin->first;
	third = begin->second;

	if (firstLabs == secondLabs)
		isSameFirstSecond = true;

	if (secondLabs == thirdLabs)
		isSameSecondThird = true;

	multimap<My_Int, Racer, greater<My_Int>>	rankTemp;

	if (isSameFirstSecond && isSameSecondThird)
	{
		My_Int lineNum1 = ComputeNearestLineIndex(first);
		My_Int lineNum2 = ComputeNearestLineIndex(second);
		My_Int lineNum3 = ComputeNearestLineIndex(third);

		if (lineNum1 == lineNum2)
		{
			const My_Vec3* vecFirstPos = mTransformVec[first]->Get_Information(CTransform::INFO_POSITION);
			const My_Vec3* vecSecPos = mTransformVec[second]->Get_Information(CTransform::INFO_POSITION);

			My_Vec3 vecDir = *vecFirstPos - *vecSecPos;
			const My_Vec3* vecLook = mTransformVec[second]->Get_Information(CTransform::INFO_LOOK);
			My_Vec3 vecLookReal = *vecLook;

			D3DXVec3Normalize(&vecDir, &vecDir);
			D3DXVec3Normalize(&vecLookReal, &vecLookReal);

			My_Float angle = D3DXToDegree(acosf(D3DXVec3Dot(&vecDir, &vecLookReal)));

			if (angle < 90.f) // first > second
			{
				if (lineNum2 == lineNum3)
				{
					const My_Vec3* vecThirdPos = mTransformVec[third]->Get_Information(CTransform::INFO_POSITION);

					vecDir = *vecThirdPos - *vecSecPos;
					D3DXVec3Normalize(&vecDir, &vecDir);

					angle = D3DXToDegree(acosf(D3DXVec3Dot(&vecDir, &vecLookReal)));

					if (angle < 90.f) // third가 더큼
					{
						My_Float length1 = D3DXVec3Length(&(*vecThirdPos - *vecSecPos));
						My_Float length2 = D3DXVec3Length(&(*vecFirstPos - *vecSecPos));

						if (length1 > length2)
						{
							rankTemp.emplace(lineNum3, third);
							rankTemp.emplace(lineNum1, first);
							rankTemp.emplace(lineNum2, second);
						}

						else
						{
							rankTemp.emplace(lineNum1, first);
							rankTemp.emplace(lineNum3, third);
							rankTemp.emplace(lineNum2, second);
						}
					}

					else
					{
						rankTemp.emplace(lineNum1, first);
						rankTemp.emplace(lineNum2, second);
						rankTemp.emplace(lineNum3, third);
					}
				}

				else if (lineNum2 > lineNum3)
				{
					rankTemp.emplace(lineNum1, first);
					rankTemp.emplace(lineNum2, second);
					rankTemp.emplace(lineNum3, third);
				}

				else if (lineNum3 > lineNum1)
				{
					rankTemp.emplace(lineNum3, third);
					rankTemp.emplace(lineNum1, first);
					rankTemp.emplace(lineNum2, second);
				}

				else
				{
					rankTemp.emplace(lineNum1, first);
					rankTemp.emplace(lineNum3, third);
					rankTemp.emplace(lineNum2, second);
				}
			}

			else // second > first
			{
				if (lineNum1 == lineNum3)
				{
					const My_Vec3* vecThirdPos = mTransformVec[third]->Get_Information(CTransform::INFO_POSITION);
					const My_Vec3* vecLookFirst = mTransformVec[first]->Get_Information(CTransform::INFO_LOOK);
					My_Vec3 vecLookReal = *vecLook;

					vecDir = *vecThirdPos - *vecFirstPos;

					D3DXVec3Normalize(&vecDir, &vecDir);
					D3DXVec3Normalize(&vecLookReal, &vecLookReal);

					angle = D3DXToDegree(acosf(D3DXVec3Dot(&vecDir, &vecLookReal)));

					if (angle < 90.f) // third가 더 큼
					{
						My_Float length1 = D3DXVec3Length(&(*vecThirdPos - *vecFirstPos));
						My_Float length2 = D3DXVec3Length(&(*vecSecPos - *vecFirstPos));

						if (length1 > length2)
						{
							rankTemp.emplace(lineNum3, third);
							rankTemp.emplace(lineNum2, second);
							rankTemp.emplace(lineNum1, first);
						}

						else
						{
							rankTemp.emplace(lineNum2, second);
							rankTemp.emplace(lineNum3, third);
							rankTemp.emplace(lineNum1, first);
						}
					}

					else
					{
						rankTemp.emplace(lineNum2, second);
						rankTemp.emplace(lineNum1, first);
						rankTemp.emplace(lineNum3, third);
					}
				}

				else if (lineNum1 > lineNum3)
				{
					rankTemp.emplace(lineNum2, second);
					rankTemp.emplace(lineNum1, first);
					rankTemp.emplace(lineNum3, third);
				}

				else if (lineNum3 > lineNum2)
				{
					rankTemp.emplace(lineNum3, third);
					rankTemp.emplace(lineNum2, second);
					rankTemp.emplace(lineNum1, first);
				}

				else
				{
					rankTemp.emplace(lineNum2, second);
					rankTemp.emplace(lineNum3, third);
					rankTemp.emplace(lineNum1, first);
				}
			}
		}

		else if (lineNum1 == lineNum3)
		{
			const My_Vec3* vecFirstPos = mTransformVec[first]->Get_Information(CTransform::INFO_POSITION);
			const My_Vec3* vecThirdPos = mTransformVec[third]->Get_Information(CTransform::INFO_POSITION);

			My_Vec3 vecDir = *vecFirstPos - *vecThirdPos;
			const My_Vec3* vecLook = mTransformVec[third]->Get_Information(CTransform::INFO_LOOK);
			My_Vec3 vecLookReal = *vecLook;

			D3DXVec3Normalize(&vecDir, &vecDir);
			D3DXVec3Normalize(&vecLookReal, &vecLookReal);

			My_Float angle = D3DXToDegree(acosf(D3DXVec3Dot(&vecDir, &vecLookReal)));

			if (angle < 90.f) // first > third
			{
				if (lineNum2 == lineNum3)
				{
					const My_Vec3* vecSecondPos = mTransformVec[second]->Get_Information(CTransform::INFO_POSITION);

					vecDir = *vecSecondPos - *vecThirdPos;
					D3DXVec3Normalize(&vecDir, &vecDir);

					angle = D3DXToDegree(acosf(D3DXVec3Dot(&vecDir, &vecLookReal)));

					if (angle < 90.f) // second가 더큼
					{
						My_Float length1 = D3DXVec3Length(&(*vecFirstPos - *vecThirdPos));
						My_Float length2 = D3DXVec3Length(&(*vecSecondPos - *vecThirdPos));

						if (length1 > length2)
						{
							rankTemp.emplace(lineNum1, first);
							rankTemp.emplace(lineNum2, second);
							rankTemp.emplace(lineNum3, third);
						}

						else
						{
							rankTemp.emplace(lineNum2, second);
							rankTemp.emplace(lineNum1, first);
							rankTemp.emplace(lineNum3, third);
						}
					}

					else
					{
						rankTemp.emplace(lineNum1, first);
						rankTemp.emplace(lineNum3, third);
						rankTemp.emplace(lineNum2, second);
					}
				}

				else if (lineNum2 > lineNum1)
				{
					rankTemp.emplace(lineNum2, second);
					rankTemp.emplace(lineNum1, first);
					rankTemp.emplace(lineNum3, third);
				}

				else if (lineNum3 > lineNum2)
				{
					rankTemp.emplace(lineNum1, first);
					rankTemp.emplace(lineNum3, third);
					rankTemp.emplace(lineNum2, second);
				}

				else
				{
					rankTemp.emplace(lineNum1, first);
					rankTemp.emplace(lineNum2, second);
					rankTemp.emplace(lineNum3, third);
				}
			}

			else // third > first
			{
				if (lineNum1 == lineNum2)
				{
					const My_Vec3* vecSecondPos = mTransformVec[second]->Get_Information(CTransform::INFO_POSITION);
					const My_Vec3* vecLookFirst = mTransformVec[first]->Get_Information(CTransform::INFO_LOOK);
					My_Vec3 vecLookReal = *vecLook;

					vecDir = *vecSecondPos - *vecFirstPos;

					D3DXVec3Normalize(&vecDir, &vecDir);
					D3DXVec3Normalize(&vecLookReal, &vecLookReal);

					angle = D3DXToDegree(acosf(D3DXVec3Dot(&vecDir, &vecLookReal)));

					if (angle < 90.f) // second가 더 큼
					{
						My_Float length1 = D3DXVec3Length(&(*vecThirdPos - *vecFirstPos));
						My_Float length2 = D3DXVec3Length(&(*vecSecondPos - *vecFirstPos));

						if (length1 > length2)
						{
							rankTemp.emplace(lineNum3, third);
							rankTemp.emplace(lineNum2, second);
							rankTemp.emplace(lineNum1, first);
						}

						else
						{
							rankTemp.emplace(lineNum2, second);
							rankTemp.emplace(lineNum3, third);
							rankTemp.emplace(lineNum1, first);
						}
					}

					else
					{
						rankTemp.emplace(lineNum3, third);
						rankTemp.emplace(lineNum1, first);
						rankTemp.emplace(lineNum2, second);
					}
				}

				else if (lineNum2 > lineNum3)
				{
					rankTemp.emplace(lineNum2, second);
					rankTemp.emplace(lineNum3, third);
					rankTemp.emplace(lineNum1, first);
				}

				else if (lineNum1 > lineNum2)
				{
					rankTemp.emplace(lineNum3, third);
					rankTemp.emplace(lineNum1, first);
					rankTemp.emplace(lineNum2, second);
				}

				else
				{
					rankTemp.emplace(lineNum3, third);
					rankTemp.emplace(lineNum2, second);
					rankTemp.emplace(lineNum1, first);
				}
			}
		}

		else if (lineNum2 == lineNum3)
		{
			const My_Vec3* vecThirdPos = mTransformVec[third]->Get_Information(CTransform::INFO_POSITION);
			const My_Vec3* vecSecPos = mTransformVec[second]->Get_Information(CTransform::INFO_POSITION);

			My_Vec3 vecDir = *vecThirdPos - *vecSecPos;
			const My_Vec3* vecLook = mTransformVec[second]->Get_Information(CTransform::INFO_LOOK);
			My_Vec3 vecLookReal = *vecLook;

			D3DXVec3Normalize(&vecDir, &vecDir);
			D3DXVec3Normalize(&vecLookReal, &vecLookReal);

			My_Float angle = D3DXToDegree(acosf(D3DXVec3Dot(&vecDir, &vecLookReal)));

			if (angle < 90.f) // third > second
			{
				if (lineNum2 == lineNum1)
				{
					const My_Vec3* vecFirstPos = mTransformVec[first]->Get_Information(CTransform::INFO_POSITION);

					vecDir = *vecFirstPos - *vecSecPos;
					D3DXVec3Normalize(&vecDir, &vecDir);

					angle = D3DXToDegree(acosf(D3DXVec3Dot(&vecDir, &vecLookReal)));

					if (angle < 90.f) // first가 더큼
					{
						My_Float length1 = D3DXVec3Length(&(*vecThirdPos - *vecSecPos));
						My_Float length2 = D3DXVec3Length(&(*vecFirstPos - *vecSecPos));

						if (length1 > length2)
						{
							rankTemp.emplace(lineNum3, third);
							rankTemp.emplace(lineNum1, first);
							rankTemp.emplace(lineNum2, second);
						}

						else
						{
							rankTemp.emplace(lineNum1, first);
							rankTemp.emplace(lineNum3, third);
							rankTemp.emplace(lineNum2, second);
						}
					}

					else
					{
						rankTemp.emplace(lineNum3, third);
						rankTemp.emplace(lineNum2, second);
						rankTemp.emplace(lineNum1, first);
					}
				}

				else if (lineNum1 > lineNum3)
				{
					rankTemp.emplace(lineNum1, first);
					rankTemp.emplace(lineNum3, third);
					rankTemp.emplace(lineNum2, second);
				}

				else if (lineNum2 > lineNum1)
				{
					rankTemp.emplace(lineNum3, third);
					rankTemp.emplace(lineNum2, second);
					rankTemp.emplace(lineNum1, first);
				}

				else
				{
					rankTemp.emplace(lineNum3, third);
					rankTemp.emplace(lineNum1, first);
					rankTemp.emplace(lineNum2, second);
				}
			}

			else // second > third
			{
				if (lineNum1 == lineNum3)
				{
					const My_Vec3* vecFirstPos = mTransformVec[first]->Get_Information(CTransform::INFO_POSITION);
					const My_Vec3* vecLookFirst = mTransformVec[first]->Get_Information(CTransform::INFO_LOOK);
					My_Vec3 vecLookReal = *vecLook;

					vecDir = *vecThirdPos - *vecFirstPos;

					D3DXVec3Normalize(&vecDir, &vecDir);
					D3DXVec3Normalize(&vecLookReal, &vecLookReal);

					angle = D3DXToDegree(acosf(D3DXVec3Dot(&vecDir, &vecLookReal)));

					if (angle < 90.f)
					{
						rankTemp.emplace(lineNum2, second);
						rankTemp.emplace(lineNum3, third);
						rankTemp.emplace(lineNum1, first);
					}

					else // first가 더큼
					{
						My_Float length1 = D3DXVec3Length(&(*vecSecPos - *vecThirdPos));
						My_Float length2 = D3DXVec3Length(&(*vecFirstPos - *vecThirdPos));

						if (length1 > length2)
						{
							rankTemp.emplace(lineNum2, second);
							rankTemp.emplace(lineNum1, first);
							rankTemp.emplace(lineNum3, third);
						}

						else
						{
							rankTemp.emplace(lineNum1, first);
							rankTemp.emplace(lineNum2, second);
							rankTemp.emplace(lineNum3, third);
						}
					}
				}

				else if (lineNum1 > lineNum2)
				{
					rankTemp.emplace(lineNum1, first);
					rankTemp.emplace(lineNum2, second);
					rankTemp.emplace(lineNum3, third);
				}

				else if (lineNum3 > lineNum1)
				{
					rankTemp.emplace(lineNum2, second);
					rankTemp.emplace(lineNum3, third);
					rankTemp.emplace(lineNum1, first);
				}

				else
				{
					rankTemp.emplace(lineNum2, second);
					rankTemp.emplace(lineNum1, first);
					rankTemp.emplace(lineNum3, third);
				}
			}
		}

		else
		{
			rankTemp.emplace(lineNum1, first);
			rankTemp.emplace(lineNum2, second);
			rankTemp.emplace(lineNum3, third);
		}

		MAP_LAPS::iterator begin = rankTemp.begin();
		MAP_LAPS::iterator end = rankTemp.end();

		for (My_uInt i = 0; begin != end; ++begin, ++i)
			mRanking[i] = begin->second;
	}

	else if (!isSameFirstSecond && isSameSecondThird)
	{
		My_Int lineNum1 = ComputeNearestLineIndex(second);
		My_Int lineNum2 = ComputeNearestLineIndex(third);

		if (lineNum1 == lineNum2)
		{
			const My_Vec3* vecSecPos = mTransformVec[second]->Get_Information(CTransform::INFO_POSITION);
			const My_Vec3* vecThirdPos = mTransformVec[third]->Get_Information(CTransform::INFO_POSITION);

			My_Vec3 vecDir = *vecSecPos - *vecThirdPos;
			const My_Vec3* vecLook = mTransformVec[third]->Get_Information(CTransform::INFO_LOOK);
			My_Vec3 vecLookReal = *vecLook;

			D3DXVec3Normalize(&vecDir, &vecDir);
			D3DXVec3Normalize(&vecLookReal, &vecLookReal);

			My_Float angle = D3DXToDegree(acosf(D3DXVec3Dot(&vecDir, &vecLookReal)));

			if (angle < 90.f)
			{
				rankTemp.emplace(lineNum1, second);
				rankTemp.emplace(lineNum2, third);
			}

			else
			{
				rankTemp.emplace(lineNum2, third);
				rankTemp.emplace(lineNum1, second);
			}
		}

		else
		{
			rankTemp.emplace(lineNum1, second);
			rankTemp.emplace(lineNum2, third);
		}

		MAP_LAPS::iterator begin = rankTemp.begin();
		MAP_LAPS::iterator end = rankTemp.end();

		for (My_uInt i = 1; i < 3 && begin != end; ++begin, ++i)
			mRanking[i] = begin->second;
	}

	else if (isSameFirstSecond && !isSameSecondThird)
	{
		My_Int lineNum1 = ComputeNearestLineIndex(first);
		My_Int lineNum2 = ComputeNearestLineIndex(second);

		if (lineNum1 == lineNum2)
		{
			const My_Vec3* vecFirstPos = mTransformVec[first]->Get_Information(CTransform::INFO_POSITION);
			const My_Vec3* vecSecPos = mTransformVec[second]->Get_Information(CTransform::INFO_POSITION);

			My_Vec3 vecDir = *vecFirstPos - *vecSecPos;
			const My_Vec3* vecLook = mTransformVec[second]->Get_Information(CTransform::INFO_LOOK);
			My_Vec3 vecLookReal = *vecLook;

			D3DXVec3Normalize(&vecDir, &vecDir);
			D3DXVec3Normalize(&vecLookReal, &vecLookReal);

			My_Float angle = D3DXToDegree(acosf(D3DXVec3Dot(&vecDir, &vecLookReal)));

			if (angle < 90.f)
			{
				rankTemp.emplace(lineNum1, first);
				rankTemp.emplace(lineNum2, second);
			}

			else
			{
				rankTemp.emplace(lineNum2, second);
				rankTemp.emplace(lineNum1, first);
			}
		}

		else
		{
			rankTemp.emplace(lineNum1, first);
			rankTemp.emplace(lineNum2, second);
		}

		MAP_LAPS::iterator begin = rankTemp.begin();
		MAP_LAPS::iterator end = rankTemp.end();

		for (My_uInt i = 0; i < 2 && begin != end; ++begin, ++i)
			mRanking[i] = begin->second;
	}
}

My_Int CRankingManager::ComputeNearestLineIndex(Racer racer)
{
	My_Float	minDistance = 999.f;
	My_uInt		minIndex = 0;

	const My_Vec3* vecPos = mTransformVec[racer]->Get_Information(CTransform::INFO_POSITION);

	if (397.f < vecPos->x && vecPos->x < 440.f && 0 < vecPos->z && vecPos->z < 480.f) // 0 ~ 1 , 18 19
	{
		for (My_uLong i = 0; i < 20; ++i)
		{
			if (i == 3)
				i = 18;

			My_Vec3 line = mLineVector[i]->startPos - mLineVector[i]->endPos;
			My_Vec3 pointVec = mLineVector[i]->startPos - *vecPos;

			D3DXVec3Normalize(&line, &line);

			My_Float base = D3DXVec3Dot(&line, &pointVec);
			line = base * line;

			My_Float distance = D3DXVec3Length(&(pointVec - line));

			if (minDistance > distance)
			{
				minDistance = distance;
				minIndex = i;
			}
		}
	}

	if (0.f < vecPos->x && vecPos->x < 450.f && 390.f < vecPos->z && vecPos->z < 480.f)
	{
		for (My_uLong i = 3; i < 8; ++i)
		{
			My_Vec3 line = mLineVector[i]->startPos - mLineVector[i]->endPos;
			My_Vec3 pointVec = mLineVector[i]->startPos - *vecPos;

			D3DXVec3Normalize(&line, &line);

			My_Float base = D3DXVec3Dot(&line, &pointVec);
			line = base * line;

			My_Float distance = D3DXVec3Length(&(pointVec - line));

			if (minDistance > distance)
			{
				minDistance = distance;
				minIndex = i;
			}
		}
	}

	if (0.f < vecPos->x && vecPos->x < 160.f && 0.f < vecPos->z && vecPos->z < 480.f)
	{
		for (My_uLong i = 8; i < 13; ++i)
		{
			My_Vec3 line = mLineVector[i]->startPos - mLineVector[i]->endPos;
			My_Vec3 pointVec = mLineVector[i]->startPos - *vecPos;

			D3DXVec3Normalize(&line, &line);

			My_Float base = D3DXVec3Dot(&line, &pointVec);
			line = base * line;

			My_Float distance = D3DXVec3Length(&(pointVec - line));

			if (minDistance > distance)
			{
				minDistance = distance;
				minIndex = i;
			}
		}
	}

	if (0.f < vecPos->x && vecPos->x < 450.f && 0 < vecPos->z && vecPos->z < 150.f)
	{
		for (My_uLong i = 13; i < 18; ++i)
		{
			My_Vec3 line = mLineVector[i]->startPos - mLineVector[i]->endPos;
			My_Vec3 pointVec = mLineVector[i]->startPos - *vecPos;

			D3DXVec3Normalize(&line, &line);

			My_Float base = D3DXVec3Dot(&line, &pointVec);
			line = base * line;

			My_Float distance = D3DXVec3Length(&(pointVec - line));

			if (minDistance > distance)
			{
				minDistance = distance;
				minIndex = i;
			}
		}
	}

	return minIndex;
}

My_uLong CRankingManager::ReleaseMemory(void)
{
	for (auto& line : mLineVector)
		Safe_Delete(line);

	mLineVector.clear();
	mRanking.clear();
	mLaps.clear();
	mIsLapsCol.clear();
	mColTime.clear();
	mTransformVec.clear();

	Safe_Release(mCollisioner);

	return My_uLong();
}
