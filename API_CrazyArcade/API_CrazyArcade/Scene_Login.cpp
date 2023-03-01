#include "pch.h"
#include "Scene_Login.h"
#include "Player.h"
#include "GameObject_Manager.h"
#include "Button.h"

CScene_Login::CScene_Login()
{
}

CScene_Login::~CScene_Login()
{
	Release_Scene();
}

int CScene_Login::Ready_Scene()
{
	CSound_Manager::Get_Instance()->StopSound(CSound_Manager::CHANNELID::BGM);
	CSound_Manager::Get_Instance()->PlayBGM(L"BGM_Login.mp3");

	CGameObject_Manager::Get_Instance()->Delete_GameObject(OBJECT::PLAYER);



	CGameObject* pObject = CButton::Create(250, 490, CButton::BT_1P);
	if (nullptr == pObject)
		return READY_FAILED;
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUTTON, pObject);	
	
	
	pObject = CButton::Create(550, 490, CButton::BT_2P);
	if (nullptr == pObject)
		return READY_FAILED;
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUTTON, pObject);


	pObject = CButton::Create(764, 30, CButton::TYPE::BT_OFF);
	if (nullptr != pObject)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUTTON, pObject);
	}

	return 0;
}

void CScene_Login::Update_Scene()
{
	auto pButtonList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::BUTTON);
	for (auto pObj : *pButtonList)
	{
		CButton* pButton = static_cast<CButton*>(pObj);

		if (pButton->Get_ButtonType() == CButton::TYPE::BT_OFF)
		{
			if (pButton->Get_Clicked() > 0)
			{
				DestroyWindow(g_hWnd);
			}
		}
		if (pButton->Get_ButtonType() == CButton::BT_1P)
		{
			if (pButton->Get_Clicked() > 0)
			{
				CGameObject* pObject = CPlayer::Create(0);
				if (nullptr == pObject)
					return;
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::PLAYER, pObject);

				CScene_Manager::Get_Instance()->Scene_Change(CScene_Manager::SCENE_ROOM);
				return;
			}
		}
		else if (pButton->Get_ButtonType() == CButton::BT_2P)
		{
			if (pButton->Get_Clicked() > 0)
			{
				CGameObject* pObject = CPlayer::Create(1);
				if (nullptr == pObject)
					return;
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::PLAYER, pObject);

				pObject = CPlayer::Create(2);
				if (nullptr == pObject)
					return;
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::PLAYER, pObject);
				static_cast<CPlayer*>(pObject)->Set_Color(1);

				CScene_Manager::Get_Instance()->Scene_Change(CScene_Manager::SCENE_ROOM);
				return;
			}
		}
	}

	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager();
}

void CScene_Login::Render_Scene(HDC hDC)
{

	HDC hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"login");
	if (nullptr == hMemDC)
		return;
	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager(hDC);

}

void CScene_Login::Release_Scene()
{
	CGameObject_Manager::Get_Instance()->Delete_GameObject(OBJECT::BUTTON);
}

CScene* CScene_Login::Create()
{
	CScene* pInstance = new CScene_Login;
	if (0 > pInstance->Ready_Scene())
		Safe_Delete(pInstance);

	return pInstance;
}
