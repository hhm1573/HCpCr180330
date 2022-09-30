#include "Include.h"
#include "Base.h"


class CSoundMgr : public CBase
{
	DECLARE_SINGLETON(CSoundMgr)

public:
	enum ChannelID {
		Channel_Intro, Channel_BGM, Channel_Button, Channel_Motor, Channel_Item, Channel_Player,
		 Channel_Effect, Channel_Car, Channel_PCar, Channel_END
	};

private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize();
	void Update();

public:
	void LoadSound();
	void PlayBGM(const TCHAR* pSoundKey);
	void PlaySound(const TCHAR* pSoundKey, ChannelID eChannel);
	void StopSound(ChannelID eChannel);
	void StopAll();


private:
	FMOD_SYSTEM*		m_pSystem;			// 사운드를 총괄 관리자
	FMOD_CHANNEL*	m_pChannel[Channel_END];	// 사운드를 재생할 채널 집합
	map<const TCHAR*, FMOD_SOUND*>	m_MapSound;

public:
	virtual Engine::My_uLong ReleaseMemory(void);
};


