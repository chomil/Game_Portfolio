#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)
public:
	enum WINMODE {MODE_FULL, MODE_WIN};
private:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;
public:
	LPDIRECT3DDEVICE9 Get_Device() { return m_pDevice; }
	LPD3DXSPRITE Get_Sprite() { return m_pSprite;  }
	LPD3DXFONT Get_Font() { return m_pFont; }
	HRESULT Set_Font(D3DXFONT_DESCW* pFont_Desc);
	LPD3DXLINE Get_Line() { return m_pLine;  }

public:
	HRESULT Ready_Graphic_Device(HWND hWnd, WINMODE eMode, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppGraphic_Device = nullptr);
	void Render_Begin(); 
	void Render_End(HWND hWND = nullptr); 
private:
	LPDIRECT3D9 m_pSDK; //���� ��ġ�� ������ �����ϰ� ��ġ�� �����ϱ� ���� �İ�ü�� ����� �ִ� ����. 
	LPDIRECT3DDEVICE9 m_pDevice; // ��ġ�� �����ϱ� ���� �İ�ü. 
	//�İ�ü -com(Component Object Model)
	LPD3DXSPRITE m_pSprite; 
	LPD3DXFONT m_pFont; 
	LPD3DXLINE m_pLine; 

public:
	virtual void Free() override;
};

END