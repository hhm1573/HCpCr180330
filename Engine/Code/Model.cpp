#include "..\Header\Model.h"



CModel::CModel(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CModel::CModel(const CModel & Ref)
	: CGameObject(Ref)
{
	//m_map_Info = Ref.m_map_Info;
	for (auto& Pair : Ref.m_map_Info) {
		INFO_MODEL*	pInfoModel = nullptr;
		pInfoModel = new INFO_MODEL;
		*pInfoModel = *Pair.second;
		m_map_Info.insert(make_pair(pInfoModel->szName, pInfoModel));
	}
}

HRESULT CModel::Initialize_Prototype(const My_TChar * szFilePath)
{
	HANDLE		fFileHandle;
	fFileHandle = CreateFile(szFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (fFileHandle == NULL)
		return E_FAIL;

	My_uLong		dwByte = 0;

	INFO_MODEL*		InfoModel = nullptr;

	while (true) {

		InfoModel = new INFO_MODEL;
		ReadFile(fFileHandle, InfoModel, sizeof(INFO_MODEL), &dwByte, NULL);

		if (dwByte == 0) {
			if (InfoModel != nullptr)
				delete InfoModel;
			break;
		}

		m_map_Info.insert(make_pair(InfoModel->szName, InfoModel));

	}

	CloseHandle(fFileHandle);

	return S_OK;
}

HRESULT CModel::Initialize(const My_uLong & dwCubeCol, const My_TChar * szCubeCol, const My_uLong & dwCubeTex, const My_TChar * szCubeTex, const My_uLong & dwTransform, const My_TChar * szTransform, const My_uLong & dwRenderer, const My_TChar * szRenderer, const My_uLong & dwTextures, const My_TChar * szTexturePrefix)
{
	if (FAILED(CModel::Initialize_Component(dwCubeCol, szCubeCol, dwCubeTex, szCubeTex, dwTransform, szTransform, dwRenderer, szRenderer)))
		return E_FAIL;
	if (FAILED(CModel::Initialize_Texture(dwTextures, szTexturePrefix)))
		return E_FAIL;

	return S_OK;
}

void CModel::Render(void)
{
	const My_Matrix*	pMatWorld = m_pComponent_Transform->Get_WorldMatrix();
	for (auto& Pair : m_map_Info) {
		My_Matrix	matLocal = Pair.second->matLocal;

		My_Matrix	matParent;
		D3DXMatrixIdentity(&matParent);
		for (auto& PairParent : m_map_ParentMat) {
			if (lstrcmp(Pair.first, PairParent.first) == 0) {
				matParent = PairParent.second;
				break;
			}
		}

		My_Matrix	matLocalWorld = matLocal * matParent * (*pMatWorld);
		m_pDevice->SetTransform(D3DTS_WORLD, &matLocalWorld);

		if (Pair.second->bIsTexture == true) {
			m_map_Texture[Pair.first]->SetUp_OnGraphicDev(0);
			m_pComponent_CubeTex->Render();
		}
		else {
			m_pComponent_CubeCol->SetColor(Pair.second->dwColor);
			m_pComponent_CubeCol->Render();
		}
	}
}

HRESULT CModel::Initialize_Component(const My_uLong & dwCubeCol, const My_TChar * szCubeCol, const My_uLong & dwCubeTex, const My_TChar * szCubeTex, const My_uLong & dwTransform, const My_TChar * szTransform, const My_uLong & dwRenderer, const My_TChar * szRenderer)
{
	m_pComponent_CubeCol = (CCubeCol*)m_pComponentMgr->Clone_Component(dwCubeCol, szCubeCol);
	if (m_pComponent_CubeCol == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(szCubeCol, m_pComponent_CubeCol)))
		return E_FAIL;

	m_pComponent_CubeTex = (CCubeTex*)m_pComponentMgr->Clone_Component(dwCubeTex, szCubeTex);
	if (m_pComponent_CubeTex == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(szCubeTex, m_pComponent_CubeTex)))
		return E_FAIL;

	m_pComponent_Renderer = (CRenderer*)m_pComponentMgr->Clone_Component(dwRenderer, szRenderer);
	if (m_pComponent_Renderer == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(szRenderer, m_pComponent_Renderer)))
		return E_FAIL;

	m_pComponent_Transform = (CTransform*)m_pComponentMgr->Clone_Component(dwTransform, szTransform);
	if (m_pComponent_Transform == nullptr)
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(szTransform, m_pComponent_Transform)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize_Texture(const My_uLong & dwTextures, const My_TChar * szTexturePrefix)
{
	My_TChar		szTexture[256] = L"";

	for (auto& Pair : m_map_Info) {
		if (Pair.second->bIsTexture == true) {
			lstrcpy(szTexture, szTexturePrefix);
			lstrcat(szTexture, Pair.second->szTexture);
			//lstrcat(szTexture, L"Bazzi_Head");
			CTexture*	pTexture = (CTexture*)m_pComponentMgr->Clone_Component(dwTextures, szTexture);
			if (pTexture == nullptr)
				return E_FAIL;
			/*if (FAILED(CGameObject::Add_Component(szTexture, pTexture)))
			return E_FAIL;*/
			CGameObject::Add_Component(szTexture, pTexture);
			m_map_Texture.insert(make_pair(Pair.first, pTexture));
		}
	}

	return S_OK;
}

INFO_MODEL * CModel::Find_Model(const My_TChar * szName)
{
	for (auto& Pair : m_map_Info) {
		if (Pair.second == nullptr)
			continue;
		if (lstrcmp(szName, Pair.second->szName) == 0) {
			return Pair.second;
		}
	}
	return nullptr;
}

HRESULT CModel::Get_LocalMat(const My_TChar * szName, My_Matrix * pMatContainer)
{
	if (szName == nullptr)
		return E_FAIL;

	INFO_MODEL*		pFind = CModel::Find_Model(szName);
	if (pFind == nullptr)
		return E_FAIL;

	if (pMatContainer == nullptr)
		return E_FAIL;

	*pMatContainer = pFind->matLocal;

	return S_OK;
}

HRESULT CModel::Set_ParentMat(const My_TChar * szName, const My_Matrix & matParent)
{
	if (szName == nullptr)
		return E_FAIL;

	INFO_MODEL*		pFind = CModel::Find_Model(szName);
	if (pFind == nullptr)
		return E_FAIL;

	for (auto& Pair : m_map_ParentMat) {
		if (lstrcmp(szName, Pair.first) == 0) {
			Pair.second = matParent;
			return S_OK;
		}
	}

	My_TChar*		szPath = new My_TChar[256];
	lstrcpy(szPath, pFind->szName);
	m_map_ParentMat.insert(make_pair(szPath, matParent));
	return S_OK;
}

//Move
#if true

HRESULT CModel::Move_Forward(const My_TChar * szName, const My_Float & fSpdPerSec)
{
	INFO_MODEL*		pInfo = Find_Model(szName);
	if (pInfo == nullptr)
		return E_FAIL;

	My_Matrix	matLocal = pInfo->matLocal;
	My_Vec3		vLook;

	memcpy(&vLook, &matLocal.m[2][0], sizeof(My_Vec3));

	*(My_Vec3*)&matLocal.m[3][0] += *D3DXVec3Normalize(&vLook, &vLook) * fSpdPerSec;

	pInfo->matLocal = matLocal;

	return S_OK;
}

HRESULT CModel::Move_Backward(const My_TChar * szName, const My_Float & fSpdPerSec)
{
	INFO_MODEL*		pInfo = Find_Model(szName);
	if (pInfo == nullptr)
		return E_FAIL;

	My_Matrix	matLocal = pInfo->matLocal;
	My_Vec3		vLook;

	memcpy(&vLook, &matLocal.m[2][0], sizeof(My_Vec3));

	*(My_Vec3*)&matLocal.m[3][0] -= *D3DXVec3Normalize(&vLook, &vLook) * fSpdPerSec;

	pInfo->matLocal = matLocal;

	return S_OK;
}

HRESULT CModel::Move_Left(const My_TChar * szName, const My_Float & fSpdPerSec)
{
	INFO_MODEL*		pInfo = Find_Model(szName);
	if (pInfo == nullptr)
		return E_FAIL;

	My_Matrix	matLocal = pInfo->matLocal;
	My_Vec3		vRight;

	memcpy(&vRight, &matLocal.m[0][0], sizeof(My_Vec3));

	*(My_Vec3*)&matLocal.m[3][0] -= *D3DXVec3Normalize(&vRight, &vRight) * fSpdPerSec;

	pInfo->matLocal = matLocal;

	return S_OK;
}

HRESULT CModel::Move_Right(const My_TChar * szName, const My_Float & fSpdPerSec)
{
	INFO_MODEL*		pInfo = Find_Model(szName);
	if (pInfo == nullptr)
		return E_FAIL;

	My_Matrix	matLocal = pInfo->matLocal;
	My_Vec3		vRight;

	memcpy(&vRight, &matLocal.m[0][0], sizeof(My_Vec3));

	*(My_Vec3*)&matLocal.m[3][0] += *D3DXVec3Normalize(&vRight, &vRight) * fSpdPerSec;

	pInfo->matLocal = matLocal;

	return S_OK;
}

HRESULT CModel::Move_Up(const My_TChar * szName, const My_Float & fSpdPerSec)
{
	INFO_MODEL*		pInfo = Find_Model(szName);
	if (pInfo == nullptr)
		return E_FAIL;

	My_Matrix	matLocal = pInfo->matLocal;
	My_Vec3		vUp;

	memcpy(&vUp, &matLocal.m[1][0], sizeof(My_Vec3));

	*(My_Vec3*)&matLocal.m[3][0] += *D3DXVec3Normalize(&vUp, &vUp) * fSpdPerSec;

	pInfo->matLocal = matLocal;

	return S_OK;
}

HRESULT CModel::Move_Down(const My_TChar * szName, const My_Float & fSpdPerSec)
{
	INFO_MODEL*		pInfo = Find_Model(szName);
	if (pInfo == nullptr)
		return E_FAIL;

	My_Matrix	matLocal = pInfo->matLocal;
	My_Vec3		vUp;

	memcpy(&vUp, &matLocal.m[1][0], sizeof(My_Vec3));

	*(My_Vec3*)&matLocal.m[3][0] -= *D3DXVec3Normalize(&vUp, &vUp) * fSpdPerSec;

	pInfo->matLocal = matLocal;

	return S_OK;
}

#endif

HRESULT CModel::Rotation_X(const My_TChar * szName, const My_Float & fRadPerSec)
{
	INFO_MODEL*		pInfo = Find_Model(szName);
	if (pInfo == nullptr)
		return E_FAIL;

	My_Matrix	matLocal = pInfo->matLocal;

	Engine::My_Vec3		vAxisX, vAxisY, vAxisZ;
	memcpy_s(&vAxisX, sizeof(Engine::My_Vec3), &matLocal.m[0][0], sizeof(Engine::My_Vec3));
	memcpy_s(&vAxisY, sizeof(Engine::My_Vec3), &matLocal.m[1][0], sizeof(Engine::My_Vec3));
	memcpy_s(&vAxisZ, sizeof(Engine::My_Vec3), &matLocal.m[2][0], sizeof(Engine::My_Vec3));

	Engine::My_Matrix	matRot;
	D3DXMatrixRotationAxis(&matRot, &vAxisX, fRadPerSec);

	D3DXVec3TransformNormal(&vAxisY, &vAxisY, &matRot);
	D3DXVec3TransformNormal(&vAxisZ, &vAxisZ, &matRot);

	memcpy_s(&matLocal.m[1][0], sizeof(Engine::My_Vec3), &vAxisY, sizeof(Engine::My_Vec3));
	memcpy_s(&matLocal.m[2][0], sizeof(Engine::My_Vec3), &vAxisZ, sizeof(Engine::My_Vec3));

	pInfo->matLocal = matLocal;

	return S_OK;
}

HRESULT CModel::Rotation_Y(const My_TChar * szName, const My_Float & fRadPerSec)
{
	INFO_MODEL*		pInfo = Find_Model(szName);
	if (pInfo == nullptr)
		return E_FAIL;

	My_Matrix	matLocal = pInfo->matLocal;

	My_Vec3			vRight, vLook;

	memcpy(&vRight, &matLocal.m[0], sizeof(My_Vec3));
	memcpy(&vLook, &matLocal.m[2], sizeof(My_Vec3));

	My_Matrix		matRot;

	D3DXMatrixRotationY(&matRot, fRadPerSec);

	D3DXVec3TransformNormal((My_Vec3*)&matLocal.m[0], &vRight, &matRot);
	D3DXVec3TransformNormal((My_Vec3*)&matLocal.m[2], &vLook, &matRot);

	//INFO_MODEL*		pInfo = Find_Model(szName);
	//if (pInfo == nullptr)
	//	return E_FAIL;

	//My_Matrix	matLocal = pInfo->matLocal;

	//Engine::My_Vec3		vAxisX, vAxisY, vAxisZ;
	//memcpy_s(&vAxisX, sizeof(Engine::My_Vec3), &matLocal.m[0][0], sizeof(Engine::My_Vec3));
	//memcpy_s(&vAxisY, sizeof(Engine::My_Vec3), &matLocal.m[1][0], sizeof(Engine::My_Vec3));
	//memcpy_s(&vAxisZ, sizeof(Engine::My_Vec3), &matLocal.m[2][0], sizeof(Engine::My_Vec3));

	//Engine::My_Matrix	matRot;
	//D3DXMatrixRotationAxis(&matRot, &vAxisY, fRadPerSec);

	//D3DXVec3TransformNormal(&vAxisX, &vAxisX, &matRot);
	//D3DXVec3TransformNormal(&vAxisZ, &vAxisZ, &matRot);

	//memcpy_s(&matLocal.m[1][0], sizeof(Engine::My_Vec3), &vAxisX, sizeof(Engine::My_Vec3));
	//memcpy_s(&matLocal.m[2][0], sizeof(Engine::My_Vec3), &vAxisZ, sizeof(Engine::My_Vec3));

	pInfo->matLocal = matLocal;

	return S_OK;
}

HRESULT CModel::Rotation_Z(const My_TChar * szName, const My_Float & fRadPerSec)
{
	INFO_MODEL*		pInfo = Find_Model(szName);
	if (pInfo == nullptr)
		return E_FAIL;

	My_Matrix	matLocal = pInfo->matLocal;

	My_Vec3			vRight, vUp;

	memcpy(&vRight, &matLocal.m[0], sizeof(My_Vec3));
	memcpy(&vUp, &matLocal.m[1], sizeof(My_Vec3));

	My_Matrix		matRot;

	D3DXMatrixRotationZ(&matRot, fRadPerSec);

	D3DXVec3TransformNormal((My_Vec3*)&matLocal.m[0], &vRight, &matRot);
	D3DXVec3TransformNormal((My_Vec3*)&matLocal.m[1], &vUp, &matRot);

	//Engine::My_Vec3		vAxisX, vAxisY, vAxisZ;
	//memcpy_s(&vAxisX, sizeof(Engine::My_Vec3), &matLocal.m[0][0], sizeof(Engine::My_Vec3));
	//memcpy_s(&vAxisY, sizeof(Engine::My_Vec3), &matLocal.m[1][0], sizeof(Engine::My_Vec3));
	//memcpy_s(&vAxisZ, sizeof(Engine::My_Vec3), &matLocal.m[2][0], sizeof(Engine::My_Vec3));

	//Engine::My_Matrix	matRot;
	//D3DXMatrixRotationAxis(&matRot, &vAxisZ, fRadPerSec);

	//D3DXVec3TransformNormal(&vAxisY, &vAxisY, &matRot);
	//D3DXVec3TransformNormal(&vAxisX, &vAxisX, &matRot);

	//memcpy_s(&matLocal.m[0][0], sizeof(Engine::My_Vec3), &vAxisX, sizeof(Engine::My_Vec3));
	//memcpy_s(&matLocal.m[1][0], sizeof(Engine::My_Vec3), &vAxisY, sizeof(Engine::My_Vec3));

	pInfo->matLocal = matLocal;

	return S_OK;
}

HRESULT CModel::Rotation_X2(const My_TChar * szName, const My_Float & fRadPerSec)
{
	INFO_MODEL*		pInfo = Find_Model(szName);
	if (pInfo == nullptr)
		return E_FAIL;

	My_Matrix	matLocal = pInfo->matLocal;

	My_Vec3			vUp, vLook;

	memcpy(&vUp, &matLocal.m[1], sizeof(My_Vec3));
	memcpy(&vLook, &matLocal.m[2], sizeof(My_Vec3));

	My_Matrix		matRot;

	D3DXMatrixRotationX(&matRot, fRadPerSec);

	D3DXVec3TransformNormal((My_Vec3*)&matLocal.m[1], &vUp, &matRot);
	D3DXVec3TransformNormal((My_Vec3*)&matLocal.m[2], &vLook, &matRot);

	pInfo->matLocal = matLocal;

	return S_OK;

}

HRESULT CModel::Rotation_Y2(const My_TChar * szName, const My_Float & fRadPerSec)
{
	INFO_MODEL*		pInfo = Find_Model(szName);
	if (pInfo == nullptr)
		return E_FAIL;

	My_Matrix	matLocal = pInfo->matLocal;

	Engine::My_Vec3		vAxisX, vAxisY, vAxisZ;
	memcpy_s(&vAxisX, sizeof(Engine::My_Vec3), &matLocal.m[0][0], sizeof(Engine::My_Vec3));
	memcpy_s(&vAxisY, sizeof(Engine::My_Vec3), &matLocal.m[1][0], sizeof(Engine::My_Vec3));
	memcpy_s(&vAxisZ, sizeof(Engine::My_Vec3), &matLocal.m[2][0], sizeof(Engine::My_Vec3));

	Engine::My_Matrix	matRot;
	D3DXMatrixRotationAxis(&matRot, &vAxisY, fRadPerSec);

	D3DXVec3TransformNormal(&vAxisX, &vAxisX, &matRot);
	D3DXVec3TransformNormal(&vAxisZ, &vAxisZ, &matRot);

	memcpy_s(&matLocal.m[1][0], sizeof(Engine::My_Vec3), &vAxisX, sizeof(Engine::My_Vec3));
	memcpy_s(&matLocal.m[2][0], sizeof(Engine::My_Vec3), &vAxisZ, sizeof(Engine::My_Vec3));

	pInfo->matLocal = matLocal;

	return S_OK;

}

HRESULT CModel::Rotation_Z2(const My_TChar * szName, const My_Float & fRadPerSec)
{
	INFO_MODEL*		pInfo = Find_Model(szName);
	if (pInfo == nullptr)
		return E_FAIL;

	My_Matrix	matLocal = pInfo->matLocal;

	Engine::My_Vec3		vAxisX, vAxisY, vAxisZ;
	memcpy_s(&vAxisX, sizeof(Engine::My_Vec3), &matLocal.m[0][0], sizeof(Engine::My_Vec3));
	memcpy_s(&vAxisY, sizeof(Engine::My_Vec3), &matLocal.m[1][0], sizeof(Engine::My_Vec3));
	memcpy_s(&vAxisZ, sizeof(Engine::My_Vec3), &matLocal.m[2][0], sizeof(Engine::My_Vec3));

	Engine::My_Matrix	matRot;
	D3DXMatrixRotationAxis(&matRot, &vAxisZ, fRadPerSec);

	D3DXVec3TransformNormal(&vAxisY, &vAxisY, &matRot);
	D3DXVec3TransformNormal(&vAxisX, &vAxisX, &matRot);

	memcpy_s(&matLocal.m[0][0], sizeof(Engine::My_Vec3), &vAxisX, sizeof(Engine::My_Vec3));
	memcpy_s(&matLocal.m[1][0], sizeof(Engine::My_Vec3), &vAxisY, sizeof(Engine::My_Vec3));

	pInfo->matLocal = matLocal;

	return S_OK;

}

HRESULT CModel::Scaling_X(const My_TChar * szName, const My_Float & fScalePerSec)
{
	INFO_MODEL*		pInfo = Find_Model(szName);
	if (pInfo == nullptr)
		return E_FAIL;

	My_Matrix	matLocal = pInfo->matLocal;

	My_Vec3		vRight = *(My_Vec3*)&matLocal.m[0][0];
	My_Vec3		vRight_Nor;
	D3DXVec3Normalize(&vRight_Nor, &vRight);
	My_Vec3		vRight_Sum = vRight + vRight_Nor * fScalePerSec;

	*(My_Vec3*)&matLocal.m[0][0] = vRight_Sum;

	pInfo->matLocal = matLocal;

	return S_OK;
}

HRESULT CModel::Scaling_Y(const My_TChar * szName, const My_Float & fScalePerSec)
{
	INFO_MODEL*		pInfo = Find_Model(szName);
	if (pInfo == nullptr)
		return E_FAIL;

	My_Matrix	matLocal = pInfo->matLocal;

	My_Vec3		vUp = *(My_Vec3*)&matLocal.m[1][0];
	My_Vec3		vUp_Nor;
	D3DXVec3Normalize(&vUp_Nor, &vUp);
	My_Vec3		vUp_Sum = vUp + vUp_Nor * fScalePerSec;

	*(My_Vec3*)&matLocal.m[1][0] = vUp_Sum;

	pInfo->matLocal = matLocal;

	return S_OK;
}

HRESULT CModel::Scaling_Z(const My_TChar * szName, const My_Float & fScalePerSec)
{
	INFO_MODEL*		pInfo = Find_Model(szName);
	if (pInfo == nullptr)
		return E_FAIL;

	My_Matrix	matLocal = pInfo->matLocal;

	My_Vec3		vRight = *(My_Vec3*)&matLocal.m[2][0];
	My_Vec3		vRight_Nor;
	D3DXVec3Normalize(&vRight_Nor, &vRight);
	My_Vec3		vRight_Sum = vRight + vRight_Nor * fScalePerSec;

	*(My_Vec3*)&matLocal.m[2][0] = vRight_Sum;

	pInfo->matLocal = matLocal;

	return S_OK;
}

HRESULT CModel::Get_Model_Pos(const My_TChar * szName, My_Vec3 * pVContainer)
{
	INFO_MODEL*		pInfo = Find_Model(szName);
	if (pInfo == nullptr)
		return E_FAIL;

	My_Matrix	matLocal = pInfo->matLocal;

	*pVContainer = *(My_Vec3*)&matLocal.m[3][0];

	return S_OK;
}

HRESULT CModel::Get_Model_Scale_X(const My_TChar * szName, My_Float * pfLength)
{
	INFO_MODEL*		pFind = Find_Model(szName);
	if (pFind == nullptr)
		return E_FAIL;

	My_Vec3		vRight = *(My_Vec3*)&pFind->matLocal.m[0][0];
	*pfLength = D3DXVec3Length(&vRight);

	return S_OK;
}

HRESULT CModel::Get_Model_Scale_Y(const My_TChar * szName, My_Float * pfLength)
{
	INFO_MODEL*		pFind = Find_Model(szName);
	if (pFind == nullptr)
		return E_FAIL;

	My_Vec3		vUp = *(My_Vec3*)&pFind->matLocal.m[1][0];
	*pfLength = D3DXVec3Length(&vUp);

	return S_OK;
}

HRESULT CModel::Get_Model_Scale_Z(const My_TChar * szName, My_Float * pfLength)
{
	INFO_MODEL*		pFind = Find_Model(szName);
	if (pFind == nullptr)
		return E_FAIL;

	My_Vec3		vLook = *(My_Vec3*)&pFind->matLocal.m[2][0];
	*pfLength = D3DXVec3Length(&vLook);

	return S_OK;
}

HRESULT CModel::Set_Model_Pos(const My_TChar * szName, My_Vec3 * pVContainer)
{
	INFO_MODEL*		pInfo = Find_Model(szName);
	if (pInfo == nullptr)
		return E_FAIL;

	My_Matrix	matLocal = pInfo->matLocal;

	*(My_Vec3*)&matLocal.m[3][0] = *pVContainer;

	return S_OK;
}

My_uLong CModel::ReleaseMemory(void)
{
	for (auto& Pair : m_map_Info) {
		if (Pair.second != nullptr) {
			delete Pair.second;
			Pair.second = nullptr;
		}
	}
	m_map_Info.clear();

	for (auto& Pair : m_map_ParentMat) {
		if (Pair.first != nullptr) {
			delete Pair.first;
		}
	}
	m_map_ParentMat.clear();

	for (auto& Pair : m_map_Texture) {
		Safe_Release(Pair.second);
	}

	Safe_Release(m_pComponent_Renderer);
	Safe_Release(m_pComponent_Transform);
	Safe_Release(m_pComponent_CubeTex);
	Safe_Release(m_pComponent_CubeCol);

	CGameObject::ReleaseMemory();

	return My_uLong();
}
