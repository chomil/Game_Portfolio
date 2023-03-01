#pragma once
#include "GameObject.h"
class CBubble :
	public CGameObject
{
private:
	CBubble();
public:
	~CBubble();
public:
	// CGameObject을(를) 통해 상속됨
	virtual int Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject() override;

public:
	void MoveFrame();
	void Set_Pop() { m_bPop = true; }
	void Set_Power(int iPower) { m_iPower = iPower; }
	void Receive_Damage()
	{
		m_timeStart = GetTickCount() - 3001;
	}
	int Hit_Object(int iX, int iY);
	const CGameObject* Get_Owner() { return m_pOwner; }
	void Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }
	void Kick_Count(DIRECTION::DIREC eDir) { m_iPushCount[eDir]++; }
public:
	void Moving_Bubble(DIRECTION::DIREC eDir);
public:
	static CGameObject* Create(int iX,int iY,int iPower, CGameObject* pOwner);
private:
	FRAME m_tFrame;
	TCHAR* m_pFrameKey;

	bool m_bPop;
	int m_iPower;
	DWORD m_timeStart;

	DIRECTION::DIREC m_eMovingDirec;
	int m_iPushCount[DIRECTION::NO_DIREC];
	DWORD m_movingTimer;

	const CGameObject* m_pOwner;
};