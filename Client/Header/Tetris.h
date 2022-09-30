#pragma once

#include "Model.h"

namespace Engine
{
	class CCollisioner;
}

class CTetris abstract
	:public CModel
{
public:
	enum TetrisState { WATING, READY, GO_DOWN, STANDING, DISAPPEAR };

protected:
	explicit CTetris(LPDIRECT3DDEVICE9 pDevice);
	explicit CTetris(const CTetris& Ref);
	virtual ~CTetris() = default;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	TetrisState			GetState() { return mState; }
	My_Float			GetStandingTime() { return mStandingTime; }
	My_Float			GetDownHeight() { return mDownHeight; }

public:
	void SetState(TetrisState state) { mState = state; }

	// 생성시 넣어줄 값들
	void SetIsMoving(My_Bool flag) { mIsMoving = flag; }
	void SetRespawnPos(const My_Vec3 pos) { mRespawnPos = pos; }
	void SetDownCount(My_Int count) { mDownCount = count; }
	void SetStandingHeight(My_Float height) { mStandingHeight = height; }
	void SetAngle(My_Float y, My_Float z);
	void SetScale();
	void SetNumber(My_Int number) { mNumber = number; }

protected:
	CCollisioner*		mCollisioner;

protected:
	TetrisState			mState;				// 상태

protected:
	My_Bool				mIsMoving;			// 조형물인지 움직이는 테트리스 인지
	My_Float			mStandingTime;		// standing상태 시간 누적
	My_Vec3				mRespawnPos;		// Waiting때 넣어줄 pos값
	My_Int				mDownCount;			// 몇번 내려가야 완전히 없어지는지
	My_Int				mCount;				// count
	My_Float			mDownHeight;		// 한칸 높이
	My_Float			mSpeed;				// 속도
	My_Float			mStandingHeight;	// Standing할 높이
	My_Int				mNumber;			// 회전된애 구분할 숫자(0이 디폴트)

	My_Int				mReceive;

protected:
	virtual My_uLong			ReleaseMemory(void);
};

