#pragma once

#include "GameObject.h"

namespace Engine {
	class CTexture;
	class CCubeCol;
	class CCubeTex;
	class CTransform;
	class CRenderer;
}

BEGIN(Engine)

//클라이언트에서 부품을 어떻게 저장하는지, 씬의 갯수를 몇개로 만들었는지 등을 알수가 없기에
//실 객체를 이니셜라이즈 하는과정(컴포넌트를 받아오고, 텍스처를 받아오는 과정)은 클라이언트에서 해결해야만 한다.

class ENGINE_DLL CModel
	:public CGameObject
{
protected:
	explicit CModel(LPDIRECT3DDEVICE9 pDevice);
	explicit CModel(const CModel& Ref);
	virtual ~CModel() = default;

protected:
	std::map<const My_TChar*, INFO_MODEL*>		m_map_Info;
	std::map<My_TChar*, My_Matrix>		m_map_ParentMat;
	std::map<const My_TChar*, CTexture*>		m_map_Texture;
protected:
	CCubeCol*									m_pComponent_CubeCol = nullptr;
	CCubeTex*									m_pComponent_CubeTex = nullptr;
	CTransform*									m_pComponent_Transform = nullptr;
	CRenderer*									m_pComponent_Renderer = nullptr;

public:
	HRESULT					Initialize_Prototype(const My_TChar* szFilePath);
	HRESULT					Initialize(const My_uLong& dwCubeCol, const My_TChar* szCubeCol,
		const My_uLong& dwCubeTex, const My_TChar* szCubeTex,
		const My_uLong& dwTransform, const My_TChar* szTransform,
		const My_uLong& dwRenderer, const My_TChar* szRenderer,
		const My_uLong& dwTextures, const My_TChar* szTexturePrefix);

	virtual My_Int			Update(const My_Float& fTimeDelta) override PURE;
	virtual My_Int			LastUpdate(const My_Float& fTimeDelta) override PURE;
	virtual void			Render(void) override;
private:
	HRESULT					Initialize_Component(const My_uLong& dwCubeCol, const My_TChar* szCubeCol,
		const My_uLong& dwCubeTex, const My_TChar* szCubeTex,
		const My_uLong& dwTransform, const My_TChar* szTransform,
		const My_uLong& dwRenderer, const My_TChar* szRenderer);
	HRESULT					Initialize_Texture(const My_uLong& dwTextures, const My_TChar* szTexturePrefix);

private:
	INFO_MODEL*			Find_Model(const My_TChar* szName);
public:
	HRESULT				Get_LocalMat(const My_TChar* szName, My_Matrix* pMatContainer);
public:
	HRESULT				Set_ParentMat(const My_TChar* szName, const My_Matrix& matParent);
public:
	HRESULT				Move_Forward(const My_TChar* szName, const My_Float& fSpdPerSec);
	HRESULT				Move_Backward(const My_TChar* szName, const My_Float& fSpdPerSec);
	HRESULT				Move_Left(const My_TChar* szName, const My_Float& fSpdPerSec);
	HRESULT				Move_Right(const My_TChar* szName, const My_Float& fSpdPerSec);
	HRESULT				Move_Up(const My_TChar* szName, const My_Float& fSpdPerSec);
	HRESULT				Move_Down(const My_TChar* szName, const My_Float& fSpdPerSec);

	HRESULT				Rotation_X(const My_TChar* szName, const My_Float& fRadPerSec);
	HRESULT				Rotation_Y(const My_TChar* szName, const My_Float& fRadPerSec);
	HRESULT				Rotation_Z(const My_TChar* szName, const My_Float& fRadPerSec);

	HRESULT				Rotation_X2(const My_TChar* szName, const My_Float& fRadPerSec);
	HRESULT				Rotation_Y2(const My_TChar* szName, const My_Float& fRadPerSec);
	HRESULT				Rotation_Z2(const My_TChar* szName, const My_Float& fRadPerSec);

	HRESULT				Scaling_X(const My_TChar* szName, const My_Float& fScalePerSec);
	HRESULT				Scaling_Y(const My_TChar* szName, const My_Float& fScalePerSec);
	HRESULT				Scaling_Z(const My_TChar* szName, const My_Float& fScalePerSec);


public:	//get
	HRESULT				Get_Model_Pos(const My_TChar* szName, My_Vec3* pVContainer);
	HRESULT				Get_Model_Scale_X(const My_TChar* szName, My_Float * pfLength);
	HRESULT				Get_Model_Scale_Y(const My_TChar* szName, My_Float * pfLength);
	HRESULT				Get_Model_Scale_Z(const My_TChar* szName, My_Float * pfLength);

public:
	HRESULT				Set_Model_Pos(const My_TChar* szName, My_Vec3* pVContainer);

public: //set
	HRESULT				Set_Pos(const My_Vec3& vPos) {
		if (m_pComponent_Transform == nullptr)				return E_FAIL;
		m_pComponent_Transform->Move_Position(vPos);		return S_OK;
	}
	HRESULT				Set_Matrix(const My_Matrix* pMatWorld) {
		if (m_pComponent_Transform == nullptr)return E_FAIL;
		m_pComponent_Transform->Set_Information(pMatWorld); return S_OK;
	}

public:
	virtual CGameObject*	Clone(void) override PURE;
protected:
	virtual My_uLong		ReleaseMemory(void) override;
};

END