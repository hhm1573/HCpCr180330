#pragma once

#include "Base.h"

namespace Engine {
	class CVIBuffer;
}

BEGIN(Engine)

class CMyLocalInfo abstract
	:public CBase
{
protected:
	explicit CMyLocalInfo(LPDIRECT3DDEVICE9 pDevice);
	explicit CMyLocalInfo(const CMyLocalInfo& Ref);
	virtual ~CMyLocalInfo(void) = default;

protected:
	My_Vec3		m_vScale;
	My_Vec3		m_vRotation;
	My_Vec3		m_vTranslation;
	My_Matrix	m_matLocal;

	CVIBuffer*	m_pBuffer = nullptr;
protected:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;

public:
	virtual HRESULT			Refresh(void);
	virtual void			Render(const My_Matrix* matWorld);

public:
	const My_Vec3*		Get_Scale(void) const {
		return &m_vScale;
	}
	const My_Vec3*		Get_Rotation(void) const {
		return &m_vRotation;
	}
	const My_Vec3*		Get_Translation(void) const {
		return &m_vTranslation;
	}
	const My_Matrix*	Get_Matrix(void) const {
		return &m_matLocal;
	}

public:
	void		Set_Scale(const My_Vec3& vScale) { m_vScale = vScale; }
	void		Set_Rotation(const My_Vec3& vRotation) { m_vRotation = vRotation; }
	void		Set_Translation(const My_Vec3& vTranslation) { m_vTranslation = vTranslation; }
	void		Set_Matrix(const My_Matrix& matLocal) { m_matLocal = matLocal; }

protected:
	virtual My_uLong		ReleaseMemory(void) override;
};

END