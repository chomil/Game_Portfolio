#pragma once
class CScene;
class CScene_Manager final
{
public:
	enum ID { SCENE_LOADING, SCENE_LOGIN, SCENE_ROOM, SCENE_STAGE, SCENE_TEST, SCENE_MONSTER1, SCENE_MONSTER2, SCENE_BOSS, SCENE_END };
public:
	static CScene_Manager* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CScene_Manager;
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
	static CScene_Manager* m_pInstance;
private:
	CScene_Manager();
	~CScene_Manager();
public:
	void Scene_Change(ID eNextID);
	void Update_Scene_Manager();
	void Render_Scene_Manager(HDC hDC);
	void Release_Scene_Manager();
public:
	CScene* Get_Scene() { return m_pScene; }
	ID Get_SceneID() { return m_eCurScene; }
private:
	CScene* m_pScene;
	ID m_eCurScene;
	ID m_eNextScene;

};