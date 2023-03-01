#pragma once
#include "GameObject.h"
class CPlayer final :
	public CGameObject
{
public:
	enum STATE { IDLE, WALK, READY, TRAP, LIVE, DIE, WIN};
private:
	explicit CPlayer();
public:
	virtual ~CPlayer();
public:
	void MoveFrame();
	void Change_State();
public:
	// CGameObject을(를) 통해 상속됨
	virtual int Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject() override;

public:
	int Get_PlayerNum() { return m_iPlayerNum; }
	void Set_PlayerNum(int iPlayerNum) { m_iPlayerNum = iPlayerNum; }

public:
	void Eat_Item(ITEM_TYPE::TYPE eItemType);
	void Receive_Damage() 
	{
		if (m_bNoDamage == true)
		{
			return;
		}
		if (m_eCurState == IDLE || m_eCurState == WALK)
		{
			m_eNextState = TRAP;
			Change_State();
		}
	}
	void Receive_Critical()
	{
		if (m_bNoDamage == true)
		{
			return;
		}
		if (m_eCurState == IDLE || m_eCurState == WALK || m_eCurState==TRAP)
		{
			m_eNextState = DIE;
			Change_State();
		}
	}
	void Set_Color(int iColor) { m_iColor = iColor; }
	int Get_Color() { return m_iColor; }
	void Set_IsShow(bool bIsShow) { m_bIsShow = bIsShow; }
	bool Get_IsShow() { return m_bIsShow; }
	int Get_NeedleNum() { return m_iNeedleNum; }

	STATE Get_State() { return m_eCurState; }

private:
	void Block_Collision(DIRECTION::DIREC eDir, float fSpeed);

public:
	static CGameObject* Create(int iPlayerNum = 0);
private:
	FRAME m_tFrame;
	TCHAR* m_pFrameKey;
	STATE m_eCurState;
	STATE m_eNextState;
	int m_iBottom;


	int m_iColor;
	float m_fSpeedMax;
	int m_iBubbleNum;
	int m_iBubbleMaxNum;
	int m_iBubblePower;
	int m_iBubbleMaxPower;
	bool m_bKick;
	int m_iNeedleNum;


	bool m_bIsShow;

	bool m_bNoDamage;

	int m_iPlayerNum;

	DWORD m_updateTimer;
	int m_iUpdateCnt;
};

