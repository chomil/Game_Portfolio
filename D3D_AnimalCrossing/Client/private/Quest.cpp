#include "stdafx.h"
#include "Quest.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "Player.h"

CQuest::CQuest(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CQuest::CQuest(const CQuest & rhs)
	: CGameObject(rhs)
{

}

HRESULT CQuest::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();


	return S_OK;
}

HRESULT CQuest::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	m_iStaticPoint = StorableInt("CQuest/m_iStaticPoint", 0);


	if (FAILED(Add_Components()))
		return E_FAIL;
	
	Set_Quest();

	m_fSizeX = g_nWinCX;
	m_fSizeY = g_nWinCY;
	m_fX = g_nWinCX >> 1;
	m_fY = g_nWinCY >> 1;

	/* 직교투영행렬을 만들어주낟. */
	D3DXMatrixOrthoLH(&m_ProjMatrix, g_nWinCX, g_nWinCY, 0.f, 1.f);

	return S_OK;
}

_int CQuest::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	m_dTimeOpen += TimeDelta;

	float fSize = (m_dTimeOpen * 10 > 1.f) ? 1.f : m_dTimeOpen * 10;
	if (m_bQuestOpen)
	{
		m_fSizeX = fSize;
		m_fSizeY = fSize;
	}
	else
	{
		m_fSizeX = (1 - fSize);
		m_fSizeY = (1 - fSize);
	}


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	if (m_bQuestOpen)
	{
		if (pGameInstance->Key_PressOnce(DIK_ESCAPE))
		{
			CPlayer* pPlayer = (CPlayer*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Player"));
			pPlayer->Set_Stop(false);
			Set_QuestOpen(false);
		}
	}


	RELEASE_INSTANCE(CGameInstance);
	return _int();
}

_int CQuest::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return 0;
}

HRESULT CQuest::Render()
{
	__super::Render();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pShaderCom->Begin_Shader(0);

	if (m_bQuestOpen || m_dTimeOpen * 10 < 1.f)
	{
		_matrix			TransformMatrix, ViewMatrix, ProjMatrix;

		D3DXMatrixIdentity(&TransformMatrix);

		/* 로컬스페이스 기준으로 크기변환과 위치변환만 셋팅한다. */
		TransformMatrix._11 = g_nWinCX * m_fSizeX;
		TransformMatrix._22 = g_nWinCY * m_fSizeY;

		TransformMatrix._41 = m_fX - (g_nWinCX >> 1);
		TransformMatrix._42 = -m_fY + (g_nWinCY >> 1);


		/* 뷰변환행렬. */
		D3DXMatrixIdentity(&ViewMatrix);


		/* 기본 변환. */
		m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", &TransformMatrix, sizeof(_matrix));
		m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &ViewMatrix, sizeof(_matrix));
		m_pShaderCom->SetUp_ConstantTable("g_ProjMatrix", &m_ProjMatrix, sizeof(_matrix));
		m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureQuestCom,0);
		m_pShaderCom->Commit();

		m_pVIBufferCom->Render_VIBuffer();



		if (m_bQuestOpen && m_dTimeOpen * 10 > 1.f)
		{
			D3DXFONT_DESCW tFontDesc;
			ZeroMemory(&tFontDesc, sizeof(D3DXFONT_DESCW));
			tFontDesc.Height = 40;
			tFontDesc.Width = 40;
			tFontDesc.CharSet = HANGUL_CHARSET;

			lstrcpy(tFontDesc.FaceName, L"a신디나루m");
			pGameInstance->Set_Font(&tFontDesc);

			LPD3DXFONT pFont = pGameInstance->Get_Font();
			//퀘스트 이름
			RECT rc = { 1180,108,1180,108 };
			_tchar szPoint[10];
			swprintf(szPoint, L"%d", m_iStaticPoint.GetValue());
			pFont = pGameInstance->Get_Font();
			pFont->DrawText(NULL, szPoint, -1, &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, 0xffbedaff);

			for (int i = 0; i < 4; i++)
			{
				_float2 pos = { 343,304 };
				if (i % 2 == 1)
				{
					pos.x = 936;
				}
				if (i / 2 == 1)
				{
					pos.y = 556;
				}

				//퀘스트 이름
				ZeroMemory(&tFontDesc, sizeof(D3DXFONT_DESCW));
				tFontDesc.Height = 30;
				tFontDesc.Width = 30;
				tFontDesc.CharSet = HANGUL_CHARSET;

				lstrcpy(tFontDesc.FaceName, L"a신디나루m");
				pGameInstance->Set_Font(&tFontDesc);

				LPD3DXFONT pFont = pGameInstance->Get_Font();
				RECT rc = { pos.x,pos.y-50,pos.x,pos.y-50 };
				pFont->DrawText(NULL, m_szQuestName[i], -1, &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, 0xff765e35);

				
				//마일리지
				ZeroMemory(&tFontDesc, sizeof(D3DXFONT_DESCW));
				tFontDesc.Height = 30;
				tFontDesc.Width = 30;
				tFontDesc.CharSet = HANGUL_CHARSET;
				lstrcpy(tFontDesc.FaceName, L"a신디나루m");
				pGameInstance->Set_Font(&tFontDesc);
				_tchar szPoint[10];
				swprintf(szPoint, L"%d", m_iQuestPoint[i]);
				pFont = pGameInstance->Get_Font();

				RECT rc_point = { pos.x, pos.y + 10 ,pos.x, pos.y + 10 };
				pFont->DrawText(NULL, szPoint, -1, &rc_point, DT_CENTER | DT_VCENTER | DT_NOCLIP, 0xffbeaf9f);

				//바
				{
					float fProgress = ((float)m_iQuestProgress[i][0] / (float)m_iQuestProgress[i][1]);
					TransformMatrix._11 = 460 * fProgress;
					TransformMatrix._22 = 25;
					TransformMatrix._41 = (pos.x-(460- TransformMatrix._11)/2) - (g_nWinCX >> 1);
					TransformMatrix._42 = -(pos.y + 65) + (g_nWinCY >> 1);

					m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", &TransformMatrix, sizeof(_matrix));
					m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &ViewMatrix, sizeof(_matrix));
					m_pShaderCom->SetUp_ConstantTable("g_ProjMatrix", &m_ProjMatrix, sizeof(_matrix));
					m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureQuestBarCom, 0);
					m_pShaderCom->Commit();

					m_pVIBufferCom->Render_VIBuffer();
				}

				//진행 텍스트
				ZeroMemory(&tFontDesc, sizeof(D3DXFONT_DESCW));
				tFontDesc.Height = 20;
				tFontDesc.Width = 20;
				tFontDesc.CharSet = HANGUL_CHARSET;
				lstrcpy(tFontDesc.FaceName, L"a신디나루b");
				pGameInstance->Set_Font(&tFontDesc);
				_tchar szProgress[20];
				swprintf(szPoint, L"%d / %d", m_iQuestProgress[i][0], m_iQuestProgress[i][1]);
				pFont = pGameInstance->Get_Font();

				RECT rc_progress = { pos.x, pos.y + 65 ,pos.x, pos.y + 65 };
				pFont->DrawText(NULL, szPoint, -1, &rc_progress, DT_CENTER | DT_VCENTER | DT_NOCLIP, 0xff765e35);


				//클리어
				if (m_iQuestProgress[i][1] == m_iQuestProgress[i][0])
				{
					TransformMatrix._11 = 550;
					TransformMatrix._22 = 220;

					TransformMatrix._41 = pos.x - (g_nWinCX >> 1);
					TransformMatrix._42 = -pos.y + (g_nWinCY >> 1);

					m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", &TransformMatrix, sizeof(_matrix));
					m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &ViewMatrix, sizeof(_matrix));
					m_pShaderCom->SetUp_ConstantTable("g_ProjMatrix", &m_ProjMatrix, sizeof(_matrix));
					m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureQuestClearCom, 0);
					m_pShaderCom->Commit();


					m_pVIBufferCom->Render_VIBuffer();
				}
				
			}
		}


	}

	m_pShaderCom->End_Shader();

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CQuest::Set_QuestOpen(bool bOpen)
{
	if (bOpen)
	{

		CSound_Manager::Get_Instance()->PlaySound(TEXT("UI_MessageWindow_Open.wav"), CSound_Manager::CHANNELID::UI);
	}
	else
	{
		CSound_Manager::Get_Instance()->PlaySound(TEXT("UI_Cmn_Close.wav"), CSound_Manager::CHANNELID::UI);
	}
	m_bQuestOpen = bOpen;
	m_dTimeOpen = 0;
}

void CQuest::Clear_Quest(int iQuestIndex, int iAmount)
{
	for (int i = 0; i < 4; i++)
	{
		if (m_iQuestIndex[i] == iQuestIndex)
		{
			if (m_iQuestProgress[i][0] < m_iQuestProgress[i][1] &&
				m_iQuestProgress[i][0] + iAmount >= m_iQuestProgress[i][1] )
			{
				m_iStaticPoint.SetValue(m_iStaticPoint.GetValue()+m_iQuestPoint[i]);
			}
			m_iQuestProgress[i][0] += iAmount;
			m_iQuestProgress[i][0] = (m_iQuestProgress[i][0] > m_iQuestProgress[i][1]) ? m_iQuestProgress[i][1] : m_iQuestProgress[i][0];
			m_iStaticProgress[i].SetValue(m_iQuestProgress[i][0]);
		}
	}
}

void CQuest::Set_Quest()
{
	time_t curTime = time(NULL);
	struct tm* pLocal = localtime(&curTime);
	int iDay = pLocal->tm_yday;
	int iYear = pLocal->tm_year;
	m_iQuestSeed = iDay + iYear;



	srand(m_iQuestSeed);
	for (int i = 0; i < 4; i++)
	{
		m_iQuestIndex[i] = rand() % 13;
		while (1)
		{
			bool bSame = false;
			for (int j = 0; j < i; j++)
			{
				if (m_iQuestIndex[i] == m_iQuestIndex[j])
				{
					bSame = true;
				}
			}
			if (bSame == true)
			{
				m_iQuestIndex[i] = rand() % 13;
			}
			else
			{
				break;
			}
		}
	}


	//m_iQuestIndex[0] = 9;
	//m_iQuestIndex[1] = 10;
	//m_iQuestIndex[2] = 11;
	//m_iQuestIndex[3] = 12;

	for (int i = 0; i < 4; i++)
	{
		switch (m_iQuestIndex[i])
		{
		case 0:
			lstrcpy(m_szQuestName[i], TEXT("실잠자리 3마리 잡기"));
			m_iQuestPoint[i] = 200;
			m_iQuestProgress[i][0] = 0;
			m_iQuestProgress[i][1] = 3;
			break;
		case 1:
			lstrcpy(m_szQuestName[i], TEXT("왕잠자리 3마리 잡기"));
			m_iQuestPoint[i] = 200;
			m_iQuestProgress[i][0] = 0;
			m_iQuestProgress[i][1] = 3;
			break;
		case 2:
			lstrcpy(m_szQuestName[i], TEXT("배추흰나비 3마리 잡기"));
			m_iQuestPoint[i] = 200;
			m_iQuestProgress[i][0] = 0;
			m_iQuestProgress[i][1] = 3;
			break;
		case 3:
			lstrcpy(m_szQuestName[i], TEXT("호랑나비 3마리 잡기"));
			m_iQuestPoint[i] = 200;
			m_iQuestProgress[i][0] = 0;
			m_iQuestProgress[i][1] = 3;
			break;
		case 4:
			lstrcpy(m_szQuestName[i], TEXT("몰포나비 3마리 잡기"));
			m_iQuestPoint[i] = 200;
			m_iQuestProgress[i][0] = 0;
			m_iQuestProgress[i][1] = 3;
			break;
		case 5:
			lstrcpy(m_szQuestName[i], TEXT("꿀벌 3마리 잡기"));
			m_iQuestPoint[i] = 200;
			m_iQuestProgress[i][0] = 0;
			m_iQuestProgress[i][1] = 3;
			break;
		case 6:
			lstrcpy(m_szQuestName[i], TEXT("사마귀 3마리 잡기"));
			m_iQuestPoint[i] = 200;
			m_iQuestProgress[i][0] = 0;
			m_iQuestProgress[i][1] = 3;
			break;
		case 7:
			lstrcpy(m_szQuestName[i], TEXT("무당벌레 3마리 잡기"));
			m_iQuestPoint[i] = 200;
			m_iQuestProgress[i][0] = 0;
			m_iQuestProgress[i][1] = 3;
			break;
		case 8:
			lstrcpy(m_szQuestName[i], TEXT("인면노린재 3마리 잡기"));
			m_iQuestPoint[i] = 200;
			m_iQuestProgress[i][0] = 0;
			m_iQuestProgress[i][1] = 3;
			break;
		case 9:
			lstrcpy(m_szQuestName[i], TEXT("아틀라스나방 3마리 잡기"));
			m_iQuestPoint[i] = 200;
			m_iQuestProgress[i][0] = 0;
			m_iQuestProgress[i][1] = 3;
			break;
		case 10:
			lstrcpy(m_szQuestName[i], TEXT("곤충 10마리 잡기"));
			m_iQuestPoint[i] = 500;
			m_iQuestProgress[i][0] = 0;
			m_iQuestProgress[i][1] = 10;
			break;
		case 11:
			lstrcpy(m_szQuestName[i], TEXT("잡초 10개 뽑기"));
			m_iQuestPoint[i] = 350;
			m_iQuestProgress[i][0] = 0;
			m_iQuestProgress[i][1] = 10;
			break;
		case 12:
			lstrcpy(m_szQuestName[i], TEXT("복숭아 5개 따기"));
			m_iQuestPoint[i] = 100;
			m_iQuestProgress[i][0] = 0;
			m_iQuestProgress[i][1] = 5;
			break;
		default:
			break;
		}
	}

	m_iStaticQuestSeed = StorableInt("CQuest/m_iStaticQuestSeed", m_iQuestSeed);
	if (m_iQuestSeed != m_iStaticQuestSeed.GetValue())
	{
		m_iStaticQuestSeed.SetValue(m_iQuestSeed);

		m_iStaticProgress[0] = StorableInt("CQuest/m_iStaticProgress0", 0);
		m_iStaticProgress[0].SetValue(0);
		m_iStaticProgress[1] = StorableInt("CQuest/m_iStaticProgress1", 0);
		m_iStaticProgress[1].SetValue(0);
		m_iStaticProgress[2] = StorableInt("CQuest/m_iStaticProgress2", 0);
		m_iStaticProgress[2].SetValue(0);
		m_iStaticProgress[3] = StorableInt("CQuest/m_iStaticProgress3", 0);
		m_iStaticProgress[3].SetValue(0);
	}
	else
	{
		m_iStaticProgress[0] = StorableInt("CQuest/m_iStaticProgress0", 0);
		m_iStaticProgress[1] = StorableInt("CQuest/m_iStaticProgress1", 0);
		m_iStaticProgress[2] = StorableInt("CQuest/m_iStaticProgress2", 0);
		m_iStaticProgress[3] = StorableInt("CQuest/m_iStaticProgress3", 0);
	}

	m_iQuestProgress[0][0] = m_iStaticProgress[0].GetValue();
	m_iQuestProgress[1][0] = m_iStaticProgress[1].GetValue();
	m_iQuestProgress[2][0] = m_iStaticProgress[2].GetValue();
	m_iQuestProgress[3][0] = m_iStaticProgress[3].GetValue();
}

HRESULT CQuest::Add_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Quest"), TEXT("Com_Texture_Quest"), (CComponent**)&m_pTextureQuestCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Quest_Clear"), TEXT("Com_Texture_Quest_Clear"), (CComponent**)&m_pTextureQuestClearCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Quest_Bar"), TEXT("Com_Texture_Quest_Bar"), (CComponent**)&m_pTextureQuestBarCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_Rect"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	return S_OK;
}

CQuest * CQuest::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CQuest*	pInstance = new CQuest(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CQuest");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CQuest::Clone(void * pArg)
{
	CQuest*	pInstance = new CQuest(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CQuest");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CQuest::Free()
{
	CGameObject::Free();

	Safe_Release(m_pShaderCom); 
	Safe_Release(m_pTextureQuestCom);
	Safe_Release(m_pTextureQuestBarCom);
	Safe_Release(m_pTextureQuestClearCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

}
