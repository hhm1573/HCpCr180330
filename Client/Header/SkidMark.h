#pragma once

#include "GameObject.h"

namespace Engine {
	class CRenderer;
	class CRectTex;
	class CTexture;
}

class CSkidMark final
	:public CGameObject
{
private:
	explicit CSkidMark(LPDIRECT3DDEVICE9 pDevice);
	explicit CSkidMark(const CSkidMark& Ref);
	virtual ~CSkidMark() = default;

private:
	list<My_Matrix>		m_List_Matrix;
private:
	CRenderer*			m_pComponent_Renderer = nullptr;
	CRectTex*			m_pComponent_RectTex = nullptr;
	CTexture*			m_pComponent_Texture = nullptr;

public:
	HRESULT				Initialize();
	HRESULT				Initialize_Prototype();
	virtual My_Int		Update(const My_Float& fTimeDelta) override;
	virtual My_Int		LastUpdate(const My_Float& fTimeDelta) override;
	virtual void		Render() override;

public:
	HRESULT				Add_SkidMark(const My_Vec3& vStart, const My_Vec3& vEnd);

public:
	static CSkidMark*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*	Clone() override;
protected:
	virtual My_uLong		ReleaseMemory() override;
};

