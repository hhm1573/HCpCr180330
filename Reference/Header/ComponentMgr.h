#pragma once

#include"Base.h"

#include "VIBuffer.h"

#include "TriCol.h"
#include "RectCol.h"
#include "RectTex.h"
#include "CubeCol.h"
#include "CubeTex.h"
#include "Terrain_Buffer.h"
#include "ViewPort_Buffer.h"
#include "Sphere_Buffer.h"

#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"

#include "Vehicle.h"
#include "NavMesh.h"
#include "Collisioner.h"

BEGIN(Engine)

class CComponent;

class ENGINE_DLL CComponentMgr final
	:public CBase
{
	DECLARE_SINGLETON(CComponentMgr)

private:
	explicit CComponentMgr();
	virtual ~CComponentMgr() = default;

private:
	map<const My_TChar*, CComponent*>*				m_pComponents = nullptr;
	map<const My_TChar*, CComponent*>*				m_pComponents_Dependent = nullptr;
	typedef	map<const My_TChar*, CComponent*>		MAPCOMPONENT;
private:
	My_uLong										m_dwSize = 0;

public:
	HRESULT			Reserve(const My_uLong& dwNum);
	HRESULT			Add_Component(const My_uLong& dwSceneID, const My_TChar* szKey, CComponent* pData);
	HRESULT			Add_Dependent_Component(const My_uLong& dwSceneID, const My_TChar* szKey, CComponent* pData);
	CComponent*		Clone_Component(const My_uLong& dwSceneID, const My_TChar* szKey);
	void			Clear_Component(const My_uLong& dwSceneID);
private:
	CComponent*		Find_Component(const My_uLong& dwSceneID, const My_TChar* szKey);
	CComponent*		Find_Dependent_Component(const My_uLong& dwSceneID, const My_TChar* szKey);

protected:
	virtual My_uLong	ReleaseMemory(void);
};

END