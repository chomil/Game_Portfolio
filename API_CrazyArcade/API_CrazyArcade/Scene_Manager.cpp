#include "pch.h"
#include "Scene_Manager.h"

#include "Scene_Stage.h"
#include "Scene_Monster1.h"
#include "Scene_Monster2.h"
#include "Scene_Boss.h"
#include "Scene_Room.h"
#include "Scene_Login.h"
#include "Scene_Test.h"

CScene_Manager* CScene_Manager::m_pInstance = nullptr;

CScene_Manager::CScene_Manager()
	:m_eCurScene(SCENE_END)
	, m_eNextScene(SCENE_END)
	, m_pScene(nullptr)
{
}


CScene_Manager::~CScene_Manager()
{
	Release_Scene_Manager();
}

void CScene_Manager::Scene_Change(ID eNextID)
{
	//FSM 유한 상태 기계 
	m_eNextScene = eNextID;

	if (m_eCurScene != m_eNextScene)
	{
		Safe_Delete(m_pScene);
		
		switch (m_eNextScene)
		{
		case CScene_Manager::SCENE_LOADING:
			break;		
		
		case CScene_Manager::SCENE_LOGIN:
			m_pScene = CScene_Login::Create();
			break;

		case CScene_Manager::SCENE_ROOM:
			m_pScene = CScene_Room::Create();
			break;

		case CScene_Manager::SCENE_STAGE:
			m_pScene = CScene_Stage::Create();
			break;
		case CScene_Manager::SCENE_MONSTER1:
			m_pScene = CScene_Monster1::Create();
			break;
		case CScene_Manager::SCENE_MONSTER2:
			m_pScene = CScene_Monster2::Create();
			break;
		case CScene_Manager::SCENE_BOSS:
			m_pScene = CScene_Boss::Create();
			break;

		case CScene_Manager::SCENE_TEST:
			m_pScene = CScene_Test::Create();
			break;
		default:
			break;
		}
		m_eCurScene = m_eNextScene;
	}
}

void CScene_Manager::Update_Scene_Manager()
{
	m_pScene->Update_Scene();
}

void CScene_Manager::Render_Scene_Manager(HDC hDC)
{
	m_pScene->Render_Scene(hDC);
}

void CScene_Manager::Release_Scene_Manager()
{
	Safe_Delete(m_pScene);
}
