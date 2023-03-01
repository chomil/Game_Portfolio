#include "pch.h"
#include "Player.h"
#include "Item.h"
#include "ComPlayer.h"
#include "GameObject_Manager.h"


CItem::CItem()
{
}

CItem::~CItem()
{
}

void CItem::MoveFrame()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();
	}
	if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		m_tFrame.iFrameStart = 0;

}

int CItem::Ready_GameObject()
{
	m_eObjectType = OBJECT::ID::ITEM;

	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;

	m_iBottom = -10;
	m_iFloatDirec = 1;
	m_floatTimer = GetTickCount();

	m_pFrameKey = (TCHAR*)L"item";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 2;
	//m_tFrame.iFrameState = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.iFrameX = 42;
	m_tFrame.iFrameY = 45;
	return 0;
}

int CItem::Update_GameObject()
{
	if (m_bIsDead == true)
	{
		return OBJ_DEAD;
	}
	if (m_floatTimer + 200 < GetTickCount())
	{
		m_iBottom += m_iFloatDirec;
		if (m_iBottom <= -12 || m_iBottom >=-8)
		{
			m_iFloatDirec *= -1;
		}
		m_floatTimer = GetTickCount();
	}
	return 0;
}

void CItem::Late_Update_GameObject()
{

	for (auto& pPlayer : *(CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER)))
	{
		if (static_cast<CPlayer*>(pPlayer)->Get_State() == CPlayer::STATE::IDLE || static_cast<CPlayer*>(pPlayer)->Get_State() == CPlayer::STATE::WALK)
		{
			if (PtInRect(&m_tRect, pPlayer->Get_Pos()))
			{
				static_cast<CPlayer*>(pPlayer)->Eat_Item(m_eItemType);
				m_bIsDead = true;
			}
		}
	}
	for (auto& pCom : *(CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::COMPLAYER)))
	{
		if (static_cast<CComPlayer*>(pCom)->Get_State() == CComPlayer::STATE::IDLE || static_cast<CComPlayer*>(pCom)->Get_State() == CComPlayer::STATE::WALK)
		{
			if (PtInRect(&m_tRect, pCom->Get_Pos()))
			{
				static_cast<CComPlayer*>(pCom)->Eat_Item(m_eItemType);
				m_bIsDead = true;
			}
		}
	}
	MoveFrame();
}

void CItem::Render_GameObject(HDC hDC)
{
	CGameObject::Update_Rect_GameObject();

	HDC hMemDC = CBitmap_Manager::Get_Instance()->FindImage(m_pFrameKey);
	if (nullptr == hMemDC)
		return;

	GdiTransparentBlt(hDC,
		m_tInfo.fX - m_tFrame.iFrameX * 0.5f,
		m_tInfo.fY - m_tFrame.iFrameY * 0.5f + m_iBottom,
		m_tFrame.iFrameX, m_tFrame.iFrameY,
		hMemDC,
		m_tFrame.iFrameStart * m_tFrame.iFrameX, m_tFrame.iFrameState * m_tFrame.iFrameY,
		m_tFrame.iFrameX, m_tFrame.iFrameY,
		RGB(255, 0, 255));
}

void CItem::Release_GameObject()
{
}


CGameObject* CItem::Create(int iX, int iY, ITEM_TYPE::TYPE eItemType)
{
	CGameObject* pInstance = new CItem;
	if (0 > pInstance->Ready_GameObject())
	{
		Safe_Delete(pInstance);
		return nullptr;
	}
	pInstance->Set_Pos(iX, iY);
	static_cast<CItem*>(pInstance)->Set_ItemType(eItemType);
	return pInstance;
}