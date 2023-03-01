#pragma once

/* 객체들을 개발자의 기준에 따라 구분하여 보관하낟. */
/* 렌더순서까지관리한다. (빡셈) */
#include "Base.h"

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;
public:
	class CComponent* Get_ComponentPtr(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex);
	class CGameObject* Get_ObjectPtr(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex);
public:
	HRESULT Reserve_Container(_uint iNumLevel);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg = nullptr);

	HRESULT Delete_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, CGameObject* pObject);
	int Get_ObjectNumInLayer(_uint iLevelIndex, const _tchar* pLayerTag);

	_int Tick(_double TimeDelta);
	_int Late_Tick(_double TimeDelta);
	void Clear(_uint iLevelIndex);

private:
	map<const _tchar*, class CGameObject*>			m_Prototypes;
	typedef map<const _tchar*, class CGameObject*>	PROTOTYPES;
private:
	map<const _tchar*, class CLayer*>*			m_pLayers = nullptr;
	typedef map<const _tchar*, class CLayer*>	LAYERS;

private:
	_uint			m_iNumLevel = 0;

private:
	CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);
public:
	virtual void Free() override;
};

END