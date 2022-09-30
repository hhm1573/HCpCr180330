#pragma once

#include "Camera.h"

class CCamera_Action final
	:public CCamera
{
	enum ActionState { START_POINT, ROT1, ROT_HOUSE, ROT2, TETRIS, ROT3, CROCDILE, ROT4, END_POINT };

private:
	explicit CCamera_Action(LPDIRECT3DDEVICE9 pDevice);
	explicit CCamera_Action(const CCamera_Action& Ref);
	virtual ~CCamera_Action() = default;

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Prototype(const CAMERA_DESC& pCameraDesc, const PROJ_DESC& pProjDesc);

public:
	virtual My_Int	Update(const My_Float& fTimeDelta);
	virtual My_Int	LastUpdate(const My_Float& fTimeDelta);

public:
	void			SetIsStage(My_Bool flag) { mIsStage = flag; }

private:
	My_Bool			mIsStage;
	ActionState		mState = START_POINT;
	My_Float		mAngle = 0.f;

public:
	static CCamera_Action*		Create(LPDIRECT3DDEVICE9 pDevice, const CAMERA_DESC& pCameraDesc, const PROJ_DESC& pProjDesc);
	virtual CGameObject*		Clone();
protected:
	virtual My_uLong			ReleaseMemory();
};

