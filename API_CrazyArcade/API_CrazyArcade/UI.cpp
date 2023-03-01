#include "pch.h"
#include "UI.h"

CUI::CUI()
{
}

CUI::~CUI()
{
}

int CUI::Ready_GameObject()
{
	m_eObjectType = OBJECT::ID::UI;
	m_eRenderGroup = OBJECT::RENDER_LIST::POP_UP;

	m_bClose = false;
	return 0;
}

int CUI::Update_GameObject()
{
	if (m_bClose == true)
	{
		return OBJ_DEAD;
	}
	return 0;
}

void CUI::Late_Update_GameObject()
{
}

void CUI::Render_GameObject(HDC hDC)
{
	HDC hMemDC = CBitmap_Manager::Get_Instance()->FindImage(m_pFrameKey);
	if (nullptr == hMemDC)
		return;

	GdiTransparentBlt(hDC,
		(int)m_tInfo.fX - m_tInfo.iCX / 2,
		(int)m_tInfo.fY - m_tInfo.iCY / 2,
		m_tInfo.iCX, m_tInfo.iCY,
		hMemDC,
		0, 0,
		m_tInfo.iCX, m_tInfo.iCY,
		RGB(255, 0, 255));
}

void CUI::Release_GameObject()
{
}

CGameObject* CUI::Create(float fX, float fY, int iCX, int iCY, const TCHAR* szKeyName)
{
	CGameObject* pInstance = new CUI;

	if (0 > pInstance->Ready_GameObject())
		Safe_Delete(pInstance);

	static_cast<CUI*>(pInstance)->Set_Pos(fX, fY);
	static_cast<CUI*>(pInstance)->Set_Size(iCX, iCY);
	static_cast<CUI*>(pInstance)->Set_KeyName(szKeyName);
	return pInstance;
}
