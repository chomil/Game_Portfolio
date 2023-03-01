#include "stdafx.h"
#include "..\public\UI.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "Player.h"
#include "Quest.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CUI::CUI(const CUI & rhs)
	: CGameObject(rhs)
{

}

HRESULT CUI::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();


	return S_OK;
}

HRESULT CUI::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_fSizeX = g_nWinCX;
	m_fSizeY = g_nWinCY;
	m_fX = g_nWinCX >> 1;
	m_fY = g_nWinCY >> 1;	

	/* 직교투영행렬을 만들어주낟. */
	D3DXMatrixOrthoLH(&m_ProjMatrix, g_nWinCX, g_nWinCY, 0.f, 1.f);

	return S_OK;
}

_int CUI::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayer = (CPlayer*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), 0);

	CQuest* pQuest = (CQuest*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Quest"), 0);

	if ((pPlayer->Get_AnimState() == CPlayer::ANIM_STATE::IDLE || pPlayer->Get_AnimState() == CPlayer::ANIM_STATE::NET_IDLE)&& pQuest->Get_QuestOpen()==false)
	{
		m_dIdleTime += TimeDelta;
		if (m_dIdleTime > 3)
		{
			m_dMoveTime = 0;
		}
	}
	else
	{
		m_dMoveTime += TimeDelta;
		m_dIdleTime = 0;
	}

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CUI::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return 0;
}

HRESULT CUI::Render()
{ 

	__super::Render();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	time_t curTime = time(NULL);
	struct tm* pLocal = localtime(&curTime);

	D3DXFONT_DESCW tFontDesc;
	ZeroMemory(&tFontDesc, sizeof(D3DXFONT_DESCW));
	tFontDesc.Height = 50;
	tFontDesc.Width = 50;
	tFontDesc.CharSet = HANGUL_CHARSET;
	
	lstrcpy(tFontDesc.FaceName, L"a신디나루m");
	pGameInstance->Set_Font(&tFontDesc);

	LPD3DXFONT pFont = pGameInstance->Get_Font();

	int iHour = pLocal->tm_hour;
	if (iHour > 12)
	{
		iHour -= 12;
	}
	RECT rc = { 95,655,95,655 };
	wchar_t szTime[50] = { };
	swprintf(szTime, L"%02d:%02d", iHour, pLocal->tm_min);

	float fAlpha = (m_dIdleTime * 5 - 15) < 0 ? 0 : (m_dIdleTime * 5 - 15);
	if (m_dMoveTime > 0)
	{
		fAlpha = (1 - m_dMoveTime * 5) < 0 ? 0 : (1 - m_dMoveTime * 5);
	}
	fAlpha = fAlpha > 1.f ? 1.f : fAlpha;
	int iAlpha = (255.f * fAlpha);


	pFont->DrawText(NULL, szTime, -1, &rc, DT_LEFT | DT_VCENTER | DT_NOCLIP, 0x00fff9e4 | (iAlpha<<24));



	tFontDesc.Height = 30;
	tFontDesc.Width = 30;
	pGameInstance->Set_Font(&tFontDesc);
	pFont = pGameInstance->Get_Font();

	rc = { 40,660,40,660 };
	if (pLocal->tm_hour >= 12)
	{
		pFont->DrawText(NULL, L"PM", -1, &rc, DT_LEFT | DT_VCENTER | DT_NOCLIP, 0x00fff9e4 | (iAlpha << 24));
	}
	else
	{
		pFont->DrawText(NULL, L"AM", -1, &rc, DT_LEFT | DT_VCENTER | DT_NOCLIP, 0x00fff9e4 | (iAlpha << 24));
	}




	tFontDesc.Height = 30;
	tFontDesc.Width = 30;
	pGameInstance->Set_Font(&tFontDesc);
	pFont = pGameInstance->Get_Font();

	rc = { 40,600,40,600 };
	wchar_t szDay[50] = { };
	wchar_t szWeek[10] = {};
	switch (pLocal->tm_wday)
	{
	case 0:
		wcscpy(szWeek, L"(일)");
		break;
	case 1:
		wcscpy(szWeek, L"(월)");
		break;
	case 2:
		wcscpy(szWeek, L"(화)");
		break;
	case 3:
		wcscpy(szWeek, L"(수)");
		break;
	case 4:
		wcscpy(szWeek, L"(목)");
		break;
	case 5:
		wcscpy(szWeek, L"(금)");
		break;
	case 6:
		wcscpy(szWeek, L"(토)");
		break;
	default:
		break;
	}
	swprintf(szTime, L"%d월 %d일 %s", pLocal->tm_mon + 1, pLocal->tm_mday, szWeek);
	pFont->DrawText(NULL, szTime, -1, &rc, DT_LEFT | DT_VCENTER | DT_NOCLIP, 0x00fff9e4 | (iAlpha << 24));

	RELEASE_INSTANCE(CGameInstance);




	return S_OK;
}

HRESULT CUI::Add_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Texture_Default"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Inven"), TEXT("Com_Texture_Inven"), (CComponent**)&m_pTextureInvenCom)))
		return E_FAIL;

	return S_OK;
}

CUI * CUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI*	pInstance = new CUI(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI::Clone(void * pArg)
{
	CUI*	pInstance = new CUI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI::Free()
{
	CGameObject::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

}
