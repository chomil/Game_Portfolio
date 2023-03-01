#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	enum OBJECT_TYPE
	{
		DEFAULT, TOOL, INSECT,WEED,PEACH
	};

protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	_float Get_CamDistance() const {
		return m_fCamDistance;
	}

public:
	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar * pComponentTag, class CComponent** ppOut, void* pArg = nullptr);
	class CComponent* Get_ComponentPtr(const _tchar* pComponentTag);
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int Late_Tick(_double TimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth() { return S_OK; }

public:
	OBJECT_TYPE Get_ObjectType()
	{
		return m_eObjectType;
	}
	int Get_ItemIndex()
	{
		return m_iItemIndex;
	}

protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	_float						m_fCamDistance = 0.f;
	
	OBJECT_TYPE m_eObjectType = DEFAULT;
	int m_iItemIndex = -1;

protected:
	map<const _tchar*, class CComponent*>			m_Components;
	typedef map<const _tchar*, class CComponent*>	COMPONENTS;

protected:
	void Compute_CameraDistance(class CTransform* pTransform);
	void Compute_ViewZ(class CTransform* pTransform);

private:
	CComponent* Find_Component(const _tchar* pComponentTag);
public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free();
};

END