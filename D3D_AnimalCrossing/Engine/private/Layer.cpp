#include "..\public\Layer.h"
#include "GameObject.h"
#include "Component.h"

CLayer::CLayer()
{
}

CComponent * CLayer::Get_ComponentPtr(const _tchar * pComponentTag, _uint iIndex)
{
	if (iIndex >= m_Objects.size())
		return nullptr;

	auto	iter = m_Objects.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;	

	return (*iter)->Get_ComponentPtr(pComponentTag);	
}

CGameObject* CLayer::Get_ObjectPtr(_uint iIndex)
{
	if (iIndex >= m_Objects.size())
		return nullptr;

	auto	iter = m_Objects.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter);
}

HRESULT CLayer::Delete_Object()
{
	for (CGameObject* pDeleteObject : m_DeleteObjects)
	{
		m_Objects.remove(pDeleteObject);
		Safe_Release(pDeleteObject);
	}
	m_DeleteObjects.clear();

	return S_OK;
}

HRESULT CLayer::Delete_Reserve(CGameObject* pObject)
{
	m_DeleteObjects.emplace_back(pObject);

	return S_OK;
}



HRESULT CLayer::NativeConstruct()
{
	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_Objects.push_back(pGameObject);

	return S_OK;
}

_int CLayer::Tick(_double TimeDelta)
{
	for (auto& pGameObject : m_Objects)
		pGameObject->Tick(TimeDelta);

	return _int();
}

_int CLayer::Late_Tick(_double TimeDelta)
{
	for (auto& pGameObject : m_Objects)
		pGameObject->Late_Tick(TimeDelta);

	return _int();
}

CLayer * CLayer::Create()
{
	CLayer*	pInstance = new CLayer;

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLayer");
		Safe_Release(pInstance);
	}


	return pInstance;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_Objects)
		Safe_Release(pGameObject);

	m_Objects.clear();
}
