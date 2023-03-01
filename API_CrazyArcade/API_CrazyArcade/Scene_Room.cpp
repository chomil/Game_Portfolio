#include "pch.h"
#include "Scene_Room.h"
#include "GameObject_Manager.h"
#include "Button.h"
#include "Player.h"
#include "UI.h"

int CScene_Room::m_iStageNum = 0;
int CScene_Room::m_iComNum = 0;

CScene_Room::CScene_Room()
{
}

CScene_Room::~CScene_Room()
{
	Release_Scene();
}

int CScene_Room::Ready_Scene()
{
	CSound_Manager::Get_Instance()->StopSound(CSound_Manager::CHANNELID::BGM);
	CSound_Manager::Get_Instance()->PlayBGM(L"BGM_Room.mp3");

	CGameObject* pButton = CButton::Create(608, 522, CButton::TYPE::BT_START);
	if (nullptr != pButton)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUTTON, pButton);
	}
	for (int i = 0; i < 4; i++)
	{
		pButton = CButton::Create(75+106*i, 162, CButton::TYPE::BT_SLOT);
		if (nullptr != pButton)
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUTTON, pButton);
		}
		m_pCharSlot[i] = pButton;
	}
	pButton = CButton::Create(521, 104, CButton::TYPE::BT_BAZZI);
	if (nullptr != pButton)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUTTON, pButton);
	}

	pButton = CButton::Create(522, 299, CButton::TYPE::BT_COLOR_RED);
	if (nullptr != pButton)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUTTON, pButton);
	}
	pButton = CButton::Create(594, 299, CButton::TYPE::BT_COLOR_BLUE);
	if (nullptr != pButton)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUTTON, pButton);
	}

	pButton = CButton::Create(740, 455, CButton::TYPE::BT_ARROW_R);
	if (nullptr != pButton)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUTTON, pButton);
	}

	pButton = CButton::Create(675, 455, CButton::TYPE::BT_ARROW_L);
	if (nullptr != pButton)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUTTON, pButton);
	}

	pButton = CButton::Create(764, 584, CButton::TYPE::BT_OFF);
	if (nullptr != pButton)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUTTON, pButton);
	}

	pButton = CButton::Create(30, 584, CButton::TYPE::BT_BACK);
	if (nullptr != pButton)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUTTON, pButton);
	}


	int iPlayerDist = 0;
	auto pPlayerList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER);
	for (auto pPlayer : *pPlayerList)
	{
		static_cast<CPlayer*>(pPlayer)->Set_IsShow(true);
		pPlayer->Set_Pos(75 + iPlayerDist * 106, 182);
		iPlayerDist++;
	}

	return 0;
}

void CScene_Room::Update_Scene()
{

	auto pButtonList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::BUTTON);

	for (auto& pObject : *pButtonList)
	{
		CButton* pButton = static_cast<CButton*>(pObject);
		if (pButton->Get_ButtonType() == CButton::TYPE::BT_BACK)
		{
			if (pButton->Get_Clicked() > 0)
			{
				CScene_Manager::Get_Instance()->Scene_Change(CScene_Manager::ID::SCENE_LOGIN);
				return;
			}
		}
		if (pButton->Get_ButtonType() == CButton::TYPE::BT_OFF)
		{
			if (pButton->Get_Clicked() > 0)
			{
				DestroyWindow(g_hWnd);
			}
		}
		if ((pButton->Get_ButtonType() == CButton::TYPE::BT_START && pButton->Get_Clicked() > 0) || CKey_Manager::Get_Instance()->Key_Pressed(VK_F5))
		{
			auto pPlayerList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER);
			int arrColor[3] = {};
			for (auto pPlayer : *pPlayerList)
			{
				arrColor[static_cast<CPlayer*>(pPlayer)->Get_Color()]++;
			}
			int iColorDiff = 0;
			for (int i = 0; i < 3; i++)
			{
				if (arrColor[i] > 0)
				{
					iColorDiff++;
				}
			}
			if ((m_iStageNum == 0 && iColorDiff > 1) || (m_iStageNum == 1 && iColorDiff == 1) || m_iStageNum == 2)
			{
				if (m_iStageNum == 0)
				{
					CScene_Manager::Get_Instance()->Scene_Change(CScene_Manager::ID::SCENE_STAGE);
				}
				else if (m_iStageNum == 1)
				{
					CScene_Manager::Get_Instance()->Scene_Change(CScene_Manager::ID::SCENE_MONSTER1);
					//CScene_Manager::Get_Instance()->Scene_Change(CScene_Manager::ID::SCENE_MONSTER2);
					//CScene_Manager::Get_Instance()->Scene_Change(CScene_Manager::ID::SCENE_BOSS);
				}
				else //Å×½ºÆ® ¸Ê
				{
					CScene_Manager::Get_Instance()->Scene_Change(CScene_Manager::ID::SCENE_TEST);
				}


			}
			else
			{
				if (CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::UI)->size() == 0)
				{
					CGameObject* pUI = CUI::Create(WINCX / 2, WINCY / 2, 320, 252, L"pop_up_ERR_team");
					if (nullptr != pUI)
					{
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::UI, pUI);
					}

					CGameObject* pButton = CButton::Create(WINCX / 2, 396, CButton::TYPE::BT_CLOSE);
					if (nullptr != pButton)
					{
						CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUTTON, pButton);
					}
				}
			}
			pButton->Set_Clicked(0);
			break;
		}
	}


	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager();
}

void CScene_Room::Render_Scene(HDC hDC)
{
	HDC hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"room");
	if (nullptr == hMemDC)
		return;
	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"map_select");
	if (nullptr == hMemDC)
		return;
	BitBlt(hDC, 481, 340, 298, 146, hMemDC, m_iStageNum* 298, 0, SRCCOPY);



	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager(hDC);

}

void CScene_Room::Release_Scene()
{
	CGameObject_Manager::Get_Instance()->Delete_GameObject(OBJECT::ID::BUTTON);

}

CScene* CScene_Room::Create()
{
	CScene* pInstance = new CScene_Room;
	if (0 > pInstance->Ready_Scene())
		Safe_Delete(pInstance);

	return pInstance;
}
