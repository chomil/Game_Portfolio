#pragma once
#include "GameObject.h"

class CBubblePop :
	public CGameObject
{
private:
	CBubblePop();
public:
	~CBubblePop();
public:
	// CGameObject을(를) 통해 상속됨
	virtual int Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject() override;

public:
	void MoveFrame();
	void SetDirection(DIRECTION::DIREC eDirec)
	{ 
		m_eDirec = eDirec;
		if (m_eDirec == DIRECTION::DIREC::NO_DIREC)
		{
			m_tFrame.iFrameState = 0;
		}
		else
		{
			m_tFrame.iFrameState = m_eDirec + 1;
		}
	}
public:
	static CGameObject* Create(int iX, int iY, DIRECTION::DIREC eDirec);
private:

	HDC hMemDC;
	FRAME m_tFrame;
	TCHAR* m_pFrameKey;

	DIRECTION::DIREC m_eDirec;
};

