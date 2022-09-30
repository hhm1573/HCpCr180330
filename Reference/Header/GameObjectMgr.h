#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CGameObject;
class CLayer;

class ENGINE_DLL CGameObjectMgr final
	:public CBase
{
	DECLARE_SINGLETON(CGameObjectMgr)

private:
	explicit CGameObjectMgr() {};
	virtual ~CGameObjectMgr() = default;

public:
	const	int&	Get_LayerSize(const My_uLong& dwSceneID, const My_TChar* szKey);

private:
	map<const My_TChar*, CGameObject*>*		m_pPrototypeObject = nullptr;
	typedef map<const My_TChar*, CGameObject*>		MAPPROTOTYPEOBJECT;
private:
	map<const My_TChar*, CLayer*>*			m_pLayers = nullptr;
	typedef map<const My_TChar*, CLayer*>			MAPLAYER;
private:
	My_uLong								m_dwNumMaxScene = 0;

public:
	HRESULT				Reserve(const My_uLong& dwNum);
	HRESULT				Add_Object_Prototype(const My_uLong& dwSceneID, const My_TChar* szKey, CGameObject* pObject);
	HRESULT				Add_Object(const My_uLong& dwSceneID, const My_TChar* szKey, CGameObject* pObject);
	CGameObject*		Clone_GameObject(const My_uLong& dwSceneID, const My_TChar* szKey);
	void				Clear_GameObject(const My_uLong& dwSceneID);
	void				Clear_Layer(const My_uLong & dwSceneID, const My_TChar * szLayerKey);
public:
	const CGameObject*		Get_GameObject(const My_uLong& dwSceneID, const My_TChar* szLayerKey, const My_uLong& dwIndex) const;
public:
	My_Int				Update(const My_Float& fTimeDelta);
	My_Int				LastUpdate(const My_Float& fTimeDelta);
private:
	CGameObject* Find_Object_Prototype(const My_uLong& dwSceneID, const My_TChar* pObjectTag);
	CLayer* Find_Layer(const My_uLong& dwSceneID, const My_TChar* pLayerTag);
	const CLayer*	Find_Layer_Const(const My_uLong& dwSceneID, const My_TChar* szLayerKey) const;

public:
	virtual My_uLong	ReleaseMemory();
};

END