#pragma once

class CGameObject;
class CGameObject_Manager
{
public:

public:
	static CGameObject_Manager* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CGameObject_Manager;
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
	static CGameObject_Manager* m_pInstance;

private:
	CGameObject_Manager();
	~CGameObject_Manager();
public:
	CGameObject* Get_Player()const { return m_listGameObject[OBJECT::PLAYER].front(); }
	list<CGameObject*>* Get_ListObject(OBJECT::ID eType) { return &m_listGameObject[eType]; }
public:
	void Add_GameObject(OBJECT::ID eID, CGameObject* pObject);
	void Delete_GameObject(OBJECT::ID eID);
public:
	void Update_GameObject_Manager();
	void Render_GameObject_Manager(HDC hDC);
	void Release_GameObject_Manager();
private:
	list<CGameObject*> m_listGameObject[OBJECT::ID::OBJECT_END];
	list<CGameObject*> m_listRenderObject[OBJECT::RENDER_END];
};