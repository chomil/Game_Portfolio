#include "pch.h"
#include "GameObject_Manager.h"
#include "GameObject.h"

CGameObject_Manager* CGameObject_Manager::m_pInstance = nullptr;

CGameObject_Manager::CGameObject_Manager()
{
}


CGameObject_Manager::~CGameObject_Manager()
{
	Release_GameObject_Manager();
}


void CGameObject_Manager::Add_GameObject(OBJECT::ID eID, CGameObject* pObject)
{
	m_listGameObject[eID].emplace_back(pObject);
	pObject->Set_ObjectType(eID);
}

void CGameObject_Manager::Delete_GameObject(OBJECT::ID eID)
{
	for (auto& pGameObject : m_listGameObject[eID])
		Safe_Delete(pGameObject);

	m_listGameObject[eID].clear();
}

void CGameObject_Manager::Update_GameObject_Manager()
{
	for (int i = 0; i < OBJECT::OBJECT_END; ++i)
	{
		for (auto iter = m_listGameObject[i].begin(); iter != m_listGameObject[i].end(); )
		{
			int iEvent = (*iter)->Update_GameObject();

			if (OBJ_DEAD == iEvent)
			{
				Safe_Delete(*iter);
				iter = m_listGameObject[i].erase(iter);
			}
			else
				++iter;
		}

		for (auto& pObject : m_listGameObject[i])
		{
			pObject->Late_Update_GameObject();
			if (m_listGameObject[i].empty())
				break;

			OBJECT::RENDER_LIST eRenderList = pObject->Get_RenderGroup();
			m_listRenderObject[eRenderList].emplace_back(pObject);
		}

	}

}

void CGameObject_Manager::Render_GameObject_Manager(HDC hDC)
{
	for (int i = 0; i < OBJECT::RENDER_END; ++i)
	{
		m_listRenderObject[i].sort([](auto& pDst, auto& pSrc)
			{
				return pDst->Get_Info()->fY < pSrc->Get_Info()->fY;
			});
	}
	for (int i = 0; i < OBJECT::RENDER_END; ++i)
	{
		for (auto& pObject : m_listRenderObject[i])
			pObject->Render_GameObject(hDC);

		m_listRenderObject[i].clear();
	}
}

void CGameObject_Manager::Release_GameObject_Manager()
{
	for (int i = 0; i < OBJECT::OBJECT_END; ++i)
	{
		for (auto& pObject : m_listGameObject[i])
			Safe_Delete(pObject);

		m_listGameObject[i].clear();
	}
}
