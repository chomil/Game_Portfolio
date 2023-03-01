#include "..\public\GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "GameInstance.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphic_Device(rhs.m_pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar * pComponentTag, CComponent** ppOut, void* pArg)
{
	// if (m_Components.end() != find_if(m_Components.begin(), m_Components.end(), CTagFinder(pPrototypeTag)))
	if(nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CComponent*		pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);

	m_Components.emplace(pComponentTag, pComponent);

	Safe_AddRef(pComponent);

	*ppOut = pComponent;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CComponent * CGameObject::Get_ComponentPtr(const _tchar * pComponentTag)
{
	return Find_Component(pComponentTag);		
}

HRESULT CGameObject::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::NativeConstruct(void* pArg)
{
	return S_OK;
}

_int CGameObject::Tick(_double TimeDelta)
{
	return _int();
}

_int CGameObject::Late_Tick(_double TimeDelta)
{
	return _int();
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

void CGameObject::Compute_CameraDistance(CTransform* pTransform)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	_float3	vWorldPosition = pTransform->Get_State(CTransform::STATE_POSITION);

	_matrix	ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);

	_matrix	CamWorldMatrix = *D3DXMatrixInverse(&CamWorldMatrix, nullptr, &ViewMatrix);

	_float3	vCamPosition = *(_float3*)&CamWorldMatrix.m[3][0];

	m_fCamDistance = D3DXVec3Length(&(vWorldPosition - vCamPosition));

	RELEASE_INSTANCE(CGameInstance);
}

void CGameObject::Compute_ViewZ(CTransform * pTransform)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	_float3	vWorldPosition = pTransform->Get_State(CTransform::STATE_POSITION);

	_matrix	ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);

	D3DXVec3TransformCoord(&vWorldPosition, &vWorldPosition, &ViewMatrix);

	m_fCamDistance = vWorldPosition.z;

	RELEASE_INSTANCE(CGameInstance);
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTagFinder(pComponentTag));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);

	m_Components.clear();

	Safe_Release(m_pGraphic_Device);
}
