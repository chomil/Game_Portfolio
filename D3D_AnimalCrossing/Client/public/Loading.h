#pragma once

/* 로딩화면이 렌더링될때 내부적으로 리소스 객등등 로드 작업을 수행하는객체 .*/

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CLoading final : public CBase	
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoading() = default;
public:
	_tchar* Get_SystemMessage() {
		return m_szSystemMessage;
	}

	bool Get_Finished() const {
		return m_isFinish;
	}
public:
	HRESULT NativeConstruct(LEVEL eLevel);
private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_CS;
	LEVEL				m_eNextLevel = LEVEL_END;

	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;

private:
	_tchar				m_szSystemMessage[MAX_PATH];
	bool				m_isFinish = false;

	int					m_iLoadingProgess = 0;
public:
	static unsigned __stdcall Thread_Main(void* pArg);
private:
	HRESULT Ready_GamePlay0();
	HRESULT Ready_GamePlay1();
public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel);
	virtual void Free() override;

};

END