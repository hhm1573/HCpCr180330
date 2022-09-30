#include "..\Header\NavMesh.h"



Engine::CNavMesh::CNavMesh(LPDIRECT3DDEVICE9 pDevice)
	:CComponent(pDevice)
{
}

Engine::CNavMesh::CNavMesh(const CNavMesh & Ref)
	: CComponent(Ref)
{
}

HRESULT Engine::CNavMesh::Initialize(const My_TChar * szPath)
{
	//MSG_BOX("Click OK To Continue Load Process");

	if (m_List_NavMesh.size() != 0) {
		for (auto& pNav : m_List_NavMesh) {
			if (pNav != nullptr) {
				delete pNav;
				pNav = nullptr;
			}
		}
		m_List_NavMesh.clear();
		m_List_NavMesh.swap(list<NAVMESH*>());
	}

	HANDLE		hFile = CreateFile(szPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, NULL);

	if (hFile == NULL)
		MSG_BOX("LoadFailed");

	My_uLong		dwByte = 0;

	My_Vec3		vLT, vRT, vLB, vRB;
	My_uShort	wState = 0;
	NAVMESH*	pNav = nullptr;

	while (true) {
		ReadFile(hFile, &vLT, sizeof(My_Vec3), &dwByte, nullptr);
		ReadFile(hFile, &vRT, sizeof(My_Vec3), &dwByte, nullptr);
		ReadFile(hFile, &vLB, sizeof(My_Vec3), &dwByte, nullptr);
		ReadFile(hFile, &vRB, sizeof(My_Vec3), &dwByte, nullptr);
		ReadFile(hFile, &wState, sizeof(My_uShort), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		pNav = new NAVMESH;
		pNav->vLT = vLT;
		pNav->vRT = vRT;
		pNav->vLB = vLB;
		pNav->vRB = vRB;
		pNav->pU = nullptr;
		pNav->pB = nullptr;
		pNav->pL = nullptr;
		pNav->pR = nullptr;
		pNav->wState = wState;

		m_List_NavMesh.push_back(pNav);
	}

	CloseHandle(hFile);

	//MSG_BOX("Load process completed");



	//MSG_BOX("Connecting process started");

	for (auto& pNav0 : m_List_NavMesh) {
		if (pNav0 == NULL)
			continue;

		for (auto& pNav1 : m_List_NavMesh) {
			if (pNav0 == pNav1)
				continue;

			if (pNav1 == NULL)
				continue;

			if (pNav0->vLT == pNav1->vLT &&
				pNav0->vRT == pNav1->vRT &&
				pNav0->vLB == pNav1->vLB &&
				pNav0->vRB == pNav1->vRB) {
				delete pNav1;
				pNav1 = nullptr;
			}
		}
	}

	auto iterBegin = m_List_NavMesh.begin();
	auto iterEnd = m_List_NavMesh.end();
	for (; iterBegin != iterEnd;) {
		if (*iterBegin == nullptr)
			iterBegin = m_List_NavMesh.erase(iterBegin);
		else
			++iterBegin;
	}

	for (auto& pNav0 : m_List_NavMesh) {
		if (pNav0->pU != nullptr && pNav0->pB != nullptr && pNav0->pL != nullptr && pNav0->pR)
			continue;

		for (auto& pNav1 : m_List_NavMesh) {
			if (pNav0->pU == nullptr) {
				if (pNav1->vLB == pNav0->vLT && pNav1->vRB == pNav0->vRT) {
					pNav0->pU = pNav1;
					pNav1->pB = pNav0;
					continue;
				}
			}

			if (pNav0->pB == nullptr) {
				if (pNav0->vLB == pNav1->vLT && pNav0->vRB == pNav1->vRT) {
					pNav0->pB = pNav1;
					pNav1->pU = pNav0;
					continue;
				}
			}

			if (pNav0->pL == nullptr) {
				if (pNav0->vLT == pNav1->vRT && pNav0->vLB == pNav1->vRB) {
					pNav0->pL = pNav1;
					pNav1->pR = pNav0;
					continue;
				}
			}

			if (pNav0->pR == nullptr) {
				if (pNav0->vRT == pNav1->vLT && pNav0->vRB == pNav1->vLB) {
					pNav0->pR = pNav1;
					pNav1->pL = pNav0;
					continue;
				}
			}
		}
	}

	//MSG_BOX("Connecting process completed");

	return S_OK;
}

NAVMESH * Engine::CNavMesh::Get_FromPos(const My_Vec3 & vPos)
{
	NAVMESH*		pClosestNav = nullptr;
	My_Float		fClosestHeight = 0.0f;

	My_Vec3			vLT = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	My_Vec3			vRB = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	My_Float		fAverHeight = 0.0f;
	My_Float		fHeightDist = 0.0f;

	for (auto& pNav : m_List_NavMesh) {
		vLT = pNav->vLT;
		vRB = pNav->vRB;
		fAverHeight = (vLT.y + vRB.y) * 0.5f;
		if (vPos.x >= vLT.x && vPos.x <= vRB.x &&
			vPos.z >= vRB.z && vPos.z <= vLT.z) {

			fHeightDist = vPos.y - fAverHeight;

			if (fHeightDist < 0)
				continue;

			if (pClosestNav == nullptr) {
				pClosestNav = pNav;
				fClosestHeight = fAverHeight;
				continue;
			}

			if (fClosestHeight > fAverHeight) {
				pClosestNav = pNav;
				fClosestHeight = fAverHeight;
				continue;
			}
		}
	}

	return pClosestNav;
}

NAVMESH * Engine::CNavMesh::Get_First()
{
	if (m_List_NavMesh.size() != 0)
		return m_List_NavMesh.front();
	return nullptr;
}

NAVMESH * Engine::CNavMesh::Get_Last()
{
	if (m_List_NavMesh.size() != 0)
		return m_List_NavMesh.back();
	return nullptr;
}

CNavMesh * Engine::CNavMesh::Create(LPDIRECT3DDEVICE9 pDevice, const My_TChar * szPath)
{
	CNavMesh*		pInstance = new CNavMesh(pDevice);
	if (FAILED(pInstance->Initialize(szPath))) {
		delete pInstance;
		pInstance = nullptr;
	}
	return pInstance;
}

CComponent * Engine::CNavMesh::Clone()
{
	CBase::AddRef();
	return this;
}

My_uLong Engine::CNavMesh::ReleaseMemory()
{
	for (auto& pNav : m_List_NavMesh) {
		if (pNav != nullptr) {
			delete pNav;
			pNav = nullptr;
		}
	}
	m_List_NavMesh.clear();

	return CComponent::ReleaseMemory();
}
