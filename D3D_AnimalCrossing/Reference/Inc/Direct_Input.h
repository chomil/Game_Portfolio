#pragma once

#include "Base.h"

BEGIN(Engine)

class CInput_Device final : public CBase
{
	DECLARE_SINGLETON(CInput_Device);
public:
	enum DIM { DIM_LBUTTON, DIM_RBUTTON, DIM_WHEELBUTTON, DIM_XBUTTON };
	enum DIMM { DIMM_X, DIMM_Y, DIMM_WHEEL  };
private:
	CInput_Device();
	virtual ~CInput_Device() = default;

public:
	_byte Get_DIKeyState(_byte byKey) {
		return m_KeyState[byKey];
	}

	_byte Get_DIMouseKeyState(DIM eMouseKey)
	{
		return m_MouseState.rgbButtons[eMouseKey];
	}

	_long Get_DIMouseMoveState(DIMM eMouseMove) {
		// return *(((_long*)&m_MouseState) + eMouseMove);
		return ((_long*)&m_MouseState)[eMouseMove];
	}

	/*if (Get_DIKState(DIK_RETURN) & 0x80)
	{

	}*/

public:
	HRESULT NativeConstruct(HINSTANCE hInst, HWND hWnd);
	_int Invalidate_Input_Device();

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;
	LPDIRECTINPUTDEVICE8	m_pKeyboard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

private:
	DIMOUSESTATE			m_MouseState;
	_byte					m_KeyState[256];
public:
	virtual void Free() override;
};

END