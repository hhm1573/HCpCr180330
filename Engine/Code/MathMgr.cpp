#include "..\Header\MathMgr.h"

USING(Engine)

CMathMgr::CMathMgr()
{
}

My_Bool CMathMgr::IntersectLine(const POINT & ptA0, const POINT & ptA1, const POINT & ptB0, const POINT & ptB1, POINT * pPtContainer)
{
	double t;
	double s;
	double under = (ptB1.y - ptB0.y)*(ptA1.x - ptA0.x) - (ptB1.x - ptB0.x)*(ptA1.y - ptA0.y);
	if (under == 0) return false;

	double _t = (ptB1.x - ptB0.x)*(ptA0.y - ptB0.y) - (ptB1.y - ptB0.y)*(ptA0.x - ptB0.x);
	double _s = (ptA1.x - ptA0.x)*(ptA0.y - ptB0.y) - (ptA1.y - ptA0.y)*(ptA0.x - ptB0.x);

	t = _t / under;
	s = _s / under;

	if (t<0.0 || t>1.0 || s<0.0 || s>1.0) return false;
	if (_t == 0 && _s == 0) return false;

	pPtContainer->x = ptA0.x + t * (double)(ptA1.x - ptA0.x);
	pPtContainer->y = ptA0.y + t * (double)(ptA1.y - ptA0.y);

	return true;
}

My_Bool CMathMgr::IntersectLine(const POINTFLOAT & ptA0, const POINTFLOAT & ptA1, const POINTFLOAT & ptB0, const POINTFLOAT & ptB1, POINTFLOAT * pPtContainer)
{
	double t;
	double s;
	double under = (ptB1.y - ptB0.y)*(ptA1.x - ptA0.x) - (ptB1.x - ptB0.x)*(ptA1.y - ptA0.y);
	if (under == 0) return false;

	double _t = (ptB1.x - ptB0.x)*(ptA0.y - ptB0.y) - (ptB1.y - ptB0.y)*(ptA0.x - ptB0.x);
	double _s = (ptA1.x - ptA0.x)*(ptA0.y - ptB0.y) - (ptA1.y - ptA0.y)*(ptA0.x - ptB0.x);

	t = _t / under;
	s = _s / under;

	if (t<0.0 || t>1.0 || s<0.0 || s>1.0) return false;
	if (_t == 0 && _s == 0) return false;

	pPtContainer->x = ptA0.x + t * (double)(ptA1.x - ptA0.x);
	pPtContainer->y = ptA0.y + t * (double)(ptA1.y - ptA0.y);

	return true;
}
