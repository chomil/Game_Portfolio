#pragma once
#include "GameObject.h"
class CItem :
	public CGameObject
{
private:
	CItem();
public:
	~CItem();

public:
	void MoveFrame();

public:
	// CGameObject을(를) 통해 상속됨
	virtual int Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject() override;

public:
	ITEM_TYPE::TYPE Get_ItemType() { return m_eItemType; }
	void Set_ItemType(ITEM_TYPE::TYPE eItemType) 
	{
		m_eItemType = eItemType;
		m_tFrame.iFrameState = m_eItemType;
	}
	void Receive_Damage() { m_bIsDead = true; }

public:
	static CGameObject* Create(int iX, int iY, ITEM_TYPE::TYPE eItemType);

private:
	FRAME m_tFrame;
	TCHAR* m_pFrameKey;

	int m_iBottom;
	int m_iFloatDirec;
	DWORD m_floatTimer;
	ITEM_TYPE::TYPE m_eItemType;
};

