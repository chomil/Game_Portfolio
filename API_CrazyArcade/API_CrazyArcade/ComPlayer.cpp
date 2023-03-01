#include "pch.h"
#include "ComPlayer.h"
#include "Bubble.h"
#include "Tile.h"
#include "Block.h"
#include "GameObject_Manager.h"
#include "Scene_Stage.h"
#include "PathFind.h"
#include "Player.h"


CComPlayer::CComPlayer()
	: m_tFrame({})
	, m_iBottom(-15)
	, m_iColor(2)
{
}


CComPlayer::~CComPlayer()
{
}



void CComPlayer::MoveFrame()
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
			m_tFrame.iFrameStart = m_tFrame.iFrameEnd - 1;
		}
		else
		{
			m_tFrame.iFrameStart = 0;
		}
	}

}

void CComPlayer::Change_State()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case CComPlayer::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameState = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();

			m_tFrame.iFrameX = 64;
			m_tFrame.iFrameY = 76;

			m_iBottom = -15;
			break;
		case CComPlayer::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameState = 0;
			m_tFrame.dwFrameSpeed = 200 / m_fSpeed;
			m_tFrame.dwFrameTime = GetTickCount();

			m_tFrame.iFrameX = 64;
			m_tFrame.iFrameY = 76;

			m_iBottom = -15;
			break;
		case CComPlayer::TRAP:

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
		case CComPlayer::DIE:
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

		case CComPlayer::READY:
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
		case CComPlayer::WIN:
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

		case CComPlayer::LIVE:
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

int CComPlayer::Ready_GameObject()
{
	m_eObjectType = OBJECT::ID::COMPLAYER;

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

	m_eCurAction = ACTION::WAIT;
	m_eNextAction = ACTION::MOVE;
	m_targetPos = { 0,0 };
	m_dwActionTimer = GetTickCount();

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

	return READY_OK;
}

int CComPlayer::Update_GameObject()
{
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


	if (m_eCurState == IDLE || m_eCurState == WALK)
	{




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

		if (m_dwActionTimer < GetTickCount())
		{
			m_eNextAction = ACTION::MOVE;
		}
		if (m_dwActionTimer + 3000 < GetTickCount())
		{
			m_eNextAction = ACTION::WAIT;
		}
		if (m_dwActionTimer + 5000 < GetTickCount())
		{
			m_dwActionTimer = GetTickCount();
		}
		
		if (m_eNextAction == ACTION::MOVE)
		{
			m_eCurAction = ACTION::MOVE;
			m_eNextAction = ACTION::WAIT;
			m_targetFinalPos = {TILESTARTX+TILECX*7 , TILESTARTY + TILECY * 2 }; //기본 추척 경로

			auto pPlayerList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER);
			
			for (auto pPlayer : *pPlayerList)
			{
				if (static_cast<CPlayer*>(pPlayer)->Get_IsShow() == true)
				{
					m_targetPos = AStarAlgorithm(Get_Pos(), pPlayer->Get_Pos(), 1); //플레이어 추적
				}
			}

			if (m_targetPos.x == 0 && m_targetPos.y == 0) //경로못찾음
			{
				m_targetPos = AStarAlgorithm(Get_Pos(), m_targetFinalPos);
			}

			if (m_tInfo.fX == m_targetFinalPos.x && m_tInfo.fY == m_targetFinalPos.y)
			{
				m_eCurAction = ACTION::WAIT;
			}
			if (m_targetPos.x == 0 && m_targetPos.y == 0) //경로못찾음
			{
				m_eCurAction = ACTION::WAIT;
			}
		}
		if (m_eCurAction == ACTION::MOVE)
		{
			if (m_tInfo.fY > m_targetPos.y)
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
				if (m_tInfo.fY <= m_targetPos.y)
				{
					m_tInfo.fY = m_targetPos.y;
				}
			}
			else if (m_tInfo.fY < m_targetPos.y)
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
				if (m_tInfo.fY >= m_targetPos.y)
				{
					m_tInfo.fY = m_targetPos.y;
				}
			}
			else if (m_tInfo.fX > m_targetPos.x)
			{
				if (m_eCurState == WALK || m_eCurState == IDLE)
				{
					m_tInfo.fX -= m_fSpeed;
					m_pFrameKey = (TCHAR*)L"bazzi_left";
					m_eNextState = WALK;
					m_tFrame.iFrameEnd = 5;

					Block_Collision(DIRECTION::LEFT, m_fSpeed);
				}
				else if (m_eCurState == TRAP)
				{
					m_tInfo.fX -= m_fSpeed / 10.f;
					Block_Collision(DIRECTION::LEFT, m_fSpeed / 10.f);
				}				
				if (m_tInfo.fX <= m_targetPos.x)
				{
					m_tInfo.fX = m_targetPos.x;
				}
			}
			else if (m_tInfo.fX < m_targetPos.x)
			{
				if (m_eCurState == WALK || m_eCurState == IDLE)
				{
					m_tInfo.fX += m_fSpeed;
					m_pFrameKey = (TCHAR*)L"bazzi_right";
					m_eNextState = WALK;
					m_tFrame.iFrameEnd = 5;

					Block_Collision(DIRECTION::RIGHT, m_fSpeed);
				}
				else if (m_eCurState == TRAP)
				{
					m_tInfo.fX += m_fSpeed / 10.f;
					Block_Collision(DIRECTION::RIGHT, m_fSpeed / 10.f);
				}
				if (m_tInfo.fX >= m_targetPos.x)
				{
					m_tInfo.fX = m_targetPos.x;
				}
			}

			if (m_targetPos.x == m_tInfo.fX && m_targetPos.y == m_tInfo.fY)
			{
				m_eNextAction = ACTION::MOVE;
			}
		}
		else if (m_eCurAction == ACTION::WAIT)
		{
			m_eNextState = IDLE;
		}

	}
	m_updateTimer = GetTickCount();

	if (m_eCurState == TRAP)
	{
		if (m_iNeedleNum > 0)
		{
			m_eNextState = STATE::LIVE;
			m_iNeedleNum--;
		}
	}

	return OBJ_NOEVENT;
}

void CComPlayer::Late_Update_GameObject()
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

void CComPlayer::Render_GameObject(HDC hDC)
{
	CGameObject::Update_Rect_GameObject();

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

}

void CComPlayer::Release_GameObject()
{

}

void CComPlayer::Eat_Item(ITEM_TYPE::TYPE eItemType)
{
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



void CComPlayer::Block_Collision(DIRECTION::DIREC eDir, float fSpeed)
{
	CTile* pTile[9] = {};
	for (int i = 0; i < 9; i++)
	{
		int iMoveX = -40 + (i % 3) * 40;
		int iMoveY = -40 + (i / 3) * 40;
		pTile[i] = static_cast<CTile*>(CTile_Manager::Get_Instance()->Pick_Tile(m_tInfo.fX + iMoveX, m_tInfo.fY + iMoveY));
	}
	for (int i = 0; i < 9; i++)
	{
		if (pTile[i])
		{
			auto listOnTile = pTile[i]->Get_ListObjectOnTile();

			for (CGameObject* pObject : *listOnTile)
			{
				if ((pObject->Get_ObjectType() == OBJECT::BLOCK && static_cast<CBlock*>(pObject)->Get_BlockType() != BLOCK_TYPE::BUSH) || (i != 4 && pObject->Get_ObjectType() == OBJECT::BUBBLE))
				{
					CGameObject::Update_Rect_GameObject();
					RECT rect{};
					RECT RectTile = pTile[i]->Get_Rect();
					if (IntersectRect(&rect, &m_tRect, &RectTile))
					{
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
										bPass = false;
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

CGameObject* CComPlayer::Create()
{
	CGameObject* pInstance = new CComPlayer;
	if (0 > pInstance->Ready_GameObject())
	{
		Safe_Delete(pInstance);
		return nullptr;
	}
	return pInstance;
}
