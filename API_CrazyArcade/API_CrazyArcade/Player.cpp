#include "pch.h"
#include "Player.h"
#include "Bubble.h"
#include "Tile.h"
#include "Block.h"
#include "GameObject_Manager.h"
#include "Scene_Stage.h"
#include "ComPlayer.h"


CPlayer::CPlayer()
	: m_tFrame({})
	, m_iBottom(-15)
	, m_iColor(0)
{
}


CPlayer::~CPlayer()
{
}



void CPlayer::MoveFrame()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();
	}
	if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
	{
		if (m_eCurState == DIE)
		{
			m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
		}		
		else if (m_eCurState == READY)
		{
			m_tFrame.iFrameStart = m_tFrame.iFrameEnd-1;
		}
		else
		{
			m_tFrame.iFrameStart = 0;
		}
	}

}

void CPlayer::Change_State()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case CPlayer::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameState = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();

			m_tFrame.iFrameX = 64;
			m_tFrame.iFrameY = 76;

			m_iBottom = -15;
			break;
		case CPlayer::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameState = 0;
			m_tFrame.dwFrameSpeed = 200/ m_fSpeed;
			m_tFrame.dwFrameTime = GetTickCount();

			m_tFrame.iFrameX = 64;
			m_tFrame.iFrameY = 76;

			m_iBottom = -15;
			break;
		case CPlayer::TRAP:			

			CSound_Manager::Get_Instance()->PlaySound(L"SFX_Player_Trap.mp3", CSound_Manager::CHANNELID::PLAYER);

			m_pFrameKey = (TCHAR*)L"bazzi_trap";
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 14;
			m_tFrame.iFrameState = 0;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();

			m_tFrame.iFrameX = 88;
			m_tFrame.iFrameY = 144;

			m_iBottom = -25;
			break;
		case CPlayer::DIE:

			CSound_Manager::Get_Instance()->PlaySound(L"SFX_Player_Die.mp3", CSound_Manager::CHANNELID::PLAYER);
			m_pFrameKey = (TCHAR*)L"bazzi_die";
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 10;
			m_tFrame.iFrameState = 0;
			m_tFrame.dwFrameSpeed = 150;
			m_tFrame.dwFrameTime = GetTickCount();

			m_tFrame.iFrameX = 88;
			m_tFrame.iFrameY = 144;

			m_iBottom = -25;

			break;

		case CPlayer::READY:
			m_pFrameKey = (TCHAR*)L"bazzi_ready";
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 17;
			m_tFrame.iFrameState = 0;
			m_tFrame.dwFrameSpeed = 60;
			m_tFrame.dwFrameTime = GetTickCount();

			m_tFrame.iFrameX = 64;
			m_tFrame.iFrameY = 86;

			m_iBottom = -20;

			break;
		case CPlayer::WIN:

			m_pFrameKey = (TCHAR*)L"bazzi_win";
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameState = 0;
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.iFrameX = 64;
			m_tFrame.iFrameY = 80;

			m_iBottom = -15;

			break;

		case CPlayer::LIVE:
			CSound_Manager::Get_Instance()->PlaySound(L"SFX_Player_Live.mp3", CSound_Manager::CHANNELID::PLAYER);
			m_pFrameKey = (TCHAR*)L"bazzi_live";
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iFrameState = 0;
			m_tFrame.dwFrameSpeed = 60;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.iFrameX = 88;
			m_tFrame.iFrameY = 144;
			m_iBottom = -25;

			break;
		default:
			break;
		}
		m_eCurState = m_eNextState;
	}
}

int CPlayer::Ready_GameObject()
{
	m_eObjectType = OBJECT::ID::PLAYER;

	//m_tInfo.fX = TILESTARTX;
	//m_tInfo.fY = TILESTARTY;
	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;

	//m_iColor = 0;

	m_eNextState = STATE::IDLE;

	m_iBottom = -15;


	m_fSpeed = 5 * 0.5f;
	m_fSpeedMax = 9 * 0.5f;
	m_iBubbleNum = 1;
	m_iBubbleMaxNum = 6;
	m_iBubblePower = 1;
	m_iBubbleMaxPower = 7;
	m_bKick = false;
	m_iNeedleNum = 0;


	m_pFrameKey = (TCHAR*)L"bazzi_down";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameState = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.iFrameX = 64;
	m_tFrame.iFrameY = 76;


	m_updateTimer = GetTickCount();
	m_iUpdateCnt = 0;

	m_bIsShow = true;

	m_bNoDamage = 0;

	return READY_OK;
}

int CPlayer::Update_GameObject()
{
	if (CKey_Manager::Get_Instance()->Key_Pressed(VK_F2))
	{
		m_bNoDamage ^= 1;
	}

	if (m_eCurState == LIVE && m_tFrame.iFrameStart != m_tFrame.iFrameEnd)
	{
		return OBJ_NOEVENT;
	}

	CScene_Manager::ID eSceneID = CScene_Manager::Get_Instance()->Get_SceneID();
	if (eSceneID >= CScene_Manager::ID::SCENE_STAGE && eSceneID <= CScene_Manager::ID::SCENE_BOSS)
	{
		if (CScene_Manager::Get_Instance()->Get_Scene()->Get_State() == CScene::STATE::READY)
		{
			m_eNextState = READY;
			return OBJ_NOEVENT;
		}
		else if (CScene_Manager::Get_Instance()->Get_Scene()->Get_State() == CScene::STATE::PLAYER1_WIN)
		{
			if (m_iPlayerNum == 1)
			{
				m_eNextState = WIN;
				if (m_eCurState == STATE::TRAP)
				{
					m_eNextState = LIVE;
				}
				return OBJ_NOEVENT;
			}
		}
		else if (CScene_Manager::Get_Instance()->Get_Scene()->Get_State() == CScene::STATE::PLAYER2_WIN)
		{
			if (m_iPlayerNum == 2)
			{
				m_eNextState = WIN;
				if (m_eCurState == STATE::TRAP)
				{
					m_eNextState = LIVE;
				}
				return OBJ_NOEVENT;
			}
		}
		else if (CScene_Manager::Get_Instance()->Get_Scene()->Get_State() == CScene::STATE::WIN)
		{
			m_eNextState = WIN;
			if (m_eCurState == STATE::TRAP)
			{
				m_eNextState = LIVE;
			}
			return OBJ_NOEVENT;
		}

		if (m_eCurState == DIE && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_bIsShow = false;
		}
	}
	else
	{
		m_pFrameKey = (TCHAR*)L"bazzi_down";
		m_eNextState = IDLE;
		return OBJ_NOEVENT;
	}


	if (m_bIsShow == false)
	{
		return OBJ_NOEVENT;
	}


	if (m_eCurState == READY)
	{
		m_pFrameKey = (TCHAR*)L"bazzi_down";
		m_eNextState = IDLE;
	}
	
	m_iUpdateCnt += (GetTickCount() - m_updateTimer);


	while (m_iUpdateCnt > 15)
	{
		m_iUpdateCnt -= 15;
		if (m_eCurState == LIVE && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_pFrameKey = (TCHAR*)L"bazzi_down";
			m_eNextState = IDLE;
		}
		if (m_eCurState == TRAP && m_tFrame.iFrameStart >= 2)
		{
			m_tFrame.dwFrameSpeed = 750;
			if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				m_eNextState = DIE;
			}
			
			list<CGameObject*>* pObject = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER);
			for (CGameObject* pPlayer : *pObject)
			{
				if (pPlayer == this)
				{
					continue;
				}
				if (static_cast<CPlayer*>(pPlayer)->Get_State() != STATE::IDLE && static_cast<CPlayer*>(pPlayer)->Get_State() != STATE::WALK)
				{
					continue;
				}
				if (PtInRect(&m_tRect, pPlayer->Get_Pos()) && m_iColor != static_cast<CPlayer*>(pPlayer)->Get_Color())
				{
					m_eNextState = DIE;
				}
				if (PtInRect(&m_tRect, pPlayer->Get_Pos()) && m_iColor == static_cast<CPlayer*>(pPlayer)->Get_Color())
				{
					m_eNextState = LIVE;
				}
			}

			list<CGameObject*>* pComList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::COMPLAYER);
			for (CGameObject* pCom : *pComList)
			{
				if (pCom == this)
				{
					continue;
				}
				if (static_cast<CComPlayer*>(pCom)->Get_State() != STATE::IDLE && static_cast<CComPlayer*>(pCom)->Get_State() != STATE::WALK)
				{
					continue;
				}
				if (PtInRect(&m_tRect, pCom->Get_Pos()) && m_iColor != static_cast<CComPlayer*>(pCom)->Get_Color())
				{
					m_eNextState = DIE;
				}
				if (PtInRect(&m_tRect, pCom->Get_Pos()) && m_iColor == static_cast<CComPlayer*>(pCom)->Get_Color())
				{
					m_eNextState = LIVE;
				}
			}
		}
		if ((m_iPlayerNum == 0 && CKey_Manager::Get_Instance()->Key_Down(VK_UP)) ||
			(m_iPlayerNum == 1 && CKey_Manager::Get_Instance()->Key_Down('R')) ||
			(m_iPlayerNum == 2 && CKey_Manager::Get_Instance()->Key_Down(VK_UP)))
		{
			if (m_eCurState == WALK || m_eCurState == IDLE)
			{
				m_tInfo.fY -= m_fSpeed;
				m_pFrameKey = (TCHAR*)L"bazzi_up";
				m_eNextState = WALK;
				m_tFrame.iFrameEnd = 7;

				Block_Collision(DIRECTION::UP, m_fSpeed);
			}
			else if (m_eCurState == TRAP)
			{
				m_tInfo.fY -= m_fSpeed / 10.f;
				Block_Collision(DIRECTION::UP, m_fSpeed / 10.f);
			}
		}
		else if ((m_iPlayerNum == 0 && CKey_Manager::Get_Instance()->Key_Down(VK_DOWN)) ||
			(m_iPlayerNum == 1 && CKey_Manager::Get_Instance()->Key_Down('F')) ||
			(m_iPlayerNum == 2 && CKey_Manager::Get_Instance()->Key_Down(VK_DOWN)))
		{
			if (m_eCurState == WALK || m_eCurState == IDLE)
			{
				m_tInfo.fY += m_fSpeed;
				m_pFrameKey = (TCHAR*)L"bazzi_down";
				m_eNextState = WALK;
				m_tFrame.iFrameEnd = 7;

				Block_Collision(DIRECTION::DOWN, m_fSpeed);
			}
			else if (m_eCurState == TRAP)
			{
				m_tInfo.fY += m_fSpeed / 10.f;
				Block_Collision(DIRECTION::DOWN, m_fSpeed / 10.f);
			}
		}
		else if ((m_iPlayerNum == 0 && CKey_Manager::Get_Instance()->Key_Down(VK_LEFT)) ||
			(m_iPlayerNum == 1 && CKey_Manager::Get_Instance()->Key_Down('D')) ||
			(m_iPlayerNum == 2 && CKey_Manager::Get_Instance()->Key_Down(VK_LEFT)))
		{
			if (m_eCurState == WALK || m_eCurState == IDLE)
			{
				m_tInfo.fX -= m_fSpeed;
				m_pFrameKey = (TCHAR*)L"bazzi_left";
				m_eNextState = WALK;
				m_tFrame.iFrameEnd = 5;

				Block_Collision(DIRECTION::LEFT,m_fSpeed);
			}
			else if (m_eCurState == TRAP)
			{
				m_tInfo.fX -= m_fSpeed / 10.f;
				Block_Collision(DIRECTION::LEFT, m_fSpeed / 10.f);
			}
		}
		else if ((m_iPlayerNum == 0 && CKey_Manager::Get_Instance()->Key_Down(VK_RIGHT)) ||
			(m_iPlayerNum == 1 && CKey_Manager::Get_Instance()->Key_Down('G')) ||
			(m_iPlayerNum == 2 && CKey_Manager::Get_Instance()->Key_Down(VK_RIGHT)))
		{
			if (m_eCurState == WALK || m_eCurState == IDLE)
			{
				m_tInfo.fX += m_fSpeed;
				m_pFrameKey = (TCHAR*)L"bazzi_right";
				m_eNextState = WALK;
				m_tFrame.iFrameEnd = 5;

				Block_Collision(DIRECTION::RIGHT,m_fSpeed);
			}
			else if (m_eCurState == TRAP)
			{
				m_tInfo.fX += m_fSpeed / 10.f;
				Block_Collision(DIRECTION::RIGHT, m_fSpeed / 10.f);
			}
		}
		else if(m_eCurState == WALK || m_eCurState == IDLE)
		{
			m_eNextState = IDLE;
		}
	}
	m_updateTimer = GetTickCount();

	if (m_eCurState == IDLE || m_eCurState == WALK)
	{
		if ((m_iPlayerNum == 0 && CKey_Manager::Get_Instance()->Key_Pressed(VK_SPACE)) ||
			(m_iPlayerNum == 1 && CKey_Manager::Get_Instance()->Key_Pressed(VK_LSHIFT)) ||
			(m_iPlayerNum == 2 && CKey_Manager::Get_Instance()->Key_Pressed(VK_RSHIFT)))
		{
			int iBubbleCnt = 0;
			const auto& pBubbleList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::BUBBLE);
			for (auto& pBubble : *pBubbleList)
			{
				if (static_cast<CBubble*>(pBubble)->Get_Owner() == this)
				{
					iBubbleCnt++;
				}
			}
			if (iBubbleCnt < m_iBubbleNum)
			{
				CGameObject* pTile = CTile_Manager::Get_Instance()->Pick_Tile(m_tInfo.fX, m_tInfo.fY);
				if (pTile)
				{
					auto listOnTile = static_cast<CTile*>(pTile)->Get_ListObjectOnTile();
					if (listOnTile->empty() || listOnTile->front()->Get_ObjectType() != OBJECT::BUBBLE)
					{
						CGameObject* pBubble = CBubble::Create(pTile->Get_Info()->fX, pTile->Get_Info()->fY, m_iBubblePower, this);
						if (nullptr != pBubble)
						{
							CSound_Manager::Get_Instance()->PlaySound(L"SFX_Bubble_make.mp3", CSound_Manager::CHANNELID::EFFECT);
							CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUBBLE, pBubble);
						}
					}
				}
			}
		}
	}
	if (m_eCurState == TRAP)
	{
		if ((m_iPlayerNum == 0 && CKey_Manager::Get_Instance()->Key_Pressed(VK_LCONTROL)) ||
			(m_iPlayerNum == 1 && CKey_Manager::Get_Instance()->Key_Pressed(VK_LCONTROL)) ||
			(m_iPlayerNum == 2 && CKey_Manager::Get_Instance()->Key_Pressed(VK_RCONTROL)) || 
			(m_iPlayerNum == 2 && CKey_Manager::Get_Instance()->Key_Pressed(VK_HANJA)))
		{
			if (m_iNeedleNum > 0)
			{
				m_eNextState = STATE::LIVE;
				m_iNeedleNum--;
			}
		}
	}

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update_GameObject()
{
	if (m_tInfo.fX < TILESTARTX)
	{
		m_tInfo.fX = TILESTARTX;
	}
	if (m_tInfo.fX > TILESTARTX + TILECX * (TILEX - 1))
	{
		m_tInfo.fX = TILESTARTX + TILECX * (TILEX - 1);
	}	
	if (m_tInfo.fY < TILESTARTY)
	{
		m_tInfo.fY = TILESTARTY;
	}
	if (m_tInfo.fY > TILESTARTY + TILECY * (TILEY - 1))
	{
		m_tInfo.fY = TILESTARTY + TILECY * (TILEY - 1);
	}


	MoveFrame();
	Change_State();
}

void CPlayer::Render_GameObject(HDC hDC)
{
	CGameObject::Update_Rect_GameObject();
	if (m_bNoDamage == true)
	{
		TextOut(hDC, 100, 0, L"무적ON", lstrlen(L"무적ON"));
	}

	if (m_bIsShow == false)
	{
		return;
	}

	CScene_Manager::ID eSceneID = CScene_Manager::Get_Instance()->Get_SceneID();
	if (eSceneID >= CScene_Manager::ID::SCENE_STAGE && eSceneID <= CScene_Manager::ID::SCENE_BOSS)
	{
		CTile* pTile = static_cast<CTile*>(CTile_Manager::Get_Instance()->Pick_Tile(m_tInfo.fX, m_tInfo.fY));
		if (pTile)
		{
			for (CGameObject* pObject : *(pTile->Get_ListObjectOnTile()))
			{
				if (pObject->Get_ObjectType() == OBJECT::BLOCK && static_cast<CBlock*>(pObject)->Get_BlockType() == BLOCK_TYPE::BUSH)
				{
					return;
				}
			}
		}
	}


	HDC hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"ShadowCharacter");
	if (nullptr == hMemDC)
		return;

	if (m_eCurState != DIE)
	{
		GdiTransparentBlt(hDC,
			m_tInfo.fX - 16,
			m_tInfo.fY + 3,
			32, 15,
			hMemDC,
			0, 0,
			32, 15,
			RGB(255, 0, 255));
	}


	hMemDC = CBitmap_Manager::Get_Instance()->FindImage(m_pFrameKey);
	if (nullptr == hMemDC)
		return;

	GdiTransparentBlt(hDC,
		m_tInfo.fX - m_tFrame.iFrameX * 0.78f * 0.5f,
		m_tInfo.fY - m_tFrame.iFrameY * 0.78f * 0.5f + m_iBottom,
		m_tFrame.iFrameX * 0.78f, m_tFrame.iFrameY * 0.78f,
		hMemDC,
		m_tFrame.iFrameStart * m_tFrame.iFrameX, m_iColor * m_tFrame.iFrameY,
		m_tFrame.iFrameX, m_tFrame.iFrameY,
		RGB(255, 0, 255));

	if (m_bIsShow == true)
	{
		if (m_eCurState != DIE)
		{
			int iMoveY = 0;
			int iTimer = GetTickCount();
			if (iTimer % 1000 < 250)
			{
				iMoveY = -2;
			}
			else if (iTimer % 1000 > 500 && iTimer % 1000 < 750)
			{
				iMoveY = +2;
			}
			if (m_iPlayerNum == 1)
			{
				hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"player1");
			}
			else if (m_iPlayerNum == 2)
			{
				hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"player2");
			}
			else
			{
				hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"solo_player");
			}
			if (nullptr == hMemDC)
				return;

			GdiTransparentBlt(hDC,
				m_tInfo.fX - 12,
				m_tInfo.fY - 14 - 60 + iMoveY,
				24, 28,
				hMemDC,
				0, 0,
				24, 28,
				RGB(255, 0, 255));
		}
	}
}

void CPlayer::Release_GameObject()
{

}

void CPlayer::Eat_Item(ITEM_TYPE::TYPE eItemType)
{

	CSound_Manager::Get_Instance()->PlaySound(L"SFX_Item_Get.mp3", CSound_Manager::CHANNELID::EFFECT);
	switch (eItemType)
	{
	case ITEM_TYPE::BUBBLE_UP:
		m_iBubbleNum = (m_iBubbleNum < m_iBubbleMaxNum) ? m_iBubbleNum + 1 : m_iBubbleMaxNum;
		break;
	case ITEM_TYPE::SPEED_UP:
		m_fSpeed = (m_fSpeed < m_fSpeedMax) ? m_fSpeed + 0.3f : m_fSpeedMax;
		break;
	case ITEM_TYPE::POWER_UP:
		m_iBubblePower = (m_iBubblePower < m_iBubbleMaxPower) ? m_iBubblePower + 1 : m_iBubbleMaxPower;
		break;
	case ITEM_TYPE::POWER_MAX:
		m_iBubblePower = m_iBubbleMaxPower;
		break;
	case ITEM_TYPE::RED_DEVIL:
		m_bKick = true;
		m_fSpeed = m_fSpeedMax;
		break;
	case ITEM_TYPE::KICK:
		m_bKick = true;
		break;
	case ITEM_TYPE::NEEDLE:
		m_iNeedleNum++;
		break;
	case ITEM_TYPE::ITEM_END:
		break;
	default:
		break;
	}
}



void CPlayer::Block_Collision(DIRECTION::DIREC eDir, float fSpeed)
{
	CTile* pTile[9] = {};
	for (int i = 0; i < 9; i++)
	{
		int iMoveX = -40 + (i % 3) * 40;
		int iMoveY = -40 + (i / 3) * 40;
		//주위 타일만 가지고 상호작용하기위해, 플레이어를 감싸는 9개의 타일을 가져옴
		pTile[i] = static_cast<CTile*>(CTile_Manager::Get_Instance()->Pick_Tile(m_tInfo.fX + iMoveX, m_tInfo.fY + iMoveY));
	}
	for (int i = 0; i < 9; i++)
	{
		if (pTile[i])
		{
			auto listOnTile = pTile[i]->Get_ListObjectOnTile();

			for (CGameObject* pObject : *listOnTile)
			{
				//각 타일에 이동을 막는 블록들이 있는지 확인
				if ((pObject->Get_ObjectType() == OBJECT::BLOCK && static_cast<CBlock*>(pObject)->Get_BlockType() != BLOCK_TYPE::BUSH)
					||(i !=4 && pObject->Get_ObjectType() == OBJECT::BUBBLE))
				{
					CGameObject::Update_Rect_GameObject();
					RECT rect{};
					RECT RectTile = pTile[i]->Get_Rect();
					if (IntersectRect(&rect, &m_tRect, &RectTile))
					{
						//겹치는 사각형을 이용해 움직이는 방향에 따라 위치 보정
						switch (eDir)
						{
						case DIRECTION::UP:
							if (i == 0 || i == 1 || i == 2)
							{
								if (pObject->Get_ObjectType() == OBJECT::BUBBLE)
								{
									m_tInfo.fY += fSpeed;
								}
								else
								{
									m_tInfo.fY = (int)m_tInfo.fY + (rect.bottom - rect.top);
								}
							}

							if (pTile[1])
							{
								bool bPass = true;
								for (CGameObject* pObject : *(pTile[1]->Get_ListObjectOnTile()))
								{
									if (pObject->Get_ObjectType() == OBJECT::BUBBLE || 
										(pObject->Get_ObjectType() == OBJECT::BLOCK && static_cast<CBlock*>(pObject)->Get_BlockType() != BLOCK_TYPE::BUSH))
									{
										bPass=false;
										if (pObject->Get_ObjectType() == OBJECT::BUBBLE && m_bKick == true && m_eCurState == STATE::WALK)
										{
											static_cast<CBubble*>(pObject)->Kick_Count(DIRECTION::UP);
										}
									}
								}
								if (bPass)
								{
									if (rect.right == m_tRect.right && (rect.right - rect.left) < m_tInfo.iCX / 2)
									{
										if ((rect.right - rect.left) < fSpeed)
										{
											m_tInfo.fX -= (rect.right - rect.left);
										}
										else
										{
											m_tInfo.fX -= fSpeed;
										}
									}
									if (rect.left == m_tRect.left && (rect.right - rect.left) < m_tInfo.iCX / 2)
									{
										if ((rect.right - rect.left) < fSpeed)
										{
											m_tInfo.fX += (rect.right - rect.left);
										}
										else
										{
											m_tInfo.fX += fSpeed;
										}
									}
								}
							}

							break;
						case DIRECTION::DOWN:							
							if (i == 6 || i == 7 || i == 8)
							{
								if (pObject->Get_ObjectType() == OBJECT::BUBBLE)
								{
									m_tInfo.fY -= fSpeed;
								}
								else
								{
									m_tInfo.fY = (int)m_tInfo.fY - (rect.bottom - rect.top);
								}
							}

							if (pTile[7])
							{
								bool bPass = true;
								for (CGameObject* pObject : *(pTile[7]->Get_ListObjectOnTile()))
								{
									if (pObject->Get_ObjectType() == OBJECT::BUBBLE ||
										(pObject->Get_ObjectType() == OBJECT::BLOCK && static_cast<CBlock*>(pObject)->Get_BlockType() != BLOCK_TYPE::BUSH))
									{
										bPass = false;
										if (pObject->Get_ObjectType() == OBJECT::BUBBLE && m_bKick == true && m_eCurState == STATE::WALK)
										{
											static_cast<CBubble*>(pObject)->Kick_Count(DIRECTION::DOWN);
										}
									}
								}
								if (bPass)
								{
									if (rect.right == m_tRect.right && (rect.right - rect.left) < m_tInfo.iCX / 2)
									{
										if ((rect.right - rect.left) < fSpeed)
										{
											m_tInfo.fX -= (rect.right - rect.left);
										}
										else
										{
											m_tInfo.fX -= fSpeed;
										}
									}
									if (rect.left == m_tRect.left && (rect.right - rect.left) < m_tInfo.iCX / 2)
									{
										if ((rect.right - rect.left) < fSpeed)
										{
											m_tInfo.fX += (rect.right - rect.left);
										}
										else
										{
											m_tInfo.fX += fSpeed;
										}
									}
								}
							}


							break;
						case DIRECTION::LEFT:
							if (i == 0 || i == 3 || i == 6)
							{
								if (pObject->Get_ObjectType() == OBJECT::BUBBLE)
								{
									m_tInfo.fX += fSpeed;
								}
								else
								{
									m_tInfo.fX = (int)m_tInfo.fX + (rect.right - rect.left);
								}
							}
							if (pTile[3])
							{
								bool bPass = true;
								for (CGameObject* pObject : *(pTile[3]->Get_ListObjectOnTile()))
								{
									if (pObject->Get_ObjectType() == OBJECT::BUBBLE ||
										(pObject->Get_ObjectType() == OBJECT::BLOCK && static_cast<CBlock*>(pObject)->Get_BlockType() != BLOCK_TYPE::BUSH))
									{
										bPass = false;										
										if (pObject->Get_ObjectType() == OBJECT::BUBBLE && m_bKick == true && m_eCurState == STATE::WALK)
										{
											static_cast<CBubble*>(pObject)->Kick_Count(DIRECTION::LEFT);
										}
									}
								}
								if (bPass)
								{
									if (rect.bottom == m_tRect.bottom && (rect.bottom - rect.top) < m_tInfo.iCY / 2)
									{
										if ((rect.bottom - rect.top) < fSpeed)
										{
											m_tInfo.fY -= (rect.bottom - rect.top);
										}
										else
										{
											m_tInfo.fY -= fSpeed;
										}
									}
									if (rect.top == m_tRect.top && (rect.bottom - rect.top) < m_tInfo.iCY / 2)
									{
										if ((rect.bottom - rect.top) < fSpeed)
										{
											m_tInfo.fY += (rect.bottom - rect.top);
										}
										else
										{
											m_tInfo.fY += fSpeed;
										}
									}
								}
							}

							break;
						case DIRECTION::RIGHT:
							if (i == 2 || i == 5 || i == 8)
							{
								if (pObject->Get_ObjectType() == OBJECT::BUBBLE)
								{
									m_tInfo.fX -= fSpeed;
								}
								else
								{
									m_tInfo.fX = (int)m_tInfo.fX - (rect.right - rect.left);
								}
							}
							if (pTile[5])
							{
								bool bPass = true;
								for (CGameObject* pObject : *(pTile[5]->Get_ListObjectOnTile()))
								{
									if (pObject->Get_ObjectType() == OBJECT::BUBBLE ||
										(pObject->Get_ObjectType() == OBJECT::BLOCK && static_cast<CBlock*>(pObject)->Get_BlockType() != BLOCK_TYPE::BUSH))
									{
										bPass = false;
										if (pObject->Get_ObjectType() == OBJECT::BUBBLE && m_bKick == true && m_eCurState == STATE::WALK)
										{
											static_cast<CBubble*>(pObject)->Kick_Count(DIRECTION::RIGHT);
										}
									}
								}
								if (bPass)
								{
									if (rect.bottom == m_tRect.bottom && (rect.bottom - rect.top) < m_tInfo.iCY / 2)
									{
										if ((rect.bottom - rect.top) < fSpeed)
										{
											m_tInfo.fY -= (rect.bottom - rect.top);
										}
										else
										{
											m_tInfo.fY -= fSpeed;
										}
									}
									if (rect.top == m_tRect.top && (rect.bottom - rect.top) < m_tInfo.iCY / 2)
									{
										if ((rect.bottom - rect.top) < fSpeed)
										{
											m_tInfo.fY += (rect.bottom - rect.top);
										}
										else
										{
											m_tInfo.fY += fSpeed;
										}
									}
								}
							}

							break;
						default:
							break;
						}
					}
				}
			}
		}
	}
}

CGameObject* CPlayer::Create(int iPlayerNum)
{
	CGameObject* pInstance = new CPlayer;
	if (0 > pInstance->Ready_GameObject())
	{
		Safe_Delete(pInstance);
		return nullptr;
	}
	static_cast<CPlayer*>(pInstance)->Set_PlayerNum(iPlayerNum);
	return pInstance;
}
