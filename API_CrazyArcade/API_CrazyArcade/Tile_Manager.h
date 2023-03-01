#pragma once
#include "Tile.h"

class CGameObject;


class CTile_Manager
{
public:
	static CTile_Manager* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CTile_Manager;
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
	static CTile_Manager* m_pInstance;
private:
	CTile_Manager();
	CTile_Manager(const CTile_Manager& rObj) = delete;
	void operator=(const CTile_Manager& rhs) = delete;
	~CTile_Manager();
public:
	void Load_Data_Tile_Manager(const TCHAR* pMapPath);
	void Load_Data_Block_Manager(const TCHAR* pMapPath);
	CGameObject* Pick_Tile(int iX, int iY);
	const vector<CGameObject*>* Get_Tiles() { return &m_vecTile; }
public:
	int Update_Tile_Manager();
	void Render_Tile_Manager(HDC hDC);
	void Release_Tile_Manager();
private:
	vector<CGameObject*> m_vecTile;
};

