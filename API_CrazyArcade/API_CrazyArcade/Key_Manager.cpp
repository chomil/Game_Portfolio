#include "pch.h"
#include "Key_Manager.h"

CKey_Manager* CKey_Manager::m_pInstance = nullptr;

CKey_Manager::CKey_Manager()
{
	for (int i = 0; i < 256; ++i)
	{
		m_eKeyState[i] = KEY_STATE::UP;
	}
}


CKey_Manager::~CKey_Manager()
{
}

void CKey_Manager::Key_Update()
{
	for (int i = 0; i < 256; ++i)
	{
		if (GetAsyncKeyState(i) & 0x8000)
		{
			if (m_eKeyState[i] & KEY_STATE::UP)
			{
				m_eKeyState[i] = KEY_STATE::DOWN | KEY_STATE::PRESSED;
			}
			else
			{
				m_eKeyState[i] = KEY_STATE::DOWN;
			}
		}
		else
		{
			if (m_eKeyState[i] & KEY_STATE::DOWN)
			{
				m_eKeyState[i] = KEY_STATE::UP | KEY_STATE::RELEASED;
			}
			else
			{
				m_eKeyState[i] = KEY_STATE::UP;
			}
		}
	}

}

bool CKey_Manager::Key_Up(int iKey)
{
	return m_eKeyState[iKey] & KEY_STATE::UP;
}

bool CKey_Manager::Key_Down(int iKey)
{
	return m_eKeyState[iKey] & KEY_STATE::DOWN;
}

bool CKey_Manager::Key_Released(int iKey)
{
	return m_eKeyState[iKey] & KEY_STATE::RELEASED;
}

bool CKey_Manager::Key_Pressed(int iKey)
{
	return m_eKeyState[iKey] & KEY_STATE::PRESSED;
}

