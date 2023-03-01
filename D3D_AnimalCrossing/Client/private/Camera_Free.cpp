#include "stdafx.h"
#include "..\public\Camera_Free.h"
#include "GameInstance.h"

CCamera_Free::CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Free::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CCamera_Free::NativeConstruct(void * pArg)
{



	if (FAILED(Add_Components()))
		return E_FAIL;

	__super::NativeConstruct(pArg);

	return S_OK;
}

_int CCamera_Free::Tick(_double TimeDelta)
{
	//POINT		ptPos = { g_nWinCX >> 1, g_nWinCY >> 1 };

	//ClientToScreen(g_hWnd, &ptPos);

	//SetCursorPos(ptPos.x, ptPos.y);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CTransform* pPlayerTransformCom = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransformCom->Get_State(CTransform::STATE::STATE_POSITION);

	if (pGameInstance->Key_PressOnce(DIK_0))
	{
		Set_CamPreset(0);
	}

	if (pGameInstance->Key_PressOnce(DIK_1))
	{
		Set_CamPreset(1);
	}

	if (pGameInstance->Key_PressOnce(DIK_2))
	{
		Set_CamPreset(2);
	}

	
	//전환시간 1초 기준..0.25초만에 전환
	Moving_Cam(TimeDelta/0.25);

	RELEASE_INSTANCE(CGameInstance);
	
	return __super::Tick(TimeDelta);
	
}
HRESULT CCamera_Free::Set_CamPreset(int iCamPreset, bool bForce)
{
	if (m_bMoving == true&& bForce==false)
	{
		return E_FAIL;
	}		
	if (m_iCurrentCamPreset != iCamPreset)
	{
		if (m_iCurrentCamPreset == 0)
		{
			CSound_Manager::Get_Instance()->PlaySound(TEXT("System_Camera_Move_Zoom_In.wav"), CSound_Manager::CHANNELID::EFFECT);
		}
		else
		{
			CSound_Manager::Get_Instance()->PlaySound(TEXT("System_Camera_Move_Zoom_Out.wav"), CSound_Manager::CHANNELID::EFFECT);
		}
	}

	m_iCurrentCamPreset = iCamPreset;
	if (m_iPrevCamPreset != m_iCurrentCamPreset)
	{
		m_bMoving = true;
		m_dMovingTime = 0;
	}
	return S_OK;
}

void CCamera_Free::Moving_Cam(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	CTransform* pPlayerTransformCom = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransformCom->Get_State(CTransform::STATE::STATE_POSITION);

	if (m_bMoving == false && m_iCurrentCamPreset == 0)
	{
		_float3 vCamPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
		_float3 vNewCamPos = vPlayerPos + _float3(0, 12, -18);
		_float3 vDir = vNewCamPos - vCamPos;


		if (D3DXVec3Length(&(vDir)) > 5.f)
		{
			m_vDefaultPos = vNewCamPos;
		}
		else
		{
			if (D3DXVec3Length(&(vDir)) > 1.5f)
			{
				vCamPos += vDir;
				D3DXVec3Normalize(&vDir, &vDir);
				vCamPos -= vDir * 1.5f;
			}
			m_vDefaultPos = vCamPos;
		}
	}

	if (m_bMoving == false) //카메라가 움직이는 중이 아니라면
	{
		//현재 프리셋으로 카메라 위치 동기화
		_float3 vCurPos = Get_PresetPos(m_iCurrentCamPreset);
		if (m_iCurrentCamPreset == 0)
		{
			vCurPos = m_vDefaultPos;
		}
		m_pTransform->Set_State(CTransform::STATE::STATE_POSITION, vCurPos);
		m_pTransform->SetUp_Rotation(_float3(1, 0, 0), Get_PresetAngle(m_iCurrentCamPreset));
	}
	else //카메라가 움직이는 중이면 (다른 카메라 프리셋으로 전환 중)
	{
		_float3 vPrevPos = Get_PresetPos(m_iPrevCamPreset);
		if (m_iPrevCamPreset == 0)
		{
			vPrevPos = m_vDefaultPos;
		}

		//이전 프리셋과 현재 프리셋을 선형 보간하여 현재 카메라 위치 동기화
		_float3 vCamPos = (1 - m_dMovingTime) * vPrevPos + 
								m_dMovingTime * (Get_PresetPos(m_iCurrentCamPreset));

		float vCamAngle = (1 - m_dMovingTime) * Get_PresetAngle(m_iPrevCamPreset) + 
								m_dMovingTime * (Get_PresetAngle(m_iCurrentCamPreset));

		m_pTransform->Set_State(CTransform::STATE::STATE_POSITION, vCamPos);
		m_pTransform->SetUp_Rotation(_float3(1, 0, 0), vCamAngle);
		
		m_dMovingTime += TimeDelta;
	}
	if (m_dMovingTime > 1.0) //전환이 끝난 후
	{
		//전환 종료후 이전 프리셋 번호에 현재 프리셋 번호를 저장
		m_bMoving = false;
		m_dMovingTime = 0;
		m_iPrevCamPreset = m_iCurrentCamPreset;
	}

	RELEASE_INSTANCE(CGameInstance);
}

_float3 CCamera_Free::Get_PresetPos(int iCamPreset)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pPlayerTransformCom = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
	
	_float3 vCamPos;
	if (iCamPreset == 0)
	{
		vCamPos = vPlayerPos + _float3(0, 12, -18);
	}
	else if (iCamPreset == 1)
	{
		vCamPos = vPlayerPos + _float3(0, 9, -15);
	}
	else if(iCamPreset == 2)
	{
		vCamPos = vPlayerPos + _float3(0, 7, -13);
	}

	RELEASE_INSTANCE(CGameInstance);

	return vCamPos;
}

float CCamera_Free::Get_PresetAngle(int iCamPreset)
{
	float vCamAngle;
	if (iCamPreset == 0)
	{
		vCamAngle = D3DXToRadian(30);
	}
	else if (iCamPreset == 1)
	{
		vCamAngle = D3DXToRadian(15);
	}
	else if (iCamPreset == 2)
	{
		vCamAngle = D3DXToRadian(25);
	}
	return vCamAngle;
}

_int CCamera_Free::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	return _int();
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

HRESULT CCamera_Free::Add_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC			TransformDesc;
	TransformDesc.fSpeedPerSec = 10.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}


CCamera_Free * CCamera_Free::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Free*	pInstance = new CCamera_Free(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CCamera_Free");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCamera_Free::Clone(void * pArg)
{
	CCamera_Free*	pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CCamera_Free");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();
}
