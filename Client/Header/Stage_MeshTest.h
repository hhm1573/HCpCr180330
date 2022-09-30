#pragma once

#include "GameObject.h"

namespace Engine {
	class CMyMesh;
	class CTransform;
	class CRenderer;
}

class CStage_MeshTest
	:public CGameObject
{
private:
	explicit CStage_MeshTest(LPDIRECT3DDEVICE9 pDevice);
	explicit CStage_MeshTest(const CStage_MeshTest& Ref);
	virtual ~CStage_MeshTest(void) = default;

private:
	CMyMesh*			m_pCom_Mesh = nullptr;
	int					m_iCubeCnt = 0;
	CTransform*			m_pCom_Transform = nullptr;
	CRenderer*			m_pCom_Renderer = nullptr;

public:
	HRESULT				Initialize(void);
	HRESULT				Initialize_Prototype(void);
	virtual My_Int Update(const My_Float & fTimeDelta) override;
	virtual My_Int LastUpdate(const My_Float & fTimeDelta) override;
	virtual void Render(void) override;

public:
	static CStage_MeshTest*				Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*				Clone(void);
protected:
	virtual My_uLong					ReleaseMemory();
};

