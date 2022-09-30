#pragma once

#include "Component.h"

namespace Engine {
	class ENGINE_DLL CNavMesh final
		:public CComponent
	{
	private:
		explicit CNavMesh(LPDIRECT3DDEVICE9 pDevice);
		explicit CNavMesh(const CNavMesh& Ref);
		virtual ~CNavMesh() = default;

	private:
		list<NAVMESH*>		m_List_NavMesh;

	public:
		HRESULT				Initialize(const My_TChar* szPath);
	public:
		NAVMESH*			Get_FromPos(const My_Vec3& vPos);
		NAVMESH*			Get_First();
		NAVMESH*			Get_Last();
		list<NAVMESH*>*		Get_List() { return &m_List_NavMesh; }
		//void				Copy_List(list<NAVMESH*>* pList);

	public:
		static CNavMesh*	Create(LPDIRECT3DDEVICE9 pDevice, const My_TChar* szPath);
		virtual CComponent*	Clone();
	protected:
		virtual My_uLong	ReleaseMemory();
	};
}