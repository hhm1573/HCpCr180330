#include "..\Header\SceneMgr.h"
#include "Scene.h"

USING(Engine)
IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
{
	m_pCurScene = nullptr;
}

CSceneMgr::~CSceneMgr()
{
}

HRESULT CSceneMgr::SetUp_CurrentScene(CScene * pNewScene)
{
	if (nullptr == pNewScene)
		return E_FAIL;

	My_uLong		dwRefCnt = 0;

	if (nullptr != m_pCurScene)
		dwRefCnt = Safe_Release(m_pCurScene);

	if (0 != dwRefCnt)
		return E_FAIL;

	m_pCurScene = pNewScene;

	return NOERROR;
}

My_Int CSceneMgr::Update(const My_Float & fTimeDelta)
{
	My_Int		iExitCode = 0;

	if (nullptr == m_pCurScene)
		return -1;

	iExitCode = m_pCurScene->Update(fTimeDelta);
	if (iExitCode & 0x80000000)
		return -1;

	iExitCode = m_pCurScene->LastUpdate(fTimeDelta);
	if (iExitCode & 0x80000000)
		return -1;

	return My_Int();
}

void CSceneMgr::Render()
{
	if (nullptr == m_pCurScene)
		return;

	m_pCurScene->Render();
}

My_uLong CSceneMgr::ReleaseMemory()
{
	My_uLong	dwRefCnt = 0;

	if (dwRefCnt = Safe_Release(m_pCurScene))
		return dwRefCnt;

	return dwRefCnt;
}
