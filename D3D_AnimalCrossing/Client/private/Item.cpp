#include "stdafx.h"
#include "Item.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "Inventory.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CItem::CItem(const CItem& rhs)
	: CGameObject(rhs)
{

}

HRESULT CItem::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();


	return S_OK;
}

HRESULT CItem::NativeConstruct(void* pArg)
{
	__super::NativeConstruct(pArg);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_ItemDesc = *(ITEM_DESC*)pArg;

	_tchar szItemTexName[20] = {};
	_tchar szIndex[20] = {}; 

	lstrcpy(szItemTexName, TEXT("item"));
	_itot_s(m_ItemDesc.m_iItemIndex, szIndex,10);
	lstrcat(szItemTexName, szIndex);

	_tchar szProtoName[50] = {};

	lstrcpy(szProtoName, TEXT("Prototype_Texture_"));
	lstrcat(szProtoName, szItemTexName);


	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, szProtoName, TEXT("Com_Texture_Item"), (CComponent**)&m_pTextureItem)))
		return E_FAIL;


	if (m_ItemDesc.m_iInvenIndex >= 0)
	{
		int iIndexVer = m_ItemDesc.m_iInvenIndex % 10;
		m_fX = 340 + iIndexVer * 67;
		if (iIndexVer == 0 || iIndexVer == 9)
		{
			m_fY = 203;
		}
		else if (iIndexVer == 1 || iIndexVer == 8)
		{
			m_fY = 187;
		}
		else if (iIndexVer == 2 || iIndexVer == 7)
		{
			m_fY = 174;
		}
		else if (iIndexVer == 3 || iIndexVer == 6)
		{
			m_fY = 167;
		}
		else if (iIndexVer == 4 || iIndexVer == 5)
		{
			m_fY = 162;
		}
		m_fY += (m_ItemDesc.m_iInvenIndex / (int)10) * 77;
	}
	m_fSizeX = 64;
	m_fSizeY = 64;





	switch (m_ItemDesc.m_iItemIndex)
	{
	case 0:
		lstrcpy(m_szName, TEXT("실잠자리"));
		break;
	case 1:
		lstrcpy(m_szName, TEXT("왕잠자리"));
		break;
	case 2:
		lstrcpy(m_szName, TEXT("배추흰나비"));
		break;
	case 3:
		lstrcpy(m_szName, TEXT("호랑나비"));
		break;
	case 4:
		lstrcpy(m_szName, TEXT("몰포나비"));
		break;
	case 5:
		lstrcpy(m_szName, TEXT("꿀벌"));
		break;
	case 6:
		lstrcpy(m_szName, TEXT("사마귀"));
		break;
	case 7:
		lstrcpy(m_szName, TEXT("무당벌레"));
		break;
	case 8:
		lstrcpy(m_szName, TEXT("인면노린재"));
		break;
	case 9:
		lstrcpy(m_szName, TEXT("아틀라스나방"));
		break;
	case 10:
		lstrcpy(m_szName, TEXT("잡초"));
		break;
	case 11:
		lstrcpy(m_szName, TEXT("복숭아"));
		break;
	case 12:
		lstrcpy(m_szName, TEXT("나뭇가지"));
		break;
	case 13:
		lstrcpy(m_szName, TEXT("잠자리채"));
		break;
	default:
		break;
	}

	/* 직교투영행렬을 만들어주낟. */
	D3DXMatrixOrthoLH(&m_ProjMatrix, g_nWinCX, g_nWinCY, 0.f, 1.f);


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_int CItem::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CInventory* pInven = (CInventory*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Inven"));
	m_bIsShow = pInven->Get_InvenOpen();

	RELEASE_INSTANCE(CGameInstance);
	return _int();
}

_int CItem::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return 0;
}

HRESULT CItem::Render()
{

	__super::Render();

	if (m_bIsShow == false)
	{
		return S_OK;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pShaderCom->Begin_Shader(0);

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
	m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureItem, 0);
	m_pShaderCom->Commit();

	m_pVIBufferCom->Render_VIBuffer();

	m_pShaderCom->End_Shader();

	if (m_iStackNum==1)
	{
		RELEASE_INSTANCE(CGameInstance);
		return S_OK;
	}

	D3DXFONT_DESCW tFontDesc;
	ZeroMemory(&tFontDesc, sizeof(D3DXFONT_DESCW));
	tFontDesc.Height = 25;
	tFontDesc.Width = 25;
	tFontDesc.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontDesc.FaceName, L"a신디나루m");
	pGameInstance->Set_Font(&tFontDesc);
	LPD3DXFONT pFont = pGameInstance->Get_Font();

	wchar_t szNum[10] = { };
	swprintf(szNum, L"%d", m_iStackNum);
	for (int i = -2; i <= 2; i += 2)
	{
		for (int j = -2; j <= 2; j += 2)
		{
			if (i + j == 4 || i + j == 0)
			{
				continue;
			}
			RECT rc = { m_fX + 15 + i,m_fY + 15 + j,m_fX + 15 + i,m_fY + 15 + j };
			pFont->DrawText(NULL, szNum, -1, &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, 0xfffff9e4);
		}
	}
	

	RECT rc = { m_fX + 15,m_fY + 15,m_fX + 15,m_fY + 15 };
	pFont->DrawText(NULL, szNum, -1, &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, 0xff7C6839);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CItem::Add_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_Rect"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;


	return S_OK;
}

CItem* CItem::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem* pInstance = new CItem(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CItem");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CItem::Clone(void* pArg)
{
	CItem* pInstance = new CItem(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CItem");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CItem::Free()
{
	CGameObject::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureItem);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

}
