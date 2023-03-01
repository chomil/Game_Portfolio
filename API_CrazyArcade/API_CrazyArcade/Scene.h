#pragma once
class CScene abstract
{
public:
	enum STATE { READY, START, WIN, LOSE, DRAW, PLAYER1_WIN, PLAYER2_WIN, END };
public:
	explicit CScene();
	virtual ~CScene();

public:
	virtual int Ready_Scene() = 0;
	virtual void Update_Scene() = 0;
	virtual void Render_Scene(HDC hDC) = 0;
	virtual void Release_Scene() = 0;

public:
	STATE Get_State() { return m_eStageState; }

protected:
	STATE m_eStageState;
};

