#pragma once

#ifndef __SPHERE_BUFFER_H__
#define __SPHERE_BUFFER_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSphere_Buffer
	:public CVIBuffer
{
private:
	explicit CSphere_Buffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CSphere_Buffer(const CSphere_Buffer& rhs);
	virtual ~CSphere_Buffer() = default;

public:
	HRESULT Ready_Sphere_Buffer(const My_uLong& dwCnt, const float& fRadius);
	void Render_Buffer();

public:
	static CSphere_Buffer*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const My_uLong& dwCnt, const float& fRadius);
	virtual CComponent*		Clone() override;
protected:
	virtual My_uLong ReleaseMemory() override;
};

END


#endif // !__SPHERE_BUFFER_H__


