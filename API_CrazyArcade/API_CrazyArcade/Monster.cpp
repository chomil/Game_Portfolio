#include "pch.h"
#include "Monster.h"
#include "Player.h"
#include "GameObject_Manager.h"
#include "Block.h"
#include "Bubble.h"
#include "BubblePop.h"

CMonster::CMonster()
{
}

CMonster::~CMonster()
{
}

void CMonster::MoveFrame()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();
	}
	if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
	{
		m_tFrame.iFrameStart = 0;
		if (m_eCurState == STATE::TRAP || m_eCurState == STATE::DIE)
		{
			m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
		}
	}
}

void CMonster::Change_State()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case CMonster::IDLE:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameState = m_eMoveDir;
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		}
		case CMonster::WALK:
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameState = m_eMoveDir;
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			if (m_iMonsterType == 2)
			{
				CSound_Manager::Get_Instance()->PlaySound(L"SFX_Boss_Move.mp3", CSound_Manager::CHANNELID::BOSS);
			}
			break;
		}
		case CMonster::HIT:
		{
			if (m_iMonsterType == 2)
			{
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 3;
				m_tFrame.iFrameState = 4;
				m_tFrame.dwFrameSpeed = 100;
				m_tFrame.dwFrameTime = GetTickCount();
			}
			break;
		}
		case CMonster::ATTACK:
		{			
			if (m_iMonsterType == 2)
			{
				CSound_Manager::Get_Instance()->PlaySound(L"SFX_Boss_Angry.mp3", CSound_Manager::CHANNELID::BOSS);
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 1;
				m_tFrame.iFrameState = 4;
				m_tFrame.dwFrameSpeed = 700;
				m_tFrame.dwFrameTime = GetTickCount();
			}
			break;
		}
		case CMonster::TRAP:
		{
			if (m_iMonsterType == 2)
			{
				CSound_Manager::Get_Instance()->PlaySound(L"SFX_Boss_Trap.mp3", CSound_Manager::CHANNELID::BOSS);
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 5;
				m_tFrame.iFrameState = 5;
				m_tFrame.dwFrameSpeed = 700;
				m_tFrame.dwFrameTime = GetTickCount();
			}
			break;
		}
		case CMonster::DIE:
		{
			if (m_iMonsterType == 0|| m_iMonsterType == 1)
			{
				CSound_Manager::Get_Instance()->PlaySound(L"SFX_Monster_Die.mp3", CSound_Manager::CHANNELID::MONSTER);
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 3;
				m_tFrame.iFrameState = 4;
				m_tFrame.dwFrameSpeed = 100;
				m_tFrame.dwFrameTime = GetTickCount();
			}
			else
			{
				CSound_Manager::Get_Instance()->PlaySound(L"SFX_Boss_Die.mp3", CSound_Manager::CHANNELID::BOSS);
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 3;
				m_tFrame.iFrameState = 6;
				m_tFrame.dwFrameSpeed = 100;
				m_tFrame.dwFrameTime = GetTickCount();
			}
			break;
		}
		default:
			break;
		}
		m_eCurState = m_eNextState;
	}
}

int CMonster::Ready_GameObject()
{
	m_eObjectType = OBJECT::MONSTER;

	m_eCurState = STATE::IDLE;
	m_eNextState = STATE::WALK;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameState = m_eMoveDir;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();

	m_dwFollowTimer = GetTickCount();
	m_bFollow = false;

	m_dwNoDamageTimer = GetTickCount();
	m_bNoDamage = false;

	m_dwPatternTimer = GetTickCount();
	m_iPattern = 0;


	if (m_iMonsterType==0)
	{
		m_pFrameKey = (TCHAR*)L"Monster01";

		m_tFrame.iFrameX = 50;
		m_tFrame.iFrameY = 50;
		m_tInfo.iCX = 40;
		m_tInfo.iCY = 40;

		m_iHP = 1;

		m_fSpeed = 1.0f;


		m_iBottom = -8;
	}
	else if (m_iMonsterType == 1)
	{
		m_pFrameKey = (TCHAR*)L"Monster02";

		m_tFrame.iFrameX = 50;
		m_tFrame.iFrameY = 50;
		m_tInfo.iCX = 40;
		m_tInfo.iCY = 40;

		m_iHP = 1;

		m_fSpeed = 1.5f;


		m_iBottom = -8;
	}
	else
	{

		m_pFrameKey = (TCHAR*)L"Boss01";

		m_tFrame.iFrameX = 200;
		m_tFrame.iFrameY = 240;
		m_tInfo.iCX = 120;
		m_tInfo.iCY = 120;


		m_iHP = 20;
		m_iMaxHP = 20;

		m_fSpeed = 1.5f;


		m_iBottom = -30;
	}


	return 0;
}

int CMonster::Update_GameObject()
{

	if (m_bNoDamage == true)
	{
		if (m_dwNoDamageTimer + 500 < GetTickCount())
		{
			m_bNoDamage = false;
		}
	}


	if (m_iHP <= 0)
	{
		if (m_iMonsterType < 2)
		{
			if (m_eCurState != STATE::DIE)
			{
				m_eNextState = STATE::DIE;
			}
			else if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				return OBJ_DEAD;
			}
		}
		else
		{
			if (m_eCurState != STATE::TRAP && m_eCurState != STATE::DIE)
			{
				m_eNextState = STATE::TRAP;
			}
		}
	}
	if (m_iMonsterType==2 && m_iHP <= m_iMaxHP/4)
	{
		m_fSpeed = 2.5f;
	}

	if (m_eCurState == STATE::TRAP)
	{
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_eNextState = STATE::DIE;
		}
		return OBJ_NOEVENT;
	}
	if (m_eCurState == STATE::DIE)
	{
		return OBJ_NOEVENT;
	}
	else
	{
		Update_Rect_GameObject(); //플레이어 충돌
		auto pPlayerList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER);
		for (CGameObject* pPlayer : *pPlayerList)
		{
			if (PtInRect(&m_tRect, pPlayer->Get_Pos()))
			{
				static_cast<CPlayer*>(pPlayer)->Receive_Critical();
			}
		}

		if (m_iMonsterType == 2) //물풍선 충돌
		{
			auto pBubbleList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::BUBBLE);
			for (CGameObject* pBubble : *pBubbleList)
			{
				if (PtInRect(&m_tRect, pBubble->Get_Pos()))
				{
					static_cast<CBubble*>(pBubble)->Set_Pop();
				}
			}
		}

		if (m_iMonsterType < 2)
		{
			if (m_eCurState == STATE::WALK)
			{
				m_tFrame.iFrameState = m_eMoveDir;

				switch (m_eMoveDir)
				{
				case DIRECTION::UP:
					m_tInfo.fY -= m_fSpeed;
					break;
				case DIRECTION::DOWN:
					m_tInfo.fY += m_fSpeed;
					break;
				case DIRECTION::LEFT:
					m_tInfo.fX -= m_fSpeed;
					break;
				case DIRECTION::RIGHT:
					m_tInfo.fX += m_fSpeed;
					break;
				case DIRECTION::NO_DIREC:
					break;
				default:
					break;
				}

				auto pPlayerList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER);
				for (auto pPlayer : *pPlayerList)
				{
					if (static_cast<CPlayer*>(pPlayer)->Get_IsShow() == false)
					{
						continue;
					}
					if (abs(m_tInfo.fX - pPlayer->Get_Pos().x) < 15)
					{
						if (m_tInfo.fY < pPlayer->Get_Pos().y && m_tInfo.fY + 80 > pPlayer->Get_Pos().y)
						{
							if (m_bFollow == false)
							{
								m_dwFollowTimer = GetTickCount();
								m_bFollow = true;
								m_eMoveDir = DIRECTION::DOWN;
							}
						}
						else if (m_tInfo.fY > pPlayer->Get_Pos().y && m_tInfo.fY - 80 < pPlayer->Get_Pos().y)
						{
							if (m_bFollow == false)
							{
								m_dwFollowTimer = GetTickCount();
								m_eMoveDir = DIRECTION::UP;
								m_bFollow = true;
							}
						}
					}

					if (abs(m_tInfo.fY - pPlayer->Get_Pos().y) < 15)
					{
						if (m_tInfo.fX < pPlayer->Get_Pos().x && m_tInfo.fX + 80 > pPlayer->Get_Pos().x)
						{
							if (m_bFollow == false)
							{
								m_dwFollowTimer = GetTickCount();
								m_eMoveDir = DIRECTION::RIGHT;
								m_bFollow = true;
							}
						}
						else if (m_tInfo.fX > pPlayer->Get_Pos().x && m_tInfo.fX - 80 < pPlayer->Get_Pos().x)
						{
							if (m_bFollow == false)
							{
								m_dwFollowTimer = GetTickCount();
								m_eMoveDir = DIRECTION::LEFT;
								m_bFollow = true;
							}
						}
					}
				}

				if (m_bFollow == true)
				{
					if (m_dwFollowTimer + 1000 < GetTickCount())
					{
						m_bFollow = false;
					}
				}

				Block_Collision(m_eMoveDir, m_fSpeed);
			}
		}
		else //보스
		{
			if (m_eCurState == STATE::HIT)
			{
				if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
				{
					m_eNextState = STATE::IDLE;
					m_dwPatternTimer = GetTickCount();
					m_iPattern = 0;
				}
			}
			else if (m_eCurState == STATE::IDLE)
			{
				m_eNextState = STATE::WALK;

				if (m_iHP <= m_iMaxHP / 4)
				{
					int iRand = rand() % 2;
					if (iRand == 0)
					{
						m_eNextState = STATE::WALK;
					}
					else
					{
						m_eNextState = STATE::ATTACK;
					}
				}
			}
			else if (m_eCurState == STATE::WALK)
			{
				if (m_iPattern == 0)
				{
					m_iPattern = 1;
					auto pPlayerList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER);
					int iDistX = 0;
					int iDistY = 0;
					for (auto pPlayer : *pPlayerList)
					{
						if (static_cast<CPlayer*>(pPlayer)->Get_IsShow() == true)
						{
							iDistX += (pPlayer->Get_Pos().x - m_tInfo.fX);
							iDistY += (pPlayer->Get_Pos().y - m_tInfo.fY);
						}
					}
					if (abs(iDistY) > abs(iDistX))
					{
						if (iDistY < 0)
						{
							m_eMoveDir = DIRECTION::UP;
						}
						else
						{
							m_eMoveDir = DIRECTION::DOWN;
						}
					}
					else
					{
						if (iDistX < 0)
						{
							m_eMoveDir = DIRECTION::LEFT;
						}
						else
						{
							m_eMoveDir = DIRECTION::RIGHT;
						}
					}
				}

				switch (m_eMoveDir)
				{
				case DIRECTION::UP:
					m_tFrame.iFrameState = 0;
					m_tInfo.fY -= m_fSpeed;
					break;
				case DIRECTION::DOWN:
					m_tFrame.iFrameState = 1;
					m_tInfo.fY += m_fSpeed;
					break;
				case DIRECTION::LEFT:
					m_tFrame.iFrameState = 2;
					m_tInfo.fX -= m_fSpeed;
					break;
				case DIRECTION::RIGHT:
					m_tFrame.iFrameState = 3;
					m_tInfo.fX += m_fSpeed;
					break;
				default:
					break;
				}

				Block_Collision(m_eMoveDir, m_fSpeed);

				if (m_dwPatternTimer + 2000 < GetTickCount())
				{
					m_eNextState = STATE::ATTACK;
					m_dwPatternTimer = GetTickCount();
					m_iPattern = 0;
				}
			}
			else if (m_eCurState == STATE::ATTACK)
			{
				if (m_dwPatternTimer + 1500 < GetTickCount() && m_iPattern == 0)
				{
					m_iPattern = 1;

					int iRand = rand() % 3;

					if (iRand == 0)
					{
						CSound_Manager::Get_Instance()->PlaySound(L"SFX_Bubble_pop.mp3", CSound_Manager::CHANNELID::BUBBLE_POP);
						int iX = CTile_Manager::Get_Instance()->Pick_Tile(m_tInfo.fX, m_tInfo.fY)->Get_Pos().x;
						int iY = CTile_Manager::Get_Instance()->Pick_Tile(m_tInfo.fX, m_tInfo.fY)->Get_Pos().y;
						for (int i = iX - 120; i <= iX + 120; i += 40)
						{
							for (int j = iY - 120; j <= iY + 120; j += 40)
							{
								if (i < TILESTARTX || i > TILESTARTX + 14 * TILECX || j<TILESTARTY || j>TILESTARTY + 12 * TILECY)
								{
									continue;
								}
								if (i == iX - 120 || i == iX + 120 || j == iY - 120 || j == iY + 120)
								{
									CGameObject* pBubblePop = CBubblePop::Create(i, j, DIRECTION::NO_DIREC);
									if (nullptr != pBubblePop)
									{
										CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::EFFECT, pBubblePop);
									}
								}

							}
						}
					}
					else if (iRand == 1)
					{
						CGameObject* pTile = CTile_Manager::Get_Instance()->Pick_Tile(m_tInfo.fX, m_tInfo.fY - 80);
						if (pTile)
						{
							CGameObject* pBubble = CBubble::Create(pTile->Get_Pos().x, pTile->Get_Pos().y, 3, this);
							if (nullptr != pBubble)
							{
								CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUBBLE, pBubble);
							}
							static_cast<CBubble*>(pBubble)->Moving_Bubble(DIRECTION::UP);
						}

						pTile = CTile_Manager::Get_Instance()->Pick_Tile(m_tInfo.fX, m_tInfo.fY + 80);
						if (pTile)
						{
							CGameObject* pBubble = CBubble::Create(pTile->Get_Pos().x, pTile->Get_Pos().y, 3, this);
							if (nullptr != pBubble)
							{
								CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUBBLE, pBubble);
							}
							static_cast<CBubble*>(pBubble)->Moving_Bubble(DIRECTION::DOWN);
						}

						pTile = CTile_Manager::Get_Instance()->Pick_Tile(m_tInfo.fX - 80, m_tInfo.fY);
						if (pTile)
						{
							CGameObject* pBubble = CBubble::Create(pTile->Get_Pos().x, pTile->Get_Pos().y, 3, this);
							if (nullptr != pBubble)
							{
								CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUBBLE, pBubble);
							}
							static_cast<CBubble*>(pBubble)->Moving_Bubble(DIRECTION::LEFT);
						}

						pTile = CTile_Manager::Get_Instance()->Pick_Tile(m_tInfo.fX + 80, m_tInfo.fY);
						if (pTile)
						{
							CGameObject* pBubble = CBubble::Create(pTile->Get_Pos().x, pTile->Get_Pos().y, 3, this);
							if (nullptr != pBubble)
							{
								CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BUBBLE, pBubble);
							}
							static_cast<CBubble*>(pBubble)->Moving_Bubble(DIRECTION::RIGHT);
						}
					}
					else if (iRand == 2)
					{
						CSound_Manager::Get_Instance()->PlaySound(L"SFX_Bubble_pop.mp3", CSound_Manager::CHANNELID::BUBBLE_POP);
						m_iPattern = 2;
						auto pPlayerList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER);
						for (CGameObject* pPlayer : *pPlayerList)
						{
							if (static_cast<CPlayer*>(pPlayer)->Get_IsShow() == true)
							{
								int iX = CTile_Manager::Get_Instance()->Pick_Tile(pPlayer->Get_Pos().x, pPlayer->Get_Pos().y)->Get_Pos().x;
								int iY = CTile_Manager::Get_Instance()->Pick_Tile(pPlayer->Get_Pos().x, pPlayer->Get_Pos().y)->Get_Pos().y;
								for (int i = iX - 80; i <= iX + 80; i += 40)
								{
									for (int j = iY - 80; j <= iY + 80; j += 40)
									{
										if (i < TILESTARTX || i > TILESTARTX + 14 * TILECX || j<TILESTARTY || j>TILESTARTY + 12 * TILECY)
										{
											continue;
										}
										if (i == iX - 80 || i == iX + 80 || j == iY - 80 || j == iY + 80)
										{
											CGameObject* pBubblePop = CBubblePop::Create(i, j, DIRECTION::NO_DIREC);
											if (nullptr != pBubblePop)
											{
												CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::EFFECT, pBubblePop);
											}
										}

									}
								}
							}
						}
						
					}
				}
				if (m_dwPatternTimer + 1800 < GetTickCount() && m_iPattern == 2)
				{
					CSound_Manager::Get_Instance()->PlaySound(L"SFX_Bubble_pop.mp3", CSound_Manager::CHANNELID::BUBBLE_POP);
					m_iPattern = 3;
					auto pPlayerList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER);
					for (CGameObject* pPlayer : *pPlayerList)
					{
						if (static_cast<CPlayer*>(pPlayer)->Get_IsShow() == true)
						{
							int iX = CTile_Manager::Get_Instance()->Pick_Tile(pPlayer->Get_Pos().x, pPlayer->Get_Pos().y)->Get_Pos().x;
							int iY = CTile_Manager::Get_Instance()->Pick_Tile(pPlayer->Get_Pos().x, pPlayer->Get_Pos().y)->Get_Pos().y;
							for (int i = iX - 40; i <= iX + 40; i += 40)
							{
								for (int j = iY - 40; j <= iY + 40; j += 40)
								{
									if (i < TILESTARTX || i > TILESTARTX + 14 * TILECX || j<TILESTARTY || j>TILESTARTY + 12 * TILECY)
									{
										continue;
									}
									if (i == iX - 40 || i == iX + 40 || j == iY - 40 || j == iY + 40)
									{
										CGameObject* pBubblePop = CBubblePop::Create(i, j, DIRECTION::NO_DIREC);
										if (nullptr != pBubblePop)
										{
											CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::EFFECT, pBubblePop);
										}
									}

								}
							}
						}
					}
				}
				if (m_dwPatternTimer + 2000 < GetTickCount())
				{
					m_eNextState = STATE::IDLE;
					m_dwPatternTimer = GetTickCount();
					m_iPattern = 0;
				}
			}


			if (m_iHP == m_iMaxHP)
			{
				m_eNextState = STATE::IDLE;
			}
		}
	}
	
	return 0;
}

void CMonster::Late_Update_GameObject()
{

	MoveFrame();
	Change_State();
}

void CMonster::Render_GameObject(HDC hDC)
{
	HDC hMemDC = CBitmap_Manager::Get_Instance()->FindImage(m_pFrameKey);
	if (nullptr == hMemDC)
		return;

	if (m_iMonsterType < 2)
	{
		GdiTransparentBlt(hDC,
			m_tInfo.fX - m_tFrame.iFrameX * 0.5f,
			m_tInfo.fY - m_tFrame.iFrameY * 0.5f + m_iBottom,
			m_tFrame.iFrameX, m_tFrame.iFrameY,
			hMemDC,
			m_tFrame.iFrameStart * m_tFrame.iFrameX, m_tFrame.iFrameState * m_tFrame.iFrameY,
			m_tFrame.iFrameX, m_tFrame.iFrameY,
			RGB(255, 0, 255));
	}
	else
	{
		GdiTransparentBlt(hDC,
			m_tInfo.fX - m_tFrame.iFrameX * 0.5f * 0.9f,
			m_tInfo.fY - m_tFrame.iFrameY * 0.5f * 0.9f + m_iBottom,
			m_tFrame.iFrameX * 0.9f, m_tFrame.iFrameY * 0.9f,
			hMemDC,
			m_tFrame.iFrameStart * m_tFrame.iFrameX, m_tFrame.iFrameState * m_tFrame.iFrameY,
			m_tFrame.iFrameX, m_tFrame.iFrameY,
			RGB(255, 0, 255));

		if (m_iHP > 0)
		{

			hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"Boss_hp");
			if (nullptr == hMemDC)
				return;

			GdiTransparentBlt(hDC,
				m_tInfo.fX - 86 / 2,
				m_tInfo.fY - 125,
				86, 11,
				hMemDC,
				0, 0,
				86, 11,
				RGB(255, 0, 255));
			if (m_iHP <= m_iMaxHP / 4)
			{
				GdiTransparentBlt(hDC,
					m_tInfo.fX - 86 / 2 + 2,
					m_tInfo.fY - 125,
					82 * m_iHP / m_iMaxHP, 11,
					hMemDC,
					0, 11,
					86, 11,
					RGB(255, 0, 255));
			}
			else
			{
				GdiTransparentBlt(hDC,
					m_tInfo.fX - 86 / 2 + 2,
					m_tInfo.fY - 125,
					82 * m_iHP / m_iMaxHP, 11,
					hMemDC,
					0, 22,
					86, 11,
					RGB(255, 0, 255));
			}
		}
	}
	
}

void CMonster::Release_GameObject()
{
}

void CMonster::Receive_Damage()
{
	if (m_bNoDamage == false && m_eCurState != STATE::TRAP && m_eCurState != STATE::DIE)
	{
		--m_iHP;
		m_bNoDamage = true;
		m_dwNoDamageTimer = GetTickCount();
		if (m_iMonsterType == 2)
		{
			CSound_Manager::Get_Instance()->PlaySound(L"SFX_Boss_Hit.mp3", CSound_Manager::CHANNELID::BOSS);
			if (m_iHP == m_iMaxHP / 4)
			{
				CSound_Manager::Get_Instance()->StopSound(CSound_Manager::CHANNELID::BGM);
				CSound_Manager::Get_Instance()->PlaySound(L"BGM_Boss_lower.mp3", CSound_Manager::CHANNELID::BOSS);
				CSound_Manager::Get_Instance()->PlayBGM(L"BGM_Boss_angry.mp3");
			}
			if (m_eCurState != STATE::ATTACK)
			{
				m_eNextState = STATE::HIT;
				Change_State();
			}
		}
	}
}

void CMonster::Block_Collision(DIRECTION::DIREC eDir, float fSpeed)
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
				if ((pObject->Get_ObjectType() == OBJECT::BLOCK && static_cast<CBlock*>(pObject)->Get_BlockType() != BLOCK_TYPE::BUSH) || (m_iMonsterType < 2 && i != 4 && pObject->Get_ObjectType() == OBJECT::BUBBLE))
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
								m_tInfo.fY = (int)m_tInfo.fY + (rect.bottom - rect.top);
							}

							if (pTile[1])
							{
								bool bPass = true;
								for (CGameObject* pObject : *(pTile[1]->Get_ListObjectOnTile()))
								{
									if (m_iMonsterType < 2 && pObject->Get_ObjectType() == OBJECT::BUBBLE ||
										(pObject->Get_ObjectType() == OBJECT::BLOCK && static_cast<CBlock*>(pObject)->Get_BlockType() != BLOCK_TYPE::BUSH))
									{
										bPass = false;
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
								else
								{
									int iRand = rand() % 2;
									if (pTile[3] && pTile[3]->Is_Pass() && pTile[5] && pTile[5]->Is_Pass())
									{
										if (iRand == 0)
										{
											m_eMoveDir = DIRECTION::LEFT;
										}
										else
										{
											m_eMoveDir = DIRECTION::RIGHT;
										}
									}
									else if (pTile[3] && pTile[3]->Is_Pass())
									{
										m_eMoveDir = DIRECTION::LEFT;
									}
									else if (pTile[5] && pTile[5]->Is_Pass())
									{
										m_eMoveDir = DIRECTION::RIGHT;
									}
									else
									{
										m_eMoveDir = DIRECTION::DOWN;
									}
								}
							}
							break;
						case DIRECTION::DOWN:
							if (i == 6 || i == 7 || i == 8)
							{
								m_tInfo.fY = (int)m_tInfo.fY - (rect.bottom - rect.top);
							}

							if (pTile[7])
							{
								bool bPass = true;
								for (CGameObject* pObject : *(pTile[7]->Get_ListObjectOnTile()))
								{
									if (m_iMonsterType < 2 && pObject->Get_ObjectType() == OBJECT::BUBBLE ||
										(pObject->Get_ObjectType() == OBJECT::BLOCK && static_cast<CBlock*>(pObject)->Get_BlockType() != BLOCK_TYPE::BUSH))
									{
										bPass = false;
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
								else
								{
									int iRand = rand() % 2;
									if (pTile[3] && pTile[3]->Is_Pass() && pTile[5] && pTile[5]->Is_Pass())
									{
										if (iRand == 0)
										{
											m_eMoveDir = DIRECTION::LEFT;
										}
										else
										{
											m_eMoveDir = DIRECTION::RIGHT;
										}
									}
									else if (pTile[3] && pTile[3]->Is_Pass())
									{
										m_eMoveDir = DIRECTION::LEFT;
									}
									else if (pTile[5] && pTile[5]->Is_Pass())
									{
										m_eMoveDir = DIRECTION::RIGHT;
									}
									else
									{
										m_eMoveDir = DIRECTION::UP;
									}
								}
							}
							break;
						case DIRECTION::LEFT:
							if (i == 0 || i == 3 || i == 6)
							{
								m_tInfo.fX = (int)m_tInfo.fX + (rect.right - rect.left);
							}
							if (pTile[3])
							{
								bool bPass = true;
								for (CGameObject* pObject : *(pTile[3]->Get_ListObjectOnTile()))
								{
									if (m_iMonsterType < 2 && pObject->Get_ObjectType() == OBJECT::BUBBLE ||
										(pObject->Get_ObjectType() == OBJECT::BLOCK && static_cast<CBlock*>(pObject)->Get_BlockType() != BLOCK_TYPE::BUSH))
									{
										bPass = false;
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
								else
								{
									int iRand = rand() % 2;
									if (pTile[1] && pTile[1]->Is_Pass() && pTile[7] && pTile[7]->Is_Pass())
									{
										if (iRand == 0)
										{
											m_eMoveDir = DIRECTION::UP;
										}
										else
										{
											m_eMoveDir = DIRECTION::DOWN;
										}
									}
									else if (pTile[1] && pTile[1]->Is_Pass())
									{
										m_eMoveDir = DIRECTION::UP;
									}
									else if (pTile[7] && pTile[7]->Is_Pass())
									{
										m_eMoveDir = DIRECTION::DOWN;
									}
									else
									{
										m_eMoveDir = DIRECTION::RIGHT;
									}
								}
							}

							break;
						case DIRECTION::RIGHT:
							if (i == 2 || i == 5 || i == 8)
							{
								m_tInfo.fX = (int)m_tInfo.fX - (rect.right - rect.left);
							}
							if (pTile[5])
							{
								bool bPass = true;
								for (CGameObject* pObject : *(pTile[5]->Get_ListObjectOnTile()))
								{
									if (m_iMonsterType < 2 && pObject->Get_ObjectType() == OBJECT::BUBBLE ||
										(pObject->Get_ObjectType() == OBJECT::BLOCK && static_cast<CBlock*>(pObject)->Get_BlockType() != BLOCK_TYPE::BUSH))
									{
										bPass = false;
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
								else
								{
									int iRand = rand()% 2;
									if (pTile[1] && pTile[1]->Is_Pass() && pTile[7] && pTile[7]->Is_Pass())
									{
										if (iRand == 0)
										{
											m_eMoveDir = DIRECTION::UP;
										}
										else
										{
											m_eMoveDir = DIRECTION::DOWN;
										}
									}
									else if (pTile[1] && pTile[1]->Is_Pass())
									{
										m_eMoveDir = DIRECTION::UP;
									}
									else if (pTile[7] && pTile[7]->Is_Pass())
									{
										m_eMoveDir = DIRECTION::DOWN;
									}
									else
									{
										m_eMoveDir = DIRECTION::LEFT;
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

	if (m_iMonsterType < 2)
	{
		if (m_tInfo.fY < TILESTARTY)
		{
			m_tInfo.fY = TILESTARTY;
			if (pTile[3] && pTile[3]->Is_Pass() && pTile[5] && pTile[5]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::DOWN;
			}
			else if (pTile[3] && pTile[3]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::LEFT;
			}
			else if (pTile[5] && pTile[5]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::RIGHT;
			}
			else
			{
				m_eMoveDir = DIRECTION::DOWN;
			}
		}
		if (m_tInfo.fY > TILESTARTY + TILECY * (TILEY - 1))
		{
			m_tInfo.fY = TILESTARTY + TILECY * (TILEY - 1);
			if (pTile[3] && pTile[3]->Is_Pass() && pTile[5] && pTile[5]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::UP;
			}
			else if (pTile[3] && pTile[3]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::LEFT;
			}
			else if (pTile[5] && pTile[5]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::RIGHT;
			}
			else
			{
				m_eMoveDir = DIRECTION::UP;
			}
		}
		if (m_tInfo.fX < TILESTARTX)
		{
			m_tInfo.fX = TILESTARTX;
			if (pTile[1] && pTile[1]->Is_Pass() && pTile[7] && pTile[7]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::RIGHT;
			}
			else if (pTile[1] && pTile[1]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::UP;
			}
			else if (pTile[7] && pTile[7]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::DOWN;
			}
			else
			{
				m_eMoveDir = DIRECTION::RIGHT;
			}
		}
		if (m_tInfo.fX > TILESTARTX + TILECX * (TILEX - 1))
		{
			m_tInfo.fX = TILESTARTX + TILECX * (TILEX - 1);
			if (pTile[1] && pTile[1]->Is_Pass() && pTile[7] && pTile[7]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::LEFT;
			}
			else if (pTile[1] && pTile[1]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::UP;
			}
			else if (pTile[7] && pTile[7]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::DOWN;
			}
			else
			{
				m_eMoveDir = DIRECTION::LEFT;
			}
		}
	}
	else //보스 화면 가장자리
	{
		if (m_tInfo.fY < TILESTARTY+40)
		{
			m_tInfo.fY = TILESTARTY+40;
			if (pTile[3] && pTile[3]->Is_Pass() && pTile[5] && pTile[5]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::DOWN;
			}
			else if (pTile[3] && pTile[3]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::LEFT;
			}
			else if (pTile[5] && pTile[5]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::RIGHT;
			}
			else
			{
				m_eMoveDir = DIRECTION::DOWN;
			}
		}
		if (m_tInfo.fY > TILESTARTY + TILECY * (TILEY - 2))
		{
			m_tInfo.fY = TILESTARTY + TILECY * (TILEY - 2);
			if (pTile[3] && pTile[3]->Is_Pass() && pTile[5] && pTile[5]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::UP;
			}
			else if (pTile[3] && pTile[3]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::LEFT;
			}
			else if (pTile[5] && pTile[5]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::RIGHT;
			}
			else
			{
				m_eMoveDir = DIRECTION::UP;
			}
		}
		if (m_tInfo.fX < TILESTARTX+40)
		{
			m_tInfo.fX = TILESTARTX+40;
			if (pTile[1] && pTile[1]->Is_Pass() && pTile[7] && pTile[7]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::RIGHT;
			}
			else if (pTile[1] && pTile[1]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::UP;
			}
			else if (pTile[7] && pTile[7]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::DOWN;
			}
			else
			{
				m_eMoveDir = DIRECTION::RIGHT;
			}
		}
		if (m_tInfo.fX > TILESTARTX + TILECX * (TILEX - 2))
		{
			m_tInfo.fX = TILESTARTX + TILECX * (TILEX - 2);
			if (pTile[1] && pTile[1]->Is_Pass() && pTile[7] && pTile[7]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::LEFT;
			}
			else if (pTile[1] && pTile[1]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::UP;
			}
			else if (pTile[7] && pTile[7]->Is_Pass())
			{
				m_eMoveDir = DIRECTION::DOWN;
			}
			else
			{
				m_eMoveDir = DIRECTION::LEFT;
			}
		}
	}


}

CGameObject* CMonster::Create(int iX, int iY, DIRECTION::DIREC eMoveDir, int iMonsterType)
{
	CGameObject* pInstance = new CMonster;
	static_cast<CMonster*>(pInstance)->Set_Direction(eMoveDir);
	static_cast<CMonster*>(pInstance)->Set_MonsterType(iMonsterType);
	if (0 > pInstance->Ready_GameObject())
	{
		Safe_Delete(pInstance);
		return nullptr;
	}
	pInstance->Set_Pos(iX, iY);
	return pInstance;
}
