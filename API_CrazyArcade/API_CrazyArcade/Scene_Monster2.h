#pragma once
#include "Scene.h"
class CScene_Monster2 :
	public CScene
{
private:
	explicit CScene_Monster2();
public:
	virtual ~CScene_Monster2();
public:
	// CScene을(를) 통해 상속됨
	virtual int Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene(HDC hDC) override;
	virtual void Release_Scene() override;


public:
	static CScene* Create();

private:
	TCHAR* m_szStageName;

	DWORD m_dwStartTimer;
	DWORD m_dwEndTimer;
};