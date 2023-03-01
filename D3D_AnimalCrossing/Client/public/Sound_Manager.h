#pragma once
#include "Client_Defines.h"
#include "Base.h"

class CSound_Manager
{
public:
	static CSound_Manager* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSound_Manager;

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
public:
	enum CHANNELID { BGM,EVENT_BGM, PLAYER, TOOL, INSECT, EFFECT, PLAYER_EFFECT, UI, MAXCHANNEL };
private:
	CSound_Manager();
	~CSound_Manager();

public:
	void Initialize();

	void Initialize_Prev();

	void Release();
public:
	void PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVol=1.0f);
	void PlayBGM(const TCHAR* pSoundKey, float fVol = 1.0f);
	void StopSound(CHANNELID eID);
	void StopAll();

	void Set_Volume(CHANNELID eID, float fVol);

private:
	void LoadSoundFile();
	void LoadSoundFilePrev(const char* szName);

private:
	static CSound_Manager* m_pInstance;
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;

};
