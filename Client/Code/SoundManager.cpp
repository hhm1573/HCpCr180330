#include "stdafx.h"
#include "..\Header\SoundManager.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
{
}


CSoundMgr::~CSoundMgr()
{
	Release();
}

void CSoundMgr::Initialize()
{
	FMOD_System_Create(&m_pSystem);
	FMOD_System_Init(m_pSystem, Channel_END, FMOD_INIT_NORMAL, nullptr);

	LoadSound();
}

void CSoundMgr::Update()
{
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::LoadSound()
{
	_finddata_t fd;

	long handle = 0;
	int iResult = 0;

	// _findfirst: 지정한 경로 상에서 제일 첫번째 파일을 찾아서 
	// _finddata_t 구조체에 기록하는 함수
	// 찾지 못한다면 -1을 반환한다.
	handle = _findfirst("../Bin/Sound/*.*", &fd);

	if (-1 == handle)
	{
		MessageBox(g_hWnd, L"Not Found Sound File!!", L"Not Found Sound File!!", MB_OK);
		return;
	}

	while (-1 != iResult)
	{
		char szFullPath[128] = "../Bin/Sound/";
		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound = nullptr;
		FMOD_RESULT result = FMOD_System_CreateSound(m_pSystem, szFullPath
			, FMOD_HARDWARE, nullptr, &pSound);

		if (FMOD_OK == result)
		{
			int iSize = strlen(fd.name) + 1;
			TCHAR* pSoundKey = new TCHAR[iSize];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iSize);

			MultiByteToWideChar(CP_ACP, 0, fd.name, strlen(fd.name),
				pSoundKey, strlen(fd.name));

			m_MapSound.insert(make_pair(pSoundKey, pSound));
		}

		// _findnext가 -1을 반환하면 더이상 찾을 내용이 없다는 의미.
		iResult = _findnext(handle, &fd);
	}

	_findclose(handle);

	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlayBGM(const TCHAR * pSoundKey)
{
	auto iter_find = find_if(m_MapSound.begin(), m_MapSound.end(),
		[&](auto& Mypair)
	{
		if (!lstrcmp(pSoundKey, Mypair.first))
			return true;

		return false;
	});

	if (m_MapSound.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter_find->second, FALSE,
		&(m_pChannel[Channel_BGM]));

	FMOD_Channel_SetMode(m_pChannel[Channel_BGM], FMOD_LOOP_NORMAL);
}

void CSoundMgr::PlaySound(const TCHAR * pSoundKey, ChannelID eChannel)
{
	auto iter_find = find_if(m_MapSound.begin(), m_MapSound.end(),
		[&](auto& Mypair)
	{
		if (!lstrcmp(pSoundKey, Mypair.first))
			return true;

		return false;
	});

	if (m_MapSound.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter_find->second, FALSE,
		&(m_pChannel[eChannel]));
}

void CSoundMgr::StopSound(ChannelID eChannel)
{
	FMOD_Channel_Stop(m_pChannel[eChannel]);
}

void CSoundMgr::StopAll()
{
	for (auto channel : m_pChannel)
		FMOD_Channel_Stop(channel);
}

Engine::My_uLong CSoundMgr::ReleaseMemory(void)
{
	for_each(m_MapSound.begin(), m_MapSound.end(),
		[](auto& MyPair)
	{
		delete[] MyPair.first;
		FMOD_Sound_Release(MyPair.second);
	});

	m_MapSound.clear();

	FMOD_System_Close(m_pSystem);
	FMOD_System_Release(m_pSystem);

	return Engine::My_uLong();
}
