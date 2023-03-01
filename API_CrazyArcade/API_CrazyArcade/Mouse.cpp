#include "pch.h"
#include "Mouse.h"


CMouse::CMouse()
	:m_bClick(0)
{
}


CMouse::~CMouse()
{
	Release_GameObject();
}

int CMouse::Ready_GameObject()
{
	m_eObjectType = OBJECT::ID::MOUSE;
	m_eRenderGroup = OBJECT::RENDER_LIST::RENDER_MOUSE;

	m_pFrameKey = (TCHAR*)L"mouse";

	m_tInfo.iCX = 33;
	m_tInfo.iCY = 36;
	ShowCursor(FALSE);
	return READY_OK;
}

int CMouse::Update_GameObject()
{
	POINT pt{};

	GetCursorPos(&pt);
	m_bClick = CKey_Manager::Get_Instance()->Key_Down(VK_LBUTTON) || CKey_Manager::Get_Instance()->Key_Down(VK_RBUTTON);
	ScreenToClient(g_hWnd, &pt);
	m_tInfo.fX = static_cast<float>(pt.x);
	m_tInfo.fY = static_cast<float>(pt.y);

	return OBJ_NOEVENT;
}

void CMouse::Late_Update_GameObject()
{
}

void CMouse::Render_GameObject(HDC hDC)
{
	m_tRect = { (LONG)m_tInfo.fX ,(LONG)m_tInfo.fY ,(LONG)m_tInfo.fX + m_tInfo.iCX ,(LONG)m_tInfo.fY + m_tInfo.iCY };


	HDC hMemDC = CBitmap_Manager::Get_Instance()->FindImage(m_pFrameKey);
	if (nullptr == hMemDC)
		return;



	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		m_tInfo.iCX, m_tInfo.iCY,
		hMemDC,
		m_bClick * m_tInfo.iCX, 0,
		m_tInfo.iCX, m_tInfo.iCY,
		RGB(255, 0, 255));
}

void CMouse::Release_GameObject()
{
}

CGameObject* CMouse::Create()
{
	CGameObject* pInstance = new CMouse;
	if (0 > pInstance->Ready_GameObject())
		Safe_Delete(pInstance);

	return pInstance;
}
