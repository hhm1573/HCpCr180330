#pragma once

#include "Base.h"

// 객체들을 모아서 관리한다.

BEGIN(Engine)

class CGameObject;
class CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() = default;
public:
	const CGameObject* Get_GameObject(const My_uInt& iIndex) const;
	const int Get_ObjectSize() const {
		return m_ObjectList.size();
	}

public:
	HRESULT Initialize();
	My_Int Update(const My_Float& fTimeDelta);
	My_Int LastUpdate(const My_Float& fTimeDelta);
	// 렌더링은 추후 CRenderer클래스를 통해 처리하자.

	HRESULT Add_GameObject(CGameObject* pGameObject);

private:
	list<CGameObject*>				m_ObjectList;
	typedef list<CGameObject*>		OBJECTLIST;
public:
	static CLayer* Create();
	virtual My_uLong ReleaseMemory(void);
};

END