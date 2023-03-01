#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT NativeConstruct() override;
	virtual _int Tick(_double TimeDelta) override;

	/* 현재 씬에서 보여줘야할 객체들의 렌더함수를 호출한다.  */
	virtual HRESULT Render() override;	
private:
	HRESULT Ready_Light();
private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);

private:
	HRESULT Spawn_Insect();
	bool IsSpawnTime_Insect(int iInsectIndex);
	_float3 SpawnPos_Insect(int iInsectIndex);
private:
	D3DXCOLOR	m_TimeColor[9] = {};
	struct tm*	m_pLocalTime = nullptr;
	int			m_iPrevHour = -1;
public:
	static CLevel_GamePlay* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END