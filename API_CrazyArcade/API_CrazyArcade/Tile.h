#pragma once
#include "GameObject.h"

class CTile :
	public CGameObject
{
private:
	CTile();
public:
	virtual ~CTile();
public:
	int Get_DrawID() { return m_iDrawID; }
	int Get_Option() { return m_iOption; }
public:
	void Set_DrawID(int iDrawID) { m_iDrawID = iDrawID; }
	void Set_Option(int iOption) { m_iOption = iOption; }
	void Set_StageNum(int iStageNum) { m_iStageNum = iStageNum; }
	const list<CGameObject*>* Get_ListObjectOnTile() { return &m_pObjectOnTile; }
	bool Is_Pass();
	bool Is_Wall();
public:
	// CGameObject을(를) 통해 상속됨
	virtual int Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject() override;
public:
	static CGameObject* Create();
private:
	int m_iDrawID;
	int m_iOption;
	int m_iStageNum;
	list<CGameObject*> m_pObjectOnTile;
};

