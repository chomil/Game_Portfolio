#pragma once
#include "GameObject.h"
class CMonster :
	public CGameObject
{
public:
	enum STATE { IDLE, WALK, HIT, ATTACK, TRAP, DIE};
private:
	explicit CMonster();
public:
	virtual ~CMonster();
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
	void Receive_Damage();

	STATE Get_State() { return m_eCurState; }
	void Set_Direction(DIRECTION::DIREC eMoveDir) { m_eMoveDir = eMoveDir; }
	void Set_MonsterType(int iMonsterType) { m_iMonsterType = iMonsterType; }
	int Get_MonsterType() { return m_iMonsterType; }

private:
	void Block_Collision(DIRECTION::DIREC eDir, float fSpeed);

public:
	static CGameObject* Create(int iX, int iY, DIRECTION::DIREC eMoveDir, int iMonsterType);
private:
	FRAME m_tFrame;
	TCHAR* m_pFrameKey;
	STATE m_eCurState;
	STATE m_eNextState;

	int m_iBottom;
	int m_iHP;
	int m_iMaxHP;

	DIRECTION::DIREC m_eMoveDir;

	DWORD m_dwPatternTimer;
	int m_iPattern;

	DWORD m_dwNoDamageTimer;
	bool m_bNoDamage;

	DWORD m_dwFollowTimer;
	bool m_bFollow;

	int m_iMonsterType;
};

