#pragma once

class CKey_Manager
{
public:
	static CKey_Manager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CKey_Manager;

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
private:
	CKey_Manager();
	~CKey_Manager();
private:
	static CKey_Manager* m_pInstance;
public:
	void Key_Update();
	bool Key_Up(int iKey);
	bool Key_Down(int iKey);
	bool Key_Released(int iKey);
	bool Key_Pressed(int iKey);
	
private:
	char m_eKeyState[256];
};