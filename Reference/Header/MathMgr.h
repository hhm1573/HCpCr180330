#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CMathMgr
{
private:
	explicit CMathMgr();
	~CMathMgr() = default;

public:
	static My_Bool IntersectLine(const POINT& ptA0, const POINT& ptA1, const POINT& ptB0, const POINT& ptB1, POINT* pPtContainer);
	static My_Bool IntersectLine(const POINTFLOAT& ptA0, const POINTFLOAT& ptA1, const POINTFLOAT& ptB0, const POINTFLOAT& ptB1, POINTFLOAT* pPtContainer);
};

END
