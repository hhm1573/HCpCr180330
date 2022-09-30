#include "..\Header\Collisioner.h"

Engine::CCollisioner::CCollisioner(LPDIRECT3DDEVICE9 pDevice)
	:CComponent(pDevice)
{
}

Engine::CCollisioner::CCollisioner(const CCollisioner & Ref)
	: CComponent(Ref)
{
}

void Engine::CCollisioner::Update(void)
{
	for (auto& pCol0 : m_List_pColSphere) {
		for (auto& pCol1 : m_List_pColSphere) {
			if (pCol0 == pCol1)
				continue;

			My_Vec3		vCenterDist = pCol0->vCenter - pCol1->vCenter;
			My_Float	fCenterDist = D3DXVec3Length(&vCenterDist);
			My_Float	fSumRadius = pCol0->fRadius + pCol1->fRadius;
			if (fCenterDist <= fSumRadius) {
				if (pCol0->piReceive != nullptr) {
					if (pCol0->fCol_CenterDist < 0 || pCol0->fCol_CenterDist > fCenterDist) {
						*(pCol0->piReceive) = pCol1->iTransmit;
						pCol0->fCol_CenterDist = fCenterDist;
					}
				}
				if (pCol1->piReceive != nullptr) {
					if (pCol1->fCol_CenterDist < 0 || pCol1->fCol_CenterDist > fCenterDist) {
						*(pCol1->piReceive) = pCol0->iTransmit;
						pCol1->fCol_CenterDist = fCenterDist;
					}
				}
			}
		}
	}

	for (auto& pCol : m_List_pColSphere) {
		if (pCol != nullptr) {
			delete pCol;
			pCol = nullptr;
		}
	}
	m_List_pColSphere.clear();

	Update_AABB();
}

void Engine::CCollisioner::Add_Collision(const My_Vec3 & vPos, const My_Float & fRadius, const My_Int & iTransmit, My_Int * piContainer)
{
	COL_SPHERE*		pCol = new COL_SPHERE;
	pCol->vCenter = vPos;
	pCol->fRadius = fRadius;
	pCol->piReceive = piContainer;
	pCol->iTransmit = iTransmit;
	pCol->fCol_CenterDist = -1.0f;
	m_List_pColSphere.push_back(pCol);
}

void Engine::CCollisioner::Add_CollisionAABB(const My_Vec3 & vMin, const My_Vec3 & vMax, const My_Int & iTransmit, My_Int * piReceiver, My_Vec3* pvIntersect)
{
	COL_AABB*		pCol = new COL_AABB;
	pCol->vMin = vMin;
	pCol->vMax = vMax;
	pCol->iTransmit = iTransmit;
	pCol->piReceive = piReceiver;
	pCol->pvIntersect = pvIntersect;
	m_List_pAABB.push_back(pCol);
}

void Engine::CCollisioner::Update_AABB(void)
{
	for (auto& pCol0 : m_List_pAABB) {
		for (auto& pCol1 : m_List_pAABB) {
			if (pCol0 == pCol1)
				continue;

			My_Float	fX_Min_Max = (pCol0->vMin.x >= pCol1->vMin.x) ? pCol0->vMin.x : pCol1->vMin.x;
			My_Float	fX_Max_Min = (pCol0->vMax.x <= pCol1->vMax.x) ? pCol0->vMax.x : pCol1->vMax.x;
			if (fX_Max_Min < fX_Min_Max)
				continue;
			My_Float	fXIntersect = fX_Min_Max - fX_Max_Min;

			My_Float	fY_Min_Max = (pCol0->vMin.y >= pCol1->vMin.y) ? pCol0->vMin.y : pCol1->vMin.y;
			My_Float	fY_Max_Min = (pCol0->vMax.y <= pCol1->vMax.y) ? pCol0->vMax.y : pCol1->vMax.y;
			if (fY_Max_Min < fY_Min_Max)
				continue;
			My_Float	fYIntersect = fY_Min_Max - fY_Max_Min;

			My_Float	fZ_Min_Max = (pCol0->vMin.z >= pCol1->vMin.z) ? pCol0->vMin.z : pCol1->vMin.z;
			My_Float	fZ_Max_Min = (pCol0->vMax.z <= pCol1->vMax.z) ? pCol0->vMax.z : pCol1->vMax.z;
			if (fZ_Max_Min < fZ_Min_Max)
				continue;
			My_Float	fZIntersect = fZ_Min_Max - fZ_Max_Min;

			My_Vec3		vIntersect = My_Vec3(fabs(fXIntersect), fabs(fYIntersect), fabs(fZIntersect));
			My_Vec3		vIntersect_Col0 = vIntersect;
			My_Vec3		vIntersect_Col1 = vIntersect;

			My_Vec3		vCenter_Col0 = (pCol0->vMin + pCol0->vMax) * 0.5f;
			My_Vec3		vCenter_Col1 = (pCol1->vMin + pCol1->vMax) * 0.5f;



			if (vCenter_Col0.x >= vCenter_Col1.x)
				vIntersect_Col1.x *= -1.0f;
			else
				vIntersect_Col0.x *= -1.0f;

			if (vCenter_Col0.y >= vCenter_Col1.y)
				vIntersect_Col1.y *= -1.0f;
			else
				vIntersect_Col0.y *= -1.0f;

			if (vCenter_Col0.z >= vCenter_Col1.z)
				vIntersect_Col1.z *= -1.0f;
			else
				vIntersect_Col0.z *= -1.0f;



			if (pCol0->iTransmit == 1 || pCol1->iTransmit == 1)
				int i = 0;

			if (pCol0->piReceive != nullptr) {
				*pCol0->piReceive = pCol1->iTransmit;
			}
			if (pCol1->piReceive != nullptr) {
				*pCol1->piReceive = pCol0->iTransmit;
			}

			if (pCol0->pvIntersect != nullptr)
				*pCol0->pvIntersect = vIntersect_Col0;
			if (pCol1->pvIntersect != nullptr)
				*pCol1->pvIntersect = vIntersect_Col1;
		}
	}

	for (auto& pCol : m_List_pAABB) {
		if (pCol != nullptr) {
			delete pCol;
			pCol = nullptr;
		}
	}
	m_List_pAABB.clear();
}

CCollisioner * Engine::CCollisioner::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCollisioner*		pInstance = new CCollisioner(pDevice);
	return pInstance;
}

CComponent * Engine::CCollisioner::Clone()
{
	AddRef();
	return this;
}

My_uLong Engine::CCollisioner::ReleaseMemory()
{
	for (auto& pColSphere : m_List_pColSphere) {
		if (pColSphere != nullptr) {
			delete pColSphere;
			pColSphere = nullptr;
		}
	}

	CComponent::ReleaseMemory();
	return My_uLong();
}
