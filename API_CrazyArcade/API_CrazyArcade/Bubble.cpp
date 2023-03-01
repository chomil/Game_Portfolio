#include "pch.h"
#include "Bubble.h"
#include "BubblePop.h"
#include "Tile.h"
#include "Block.h"

#include "GameObject_Manager.h"

CBubble::CBubble()
	: m_tFrame({})
	, m_iPushCount{}
{
}

CBubble::~CBubble()
{
}

void CBubble::MoveFrame()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();
	}
	if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		m_tFrame.iFrameStart = 0;

}

int CBubble::Ready_GameObject()
{
	m_eObjectType = OBJECT::ID::BUBBLE;

	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;
	m_bPop = false;
	m_timeStart = GetTickCount64();


	m_pFrameKey = (TCHAR*)L"bubble";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 2;
	m_tFrame.iFrameState = 0;
	m_tFrame.dwFrameSpeed = 300;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.iFrameX = 44;
	m_tFrame.iFrameY = 41;

	m_eMovingDirec = DIRECTION::NO_DIREC;
	return 0;
}

int CBubble::Update_GameObject()
{
	DIRECTION::DIREC prevDir = m_eMovingDirec;
	for (int i = DIRECTION::UP; i < DIRECTION::NO_DIREC; ++i)
	{
		if (m_iPushCount[i] > 5)
		{
			m_eMovingDirec = (DIRECTION::DIREC)i;
			m_iPushCount[DIRECTION::UP] = 0;
			m_iPushCount[DIRECTION::DOWN] = 0;
			m_iPushCount[DIRECTION::LEFT] = 0;
			m_iPushCount[DIRECTION::RIGHT] = 0;
		}
	}
	if (m_eMovingDirec != DIRECTION::NO_DIREC && m_bPop==false)
	{
		Moving_Bubble(m_eMovingDirec);
	}
	if (prevDir == DIRECTION::NO_DIREC && m_eMovingDirec != DIRECTION::NO_DIREC)
	{
		CSound_Manager::Get_Instance()->PlaySound(L"SFX_Bubble_move.mp3", CSound_Manager::CHANNELID::BUBBLE);
	}
	else if (m_timeStart + 3000 <= GetTickCount64() || m_bPop == true)
	{
		CSound_Manager::Get_Instance()->PlaySound(L"SFX_Bubble_pop.mp3", CSound_Manager::CHANNELID::BUBBLE_POP);
		CGameObject* pBubblePop = CBubblePop::Create(m_tInfo.fX, m_tInfo.fY, DIRECTION::NO_DIREC);
		if (nullptr != pBubblePop)
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::EFFECT, pBubblePop);
		}

		Hit_Object(m_tInfo.fX, m_tInfo.fY);

		for (int i = 1; i <= m_iPower; ++i)
		{
			//물풍선의 power만큼 퍼짐
			if (1 == Hit_Object(m_tInfo.fX, m_tInfo.fY - i * TILECY))
			{
				//물줄기가 블럭에 막히면 for문을 빠져나감 
				break;
			}
			//물줄기가 블럭에 막히지 않으면 그 타일 위치에 물줄기를 생성함
			CGameObject* pBubblePop = CBubblePop::Create(m_tInfo.fX, m_tInfo.fY - i * TILECY,  DIRECTION::UP);
			if (nullptr != pBubblePop)
			{
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::EFFECT, pBubblePop);
			}
		}

		for (int i = 1; i <= m_iPower; ++i)
		{
			if (1 == Hit_Object(m_tInfo.fX, m_tInfo.fY + i * TILECY))
			{
				break;
			}
			CGameObject* pBubblePop = CBubblePop::Create(m_tInfo.fX, m_tInfo.fY + i * TILECY,  DIRECTION::DOWN);
			if (nullptr != pBubblePop)
			{
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::EFFECT, pBubblePop);
			}

		}

		for (int i = 1; i <= m_iPower; ++i)
		{
			if (1 == Hit_Object(m_tInfo.fX - i * TILECX, m_tInfo.fY))
			{
				break;
			}
			CGameObject* pBubblePop = CBubblePop::Create(m_tInfo.fX - i * TILECX, m_tInfo.fY,  DIRECTION::LEFT);
			if (nullptr != pBubblePop)
			{
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::EFFECT, pBubblePop);
			}

		}

		for (int i = 1; i <= m_iPower; ++i)
		{
			if (1 == Hit_Object(m_tInfo.fX + i * TILECX, m_tInfo.fY))
			{
				break;
			}
			CGameObject* pBubblePop = CBubblePop::Create(m_tInfo.fX + i * TILECX, m_tInfo.fY,  DIRECTION::RIGHT);
			if (nullptr != pBubblePop)
			{
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::EFFECT, pBubblePop);
			}

		}

		return OBJ_DEAD;
	}

	return OBJ_NOEVENT;
}

void CBubble::Late_Update_GameObject()
{
	MoveFrame();
}

void CBubble::Render_GameObject(HDC hDC)
{
	CGameObject::Update_Rect_GameObject();

	CTile* pTile = static_cast<CTile*>(CTile_Manager::Get_Instance()->Pick_Tile(m_tInfo.fX, m_tInfo.fY));
	if (pTile)
	{
		for (CGameObject* pObject : *(pTile->Get_ListObjectOnTile()))
		{
			if (pObject->Get_ObjectType()==OBJECT::BLOCK && static_cast<CBlock*>(pObject)->Get_BlockType() == BLOCK_TYPE::BUSH)
			{
				return;
			}
		}
	}

	HDC hMemDC = CBitmap_Manager::Get_Instance()->FindImage(m_pFrameKey);
	if (nullptr == hMemDC)
		return;

	GdiTransparentBlt(hDC,
		m_tInfo.fX - m_tFrame.iFrameX  * 0.5f,
		m_tInfo.fY - m_tFrame.iFrameY  * 0.5f ,
		m_tFrame.iFrameX , m_tFrame.iFrameY ,
		hMemDC,
		m_tFrame.iFrameStart * m_tFrame.iFrameX, m_tFrame.iFrameState * m_tFrame.iFrameY,
		m_tFrame.iFrameX, m_tFrame.iFrameY,
		RGB(255, 0, 255));
}

void CBubble::Release_GameObject()
{
}

int CBubble::Hit_Object(int iX, int iY)
{
	int isHit = 0;
	auto pTile = CTile_Manager::Get_Instance()->Pick_Tile(iX, iY);
	if (pTile != nullptr)
	{
		auto pListOnTile = static_cast<CTile*>(pTile)->Get_ListObjectOnTile();
		for (auto& pObject : *pListOnTile)
		{
			if (pObject->Get_ObjectType() == OBJECT::ID::BUBBLE)
			{
				static_cast<CBubble*>(pObject)->Receive_Damage();
			}
			else if (pObject->Get_ObjectType() == OBJECT::ID::BLOCK)
			{
				if (static_cast<CBlock*>(pObject)->Get_BlockType() != BLOCK_TYPE::BUSH)
				{
					static_cast<CBlock*>(pObject)->Receive_Damage();
					isHit = 1; //블럭일 경우 물줄기 막힘
				}
			}
		}
	}
	else
	{
		isHit = 1;
	}
	return isHit;
}

void CBubble::Moving_Bubble(DIRECTION::DIREC eDir)
{
	int iMoveX = 0;
	int iMoveY = 0;
	switch (eDir)
	{
	case DIRECTION::UP:
		iMoveY = -40;
		break;
	case DIRECTION::DOWN:
		iMoveY = +40;
		break;
	case DIRECTION::LEFT:
		iMoveX = -40;
		break;
	case DIRECTION::RIGHT:
		iMoveX = +40;
		break;
	default:
		break;
	}

	CTile* pTile = static_cast<CTile*>(CTile_Manager::Get_Instance()->Pick_Tile(m_tInfo.fX + iMoveX, m_tInfo.fY + iMoveY));
	if (pTile)
	{
		bool isMove = true;
		for (CGameObject* pObject : *(pTile->Get_ListObjectOnTile()))
		{
			if ((pObject->Get_ObjectType() == OBJECT::BUBBLE) ||
				(pObject->Get_ObjectType() == OBJECT::BLOCK && static_cast<CBlock*>(pObject)->Get_BlockType() != BLOCK_TYPE::BUSH) ||
				(pObject->Get_ObjectType() == OBJECT::PLAYER) ||
				(pObject->Get_ObjectType() == OBJECT::COMPLAYER))
			{
				isMove = false;
				m_eMovingDirec = DIRECTION::NO_DIREC;

			}
		}
		if (isMove)
		{
			m_tInfo.fX += iMoveX;
			m_tInfo.fY += iMoveY;

			m_eMovingDirec = eDir;
		}
	}
	else
	{
		m_eMovingDirec = DIRECTION::NO_DIREC;
	}

}

CGameObject* CBubble::Create(int iX, int iY, int iPower, CGameObject* pOwner)
{
	CGameObject* pInstance = new CBubble;
	if (0 > pInstance->Ready_GameObject())
	{
		Safe_Delete(pInstance);
		return nullptr;
	}
	pInstance -> Set_Pos(iX, iY);
	static_cast<CBubble*>(pInstance)->Set_Power(iPower);
	static_cast<CBubble*>(pInstance)->Set_Owner(pOwner);
	return pInstance;
}
