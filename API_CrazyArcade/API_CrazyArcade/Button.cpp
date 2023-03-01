#include "pch.h"
#include "Button.h"
#include "Scene.h"
#include "GameObject_Manager.h"
#include "Player.h"
#include "Scene_Room.h"
#include "UI.h"

CButton::CButton()
	:m_pFrameKey(nullptr), m_tFrame({}), m_eState(STATE::NORMAL)
{
}


CButton::~CButton()
{
	Release_GameObject();
}

int CButton::Ready_GameObject()
{
	m_eObjectType = OBJECT::ID::BUTTON;
	m_eRenderGroup = OBJECT::RENDER_LIST::RENDER_UI;


	m_iClicked = 0;

	switch (m_eType)
	{
	case CButton::BT_START:
		m_pFrameKey = (TCHAR*)L"start_button";
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameState = 0;
		m_tFrame.iFrameX = 193;
		m_tFrame.iFrameY = 57;
		m_tInfo.iCX = 193;
		m_tInfo.iCY = 57;
		break;
	case CButton::BT_BAZZI:
		m_pFrameKey = (TCHAR*)L"bazzi_select";
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameState = 0;
		m_tFrame.iFrameX = 95;
		m_tFrame.iFrameY = 61;
		m_tInfo.iCX = 65;
		m_tInfo.iCY = 43;
		m_eRenderGroup = OBJECT::RENDER_LIST::POP_UP;
		break;
	case CButton::BT_SLOT:
		m_pFrameKey = (TCHAR*)L"CharacterSlot";
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameState = 0;
		m_tFrame.iFrameX = 98;
		m_tFrame.iFrameY = 98;
		m_tInfo.iCX = 98;
		m_tInfo.iCY = 98;
		break;
	case CButton::BT_COLOR_RED:
		m_pFrameKey = (TCHAR*)L"color_button";
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameState = 0;
		m_tFrame.iFrameX = 72;
		m_tFrame.iFrameY = 57;
		m_tInfo.iCX = 70;
		m_tInfo.iCY = 37;
		break;
	case CButton::BT_COLOR_BLUE:
		m_pFrameKey = (TCHAR*)L"color_button";
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameState = 1;
		m_tFrame.iFrameX = 72;
		m_tFrame.iFrameY = 57;
		m_tInfo.iCX = 70;
		m_tInfo.iCY = 37;
		break;
	case CButton::BT_EXIT:
		m_pFrameKey = (TCHAR*)L"exit_button";
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameState = 0;
		m_tFrame.iFrameX = 156;
		m_tFrame.iFrameY = 38;
		m_tInfo.iCX = 156;
		m_tInfo.iCY = 38;

		m_eRenderGroup = OBJECT::RENDER_LIST::POP_UP_BUTTON;
		break;
	case CButton::BT_CLOSE:
		m_pFrameKey = (TCHAR*)L"close_button";
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameState = 0;
		m_tFrame.iFrameX = 110;
		m_tFrame.iFrameY = 38;
		m_tInfo.iCX = 110;
		m_tInfo.iCY = 38;

		m_eRenderGroup = OBJECT::RENDER_LIST::POP_UP_BUTTON;
		break;
	case CButton::BT_ARROW_R:
		m_pFrameKey = (TCHAR*)L"arrow_button";
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameState = 0;
		m_tFrame.iFrameX = 58;
		m_tFrame.iFrameY = 42;
		m_tInfo.iCX = 58;
		m_tInfo.iCY = 42;
		break;
	case CButton::BT_ARROW_L:
		m_pFrameKey = (TCHAR*)L"arrow_button";
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameState = 1;
		m_tFrame.iFrameX = 58;
		m_tFrame.iFrameY = 42;
		m_tInfo.iCX = 58;
		m_tInfo.iCY = 42;
		break;
	case CButton::BT_1P:
		m_pFrameKey = (TCHAR*)L"1p_button";
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameState = 0;
		m_tFrame.iFrameX = 176;
		m_tFrame.iFrameY = 99;
		m_tInfo.iCX = 176;
		m_tInfo.iCY = 99;
		break;
	case CButton::BT_2P:
		m_pFrameKey = (TCHAR*)L"2p_button";
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameState = 0;
		m_tFrame.iFrameX = 176;
		m_tFrame.iFrameY = 99;
		m_tInfo.iCX = 176;
		m_tInfo.iCY = 99;
		break;
	case CButton::BT_OFF:
		m_pFrameKey = (TCHAR*)L"off_button";
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameState = 0;
		m_tFrame.iFrameX = 31;
		m_tFrame.iFrameY = 24;
		m_tInfo.iCX = 31;
		m_tInfo.iCY = 24;
		break;
	case CButton::BT_BACK:
		m_pFrameKey = (TCHAR*)L"back_button";
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameState = 0;
		m_tFrame.iFrameX = 31;
		m_tFrame.iFrameY = 24;
		m_tInfo.iCX = 31;
		m_tInfo.iCY = 24;
		break;
	default:
		break;
	}


	return READY_OK;
}

int CButton::Update_GameObject()
{
	POINT ptCursor = {};
	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);
	Update_Rect_GameObject();

	if (m_eType == TYPE::BT_BAZZI)
	{
		m_tFrame.iFrameStart = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER)->size() - 1;
	}

	switch (m_eType)
	{
	case CButton::BT_EXIT:
	case CButton::BT_START:
	case CButton::BT_ARROW_L:
	case CButton::BT_ARROW_R:
	case CButton::BT_CLOSE:
	case CButton::BT_1P:
	case CButton::BT_2P:
	case CButton::BT_BACK:
	case CButton::BT_OFF:
		if (PtInRect(&m_tRect, ptCursor))
		{
			if (m_eState == STATE::NORMAL)
			{
				CSound_Manager::Get_Instance()->PlaySound(L"SFX_Button_On.mp3", CSound_Manager::CHANNELID::UI);
			}
			m_tFrame.iFrameStart = 1;
			if (CKey_Manager::Get_Instance()->Key_Down(VK_LBUTTON) || CKey_Manager::Get_Instance()->Key_Down(VK_RBUTTON))
			{
				if (m_eState == STATE::ON)
				{
					CSound_Manager::Get_Instance()->PlaySound(L"SFX_Button_Click.mp3", CSound_Manager::CHANNELID::UI);
				}
				m_eState = STATE::DOWN;
				m_tFrame.iFrameStart = 2;
			}
			else
			{
				m_eState = STATE::ON;
			}
			if (CKey_Manager::Get_Instance()->Key_Released(VK_LBUTTON) || CKey_Manager::Get_Instance()->Key_Released(VK_RBUTTON))
			{
				m_iClicked++;
				if (m_eType == BT_ARROW_L)
				{
					CScene_Room::Move_StageNum(-1);
				}
				else if (m_eType == BT_ARROW_R)
				{
					CScene_Room::Move_StageNum(+1);
				}
				else if (m_eType == BT_CLOSE)
				{
					auto pObjectList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::ID::UI);
					for (auto pUI : *pObjectList)
					{
						static_cast<CUI*>(pUI)->Set_Close(true);
					}

					return OBJ_DEAD;
				}
			}
		}
		else
		{
			m_eState = STATE::NORMAL;
			m_tFrame.iFrameStart = 0;
		}
		break;
	case CButton::BT_BAZZI:
		m_tFrame.iFrameStart = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER)->size() - 1;
		if (PtInRect(&m_tRect, ptCursor))
		{
			if (m_eState == STATE::NORMAL)
			{
				CSound_Manager::Get_Instance()->PlaySound(L"SFX_Button_On.mp3", CSound_Manager::CHANNELID::UI);
			}

			if (CKey_Manager::Get_Instance()->Key_Down(VK_LBUTTON) || CKey_Manager::Get_Instance()->Key_Down(VK_RBUTTON))
			{
				if (m_eState == STATE::ON)
				{
					CSound_Manager::Get_Instance()->PlaySound(L"SFX_Button_Click.mp3", CSound_Manager::CHANNELID::UI);
				}
				m_eState = STATE::DOWN;
			}
			else
			{
				m_eState = STATE::ON;
			}
		}
		else
		{
			m_eState = STATE::NORMAL;
		}
		break;
	case CButton::BT_SLOT:
		if (PtInRect(&m_tRect, ptCursor))
		{
			if (m_eState == STATE::NORMAL)
			{
				CSound_Manager::Get_Instance()->PlaySound(L"SFX_Button_On.mp3", CSound_Manager::CHANNELID::UI);
			}
			m_tFrame.iFrameStart = 1;
			if (CKey_Manager::Get_Instance()->Key_Down(VK_LBUTTON) || CKey_Manager::Get_Instance()->Key_Down(VK_RBUTTON))
			{
				if (m_eState == STATE::ON)
				{
					CSound_Manager::Get_Instance()->PlaySound(L"SFX_Button_Click.mp3", CSound_Manager::CHANNELID::UI);
				}
				m_eState = STATE::DOWN;
				m_tFrame.iFrameStart = 2;
			}
			else
			{
				m_eState = STATE::ON;
			}
		}
		else
		{
			m_eState = STATE::NORMAL;
			m_tFrame.iFrameStart = 0;
		}
		break;
	case CButton::BT_COLOR_RED:
	{
		auto pPlayerList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER);
		if (pPlayerList->size() == 1)
		{
			if (static_cast<CPlayer*>(pPlayerList->front())->Get_Color()==0)
			{
				m_tFrame.iFrameStart = 3;
			}
			else
			{
				m_tFrame.iFrameStart = 0;
			}
		}
		else if(pPlayerList->size() == 2)
		{
			m_tFrame.iFrameStart = 2;
			if (static_cast<CPlayer*>(pPlayerList->front())->Get_Color() == 0)
			{
				m_tFrame.iFrameStart += 1;
			}
			if (static_cast<CPlayer*>(pPlayerList->back())->Get_Color() == 0)
			{
				m_tFrame.iFrameStart += 2;
			}
		}
		if (PtInRect(&m_tRect, ptCursor))
		{
			if (m_eState == STATE::NORMAL)
			{
				CSound_Manager::Get_Instance()->PlaySound(L"SFX_Button_On.mp3", CSound_Manager::CHANNELID::UI);
			}
			if (m_tFrame.iFrameStart < 3)
			{
				m_tFrame.iFrameStart = 1;
			}
			if (CKey_Manager::Get_Instance()->Key_Down(VK_LBUTTON) || CKey_Manager::Get_Instance()->Key_Down(VK_RBUTTON))
			{
				if (m_eState == STATE::ON)
				{
					CSound_Manager::Get_Instance()->PlaySound(L"SFX_Button_Click.mp3", CSound_Manager::CHANNELID::UI);
				}
				m_eState = STATE::DOWN;
				if (m_tFrame.iFrameStart < 3)
				{
					m_tFrame.iFrameStart = 2;
				}
			}
			else
			{
				m_eState = STATE::ON;
			}
			if (CKey_Manager::Get_Instance()->Key_Released(VK_LBUTTON))
			{
				static_cast<CPlayer*>(pPlayerList->front())->Set_Color(0);
			}
			if (CKey_Manager::Get_Instance()->Key_Released(VK_RBUTTON))
			{
				static_cast<CPlayer*>(pPlayerList->back())->Set_Color(0);
			}
		}
		else
		{
			m_eState = STATE::NORMAL;
			if (m_tFrame.iFrameStart < 3)
			{
				m_tFrame.iFrameStart = 0;
			}
		}
		break;
	}
	case CButton::BT_COLOR_BLUE:
	{
		auto pPlayerList = CGameObject_Manager::Get_Instance()->Get_ListObject(OBJECT::PLAYER);
		if (pPlayerList->size() == 1)
		{
			if (static_cast<CPlayer*>(pPlayerList->front())->Get_Color() == 1)
			{
				m_tFrame.iFrameStart = 3;
			}
			else
			{
				m_tFrame.iFrameStart = 0;
			}
		}
		else if (pPlayerList->size() == 2)
		{
			m_tFrame.iFrameStart = 2;
			if (static_cast<CPlayer*>(pPlayerList->front())->Get_Color() == 1)
			{
				m_tFrame.iFrameStart += 1;
			}
			if (static_cast<CPlayer*>(pPlayerList->back())->Get_Color() == 1)
			{
				m_tFrame.iFrameStart += 2;
			}
		}
		if (PtInRect(&m_tRect, ptCursor))
		{
			if (m_eState == STATE::NORMAL)
			{
				CSound_Manager::Get_Instance()->PlaySound(L"SFX_Button_On.mp3", CSound_Manager::CHANNELID::UI);
			}
			if (m_tFrame.iFrameStart < 3)
			{
				m_tFrame.iFrameStart = 1;
			}
			if (CKey_Manager::Get_Instance()->Key_Down(VK_LBUTTON) || CKey_Manager::Get_Instance()->Key_Down(VK_RBUTTON))
			{
				if (m_eState == STATE::ON)
				{
					CSound_Manager::Get_Instance()->PlaySound(L"SFX_Button_Click.mp3", CSound_Manager::CHANNELID::UI);
				}
				m_eState = STATE::DOWN;
				if (m_tFrame.iFrameStart < 3)
				{
					m_tFrame.iFrameStart = 2;
				}
			}
			else
			{
				m_eState = STATE::ON;
			}
			if (CKey_Manager::Get_Instance()->Key_Released(VK_LBUTTON))
			{
				static_cast<CPlayer*>(pPlayerList->front())->Set_Color(1);
			}
			if (CKey_Manager::Get_Instance()->Key_Released(VK_RBUTTON))
			{
				static_cast<CPlayer*>(pPlayerList->back())->Set_Color(1);
			}
		}
		else
		{
			m_eState = STATE::NORMAL;
			if (m_tFrame.iFrameStart < 3)
			{
				m_tFrame.iFrameStart = 0;
			}
		}
		break;
	}


	default:
		break;
	}


	return OBJ_NOEVENT;
}

void CButton::Late_Update_GameObject()
{
}

void CButton::Render_GameObject(HDC hDC)
{
	HDC hMemDC = CBitmap_Manager::Get_Instance()->FindImage(m_pFrameKey);
	if (nullptr == hMemDC)
		return;

	GdiTransparentBlt(hDC,
		(int)m_tInfo.fX - m_tFrame.iFrameX / 2,
		(int)m_tInfo.fY - m_tFrame.iFrameY / 2,
		m_tFrame.iFrameX, m_tFrame.iFrameY,
		hMemDC,
		m_tFrame.iFrameStart * m_tFrame.iFrameX, m_tFrame.iFrameState* m_tFrame.iFrameY,
		m_tFrame.iFrameX, m_tFrame.iFrameY,
		RGB(255, 0, 255));

	if (m_eType == TYPE::BT_BAZZI && m_eState == STATE::ON)
	{
		hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"bazzi_info");
		if (nullptr == hMemDC)
			return;

		GdiTransparentBlt(hDC,
			277,
			56,
			205, 92,
			hMemDC,
			0, 0,
			205, 92,
			RGB(255, 0, 255));
	}
}

void CButton::Release_GameObject()
{
}

CGameObject* CButton::Create(float fX, float fY, CButton::TYPE eType)
{
	CGameObject* pInstance = new CButton;
	
	static_cast<CButton*>(pInstance)->Set_ButtonType(eType);

	if (0 > pInstance->Ready_GameObject())
		Safe_Delete(pInstance);

	static_cast<CButton*>(pInstance)->Set_Pos(fX,fY);
	return pInstance;
}
