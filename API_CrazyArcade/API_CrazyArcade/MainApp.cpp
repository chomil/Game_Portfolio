#include "pch.h"
#include "MainApp.h"
#include "Player.h"
#include "GameObject_Manager.h"
#include "Bitmap_Manager.h"
#include "Scene_Manager.h"
#include "Mouse.h"

CMainApp::CMainApp()
	:m_iFPS(0)
	, m_szFPS(L"")
	, m_dwFPSTime(0)
	, m_hDC(nullptr)
	, m_bToggle(false)
{
}
CMainApp::~CMainApp()
{
	Release_MainApp();
}

int CMainApp::Ready_MainApp()
{
	srand(time(NULL));

	m_hDC = GetDC(g_hWnd);
	m_dwFPSTime = GetTickCount();


	CSound_Manager::Get_Instance()->Initialize();

	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/character/bazzi/bazzi_up.bmp", L"bazzi_up");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/character/bazzi/bazzi_down.bmp", L"bazzi_down");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/character/bazzi/bazzi_left.bmp", L"bazzi_left");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/character/bazzi/bazzi_right.bmp", L"bazzi_right");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/character/bazzi/bazzi_trap.bmp", L"bazzi_trap");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/character/bazzi/bazzi_die.bmp", L"bazzi_die");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/character/bazzi/bazzi_win.bmp", L"bazzi_win");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/character/bazzi/bazzi_ready.bmp", L"bazzi_ready");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/character/bazzi/bazzi_live.bmp", L"bazzi_live");


	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/monster/Monster01.bmp", L"Monster01");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/monster/Monster02.bmp", L"Monster02");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/monster/Boss01.bmp", L"Boss01");

	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/effect/solo_player.bmp", L"solo_player");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/effect/player1.bmp", L"player1");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/effect/player2.bmp", L"player2");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/effect/ShadowCharacter.bmp", L"ShadowCharacter");

	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/item/item.bmp", L"item");

	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/effect/bubble.bmp", L"bubble");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/effect/Explosion.bmp", L"Explosion");

	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/mouse.bmp", L"mouse");

	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/room.bmp", L"room");

	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/StageFrame.bmp", L"StageFrame");

	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/START.bmp", L"START");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/bazzi_info.bmp", L"bazzi_info");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/player1_win.bmp", L"player1_win");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/player2_win.bmp", L"player2_win");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/draw.bmp", L"draw");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/win.bmp", L"win");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/lose.bmp", L"lose");

	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/start_button.bmp", L"start_button");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/bazzi_select.bmp", L"bazzi_select");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/CharacterSlot.bmp", L"CharacterSlot");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/color_button.bmp", L"color_button");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/map_select.bmp", L"map_select");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/arrow_button.bmp", L"arrow_button");

	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/exit_button.bmp", L"exit_button");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/close_button.bmp", L"close_button");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/off_button.bmp", L"off_button");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/back_button.bmp", L"back_button");


	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/login.bmp", L"login");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/1p_button.bmp", L"1p_button");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/2p_button.bmp", L"2p_button");

	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/pop_up_ERR_team.bmp", L"pop_up_ERR_team");

	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/Boss_hp.bmp", L"Boss_hp");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/ui/Item_slot_2p.bmp", L"Item_slot_2p");

	


	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/tile/Tile_town.bmp", L"Tile_town");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/block/block_town.bmp", L"block_town");

	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/tile/Tile_pirate.bmp", L"Tile_pirate");
	CBitmap_Manager::Get_Instance()->Insert_Bitmap(L"../Image/block/block_pirate.bmp", L"block_pirate");




	CGameObject* pObject = CMouse::Create();
	assert(!(pObject == nullptr));
	CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::MOUSE, pObject);


	CScene_Manager::Get_Instance()->Scene_Change(CScene_Manager::SCENE_LOGIN);
	return READY_OK;
}

void CMainApp::Update_MainApp()
{
	CKey_Manager::Get_Instance()->Key_Update();
	CScene_Manager::Get_Instance()->Update_Scene_Manager();

}

void CMainApp::Render_MainApp()
{
	static HDC MemDC, tempDC;
	static HBITMAP  BackBit, oldBackBit;

	MemDC = CreateCompatibleDC(m_hDC);
	BackBit = CreateCompatibleBitmap(m_hDC, WINCX, WINCY);
	oldBackBit = (HBITMAP)SelectObject(MemDC, BackBit);
	PatBlt(MemDC, 0, 0, WINCX, WINCY, WHITENESS);

	tempDC = m_hDC;
	m_hDC = MemDC;
	MemDC = tempDC;

	CScene_Manager::Get_Instance()->Render_Scene_Manager(m_hDC);

	++m_iFPS;
	if (m_dwFPSTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		m_iFPS = 0;
		m_dwFPSTime = GetTickCount();
	}
	if (CKey_Manager::Get_Instance()->Key_Pressed(VK_F1))
	{
		m_bToggle ^= 1;
	}
	if (m_bToggle)
	{
		TextOut(m_hDC, 0, 0, m_szFPS, lstrlen(m_szFPS));
	}

	tempDC = m_hDC;
	m_hDC = MemDC;
	MemDC = tempDC;

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, MemDC, 0, 0, SRCCOPY);
	SelectObject(MemDC, oldBackBit);
	DeleteObject(BackBit);
	DeleteDC(MemDC);
}

void CMainApp::Release_MainApp()
{
	ReleaseDC(g_hWnd, m_hDC);
	CKey_Manager::Destroy_Instance();
	CBitmap_Manager::Destroy_Instance();
	CGameObject_Manager::Destroy_Instance();
	CScene_Manager::Destroy_Instance();
	CTile_Manager::Destroy_Instance();
	 
}