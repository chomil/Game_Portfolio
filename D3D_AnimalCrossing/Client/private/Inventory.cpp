#include "stdafx.h"
#include "Inventory.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "Player.h"

CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CInventory::CInventory(const CInventory & rhs)
	: CGameObject(rhs)
{

}

HRESULT CInventory::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();


	return S_OK;
}

HRESULT CInventory::NativeConstruct(void * pArg)
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

_int CInventory::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	m_dTimeOpen += TimeDelta;
	float fSize = (m_dTimeOpen * 10 > 1.f) ? 1.f : m_dTimeOpen * 10;
	if (m_bInvenOpen)
	{
		m_fSizeX = g_nWinCX * fSize;
		m_fSizeY = g_nWinCY * fSize;
	}
	else
	{
		m_fSizeX = g_nWinCX * (1 - fSize);
		m_fSizeY = g_nWinCY * (1 - fSize);
	}
	return _int();
}

_int CInventory::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return 0;
}

HRESULT CInventory::Render()
{

	__super::Render();


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pShaderCom->Begin_Shader(0);

	if (m_bInvenOpen || m_dTimeOpen * 10 < 1.f)
	{
		_matrix			TransformMatrix, ViewMatrix, ProjMatrix;

		D3DXMatrixIdentity(&TransformMatrix);

		/* 로컬스페이스 기준으로 크기변환과 위치변환만 셋팅한다. */
		TransformMatrix._11 = m_fSizeX;
		TransformMatrix._22 = m_fSizeY;

		TransformMatrix._41 = m_fX - (g_nWinCX >> 1);
		TransformMatrix._42 = -m_fY + (g_nWinCY >> 1);


		/* 뷰변환행렬. */
		D3DXMatrixIdentity(&ViewMatrix);


		/* 기본 변환. */
		m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", &TransformMatrix, sizeof(_matrix));
		m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &ViewMatrix, sizeof(_matrix));
		m_pShaderCom->SetUp_ConstantTable("g_ProjMatrix", &m_ProjMatrix, sizeof(_matrix));
		m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureInvenCom,0);
		m_pShaderCom->Commit();

		m_pVIBufferCom->Render_VIBuffer();

		if (m_bInvenOpen && m_dTimeOpen * 10 > 1.f)
		{
			CPlayer* pPlayer = (CPlayer*) pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), 0);

			D3DXFONT_DESCW tFontDesc;
			ZeroMemory(&tFontDesc, sizeof(D3DXFONT_DESCW));
			tFontDesc.Height = 35;
			tFontDesc.Width = 35;
			tFontDesc.CharSet = HANGUL_CHARSET;
			lstrcpy(tFontDesc.FaceName, L"a신디나루m");
			pGameInstance->Set_Font(&tFontDesc);
			LPD3DXFONT pFont = pGameInstance->Get_Font();

			RECT rc = { 390,400,390,400 };
			wchar_t szMoney[10] = { };
			swprintf(szMoney, L"%d", pPlayer->Get_Money());
			pFont->DrawText(NULL, szMoney, -1, &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, 0xfffff9e4);

		}
	}

	m_pShaderCom->End_Shader();

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CInventory::Add_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Inven"), TEXT("Com_Texture_Inven"), (CComponent**)&m_pTextureInvenCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_Rect"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	return S_OK;
}

CInventory * CInventory::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CInventory*	pInstance = new CInventory(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CInventory");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CInventory::Clone(void * pArg)
{
	CInventory*	pInstance = new CInventory(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CInventory");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CInventory::Free()
{
	CGameObject::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureInvenCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

}
