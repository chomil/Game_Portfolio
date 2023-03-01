#pragma once
#include "GameObject.h"

class CButton :
	public CGameObject
{
public:
	enum STATE { NORMAL, ON, DOWN };
	enum TYPE { BT_START, BT_BAZZI, BT_SLOT, BT_COLOR_RED, BT_COLOR_BLUE, 
				BT_EXIT, BT_CLOSE, BT_OFF, BT_BACK, BT_ARROW_R, BT_ARROW_L, BT_1P, BT_2P };
public:
	explicit CButton();
	virtual ~CButton();
public:
	// CGameObject을(를) 통해 상속됨
	virtual int Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject() override;

public:
	void Set_ButtonType(CButton::TYPE eType) { m_eType = eType; }
	CButton::TYPE Get_ButtonType() { return m_eType; }
	int Get_Clicked() { return m_iClicked; }
	void Set_Clicked(int iClicked) { m_iClicked= iClicked; }
public:
	static CGameObject* Create(float fX, float fY, CButton::TYPE eType);

private:
	TCHAR* m_pFrameKey;
	FRAME m_tFrame;

	STATE m_eState;
	TYPE m_eType;

	int m_iClicked;
};

