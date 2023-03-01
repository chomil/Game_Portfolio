#include "pch.h"
#include "GameObject.h"


CGameObject::CGameObject()
	:m_fSpeed(0.f)
	, m_bIsDead(false)
	, m_eRenderGroup(OBJECT::GAMEOBJECT)
	, m_eObjectType(OBJECT::OBJECT_END)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
}


CGameObject::~CGameObject()
{
}

void CGameObject::Update_Rect_GameObject()
{
	m_tRect.left = LONG(m_tInfo.fX - (m_tInfo.iCX >> 1));
	m_tRect.top = static_cast<LONG>(m_tInfo.fY - (m_tInfo.iCY * 0.5f));
	m_tRect.right = LONG(m_tInfo.fX + (m_tInfo.iCX >> 1));
	m_tRect.bottom = static_cast<LONG>(m_tInfo.fY + (m_tInfo.iCY * 0.5f));
}
