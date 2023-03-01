#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* 로딩씬 */
/* 로딩화면? */

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
public:
	CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Loading() = default;
public:
	virtual HRESULT NativeConstruct(LEVEL eNextLevel);
	virtual _int Tick(_double TimeDelta) override;	
	virtual HRESULT Render() override;
private:
	class CLoading*			m_pLoader = nullptr;
	LEVEL					m_eNextLevel = LEVEL_END;
private:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UserInterface(const _tchar* pLayerTag);

public:
	static CLevel_Loading* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel);
	virtual void Free() override;
};

END