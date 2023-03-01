#include "pch.h"
#include "BubblePop.h"
#include "Block.h"
#include "Player.h"
#include "Item.h"
#include "Monster.h"
#include "ComPlayer.h"

CBubblePop::CBubblePop()
	: m_tFrame({})
{
}

CBubblePop::~CBubblePop()
{
}

void CBubblePop::MoveFrame()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();
	}
	if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		m_tFrame.iFrameStart = 0;

}

int CBubblePop::Ready_GameObject()
{
	m_eObjectType = OBJECT::ID::EFFECT;
	m_eRenderGroup = OBJECT::RENDER_LIST::UNDER_EFFECT;
	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;
	
	m_pFrameKey = (TCHAR*)L"Explosion";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iFrameState = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.iFrameX = 40;
	m_tFrame.iFrameY = 40;

	hMemDC = CBitmap_Manager::Get_Instance()->FindImage(m_pFrameKey);
	return 0;
}

int CBubblePop::Update_GameObject()
{
	if (m_bIsDead == true)
	{
		return OBJ_DEAD;
	}
	auto pTile = CTile_Manager::Get_Instance()->Pick_Tile(m_tInfo.fX, m_tInfo.fY);
	if (pTile != nullptr)
	{
		auto pListOnTile = static_cast<CTile*>(pTile)->Get_ListObjectOnTile();
		for (auto& pObject : *pListOnTile)
		{
			if (pObject->Get_ObjectType() == OBJECT::ID::BLOCK && static_cast<CBlock*>(pObject)->Get_BlockType() == BLOCK_TYPE::BUSH)
			{
				static_cast<CBlock*>(pObject)->Receive_Damage();
			}
			else if (pObject->Get_ObjectType() == OBJECT::ID::ITEM)
			{
				static_cast<CItem*>(pObject)->Receive_Damage();
			}
			else if (pObject->Get_ObjectType() == OBJECT::ID::PLAYER)
			{
				static_cast<CPlayer*>(pObject)->Receive_Damage();
			}
			else if (pObject->Get_ObjectType() == OBJECT::ID::MONSTER)
			{
				static_cast<CMonster*>(pObject)->Receive_Damage();
			}
			else if (pObject->Get_ObjectType() == OBJECT::ID::COMPLAYER)
			{
				static_cast<CComPlayer*>(pObject)->Receive_Damage();
			}
		}
	}
	return OBJ_NOEVENT;
}

void CBubblePop::Late_Update_GameObject()
{
	MoveFrame();
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		m_bIsDead = true;
	}
}

void CBubblePop::Render_GameObject(HDC hDC)
{
	CGameObject::Update_Rect_GameObject();

	if (nullptr == hMemDC)
		return;

	GdiTransparentBlt(hDC,
		m_tInfo.fX - m_tFrame.iFrameX * 0.5f,
		m_tInfo.fY - m_tFrame.iFrameY * 0.5f,
		m_tFrame.iFrameX, m_tFrame.iFrameY,
		hMemDC,
		m_tFrame.iFrameStart * m_tFrame.iFrameX, m_tFrame.iFrameState * m_tFrame.iFrameY,
		m_tFrame.iFrameX, m_tFrame.iFrameY,
		RGB(255, 0, 255));
}

void CBubblePop::Release_GameObject()
{
}

CGameObject* CBubblePop::Create(int iX, int iY, DIRECTION::DIREC eDirec)
{
	CGameObject* pInstance = new CBubblePop;
	if (0 > pInstance->Ready_GameObject())
	{
		Safe_Delete(pInstance);
		return nullptr;
	}
	pInstance->Set_Pos(iX, iY);
	static_cast<CBubblePop*>(pInstance)->SetDirection(eDirec);

	return pInstance;
}
