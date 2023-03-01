#include "stdafx.h"
#include "TextBalloon.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "Player.h"

CTextBalloon::CTextBalloon(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CTextBalloon::CTextBalloon(const CTextBalloon & rhs)
	: CGameObject(rhs)
{

}

HRESULT CTextBalloon::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();


	return S_OK;
}

HRESULT CTextBalloon::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_fSizeX = g_nWinCX;
	m_fSizeY = g_nWinCY/2;
	m_fX = g_nWinCX >> 1;
	m_fY = (g_nWinCY >> 1) + m_fSizeY / 2;

	/* 직교투영행렬을 만들어주낟. */
	D3DXMatrixOrthoLH(&m_ProjMatrix, g_nWinCX, g_nWinCY, 0.f, 1.f);


	Set_Text(1);
	return S_OK;
}

_int CTextBalloon::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);


	m_dTimeShow += TimeDelta;
	float fSize = (m_dTimeShow * 10 > 1.f) ? 1.f : m_dTimeShow * 10;
	if (m_bShowBalloon)
	{
		m_fSizeX = g_nWinCX * fSize;
		m_fSizeY = g_nWinCY / 2 * fSize;
	}
	else
	{
		m_fSizeX = g_nWinCX * (1 - fSize);
		m_fSizeY = g_nWinCY / 2 * (1 - fSize);
	}


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_dTimeShow > 2.0f && m_bShowBalloon==true)
	{
		if (pGameInstance->Key_PressOnce(DIK_SPACE))
		{
			Set_ShowBalloon(false);

			CSound_Manager::Get_Instance()->PlaySound(TEXT("UI_Cancel.wav"), CSound_Manager::CHANNELID::UI);
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::CHANNELID::EVENT_BGM);
			CSound_Manager::Get_Instance()->Set_Volume(CSound_Manager::CHANNELID::BGM, 0.5f);
		}
	}
	RELEASE_INSTANCE(CGameInstance);
	return _int();
}

_int CTextBalloon::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return 0;
}

HRESULT CTextBalloon::Render()
{

	__super::Render();


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	m_pShaderCom->Begin_Shader(0);

	if (m_bShowBalloon || m_dTimeShow < 0.1)
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
		m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureBalloonCom, 0);
		m_pShaderCom->Commit();

		m_pVIBufferCom->Render_VIBuffer();



		if (m_bShowBalloon)
		{
			D3DXFONT_DESCW tFontDesc;
			ZeroMemory(&tFontDesc, sizeof(D3DXFONT_DESCW));
			tFontDesc.Height = 30;
			tFontDesc.Width = 30;
			tFontDesc.CharSet = HANGUL_CHARSET;
			lstrcpy(tFontDesc.FaceName, L"a신디나루m");
			pGameInstance->Set_Font(&tFontDesc);

			LPD3DXFONT pFont = pGameInstance->Get_Font();

			TCHAR szOut[MAX_PATH] = {};
			int iLen = 0;
			float fSize = ((m_dTimeShow) > 1.f) ? 1.f : (m_dTimeShow);
			iLen = lstrlen(m_szText) * ((fSize > 0) ? fSize : 0) + 1;
			lstrcpynW(szOut, m_szText, iLen);
			RECT rc = { 640,505,640,505 };
			pFont->DrawText(NULL, szOut, -1, &rc, DT_CENTER | DT_NOCLIP, 0xff837156);

		}
	}
	if (m_dTimeShow > 2.0f && m_bShowBalloon == true)
	{
		m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureArrowCom, 0);
		m_pShaderCom->Commit();
		m_pVIBufferCom->Render_VIBuffer();
	}
	m_pShaderCom->End_Shader();


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

void CTextBalloon::Set_ShowBalloon(bool bShow, int iTextIndex, bool bName)
{
	m_bShowBalloon = bShow;
	m_iTextIndex = iTextIndex;
	m_bNameBalloon = bName;
	m_dTimeShow = 0;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CPlayer* pPlayer = (CPlayer*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), 0);

	pPlayer->Set_Stop(bShow);
	RELEASE_INSTANCE(CGameInstance);
	Set_Text(m_iTextIndex);


	CSound_Manager::Get_Instance()->PlaySound(TEXT("UI_MessageWindow_Open.wav"), CSound_Manager::CHANNELID::UI);
}

void CTextBalloon::Set_Text(int iTextIndex)
{
	if (iTextIndex == 0)
	{
		lstrcpy(m_szText, TEXT("실잠자리를 잡았다!\n\n배가 가늘고 길어!"));
	}
	else if (iTextIndex == 1)
	{
		lstrcpy(m_szText, TEXT("왕잠자리를 잡았다!\n\n왕은 아니지만 말이야!"));
	}
	else if (iTextIndex == 2)
	{
		lstrcpy(m_szText, TEXT("배추흰나비를 잡았다!\n\n하얗고 매력적이야!"));
	}
	else if (iTextIndex == 3)
	{
		lstrcpy(m_szText, TEXT("호랑나비를 잡았다!\n\n아싸~호랑나비!"));
	}
	else if (iTextIndex == 4)
	{
		lstrcpy(m_szText, TEXT("몰포나비를 잡았다!\n\n환상적으로 아름다운 나비!!"));
	}
	else if (iTextIndex == 5)
	{
		lstrcpy(m_szText, TEXT("꿀벌을 잡았다!\n\n쏘이지 않게 조심해!"));
	}
	else if (iTextIndex == 6)
	{
		lstrcpy(m_szText, TEXT("사마귀를 잡았다!\n\n얼굴 진짜 작다~!"));
	}
	else if (iTextIndex == 7)
	{
		lstrcpy(m_szText, TEXT("무당벌레를 잡았다!\n\n앙증맞고 귀여워!!"));
	}
	else if (iTextIndex == 8)
	{
		lstrcpy(m_szText, TEXT("인면노린재를 잡았다!\n\n아무리 봐도 사람 얼굴이야!"));
	}
	else if (iTextIndex == 9)
	{
		lstrcpy(m_szText, TEXT("아틀라스나방을 잡았다!\n\n으~ 팅커벨;;"));
	}
}

HRESULT CTextBalloon::Add_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_TextBalloon"), TEXT("Com_Texture_TextBalloon"), (CComponent**)&m_pTextureBalloonCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_TextBalloon_Name"), TEXT("Com_Texture_TextBalloon_Name"), (CComponent**)&m_pTextureNameCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_TextBalloon_Arrow"), TEXT("Com_Texture_TextBalloon_Arrow"), (CComponent**)&m_pTextureArrowCom)))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_Rect"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CTextBalloon * CTextBalloon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTextBalloon*	pInstance = new CTextBalloon(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CTextBalloon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTextBalloon::Clone(void * pArg)
{
	CTextBalloon*	pInstance = new CTextBalloon(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTextBalloon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTextBalloon::Free()
{
	CGameObject::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureBalloonCom);
	Safe_Release(m_pTextureNameCom);
	Safe_Release(m_pTextureArrowCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

}
