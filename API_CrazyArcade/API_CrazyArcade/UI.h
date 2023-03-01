#pragma once
#include "GameObject.h"
class CUI :
	public CGameObject
{
private:
	CUI();
public:
	~CUI();
public:
	// CGameObject을(를) 통해 상속됨
	virtual int Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject(HDC hDC) override;
	virtual void Release_GameObject() override;

public:
	void Set_KeyName(const TCHAR* szKeyName) { m_pFrameKey = (TCHAR*)szKeyName; }
	void Set_Size(int iCX, int iCY) { m_tInfo.iCX = iCX; m_tInfo.iCY = iCY; }
	void Set_Close(bool bClose) { m_bClose = bClose; }
public:
	static CGameObject* Create(float fX, float fY, int iCX, int iCY, const TCHAR* szKeyName);

private:
	TCHAR* m_pFrameKey;
	bool m_bClose;
};

