#include "stdafx.h"
#include "..\public\Level_GamePlay.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Insect.h"
#include "Transition.h"


CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_GamePlay::NativeConstruct()
{
	__super::NativeConstruct();

	if (FAILED(Ready_Light()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransition* pTransition = (CTransition*)pGameInstance->Get_ObjectPtr(LEVEL_STATIC, TEXT("Layer_Transition"));
	pTransition->Set_Transition(1);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_int CLevel_GamePlay::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	/* 현재 시간에 따른 조명 셋팅*/
	D3DLIGHT9			LightDesc;
	ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));
	LightDesc.Type = D3DLIGHT_DIRECTIONAL;

	D3DXCOLOR MixedColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	time_t curTime = time(NULL);
	m_pLocalTime = localtime(&curTime);

	if (m_iPrevHour != m_pLocalTime->tm_hour) //매시간에따른 BGM
	{
		m_iPrevHour = m_pLocalTime->tm_hour;
		_tchar szHour[50];
	
		swprintf(szHour, L"BGM_24Hour_%02d_Sunny.wav", m_pLocalTime->tm_hour);

		CSound_Manager::Get_Instance()->PlayBGM(szHour);
	}

	int curTimeIndex = (m_pLocalTime->tm_hour) / 3; //3시간 단위로 나눈 조명 프리셋 인덱스
	int iMinute = ((m_pLocalTime->tm_hour) % 3) * 60 + m_pLocalTime->tm_min;
	//두 프리셋 사이의 현재 시간 만큼 선형 보간
	D3DXColorLerp(&MixedColor, &m_TimeColor[curTimeIndex], &m_TimeColor[curTimeIndex + 1], (float)iMinute / 180.f);

	LightDesc.Diffuse = D3DXCOLOR(MixedColor.r, MixedColor.g, MixedColor.b, 1.f);
	LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	//시간에따른 광원 방향 이동
	float fRadian = D3DXToRadian((float(m_pLocalTime->tm_hour * 60 + m_pLocalTime->tm_min) / 1440.f) * 360.f);
	LightDesc.Direction = _float3(-sinf(fRadian)*2, -2.f, cosf(fRadian) + 2.f);

	if (FAILED(pGameInstance->Set_LightDesc(LightDesc))) //현재 시간과 동기화 된 조명으로 세팅
		return E_FAIL;


	/* 스폰할 객체들 스폰*/

	//시간에 따른 곤충 스폰 수 조절
	int iNumToSpawn = 7 - (int)(fabsf(12 - (m_pLocalTime->tm_hour)) / 12.f * 4.f);

	if (pGameInstance->Get_ObjectNumInLayer(LEVEL_GAMEPLAY0, TEXT("Layer_Insect")) < iNumToSpawn)
	{
		Spawn_Insect();
	}



	RELEASE_INSTANCE(CGameInstance);



	return _int();
}

HRESULT CLevel_GamePlay::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Light()
{
	m_TimeColor[0] = D3DXCOLOR(0.3f, 0.3f, 0.5f, 1.f);		//0시
	m_TimeColor[1] = D3DXCOLOR(0.4f, 0.4f, 0.5f, 1.f);		//3시
	m_TimeColor[2] = D3DXCOLOR(0.85f, 0.95f, 0.8f, 1.f);	//6시
	m_TimeColor[3] = D3DXCOLOR(0.95f, 0.95f, 0.95f, 1.f);	//9시
	m_TimeColor[4] = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);			//12시
	m_TimeColor[5] = D3DXCOLOR(1.f, 1.f, 0.9f, 1.f);		//15시
	m_TimeColor[6] = D3DXCOLOR(0.9f, 0.7f, 0.7f, 1.f);		//18시
	m_TimeColor[7] = D3DXCOLOR(0.5f, 0.5f, 0.6f, 1.f);		//21시
	m_TimeColor[8] = D3DXCOLOR(0.3f, 0.3f, 0.5f, 1.f);		//24시


	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* 방향성 광원을 셋팅한다. */
	D3DLIGHT9			LightDesc;
	ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));
	LightDesc.Type = D3DLIGHT_DIRECTIONAL;
	LightDesc.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.f);
	LightDesc.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.f);
	LightDesc.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	LightDesc.Direction = _float3(1.f, -1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pGraphic_Device, LightDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vEye = _float3(0.0f, 5.0f, -8.f);
	CameraDesc.fFovy = D3DXToRadian(45.f);
	CameraDesc.fAspect = (_float)g_nWinCX / g_nWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Camera_Free"), pLayerTag, &CameraDesc);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* 플레이어 객체를 생성ㅇㅎ나다. */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Player"), pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Player_Hair"), pLayerTag)))
		return E_FAIL;

	/* 스워드 객체를 생성ㅇㅎ나다. */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Sword"), pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;

}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* 지형 객체 */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Terrain"), pLayerTag)))
		return E_FAIL;



	/* 하늘 객체 */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Sky"), pLayerTag)))
		return E_FAIL;

	/* 나무 객체 */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Tree"), TEXT("Layer_Tree"), _float3(100.f, 2.5f, 100.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Tree"), TEXT("Layer_Tree"), _float3(80.f, 2.5f, 90.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Tree"), TEXT("Layer_Tree"), _float3(110.f, 2.5f, 90.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Tree"), TEXT("Layer_Tree"), _float3(90.f, 2.5f, 120.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Tree"), TEXT("Layer_Tree"), _float3(90.f, 2.5f, 70.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Tree"), TEXT("Layer_Tree"), _float3(160.f, 2.5f, 70.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Tree"), TEXT("Layer_Tree"), _float3(170.f, 2.5f, 90.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Tree"), TEXT("Layer_Tree"), _float3(188.f, 2.5f, 80.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Tree"), TEXT("Layer_Tree"), _float3(70.f, 2.5f, 180.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Tree"), TEXT("Layer_Tree"), _float3(100.f, 2.5f, 160.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Tree"), TEXT("Layer_Tree"), _float3(140.f, 2.5f, 180.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Tree"), TEXT("Layer_Tree"), _float3(150.f, 2.5f, 160.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Tree"), TEXT("Layer_Tree"), _float3(180.f, 2.5f, 170.f))))
		return E_FAIL;


	//X,Z) 50~200
	for (int i = 0; i < 50; i++)
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Weed"), TEXT("Layer_Weed"), _float3(50.f + (rand() % 75) * 2, 2.5f, 50.f + (rand() % 75) * 2))))
			return E_FAIL;
	}
	for (int i = 0; i < 10; i++)
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Flower"), TEXT("Layer_Flower"), _float3(50.f + (rand() % 75) * 2, 2.5f, 50.f + (rand() % 75) * 2))))
			return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_NoticeBoard"), TEXT("Layer_Building"), _float3(120.f, 2.5f, 104.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_NookTent"), TEXT("Layer_Building"), _float3(140.f, 2.5f, 100.f))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* 유아이객체를 생성한다.  */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_UI"), pLayerTag)))
		return E_FAIL;

	/* 유아이객체를 생성한다.  */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Inven"), TEXT("Layer_Inven"))))
		return E_FAIL;

	/* 유아이객체를 생성한다.  */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Quest"), TEXT("Layer_Quest"))))
		return E_FAIL;

	/* 유아이객체를 생성한다.  */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_TextBalloon"), TEXT("Layer_TextBalloon"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Spawn_Insect()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CInsect::INSECT_DESC tagInsectDesc;
	tagInsectDesc.iInsectIndex = rand() % 10;

	while (!IsSpawnTime_Insect(tagInsectDesc.iInsectIndex))
	{
		tagInsectDesc.iInsectIndex = rand() % 10;
	}
	tagInsectDesc.vSpawnPos = SpawnPos_Insect(tagInsectDesc.iInsectIndex);

	int iInsectNum = pGameInstance->Get_ObjectNumInLayer(LEVEL_GAMEPLAY0, TEXT("Layer_Insect"));
	for (int i = 0; i < iInsectNum; i++)
	{
		CTransform* pTransCom = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Insect"), TEXT("Com_Transform"), i);
		_float3 fDist = pTransCom->Get_State(CTransform::STATE::STATE_POSITION) - tagInsectDesc.vSpawnPos;
		if (D3DXVec3Length(&fDist) < 1.f)
		{
			RELEASE_INSTANCE(CGameInstance);
			return S_OK;
		}
	}


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Insect"), TEXT("Layer_Insect"), &tagInsectDesc)))
		return E_FAIL;



	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

bool CLevel_GamePlay::IsSpawnTime_Insect(int iInsectIndex)
{
	int StartTime = 0;
	int EndTime = 24;
	switch (iInsectIndex)
	{
	case 0:
		break;
	case 1:
		StartTime = 8;
		EndTime = 17;
		break;
	case 2:
	case 3:
		StartTime = 4;
		EndTime = 19;
		break;
	case 4:
		StartTime = 17;
		EndTime = 8;
		break;
	case 5:
	case 6:
	case 7:
		StartTime = 8;
		EndTime = 17;
		break;
	case 8:
		StartTime = 19;
		EndTime = 8;
		break;
	case 9:
		StartTime = 19;
		EndTime = 4;
		break;
	default:
		break;
	}

	if (StartTime < EndTime)
	{
		if (m_pLocalTime->tm_hour >= StartTime && m_pLocalTime->tm_hour < EndTime)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (m_pLocalTime->tm_hour >= StartTime || m_pLocalTime->tm_hour < EndTime)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

_float3 CLevel_GamePlay::SpawnPos_Insect(int iInsectIndex)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	_float3 vPos = {};
	switch (iInsectIndex)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		//공중 스폰
		vPos = _float3(50.f + (rand() % 75) * 2, 7.f, 50.f + (rand() % 75) * 2);
		break;
	case 6:
	case 7:
	case 8:
		//꽃 스폰
	{
		int iFlowerNum = pGameInstance->Get_ObjectNumInLayer(LEVEL_GAMEPLAY0, TEXT("Layer_Flower"));
		CTransform* pTrans = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Flower"), TEXT("Com_Transform"), rand() % iFlowerNum);
		vPos = pTrans->Get_State(CTransform::STATE_POSITION);
		vPos.y = vPos.y + 1.f; 
	}
		break;
	case 9:
		//나무 스폰
	{
		int iTreeNum = pGameInstance->Get_ObjectNumInLayer(LEVEL_GAMEPLAY0, TEXT("Layer_Tree"));
		CTransform* pTrans = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Tree"), TEXT("Com_Transform"), rand() % iTreeNum);
		vPos = pTrans->Get_State(CTransform::STATE_POSITION);
		vPos.z = vPos.z - 1.5f;
		vPos.y = vPos.y + 3.f; 
	}
		break;
	default:
		break;
	}

	RELEASE_INSTANCE(CGameInstance);
	return vPos;
}



CLevel_GamePlay* CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_GamePlay");
		Safe_Release(pInstance);
	}


	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();




}
