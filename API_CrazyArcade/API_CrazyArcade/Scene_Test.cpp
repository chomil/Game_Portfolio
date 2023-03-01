#include "pch.h"
#include "Scene_Test.h"
#include "GameObject_Manager.h"
#include "Player.h"
#include "Mouse.h"
#include "Item.h"
#include "Button.h"
#include "Scene_Room.h"
#include "Monster.h"
#include "ComPlayer.h"


CScene_Test::CScene_Test()
{
}


CScene_Test::~CScene_Test()
{
	Release_Scene();
}

int CScene_Test::Ready_Scene()
{
	CSound_Manager::Get_Instance()->StopSound(CSound_Manager::CHANNELID::BGM);
	CSound_Manager::Get_Instance()->PlayBGM(L"BGM_Town.mp3");
	CSound_Manager::Get_Instance()->PlaySound(L"SFX_GameStart.mp3", CSound_Manager::CHANNELID::EFFECT);

	m_eStageState = STATE::READY;


	CGameObject* pButton = CButton::Create(717, 577, CButton::TYPE::BT_EXIT);
	if (nullptr != pButton)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUTTON, pButton);
	}

	if (CScene_Room::Get_StageNum() == 2)
	{
		CTile_Manager::Get_Instance()->Load_Data_Tile_Manager(L"../Data/tile_test.dat");
		CTile_Manager::Get_Instance()->Load_Data_Block_Manager(L"../Data/block_test.dat");

		CGameObject* pItem = CItem::Create(TILESTARTX + TILECX * 2, TILESTARTY + TILECX * 9, ITEM_TYPE::BUBBLE_UP);
		if (nullptr != pItem)
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::ITEM, pItem);
		}
		pItem = CItem::Create(TILESTARTX + TILECX * 3, TILESTARTY + TILECX * 9, ITEM_TYPE::BUBBLE_UP);
		if (nullptr != pItem)
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::ITEM, pItem);
		}		
		pItem = CItem::Create(TILESTARTX + TILECX * 4, TILESTARTY + TILECX * 9, ITEM_TYPE::POWER_UP);
		if (nullptr != pItem)
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::ITEM, pItem);
		}		
		pItem = CItem::Create(TILESTARTX + TILECX * 5, TILESTARTY + TILECX * 9, ITEM_TYPE::POWER_UP);
		if (nullptr != pItem)
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::ITEM, pItem);
		}
		pItem = CItem::Create(TILESTARTX + TILECX * 6, TILESTARTY + TILECX * 9, ITEM_TYPE::SPEED_UP);
		if (nullptr != pItem)
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::ITEM, pItem);
		}
		pItem = CItem::Create(TILESTARTX + TILECX * 7, TILESTARTY + TILECX * 9, ITEM_TYPE::POWER_MAX);
		if (nullptr != pItem)
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::ITEM, pItem);
		}
		pItem = CItem::Create(TILESTARTX + TILECX * 8, TILESTARTY + TILECX * 9, ITEM_TYPE::KICK);
		if (nullptr != pItem)
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::ITEM, pItem);
		}
		pItem = CItem::Create(TILESTARTX + TILECX * 9, TILESTARTY + TILECX * 9, ITEM_TYPE::RED_DEVIL);
		if (nullptr != pItem)
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::ITEM, pItem);
		}
		pItem = CItem::Create(TILESTARTX + TILECX * 10, TILESTARTY + TILECX * 9, ITEM_TYPE::NEEDLE);
		if (nullptr != pItem)
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::ITEM, pItem);
		}
		pItem = CItem::Create(TILESTARTX + TILECX * 11, TILESTARTY + TILECX * 9, ITEM_TYPE::BUBBLE_UP);
		if (nullptr != pItem)
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::ITEM, pItem);
		}
		pItem = CItem::Create(TILESTARTX + TILECX * 12, TILESTARTY + TILECX * 9, ITEM_TYPE::BUBBLE_UP);
		if (nullptr != pItem)
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::ITEM, pItem);
		}



		POINT startPos[4] = { {200,500},{280, 500},{360,500},{440,500} };

		CPlayer* pPlayer1 = static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER)->front());
		CPlayer* pPlayer2 = static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER)->back());

		pPlayer1->Ready_GameObject();
		pPlayer1->Set_IsShow(true);
		while (1)
		{
			int iRand = rand() % 4;
			if (startPos[iRand].x != 0)
			{
				pPlayer1->Set_Pos(startPos[iRand].x, startPos[iRand].y);
				startPos[iRand].x = 0;
				startPos[iRand].y = 0;
				break;
			}
		}

		pPlayer2->Ready_GameObject();
		pPlayer2->Set_IsShow(true);
		while (1)
		{
			int iRand = rand() % 4;
			if (startPos[iRand].x != 0)
			{
				pPlayer2->Set_Pos(startPos[iRand].x, startPos[iRand].y);
				startPos[iRand].x = 0;
				startPos[iRand].y = 0;
				break;
			}
		}

		for (int i = 0; i < CScene_Room::Get_ComNum(); i++)
		{
			CGameObject* pCom = CComPlayer::Create();
			if (nullptr != pCom)
			{
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::COMPLAYER, pCom);
				while (1)
				{
					int iRand = rand() % 4;
					if (startPos[iRand].x != 0)
					{
						pCom->Set_Pos(startPos[iRand].x, startPos[iRand].y);
						startPos[iRand].x = 0;
						startPos[iRand].y = 0;
						break;
					}
				}
			}
		}
	}

	m_dwStartTimer = GetTickCount();
	m_dwEndTimer = 0;
	return READY_OK;
}

void CScene_Test::Update_Scene()
{
	if (m_eStageState == STATE::READY)
	{
		if (m_dwStartTimer + 2000 < GetTickCount())
		{
			m_eStageState = STATE::START;
		}
	}
	else if (m_eStageState == STATE::START)
	{
		list<CGameObject*>* pButtonList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::BUTTON);
		for (auto pButton : *pButtonList)
		{
			if (static_cast<CButton*>(pButton)->Get_ButtonType() == CButton::TYPE::BT_EXIT &&
				static_cast<CButton*>(pButton)->Get_Clicked() > 0)
			{
				m_eStageState = STATE::END;
			}
		}
	}
	else if (m_eStageState == STATE::END)
	{
		CScene_Manager::Get_Instance()->Scene_Change(CScene_Manager::ID::SCENE_ROOM);
		return;
	}


	CTile_Manager::Get_Instance()->Update_Tile_Manager();
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager();

}

void CScene_Test::Render_Scene(HDC hDC)
{
	CTile_Manager::Get_Instance()->Render_Tile_Manager(hDC);

	HDC hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"StageFrame");
	if (nullptr == hMemDC)
		return;
	//GdiTransparentBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, WINCX, WINCY, RGB(255, 0, 255)); 

	BitBlt(hDC, 0, 0, WINCX, 40, hMemDC, 0, 0, SRCCOPY);
	BitBlt(hDC, 0, 0, 20, WINCY, hMemDC, 0, 0, SRCCOPY);
	BitBlt(hDC, 620, 0, 180, WINCY, hMemDC, 620, 0, SRCCOPY);
	BitBlt(hDC, 0, 560, WINCX, 40, hMemDC, 0, 560, SRCCOPY);

	auto pPlayerList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER);
	if (pPlayerList->size() == 2)
	{
		hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"Item_slot_2p");
		if (nullptr == hMemDC)
			return;
		BitBlt(hDC, 639, 450, 157, 105, hMemDC, 0, 0, SRCCOPY);
		if (static_cast<CPlayer*>(pPlayerList->front())->Get_NeedleNum() > 0)
		{
			hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"item");
			if (nullptr == hMemDC)
				return;
			GdiTransparentBlt(hDC, 665, 486, 42, 45, hMemDC, 0, 270, 42, 45, RGB(255, 0, 255));
		}
		if (static_cast<CPlayer*>(pPlayerList->back())->Get_NeedleNum() > 0)
		{
			hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"item");
			if (nullptr == hMemDC)
				return;
			GdiTransparentBlt(hDC, 730, 486, 42, 45, hMemDC, 0, 270, 42, 45, RGB(255, 0, 255));
		}
	}
	else
	{
		if (static_cast<CPlayer*>(pPlayerList->front())->Get_NeedleNum() > 0)
		{
			hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"item");
			if (nullptr == hMemDC)
				return;
			GdiTransparentBlt(hDC, 671, 486, 42, 45, hMemDC, 0, 270, 42, 45, RGB(255, 0, 255));
		}
	}



	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager(hDC);



	switch (m_eStageState)
	{
	case CScene::READY:
		hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"START");
		if (nullptr == hMemDC)
			return;
		GdiTransparentBlt(hDC, STAGE_CENTERX - 477 / 2, STAGE_CENTERY - 77 / 2, 477, 77, hMemDC, 0, 0, 477, 77, RGB(255, 0, 255));
		break;
		break;
	case CScene::START:
		break;
	case CScene::WIN:
		hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"win");
		if (nullptr == hMemDC)
			return;
		GdiTransparentBlt(hDC, STAGE_CENTERX - 258 / 2, STAGE_CENTERY - 59 / 2, 258, 59, hMemDC, 0, 0, 258, 59, RGB(255, 0, 255));
		break;
	case CScene::LOSE:
		hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"lose");
		if (nullptr == hMemDC)
			return;
		GdiTransparentBlt(hDC, STAGE_CENTERX - 336 / 2, STAGE_CENTERY - 59 / 2, 336, 59, hMemDC, 0, 0, 336, 59, RGB(255, 0, 255));
		break;
	case CScene::DRAW:
		hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"draw");
		if (nullptr == hMemDC)
			return;
		GdiTransparentBlt(hDC, STAGE_CENTERX - 362 / 2, STAGE_CENTERY - 59 / 2, 362, 59, hMemDC, 0, 0, 362, 59, RGB(255, 0, 255));
		break;
	case CScene::PLAYER1_WIN:
		hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"player1_win");
		if (nullptr == hMemDC)
			return;
		GdiTransparentBlt(hDC, STAGE_CENTERX - 258 / 2, STAGE_CENTERY - 88 / 2, 258, 88, hMemDC, 0, 0, 258, 88, RGB(255, 0, 255));
		break;
	case CScene::PLAYER2_WIN:
		hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"player2_win");
		if (nullptr == hMemDC)
			return;
		GdiTransparentBlt(hDC, STAGE_CENTERX - 258 / 2, STAGE_CENTERY - 88 / 2, 258, 88, hMemDC, 0, 0, 258, 88, RGB(255, 0, 255));
		break;
	case CScene::END:

		break;
	default:
		break;
	}



}

void CScene_Test::Release_Scene()
{
	CGameObject_Manager::Get_Instance()->Delete_GameObject(OBJECT::ID::TILE);
	CGameObject_Manager::Get_Instance()->Delete_GameObject(OBJECT::ID::BLOCK);
	CGameObject_Manager::Get_Instance()->Delete_GameObject(OBJECT::ID::BUBBLE);
	CGameObject_Manager::Get_Instance()->Delete_GameObject(OBJECT::ID::MONSTER);
	CGameObject_Manager::Get_Instance()->Delete_GameObject(OBJECT::ID::EFFECT);
	CGameObject_Manager::Get_Instance()->Delete_GameObject(OBJECT::ID::ITEM);
	CGameObject_Manager::Get_Instance()->Delete_GameObject(OBJECT::ID::BUTTON);
	CGameObject_Manager::Get_Instance()->Delete_GameObject(OBJECT::ID::COMPLAYER);

	CPlayer* pPlayer1 = static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER)->front());
	CPlayer* pPlayer2 = static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER)->back());
	pPlayer1->Set_IsShow(true);
	pPlayer2->Set_IsShow(true);
}

CScene* CScene_Test::Create()
{
	CScene* pInstance = new CScene_Test;

	if (0 > pInstance->Ready_Scene())
		Safe_Delete(pInstance);

	return pInstance;
}
