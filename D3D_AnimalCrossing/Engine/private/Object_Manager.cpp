#include "..\public\Object_Manager.h"
#include "Layer.h"
#include "Component.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}


CComponent * CObject_Manager::Get_ComponentPtr(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	CLayer*	pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_ComponentPtr(pComponentTag, iIndex);

	
}

CGameObject* CObject_Manager::Get_ObjectPtr(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_ObjectPtr(iIndex);
}

HRESULT CObject_Manager::Reserve_Container(_uint iNumLevel)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_pLayers = new LAYERS[iNumLevel];

	m_iNumLevel = iNumLevel;

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == pPrototype ||
		nullptr != Find_Prototype(pPrototypeTag))
		return E_FAIL;

	m_Prototypes.emplace(pPrototypeTag, pPrototype);

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pLayerTag, void* pArg)
{
	if (m_iNumLevel <= iLevelIndex)
		return E_FAIL;

	CGameObject*	pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject*	pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);

	/* 내가 복제한 객체를 추가하고자하는 레이어가 없는 경우*/
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	/* 내가 복제한 객체를 추가하고자하는 레이어가 있는 경우*/
	else
		pLayer->Add_GameObject(pGameObject);

	return S_OK;
}

HRESULT CObject_Manager::Delete_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, CGameObject* pObject)
{
	if (m_iNumLevel <= iLevelIndex)
		return E_FAIL;

	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr != pLayer)
	{
		return pLayer->Delete_Reserve(pObject);
	}
	else
		return E_FAIL;
}

int CObject_Manager::Get_ObjectNumInLayer(_uint iLevelIndex, const _tchar* pLayerTag)
{
	if (m_iNumLevel <= iLevelIndex)
		return 0;

	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr != pLayer)
	{
		return pLayer->Get_ObjectNum();
	}
	else
		return 0;
}

_int CObject_Manager::Tick(_double TimeDelta)
{
	for (size_t i = 0; i < m_iNumLevel; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Tick(TimeDelta);
	}

	//삭제할 오브젝트들 삭제
	for (size_t i = 0; i < m_iNumLevel; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Delete_Object();
	}


	return _int();
}

_int CObject_Manager::Late_Tick(_double TimeDelta)
{
	for (size_t i = 0; i < m_iNumLevel; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Late_Tick(TimeDelta);
	}


	return _int();
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	for (auto& Pair : m_pLayers[iLevelIndex])
		Safe_Release(Pair.second);

	m_pLayers[iLevelIndex].clear();
}

CGameObject * CObject_Manager::Find_Prototype(const _tchar * pPrototypeTag)
{
	auto	iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTagFinder(pPrototypeTag));

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;		
}

CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (m_iNumLevel <= iLevelIndex)
		return nullptr;

	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	
	for (size_t i = 0; i < m_iNumLevel; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}

	Safe_Delete_Array(m_pLayers);

	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);

	m_Prototypes.clear();


	
}
