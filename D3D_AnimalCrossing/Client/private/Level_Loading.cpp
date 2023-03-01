#include "stdafx.h"
#include "..\public\Level_Loading.h"
#include "Loading.h"
#include "GameInstance.h"
#include "Level_GamePlay.h"

CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel)
{
	m_eNextLevel = eNextLevel;

	/* 로딩씬이 생성될때 로더를 하나 생성하낟. */
	/* 다음넘어갈씬에 필요한 객체원형, 자원원형 생성. */
	m_pLoader = CLoading::Create(m_pGraphic_Device, m_eNextLevel);

	if (nullptr == m_pLoader)
		return E_FAIL;

	/* 실제 게임내에 출현할 객체의 복사본을 생성한다. */
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UserInterface(TEXT("Layer_UserInterface"))))
		return E_FAIL;

	return S_OK;
}

_int CLevel_Loading::Tick(_double TimeDelta)
{
	if (true == m_pLoader->Get_Finished())
	{
		SetWindowText(g_hWnd, m_pLoader->Get_SystemMessage());

		CGameInstance*	pGameInstance = CGameInstance::Get_Instance();

		CLevel*		pLevel = nullptr;

		switch(m_eNextLevel)
		{ 
		case LEVEL_GAMEPLAY0:
			pLevel = CLevel_GamePlay::Create(m_pGraphic_Device);
			break;
		case LEVEL_GAMEPLAY1:
			break;
		}

		pGameInstance->Clear(LEVEL_LOADING);
		pGameInstance->SetUp_Level(pLevel);
	}

	return _int();
}

HRESULT CLevel_Loading::Render()
{
	SetWindowText(g_hWnd, m_pLoader->Get_SystemMessage());

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* For.BackGround */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, TEXT("Prototype_BackGround"), pLayerTag, nullptr)))
		return E_FAIL;



	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_UserInterface(const _tchar * pLayerTag)
{
	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSGBOX("Failed to Creating CLevel_Loading");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);

}
