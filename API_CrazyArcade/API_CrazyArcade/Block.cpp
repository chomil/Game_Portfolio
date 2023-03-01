#include "pch.h"
#include "Block.h"
#include "Item.h"
#include "Tile.h"
#include "GameObject_Manager.h"
#include "Player.h"
#include "ComPlayer.h"
#include "Scene_Room.h"

CBlock::CBlock()
	:m_pushTimer{}
{
}

CBlock::~CBlock()
{

}

int CBlock::Ready_GameObject()
{

	m_iStageNum = CScene_Room::Get_StageNum();
	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;

	if (m_iStageNum == 0 || m_iStageNum == 2)
	{
		m_pFrameKey = (TCHAR*)L"block_town";
	}
	else
	{
		m_pFrameKey = (TCHAR*)L"block_pirate";
	}
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameState = 0;
	m_tFrame.dwFrameSpeed = 0;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.iFrameX = 40;
	m_tFrame.iFrameY = 80;

	m_eItemType = ITEM_TYPE::ITEM_END;

	m_eMovingDirec = DIRECTION::DIREC::NO_DIREC;
	m_movingTimer = 0;


	m_iPrevInBush = 0;
	m_iCurInBush = 0;
	m_bBushShake = false;
	m_bushTimer = 0;
	return 0;
}

int CBlock::Update_GameObject()
{
	if (m_bIsDead == true)
	{
		return OBJ_DEAD;
	}
	if (m_eBlockType == BLOCK_TYPE::BOX)
	{
		CGameObject::Update_Rect_GameObject();
		for (auto& pPlayer : *(CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER)))
		{
			if (static_cast<CPlayer*>(pPlayer)->Get_State() != CPlayer::STATE::WALK)
			{
				continue;
			}
			const INFO* playerInfo = pPlayer->Get_Info();
			int iPlayerNum = static_cast<CPlayer*>(pPlayer)->Get_PlayerNum();
			if (abs(m_tInfo.fX - playerInfo->fX) <= TILECX / 2)
			{
				if (m_tInfo.fY == playerInfo->fY - TILECY)
				{
					if ((iPlayerNum == 0 && CKey_Manager::Get_Instance()->Key_Down(VK_UP)) ||
						(iPlayerNum == 1 && CKey_Manager::Get_Instance()->Key_Down('R')) || 
						(iPlayerNum == 2 && CKey_Manager::Get_Instance()->Key_Down(VK_UP)) )
					{
						++m_pushTimer[iPlayerNum][0];
					}
				}
				if (m_tInfo.fY == playerInfo->fY + TILECY)
				{
					if ((iPlayerNum == 0 && CKey_Manager::Get_Instance()->Key_Down(VK_DOWN)) ||
						(iPlayerNum == 1 && CKey_Manager::Get_Instance()->Key_Down('F')) ||
						(iPlayerNum == 2 && CKey_Manager::Get_Instance()->Key_Down(VK_DOWN)))
					{
						++m_pushTimer[iPlayerNum][1];
					}
				}
			}
			else if (abs(m_tInfo.fY - playerInfo->fY) <= TILECY / 2)
			{

				if (m_tInfo.fX == playerInfo->fX - TILECX)
				{
					if ((iPlayerNum == 0 && CKey_Manager::Get_Instance()->Key_Down(VK_LEFT)) ||
						(iPlayerNum == 1 && CKey_Manager::Get_Instance()->Key_Down('D')) ||
						(iPlayerNum == 2 && CKey_Manager::Get_Instance()->Key_Down(VK_LEFT)))
					{
						++m_pushTimer[iPlayerNum][2];
					}
				}
				if (m_tInfo.fX == playerInfo->fX + TILECX)
				{
					if ((iPlayerNum == 0 && CKey_Manager::Get_Instance()->Key_Down(VK_RIGHT)) ||
						(iPlayerNum == 1 && CKey_Manager::Get_Instance()->Key_Down('G')) ||
						(iPlayerNum == 2 && CKey_Manager::Get_Instance()->Key_Down(VK_RIGHT)))
					{
						++m_pushTimer[iPlayerNum][3];
					}
				}
			}
			else
			{
				m_pushTimer[iPlayerNum][0] = 0;
				m_pushTimer[iPlayerNum][1] = 0;
				m_pushTimer[iPlayerNum][2] = 0;
				m_pushTimer[iPlayerNum][3] = 0;
			}
		}
		int iComNum = 3;
		for (auto& pCom : *(CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::COMPLAYER)))
		{
			if (iComNum > 5)
			{
				break;
			}
			if (static_cast<CComPlayer*>(pCom)->Get_State() != CComPlayer::STATE::WALK)
			{
				continue;
			}
			const INFO* comInfo = pCom->Get_Info();

			if (abs(m_tInfo.fX - comInfo->fX) <= TILECX / 2)
			{
				if (m_tInfo.fY == comInfo->fY - TILECY)
				{
					if (static_cast<CComPlayer*>(pCom)->Get_FrameKey()==L"bazzi_up")
					{
						++m_pushTimer[iComNum][0];
					}
				}
				if (m_tInfo.fY == comInfo->fY + TILECY)
				{
					if (static_cast<CComPlayer*>(pCom)->Get_FrameKey() == L"bazzi_down")
					{
						++m_pushTimer[iComNum][1];
					}
				}
			}
			else if (abs(m_tInfo.fY - comInfo->fY) <= TILECY / 2)
			{

				if (m_tInfo.fX == comInfo->fX - TILECX)
				{
					if (static_cast<CComPlayer*>(pCom)->Get_FrameKey() == L"bazzi_left")
					{
						++m_pushTimer[iComNum][2];
					}
				}
				if (m_tInfo.fX == comInfo->fX + TILECX)
				{
					if (static_cast<CComPlayer*>(pCom)->Get_FrameKey() == L"bazzi_right")
					{
						++m_pushTimer[iComNum][3];
					}
				}
			}
			else
			{
				m_pushTimer[iComNum][0] = 0;
				m_pushTimer[iComNum][1] = 0;
				m_pushTimer[iComNum][2] = 0;
				m_pushTimer[iComNum][3] = 0;
			}

			iComNum++;
		}
	}
	else if (m_eBlockType == BLOCK_TYPE::BUSH)
	{
		CTile* pTile = static_cast<CTile*>(CTile_Manager::Get_Instance()->Pick_Tile(m_tInfo.fX, m_tInfo.fY));
		if (pTile)
		{
			m_iCurInBush = 0;
			for (CGameObject* pObject : *(pTile->Get_ListObjectOnTile()))
			{
				if (pObject->Get_ObjectType() == OBJECT::PLAYER || pObject->Get_ObjectType() == OBJECT::BLOCK)
				{
					++m_iCurInBush;
				}
			}
			if (m_bBushShake == false && m_iCurInBush > 1 && m_iCurInBush > m_iPrevInBush)
			{
				m_bBushShake = true;
				m_bushTimer = GetTickCount();
			}
			m_iPrevInBush = m_iCurInBush;
		}
	}

	return 0;
}

void CBlock::Late_Update_GameObject()
{
	for (int p = 0; p < 6; p++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_pushTimer[p][i] > 10)
			{
				Move_Block((DIRECTION::DIREC)i);
				m_pushTimer[p][0] = 0;
				m_pushTimer[p][1] = 0;
				m_pushTimer[p][2] = 0;
				m_pushTimer[p][3] = 0;
			}
		}
	}
}

void CBlock::Render_GameObject(HDC hDC)
{
	CGameObject::Update_Rect_GameObject();

	CTile* pTile = static_cast<CTile*>(CTile_Manager::Get_Instance()->Pick_Tile(m_tInfo.fX, m_tInfo.fY));
	if (pTile)
	{
		for (CGameObject* pObject : *(pTile->Get_ListObjectOnTile()))
		{
			if (m_eBlockType != BLOCK_TYPE::BUSH && pObject->Get_ObjectType() == OBJECT::BLOCK && static_cast<CBlock*>(pObject)->Get_BlockType() == BLOCK_TYPE::BUSH)
			{
				return;
			}
		}
	}

	HDC hMemDC = CBitmap_Manager::Get_Instance()->FindImage(m_pFrameKey);
	if (nullptr == hMemDC)
		return;

	if (m_eMovingDirec != DIRECTION::NO_DIREC && m_movingTimer != 0)
	{
		int iMoveTime = GetTickCount() - m_movingTimer;
		int iMoveX = 0;
		int iMoveY = 0;		
		
		if (iMoveTime > 100)
		{
			m_eMovingDirec = DIRECTION::NO_DIREC;
			m_movingTimer = 0;
		}
		switch (m_eMovingDirec)
		{
		case DIRECTION::UP:
			iMoveY = 40 - 40 * (iMoveTime / 100.f);
			break;
		case DIRECTION::DOWN:
			iMoveY = - 40 + 40 * (iMoveTime / 100.f);
			break;
		case DIRECTION::LEFT:
			iMoveX = 40 - 40 * (iMoveTime / 100.f);
			break;
		case DIRECTION::RIGHT:
			iMoveX = -40 + 40 * (iMoveTime / 100.f);
			break;
		case DIRECTION::NO_DIREC:
			break;
		default:
			break;
		}
		GdiTransparentBlt(hDC,
			m_tInfo.fX - m_tFrame.iFrameX * 0.5f+ iMoveX,
			m_tInfo.fY - m_tFrame.iFrameY * 0.5f - 20 + iMoveY,
			m_tFrame.iFrameX, m_tFrame.iFrameY,
			hMemDC,
			m_iDrawID * m_tFrame.iFrameX, 0,
			m_tFrame.iFrameX, m_tFrame.iFrameY,
			RGB(255, 0, 255));


	}
	if (m_eMovingDirec == DIRECTION::NO_DIREC && m_movingTimer == 0)
	{
		int iMoveY = 0;
		if (m_bBushShake == true)
		{
			if (m_bushTimer < GetTickCount())
			{
				iMoveY = -3;
			}
			if (m_bushTimer + 100 < GetTickCount())
			{
				iMoveY = +3;
			}
		
			if (m_bushTimer + 200 < GetTickCount())
			{
				iMoveY = 0;
				m_bBushShake = false;
				m_bushTimer = 0;
			}
		}

		GdiTransparentBlt(hDC,
			m_tInfo.fX - m_tFrame.iFrameX * 0.5f,
			m_tInfo.fY - m_tFrame.iFrameY * 0.5f - 20 + iMoveY,
			m_tFrame.iFrameX, m_tFrame.iFrameY,
			hMemDC,
			m_iDrawID * m_tFrame.iFrameX, 0,
			m_tFrame.iFrameX, m_tFrame.iFrameY,
			RGB(255, 0, 255));
	}
}

void CBlock::Release_GameObject()
{
}

void CBlock::Receive_Damage()

{
	if (m_eBlockType == BLOCK_TYPE::NORMAL || m_eBlockType == BLOCK_TYPE::BOX || m_eBlockType == BLOCK_TYPE::BUSH)
	{
		m_bIsDead = true;
		if (m_eItemType < ITEM_TYPE::ITEM_END)
		{
			CGameObject* pItem = CItem::Create(m_tInfo.fX, m_tInfo.fY, m_eItemType);
			if (nullptr != pItem)
			{
				CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::ITEM, pItem);
			}
		}
	}
}

void CBlock::Set_ItemType()
{
	if (m_eBlockType == BLOCK_TYPE::NORMAL || m_eBlockType == BLOCK_TYPE::BOX)
	{
		int iRand = rand() % 100;
		if (iRand < 60)
		{
			m_eItemType = ITEM_TYPE::ITEM_END;
		}
		else if (iRand < 78)
		{
			m_eItemType = ITEM_TYPE::BUBBLE_UP;
		}
		else if (iRand < 88)
		{
			m_eItemType = ITEM_TYPE::POWER_UP;
		}
		else if (iRand < 90)
		{
			m_eItemType = ITEM_TYPE::POWER_MAX;
		}
		else if (iRand < 97)
		{
			m_eItemType = ITEM_TYPE::SPEED_UP;
		}
		else if (iRand < 98)
		{
			m_eItemType = ITEM_TYPE::RED_DEVIL;
		}
		else if (iRand < 99)
		{
			m_eItemType = ITEM_TYPE::NEEDLE;
		}
		else
		{
			m_eItemType = ITEM_TYPE::KICK;
		}
	}
}

void CBlock::Move_Block(DIRECTION::DIREC eDir)
{
	int iMoveX = 0;
	int iMoveY = 0;
	switch (eDir)
	{
	case DIRECTION::UP:
		iMoveY = - 40;
		break;
	case DIRECTION::DOWN:
		iMoveY = + 40;
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
				(pObject->Get_ObjectType() == OBJECT::MONSTER)||
				(pObject->Get_ObjectType() == OBJECT::COMPLAYER))
			{
				isMove = false;
			}
		}
		if (isMove)
		{
			m_tInfo.fX += iMoveX;
			m_tInfo.fY += iMoveY;

			m_eMovingDirec = eDir;
			m_movingTimer = GetTickCount();


			CTile* pTile = static_cast<CTile*>(CTile_Manager::Get_Instance()->Pick_Tile(m_tInfo.fX, m_tInfo.fY));
			if (pTile)
			{
				for (auto& pObject : *(pTile->Get_ListObjectOnTile()))
				{
					if (pObject->Get_ObjectType() == OBJECT::ITEM)
					{
						static_cast<CItem*>(pObject)->Receive_Damage();
					}
				}
			}
		}
	}

}


CGameObject* CBlock::Create(int iDrawID, BLOCK_TYPE::TYPE eBlockType)
{
	CGameObject* pInstance = new CBlock;
	if (0 > pInstance->Ready_GameObject())
	{
		Safe_Delete(pInstance);
		return nullptr;
	}
	static_cast<CBlock*>(pInstance)->Set_DrawID(iDrawID);
	static_cast<CBlock*>(pInstance)->Set_BlockType(eBlockType);
	static_cast<CBlock*>(pInstance)->Set_ItemType();
	return pInstance;
}

