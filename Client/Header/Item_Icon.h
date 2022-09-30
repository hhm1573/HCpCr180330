#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CViewPort_Buffer;
	class CTransform;
	class CTexture;
}

class CItem_Icon final : public CGameObject
{
private:
	explicit CItem_Icon(LPDIRECT3DDEVICE9 pDevice);
	explicit CItem_Icon(const CItem_Icon& Ref);
	virtual ~CItem_Icon() = default;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CViewPort_Buffer*		m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

public:
	void			Set_Number(const int& iNumber) {
		m_iNumber = iNumber;
	}
	void			Set_IsUsed(bool bIsUsed) {
		m_bIsUsed = bIsUsed;
	}
	void		SetIsAward(My_Bool flag) { mIsAward = flag; }

private:
	HRESULT		Initialize();
	HRESULT		Initialize_Prototype();
	My_Int		Update(const My_Float& fTimeDelta);
	My_Int		LastUpdate(const My_Float& fTimeDelta);
	void			Render();

private:	
	int				m_iNumber;
	float			m_fFirstTime;
	float			m_fAccTime;
	bool			m_bIsFirst;
	bool			m_bIsUsed;

	My_Bool			mIsAward = false;

private:
	HRESULT		Initialize_Component();

private:
	void		Set_RenderState();
	void		Release_RenderState();

public:
	static		CItem_Icon*			Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*		Clone();

protected:
	virtual		My_uLong			ReleaseMemory();
};

