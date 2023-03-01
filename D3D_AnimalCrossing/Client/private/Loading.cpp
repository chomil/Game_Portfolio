#include "stdafx.h"
#include "Loading.h"

#include "UI.h"
#include "Sky.h"
#include "Tool_Net.h"
#include "Effect.h"
#include "Player.h"
#include "Player_Hair.h"
#include "Terrain.h"
#include "Tree.h"
#include "Inventory.h"
#include "NookTent.h"
#include "NoticeBoard.h"
#include "Weed.h"
#include "Insect.h"
#include "TextBalloon.h"
#include "Item.h"
#include "Flower.h"
#include "Quest.h"
#include "Peach.h"

#include "GameInstance.h"



CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CLoading::NativeConstruct(LEVEL eNextLevel)
{
	m_eNextLevel = eNextLevel;

	lstrcpy(m_szSystemMessage, TEXT("Animal Crossing : Jusin"));

	InitializeCriticalSection(&m_CS);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

unsigned CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	EnterCriticalSection(&pLoading->m_CS);

	HRESULT hr = 0;

	switch (pLoading->m_eNextLevel)
	{
	case LEVEL_GAMEPLAY0:
		hr = pLoading->Ready_GamePlay0();
		break;
	case LEVEL_GAMEPLAY1:
		hr = pLoading->Ready_GamePlay1();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;	

	LeaveCriticalSection(&pLoading->m_CS);
	
	return 0;
}

HRESULT CLoading::Ready_GamePlay0()
{

	CSound_Manager::Get_Instance()->Initialize();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* 원형 게임오브젝 객체를 생성*/
	lstrcpy(m_szSystemMessage, TEXT("Animal Crossing : Jusin"));
	lstrcat(m_szSystemMessage, TEXT(" ...Loading "));

	/* For.Prototype_Player*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Player"), CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Insect*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Insect"), CInsect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Player_Hair"), CPlayer_Hair::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Terrain"), CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Effect"), CPointEffect::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Sky*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Sky"), CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_UI */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_UI"), CUI::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Inven"), CInventory::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Quest"), CQuest::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_TextBalloon"), CTextBalloon::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Item"), CItem::Create(m_pGraphic_Device))))
		return E_FAIL;


	/* For.Prototype_Sword */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Sword"), CTool_Net::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Tree"), CTree::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Peach"), CPeach::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Weed"), CWeed::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Flower"), CFlower::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_NookTent"), CNookTent::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_NoticeBoard"), CNoticeBoard::Create(m_pGraphic_Device))))
		return E_FAIL;



	/* 원형 리소스 객체를 생성한다. */
	//lstrcpy(m_szSystemMessage, TEXT("리소스 생성"));


	/* For.Prototype_Mesh_Player*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Player"), CMesh_Dynamic::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/DynamicMesh/Player/"), TEXT("PlayerFin.x")))))
		return E_FAIL;

	/* For.Prototype_Mesh_Insect*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_실잠자리"), CMesh_Dynamic::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/DynamicMesh/Insect/실잠자리/"), TEXT("실잠자리.x")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_왕잠자리"), CMesh_Dynamic::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/DynamicMesh/Insect/왕잠자리/"), TEXT("왕잠자리.x")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_배추흰나비"), CMesh_Dynamic::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/DynamicMesh/Insect/배추흰나비/"), TEXT("배추흰나비.x")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_호랑나비"), CMesh_Dynamic::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/DynamicMesh/Insect/호랑나비/"), TEXT("호랑나비.x")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_몰포나비"), CMesh_Dynamic::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/DynamicMesh/Insect/몰포나비/"), TEXT("몰포나비.x")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_꿀벌"), CMesh_Dynamic::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/DynamicMesh/Insect/꿀벌/"), TEXT("꿀벌.x")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_사마귀"), CMesh_Dynamic::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/DynamicMesh/Insect/사마귀/"), TEXT("사마귀.x")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_무당벌레"), CMesh_Dynamic::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/DynamicMesh/Insect/무당벌레/"), TEXT("무당벌레.x")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_인면노린재"), CMesh_Dynamic::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/DynamicMesh/Insect/인면노린재/"), TEXT("인면노린재.x")))))
		return E_FAIL;	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_아틀라스나방"), CMesh_Dynamic::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/DynamicMesh/Insect/아틀라스나방/"), TEXT("아틀라스나방.x")))))
		return E_FAIL;


	for (int i = 0; i < 14; i++) //아이템 텍스쳐로딩
	{
		_tchar szItemTexName[20] = {};
		_tchar szIndex[20] = {};

		lstrcpy(szItemTexName, TEXT("item"));
		_itot_s(i, szIndex, 10);
		lstrcat(szItemTexName, szIndex);

		_tchar szPath[50] = {};
		lstrcpy(szPath, TEXT("../Bin/Resources/Textures/Item/"));
		lstrcat(szPath, szItemTexName);
		lstrcat(szPath, TEXT(".png"));

		_tchar* szProtoName = new _tchar[50];

		lstrcpy(szProtoName, TEXT("Prototype_Texture_"));
		lstrcat(szProtoName, szItemTexName);

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, szProtoName, CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, szPath))))
			return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Player_Hair"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/StaticMesh/PlayerHair/"), TEXT("PlayerHair.x")))))
		return E_FAIL;

	/* For.Prototype_Mesh_Net */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Net"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/DynamicMesh/Tool/Net/"), TEXT("ToolNet.x")))))
		return E_FAIL;

	/* For.Prototype_Mesh_Tree */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Tree"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/StaticMesh/tree/"), TEXT("tree.x")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Weed0"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/StaticMesh/Weed/"), TEXT("weed_0.x")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Weed1"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/StaticMesh/Weed/"), TEXT("weed_1.x")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Cosmos"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/StaticMesh/Cosmos/"), TEXT("cosmos.x")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_PickedWeed"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/StaticMesh/Weed/"), TEXT("Picked_Weed.x")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Peach"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/StaticMesh/peach/"), TEXT("peach.x")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Branch"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/StaticMesh/branch/"), TEXT("branch.x")))))
		return E_FAIL;

	/* For.Prototype_Mesh_NookTent */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_NookTent"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/StaticMesh/building/nooktent/"), TEXT("nooktent.x")))))
		return E_FAIL;

	/* For.Prototype_Mesh_Notice */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_NoticeBoard"), CMesh_Static::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Meshes/StaticMesh/building/noticeboard/"), TEXT("noticeboard.x")))))
		return E_FAIL;


	/* For.Prototype_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Sky"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/SkyBox/burger%d.dds"), 4))))
		return E_FAIL;

	/* For.Prototype_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Terrain_Grass"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Meshes/StaticMesh/sea/mGrass_Alb.png"), 1))))
		return E_FAIL;	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Terrain_Sand"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Meshes/StaticMesh/sea/mSand_Alb.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Texture_Filter*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Terrain_Filter"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Terrain/SeaFilter.bmp")))))
		return E_FAIL;

	/* For.Prototype_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Smoke"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/smoke/smoke_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Texture_UI */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Inven"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Inven.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_TextBalloon"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/TextBalloon/TextBalloon.png")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_TextBalloon_Arrow"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/TextBalloon/TextBalloon_Arrow.png")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_TextBalloon_Name"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/TextBalloon/TextBalloon_Name.png")))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Quest"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Quest/quest.png")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Quest_Bar"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Quest/quest_bar.png")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Quest_Clear"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Quest/quest_clear.png")))))
		return E_FAIL;


	/* For.Prototype_VIBuffer_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_VIBuffer_Terrain"), CVIBuffer_Terrain::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp"),2.f))))
		return E_FAIL;

	/* For.Prototype_VIBuffer_Cube*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_VIBuffer_Point */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_VIBuffer_Point"), CVIBuffer_Point::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* Prototype_Shader_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Terrain"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Terrain.hlsl")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Sky"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Sky.hlsl")))))
		return E_FAIL;

	/* Prototype_Shader_Mesh */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Mesh"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Mesh.hlsl")))))
		return E_FAIL;

	/* Prototype_Shader_Mesh */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Player"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Player.hlsl")))))
		return E_FAIL;

	/* Prototype_Shader_Mesh */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Point"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Point.hlsl")))))
		return E_FAIL;

	lstrcpy(m_szSystemMessage, TEXT("Animal Crossing : Jusin"));
	m_isFinish = true;



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoading::Ready_GamePlay1()
{
	/* 원형 게임오브젝 객체를 생성ㅇ핞다. */

	/* 원형 리소스 객체를 생성한다. */
	return S_OK;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
	CLoading*	pInstance = new CLoading(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSGBOX("Failed to Creating CLoading");
		Safe_Release(pInstance);
	}


	return pInstance;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CS);

	Safe_Release(m_pGraphic_Device);

	CloseHandle(m_hThread);

}
