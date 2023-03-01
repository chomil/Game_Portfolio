#pragma once
class CMyBitmap;
class CBitmap_Manager
{
public:
	static CBitmap_Manager* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CBitmap_Manager;
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
	static CBitmap_Manager* m_pInstance;
private:
	CBitmap_Manager();
	~CBitmap_Manager();
public:
	HDC FindImage(const TCHAR* pImageKey);
	void Insert_Bitmap(const TCHAR* pPath, const TCHAR* pImageKey);
	void Release_Bitmap();

private:
	map<const TCHAR*, CMyBitmap* > m_mapBmp;
};

