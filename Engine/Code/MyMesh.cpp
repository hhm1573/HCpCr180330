#include "..\Header\MyMesh.h"
#include "CubeCol.h"
#include "MyLocalInfo.h"
#include "MyLocalInfo_CubeCol.h"



CMyMesh::CMyMesh(LPDIRECT3DDEVICE9 pDevice)
	:CComponent(pDevice)
{
}

CMyMesh::CMyMesh(const CMyMesh & Ref)
	: CComponent(Ref)
{
	m_pCom_CubeCol = Ref.m_pCom_CubeCol;
	m_map_pCubeColInfo = Ref.m_map_pCubeColInfo;
}

HRESULT CMyMesh::Initialize(CCubeCol* pCubeCol)
{
	if (pCubeCol == nullptr)
		return E_FAIL;
	m_pCom_CubeCol = pCubeCol;
	m_pCom_CubeCol->AddRef();

	return S_OK;
}

void CMyMesh::Render(const My_Matrix * pMatWorld)
{
	My_Matrix	matLocalWorld;

	for (auto& Pair : m_map_pCubeColInfo) {
		Pair.second->Render(pMatWorld);
	}
}

HRESULT CMyMesh::Refresh_ALL(void)
{
	if (FAILED(Refresh_CubeCol()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMesh::Refresh_Single(BUFFER_TYPE eType, const My_TChar * szKey)
{
	CMyLocalInfo* pFind = Find_LocalInfo(eType, szKey);
	if (pFind == nullptr)
		return E_FAIL;
	if (FAILED(pFind->Refresh()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMesh::Refresh_CubeCol(void)
{
	for (auto& Pair : m_map_pCubeColInfo) {
		if (FAILED(Refresh_Single(TYPE_CUBE_COL, Pair.first)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMyMesh::Add_CubeCol(const My_TChar * szKey, D3DCOLOR dwColor)
{
	if (CheckData(CMyMesh::TYPE_CUBE_COL, szKey) == true)
		return E_FAIL;
	pair<const My_TChar*, CMyLocalInfo*> pair;
	pair.first = szKey;
	pair.second = CMyLocalInfo_CubeCol::Create(m_pDevice, m_pCom_CubeCol);
	if (pair.second == nullptr)
		return E_FAIL;

	((CMyLocalInfo_CubeCol*)pair.second)->Set_Color(dwColor);

	m_map_pCubeColInfo.insert(pair);

	return S_OK;
}

HRESULT CMyMesh::Delete_CubeCol(const My_TChar * szKey)
{
	auto iter = Find_Iter(TYPE_CUBE_COL, szKey);
	if (iter == m_map_pCubeColInfo.end())
		return E_FAIL;
	if (iter->second->Release() != 0)
		return E_FAIL;
	m_map_pCubeColInfo.erase(iter);

	return S_OK;
}

#if true
// simple set functions
HRESULT CMyMesh::Move_Local_Forward(BUFFER_TYPE eType, const My_TChar * szKey, const My_Float & fSpd, const My_Float & fTime)
{
	CMyLocalInfo*	pFind_Info = Find_LocalInfo(eType, szKey);
	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	My_Matrix	matLocal = *pFind_Info->Get_Matrix();
	My_Vec3		vPos = *(My_Vec3*)&matLocal.m[3][0];
	My_Vec3		vLook = *(My_Vec3*)&matLocal.m[2][0];

	vPos += vLook * fSpd * fTime;

	pFind_Info->Set_Translation(vPos);

	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMesh::Move_Local_Backward(BUFFER_TYPE eType, const My_TChar * szKey, const My_Float & fSpd, const My_Float & fTime)
{
	CMyLocalInfo*	pFind_Info = Find_LocalInfo(eType, szKey);
	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	My_Matrix	matLocal = *pFind_Info->Get_Matrix();
	My_Vec3		vPos = *(My_Vec3*)&matLocal.m[3][0];
	My_Vec3		vLook = *(My_Vec3*)&matLocal.m[2][0];

	vPos -= vLook * fSpd * fTime;

	pFind_Info->Set_Translation(vPos);

	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMesh::Move_Local_Right(BUFFER_TYPE eType, const My_TChar * szKey, const My_Float & fSpd, const My_Float & fTime)
{
	CMyLocalInfo*	pFind_Info = Find_LocalInfo(eType, szKey);
	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	My_Matrix	matLocal = *pFind_Info->Get_Matrix();
	My_Vec3		vPos = *(My_Vec3*)&matLocal.m[3][0];
	My_Vec3		vRight = *(My_Vec3*)&matLocal.m[0][0];

	vPos += vRight * fSpd * fTime;

	pFind_Info->Set_Translation(vPos);

	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMesh::Move_Local_Left(BUFFER_TYPE eType, const My_TChar * szKey, const My_Float & fSpd, const My_Float & fTime)
{
	CMyLocalInfo*	pFind_Info = Find_LocalInfo(eType, szKey);
	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	My_Matrix	matLocal = *pFind_Info->Get_Matrix();
	My_Vec3		vPos = *(My_Vec3*)&matLocal.m[3][0];
	My_Vec3		vRight = *(My_Vec3*)&matLocal.m[0][0];

	vPos -= vRight * fSpd * fTime;

	pFind_Info->Set_Translation(vPos);

	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMesh::Move_Local_Up(BUFFER_TYPE eType, const My_TChar * szKey, const My_Float & fSpd, const My_Float & fTime)
{
	CMyLocalInfo*	pFind_Info = Find_LocalInfo(eType, szKey);
	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	My_Matrix	matLocal = *pFind_Info->Get_Matrix();
	My_Vec3		vPos = *(My_Vec3*)&matLocal.m[3][0];
	My_Vec3		vUp = *(My_Vec3*)&matLocal.m[1][0];

	vPos += vUp * fSpd * fTime;

	pFind_Info->Set_Translation(vPos);

	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMesh::Move_Local_Down(BUFFER_TYPE eType, const My_TChar * szKey, const My_Float & fSpd, const My_Float & fTime)
{
	CMyLocalInfo*	pFind_Info = Find_LocalInfo(eType, szKey);
	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	My_Matrix	matLocal = *pFind_Info->Get_Matrix();
	My_Vec3		vPos = *(My_Vec3*)&matLocal.m[3][0];
	My_Vec3		vUp = *(My_Vec3*)&matLocal.m[1][0];

	vPos -= vUp * fSpd * fTime;

	pFind_Info->Set_Translation(vPos);

	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMesh::Rotation_Local_X(BUFFER_TYPE eType, const My_TChar * szKey, const My_Float & fRad, const My_Float & fTime)
{
	CMyLocalInfo*	pFind_Info = Find_LocalInfo(eType, szKey);
	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	My_Vec3 vRotation = *pFind_Info->Get_Rotation();
	vRotation.x += fRad * fTime;

	pFind_Info->Set_Rotation(vRotation);

	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMesh::Rotation_Local_Y(BUFFER_TYPE eType, const My_TChar * szKey, const My_Float & fRad, const My_Float & fTime)
{
	CMyLocalInfo*	pFind_Info = Find_LocalInfo(eType, szKey);
	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	My_Vec3 vRotation = *pFind_Info->Get_Rotation();
	vRotation.y += fRad * fTime;

	pFind_Info->Set_Rotation(vRotation);

	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMesh::Rotation_Local_Z(BUFFER_TYPE eType, const My_TChar * szKey, const My_Float & fRad, const My_Float & fTime)
{
	CMyLocalInfo*	pFind_Info = Find_LocalInfo(eType, szKey);
	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	My_Vec3 vRotation = *pFind_Info->Get_Rotation();
	vRotation.z += fRad * fTime;

	pFind_Info->Set_Rotation(vRotation);

	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMesh::Scale_Local_X(BUFFER_TYPE eType, const My_TChar * szKey, const My_Float & fSpd, const My_Float & fTime)
{
	CMyLocalInfo*	pFind_Info = Find_LocalInfo(eType, szKey);
	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	My_Vec3 vScale = *pFind_Info->Get_Scale();
	vScale.x += fSpd * fTime;

	pFind_Info->Set_Rotation(vScale);

	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMesh::Scale_Local_Y(BUFFER_TYPE eType, const My_TChar * szKey, const My_Float & fSpd, const My_Float & fTime)
{
	CMyLocalInfo*	pFind_Info = Find_LocalInfo(eType, szKey);
	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	My_Vec3 vScale = *pFind_Info->Get_Scale();
	vScale.y += fSpd * fTime;

	pFind_Info->Set_Rotation(vScale);

	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMesh::Scale_Local_Z(BUFFER_TYPE eType, const My_TChar * szKey, const My_Float & fSpd, const My_Float & fTime)
{
	CMyLocalInfo*	pFind_Info = Find_LocalInfo(eType, szKey);
	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	My_Vec3 vScale = *pFind_Info->Get_Scale();
	vScale.z += fSpd * fTime;

	pFind_Info->Set_Rotation(vScale);

	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMesh::Move_Local_Forward_All(const My_Float & fSpd, const My_Float & fTime)
{
	for (auto& Pair : m_map_pCubeColInfo) {
		My_Matrix	matLocal = *Pair.second->Get_Matrix();
		My_Vec3		vPos = *(My_Vec3*)&matLocal.m[3][0];
		My_Vec3		vLook = *(My_Vec3*)&matLocal.m[2][0];

		vPos += vLook * fSpd * fTime;

		Pair.second->Set_Translation(vPos);

		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CMyMesh::Move_Local_Backward_All(const My_Float & fSpd, const My_Float & fTime)
{

	for (auto& Pair : m_map_pCubeColInfo) {
		My_Matrix	matLocal = *Pair.second->Get_Matrix();
		My_Vec3		vPos = *(My_Vec3*)&matLocal.m[3][0];
		My_Vec3		vLook = *(My_Vec3*)&matLocal.m[2][0];

		vPos -= vLook * fSpd * fTime;

		Pair.second->Set_Translation(vPos);

		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMyMesh::Move_Local_Right_All(const My_Float & fSpd, const My_Float & fTime)
{
	for (auto& Pair : m_map_pCubeColInfo) {
		My_Matrix	matLocal = *Pair.second->Get_Matrix();
		My_Vec3		vPos = *(My_Vec3*)&matLocal.m[3][0];
		My_Vec3		vRight = *(My_Vec3*)&matLocal.m[0][0];

		vPos += vRight * fSpd * fTime;

		Pair.second->Set_Translation(vPos);

		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMyMesh::Move_Local_Left_All(const My_Float & fSpd, const My_Float & fTime)
{
	for (auto& Pair : m_map_pCubeColInfo) {
		My_Matrix	matLocal = *Pair.second->Get_Matrix();
		My_Vec3		vPos = *(My_Vec3*)&matLocal.m[3][0];
		My_Vec3		vRight = *(My_Vec3*)&matLocal.m[0][0];

		vPos -= vRight * fSpd * fTime;

		Pair.second->Set_Translation(vPos);

		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMyMesh::Move_Local_Up_All(const My_Float & fSpd, const My_Float & fTime)
{
	for (auto& Pair : m_map_pCubeColInfo) {
		My_Matrix	matLocal = *Pair.second->Get_Matrix();
		My_Vec3		vPos = *(My_Vec3*)&matLocal.m[3][0];
		My_Vec3		vUp = *(My_Vec3*)&matLocal.m[1][0];

		vPos += vUp * fSpd * fTime;

		Pair.second->Set_Translation(vPos);

		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMyMesh::Move_Local_Down_All(const My_Float & fSpd, const My_Float & fTime)
{
	for (auto& Pair : m_map_pCubeColInfo) {
		My_Matrix	matLocal = *Pair.second->Get_Matrix();
		My_Vec3		vPos = *(My_Vec3*)&matLocal.m[3][0];
		My_Vec3		vUp = *(My_Vec3*)&matLocal.m[1][0];

		vPos -= vUp * fSpd * fTime;

		Pair.second->Set_Translation(vPos);

		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMyMesh::Rotation_Local_X_All(const My_Float & fRad, const My_Float & fTime)
{
	for (auto& Pair : m_map_pCubeColInfo) {
		My_Vec3 vRotation = *Pair.second->Get_Rotation();
		vRotation.x += fRad * fTime;

		Pair.second->Set_Rotation(vRotation);

		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMyMesh::Rotation_Local_Y_All(const My_Float & fRad, const My_Float & fTime)
{
	for (auto& Pair : m_map_pCubeColInfo) {
		My_Vec3 vRotation = *Pair.second->Get_Rotation();
		vRotation.y += fRad * fTime;

		Pair.second->Set_Rotation(vRotation);

		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMyMesh::Rotation_Local_Z_All(const My_Float & fRad, const My_Float & fTime)
{
	for (auto& Pair : m_map_pCubeColInfo) {
		My_Vec3 vRotation = *Pair.second->Get_Rotation();
		vRotation.z += fRad * fTime;

		Pair.second->Set_Rotation(vRotation);

		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMyMesh::Scale_Local_X_All(const My_Float & fSpd, const My_Float & fTime)
{
	for (auto& Pair : m_map_pCubeColInfo) {
		My_Vec3 vScale = *Pair.second->Get_Scale();
		vScale.x += fSpd * fTime;

		Pair.second->Set_Scale(vScale);

		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMyMesh::Scale_Local_Y_All(const My_Float & fSpd, const My_Float & fTime)
{
	for (auto& Pair : m_map_pCubeColInfo) {
		My_Vec3 vScale = *Pair.second->Get_Scale();
		vScale.y += fSpd * fTime;

		Pair.second->Set_Scale(vScale);

		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMyMesh::Scale_Local_Z_All(const My_Float & fSpd, const My_Float & fTime)
{
	for (auto& Pair : m_map_pCubeColInfo) {
		My_Vec3 vScale = *Pair.second->Get_Scale();
		vScale.z += fSpd * fTime;

		Pair.second->Set_Scale(vScale);

		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMyMesh::Set_Position(BUFFER_TYPE eType, const My_TChar * szKey, const My_Vec3 & vPos)
{
	CMyLocalInfo*	pFind_Info = Find_LocalInfo(eType, szKey);
	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	pFind_Info->Set_Translation(vPos);

	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMesh::Set_Rotation(BUFFER_TYPE eType, const My_TChar * szKey, const My_Vec3 & vRotation)
{
	CMyLocalInfo*	pFind_Info = Find_LocalInfo(eType, szKey);
	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	pFind_Info->Set_Rotation(vRotation);

	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMesh::Set_Scale(BUFFER_TYPE eType, const My_TChar * szKey, const My_Vec3 & vScale)
{
	CMyLocalInfo*	pFind_Info = Find_LocalInfo(eType, szKey);
	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	pFind_Info->Set_Scale(vScale);

	if (FAILED(pFind_Info->Refresh()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMesh::Set_Position_All(const My_Vec3 & vPos)
{
	for (auto& Pair : m_map_pCubeColInfo) {
		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;

		Pair.second->Set_Translation(vPos);

		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;
	}

	return S_OK;
}
HRESULT CMyMesh::Set_Rotation_All(const My_Vec3 & vRotation)
{
	for (auto& Pair : m_map_pCubeColInfo) {
		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;

		Pair.second->Set_Rotation(vRotation);

		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;
	}

	return S_OK;
}
HRESULT CMyMesh::Set_Scale_All(const My_Vec3 & vScale)
{
	for (auto& Pair : m_map_pCubeColInfo) {
		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;

		Pair.second->Set_Scale(vScale);

		if (FAILED(Pair.second->Refresh()))
			return E_FAIL;
	}

	return S_OK;
}
#endif

My_Bool CMyMesh::CheckData(BUFFER_TYPE eType, const My_TChar * szKey)
{
	auto pMap_LocalInfo = Get_Container(eType);

	for (auto& Pair : *pMap_LocalInfo) {
		if (lstrcmp(szKey, Pair.first) == 0)
			return true;
	}

	return false;
}

CMyLocalInfo * CMyMesh::Find_LocalInfo(BUFFER_TYPE eType, const My_TChar * szKey)
{
	auto pMap_LocalInfo = Get_Container(eType);
	for (auto& Pair : *pMap_LocalInfo) {
		if (lstrcmp(szKey, Pair.first) == 0)
			return Pair.second;
	}
	return nullptr;
}

map<const My_TChar*, CMyLocalInfo*>::iterator CMyMesh::Find_Iter(BUFFER_TYPE eType, const My_TChar * szKey)
{
	map<const My_TChar*, CMyLocalInfo*>*	pMap_Find = Get_Container(eType);
	map<const My_TChar*, CMyLocalInfo*>::iterator iterBegin = pMap_Find->begin();
	map<const My_TChar*, CMyLocalInfo*>::iterator iterEnd = pMap_Find->end();

	for (; iterBegin != iterEnd; ++iterBegin) {
		if (lstrcmp(szKey, iterBegin->first) == 0)
			return iterBegin;
	}

	return iterEnd;
}

map<const My_TChar*, CMyLocalInfo*>* CMyMesh::Get_Container(BUFFER_TYPE eType)
{
	map<const My_TChar*, CMyLocalInfo*>*		Arr_pMap_LocalInfo[CMyMesh::TYPE_END] = {
		&m_map_pCubeColInfo
	};

	return Arr_pMap_LocalInfo[eType];
}

CMyMesh * CMyMesh::Create(LPDIRECT3DDEVICE9 pDevice, CCubeCol * pCubeCol)
{
	CMyMesh*	pMesh = new CMyMesh(pDevice);

	if (FAILED(pMesh->Initialize(pCubeCol))) {
		delete pMesh;
		pMesh = nullptr;
	}

	return pMesh;
}

CComponent * CMyMesh::Clone(void)
{
	CMyMesh* pMesh = new CMyMesh(*this);
	return pMesh;
}

My_uLong CMyMesh::ReleaseMemory(void)
{
	My_uLong dwRefCnt = 0;
	My_uLong dwTotal = 0;
	for (auto& Pair : m_map_pCubeColInfo) {
		dwRefCnt = Pair.second->Release();
		if (dwRefCnt != 0)
			dwTotal += dwRefCnt;
	}
	m_map_pCubeColInfo.clear();

	CComponent::ReleaseMemory();

	return dwTotal;
}
