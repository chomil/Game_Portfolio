#pragma once
#include "GameObject.h"
class CBlock :
	public CGameObject
{
private:
	CBlock();
public:
	~CBlock();
public:
	// CGameObject을(를) 통해 상속됨
	virtual int Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject() override;

public:
	void Receive_Damage();
	void Set_BlockType(BLOCK_TYPE::TYPE eBlockType) { m_eBlockType = eBlockType; }
	BLOCK_TYPE::TYPE Get_BlockType() { return m_eBlockType; }

	void Set_ItemType();

	void Set_DrawID(int iDrawID) { m_iDrawID = iDrawID; }
	void Move_Block(DIRECTION::DIREC eDir);

private:
	int m_iStageNum;
	int m_iDrawID;
	FRAME m_tFrame;
	TCHAR* m_pFrameKey;

	BLOCK_TYPE::TYPE m_eBlockType;
	ITEM_TYPE::TYPE m_eItemType;

	int m_pushTimer[6][4];
	DIRECTION::DIREC m_eMovingDirec;
	DWORD m_movingTimer;

	bool m_bBushShake;
	int m_iPrevInBush;
	int m_iCurInBush;
	DWORD m_bushTimer;

public:
	static CGameObject* Create(int iDrawID, BLOCK_TYPE::TYPE eType);
};

