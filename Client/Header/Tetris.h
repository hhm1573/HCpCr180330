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

	// ������ �־��� ����
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
	TetrisState			mState;				// ����

protected:
	My_Bool				mIsMoving;			// ���������� �����̴� ��Ʈ���� ����
	My_Float			mStandingTime;		// standing���� �ð� ����
	My_Vec3				mRespawnPos;		// Waiting�� �־��� pos��
	My_Int				mDownCount;			// ��� �������� ������ ����������
	My_Int				mCount;				// count
	My_Float			mDownHeight;		// ��ĭ ����
	My_Float			mSpeed;				// �ӵ�
	My_Float			mStandingHeight;	// Standing�� ����
	My_Int				mNumber;			// ȸ���Ⱦ� ������ ����(0�� ����Ʈ)

	My_Int				mReceive;

protected:
	virtual My_uLong			ReleaseMemory(void);
};

