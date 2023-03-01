#pragma once
class CMainApp
{
public:
	CMainApp();
	~CMainApp();
public:
	int Ready_MainApp();
	void Update_MainApp();
	void Render_MainApp();
	void Release_MainApp();
private:
	HDC m_hDC;
	int m_iFPS;
	DWORD m_dwFPSTime;
	TCHAR m_szFPS[32];
	bool m_bToggle;
};
