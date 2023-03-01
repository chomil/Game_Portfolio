#include "stdafx.h"
#include "..\public\Level_Logo.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Level_Loading.h"

#include "Transition.h"

CLevel_Logo::CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_Logo::NativeConstruct()
{
	__super::NativeConstruct();

	/* 현재 레벨에 사용할 객체(플레이어, 몬스터, 건물, 지형 등) 들을 생성한다. */
	/* 각 객체에게 장치를 전달한다. */
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransition* pTransition = (CTransition*)pGameInstance->Get_ObjectPtr(LEVEL_STATIC, TEXT("Layer_Transition"));
	pTransition->Set_Transition(1);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_int CLevel_Logo::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 스테이지 레벨로 넘어간다.  */	
	if (pGameInstance->Key_PressOnce(DIK_RETURN))
	{
		CTransition* pTransition = (CTransition*)pGameInstance->Get_ObjectPtr(LEVEL_STATIC, TEXT("Layer_Transition"));
		pTransition->Set_Transition(-1);

		CSound_Manager::Get_Instance()->PlaySound(TEXT("UI_Decide_Title.wav"), CSound_Manager::CHANNELID::EFFECT);
		

		if (SUCCEEDED(pGameInstance->SetUp_Level(CLevel_Loading::Create(m_pGraphic_Device, LEVEL_GAMEPLAY0))))
		{
			pGameInstance->Clear(LEVEL_LOGO);
			goto succeeded;
		}
	}
	
	RELEASE_INSTANCE(CGameInstance);
	return -1;	

succeeded:
	RELEASE_INSTANCE(CGameInstance);
	return _int();
}

HRESULT CLevel_Logo::Render()
{
	__super::Render();




	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_BackGround"), pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_Transition"), TEXT("Layer_Transition"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}


CLevel_Logo * CLevel_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_Logo");
		Safe_Release(pInstance);
	}


	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
	


	
}
