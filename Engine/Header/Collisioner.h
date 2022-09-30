#pragma once

#include "Component.h"

namespace Engine {
	class ENGINE_DLL CCollisioner final
		:public CComponent
	{
	private:
		explicit CCollisioner(LPDIRECT3DDEVICE9 pDevice);
		explicit CCollisioner(const CCollisioner& Ref);
		virtual ~CCollisioner() = default;

	private:
		list<COL_SPHERE*>		m_List_pColSphere;
		list<COL_AABB*>			m_List_pAABB;

	public:
		void				Update(void);
		void				Add_Collision(const My_Vec3& vPos, const My_Float& fRadius, const My_Int& iTransmit, My_Int* piContainer);
		void				Add_CollisionAABB(const My_Vec3& vMin, const My_Vec3& vMax, const My_Int& iTransmit, My_Int* piReceiver, My_Vec3* pvIntersect);
	private:
		void				Update_AABB(void);

	public:
		static CCollisioner*		Create(LPDIRECT3DDEVICE9 pDevice);
		virtual CComponent*			Clone() override;
	protected:
		virtual My_uLong			ReleaseMemory() override;
	};
}