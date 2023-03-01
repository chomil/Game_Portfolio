
#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;
public:
	class CComponent* Get_ComponentPtr(const _tchar* pComponentTag, _uint iIndex);
	class CGameObject* Get_ObjectPtr( _uint iIndex);

	HRESULT Delete_Object();
	HRESULT Delete_Reserve(class CGameObject* pObject);
	int Get_ObjectNum()
	{
		return m_Objects.size();
	}

public:
	HRESULT NativeConstruct();
	HRESULT Add_GameObject(CGameObject* pGameObject);
	_int Tick(_double TimeDelta);
	_int Late_Tick(_double TimeDelta);

private:
	list<class CGameObject*>		m_Objects;

	list<class CGameObject*>		m_DeleteObjects;
public:
	static CLayer* Create();
	virtual void Free() override;
};

END