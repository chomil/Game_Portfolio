#pragma once
#include "Scene.h"
class CScene_Room :
	public CScene
{
private:
	explicit CScene_Room();
public:
	virtual ~CScene_Room();
public:
	// CScene을(를) 통해 상속됨
	virtual int Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene(HDC hDC) override;
	virtual void Release_Scene() override;

	static void Move_StageNum(int iNum) 
	{ 
		m_iStageNum += iNum; 
		m_iStageNum = (m_iStageNum%3+3)%3;
	}
	static int Get_StageNum() { return m_iStageNum; }
	static int Get_ComNum() { return m_iComNum; }

public:
	static CScene* Create();

private:
	static int m_iStageNum;
	static int m_iComNum;

	CGameObject* m_pCharSlot[4];
};

