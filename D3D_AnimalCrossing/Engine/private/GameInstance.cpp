#include "..\public\GameInstance.h"

#include "Graphic_Device.h"
#include "Level_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::Get_Instance())
	, m_pLevel_Manager(CLevel_Manager::Get_Instance())
	, m_pObject_Manager(CObject_Manager::Get_Instance())
	, m_pComponent_Manager(CComponent_Manager::Get_Instance())
	, m_pTimer_Manager(CTimer_Manager::Get_Instance())
	, m_pPipeLine(CPipeLine::Get_Instance())
	, m_pInput_Device(CInput_Device::Get_Instance())
	, m_pLight_Manager(CLight_Manager::Get_Instance())
{

	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pGraphic_Device);

}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, HWND hWnd, _uint iNumLevel)
{
	if (nullptr == m_pObject_Manager || 
		nullptr == m_pComponent_Manager || 
		nullptr == m_pInput_Device)
		return E_FAIL;

	if (FAILED(m_pInput_Device->NativeConstruct(hInst, hWnd)))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Reserve_Container(iNumLevel)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Reserve_Container(iNumLevel)))
		return E_FAIL;
	
	return S_OK;
}

_int CGameInstance::Tick(_double TimeDelta)
{
	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pObject_Manager || 
		nullptr == m_pInput_Device)
		return -1;

	m_pInput_Device->Invalidate_Input_Device();

	m_pLevel_Manager->Tick(TimeDelta);

	m_pObject_Manager->Tick(TimeDelta);
	

	return _int();
}

_int CGameInstance::LateTick(_double TimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return -1;

	_int iProgress = m_pObject_Manager->Late_Tick(TimeDelta);

	return iProgress;		

	return _int();

}

void CGameInstance::Clear(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return;

	m_pObject_Manager->Clear(iLevelIndex);
	m_pComponent_Manager->Clear(iLevelIndex);
}


#pragma region GRAPHIC_DEVICE






HRESULT CGameInstance::Ready_Graphic_Device(HWND hWnd, CGraphic_Device::WINMODE eMode, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppGraphic_Device)
{
	if (nullptr != m_pGraphic_Device)
		return m_pGraphic_Device->Ready_Graphic_Device(hWnd, eMode, iWinCX, iWinCY, ppGraphic_Device);

	return E_FAIL;
}

void CGameInstance::Render_Begin()
{
	if (nullptr != m_pGraphic_Device)
		m_pGraphic_Device->Render_Begin();
}

void CGameInstance::Render_End(HWND hWND)
{
	if (nullptr != m_pGraphic_Device)
		m_pGraphic_Device->Render_End(hWND);
}


LPD3DXFONT CGameInstance::Get_Font()
{
	if (nullptr != m_pGraphic_Device)
		return m_pGraphic_Device->Get_Font();
	return nullptr;
}

HRESULT CGameInstance::Set_Font(D3DXFONT_DESCW* pFont_Desc)
{
	if (nullptr != m_pGraphic_Device)
	{
		m_pGraphic_Device->Set_Font(pFont_Desc);
		return S_OK;
	}
	return E_FAIL;
}

#pragma endregion

#pragma region LEVEL_MANAGER
HRESULT CGameInstance::SetUp_Level(CLevel * pLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->SetUp_Level(pLevel);
}

HRESULT CGameInstance::Render_Level()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Render();
}

#pragma endregion

#pragma region OBJECT_MANAGER

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pLayerTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject(iLevelIndex, pPrototypeTag, pLayerTag, pArg);
}

CComponent * CGameInstance::Get_ComponentPtr(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_ComponentPtr(iLevelIndex, pLayerTag, pComponentTag, iIndex);	
}

CGameObject* CGameInstance::Get_ObjectPtr(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_ObjectPtr(iLevelIndex, pLayerTag,iIndex);
}

HRESULT CGameInstance::Delete_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, CGameObject* pObject)
{
	if (nullptr == m_pObject_Manager || nullptr == pObject)
		return E_FAIL;

	return m_pObject_Manager->Delete_GameObject(iLevelIndex, pLayerTag, pObject);
}

int CGameInstance::Get_ObjectNumInLayer(_uint iLevelIndex, const _tchar* pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return 0;

	return m_pObject_Manager->Get_ObjectNumInLayer(iLevelIndex, pLayerTag);
}

#pragma endregion

#pragma region COMPONENT_MANAGER

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}


#pragma endregion

#pragma region TIMER_MANAGER

HRESULT CGameInstance::Add_Timers(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timers(pTimerTag);
}

_float CGameInstance::Compute_TimeDelta(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0f;

	return m_pTimer_Manager->Compute_TimeDelta(pTimerTag);
}

#pragma endregion

_matrix CGameInstance::Get_Transform(D3DTRANSFORMSTATETYPE eStateType)
{
	if (nullptr == m_pPipeLine)
		return _matrix();

	return m_pPipeLine->Get_Transform(eStateType);
}

_float3 CGameInstance::Get_CamPosition()
{
	if (nullptr == m_pPipeLine)
		return _float3();

	return m_pPipeLine->Get_CamPosition();
}

#pragma region INPUT_DEVICE

_byte CGameInstance::Get_DIKeyState(_byte byKey)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKeyState(byKey);
}

_byte CGameInstance::Get_DIMouseKeyState(CInput_Device::DIM eMouseKey)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseKeyState(eMouseKey);
}

_long CGameInstance::Get_DIMouseMoveState(CInput_Device::DIMM eMouseMove)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseMoveState(eMouseMove);
}

bool CGameInstance::Key_PressOnce(_byte byKey)
{
	return m_pInput_Device->Key_PressOnce(byKey);
}

bool CGameInstance::Key_Pressing(_byte byKey)
{
	return m_pInput_Device->Key_Pressing(byKey);
}

bool CGameInstance::Key_ReleaseOnce(_byte byKey)
{
	return m_pInput_Device->Key_ReleaseOnce(byKey);
}

bool CGameInstance::Key_Releasing(_byte byKey)
{
	return m_pInput_Device->Key_Releasing(byKey);
}

#pragma endregion

#pragma region LIGHT_MANAGER

D3DLIGHT9 CGameInstance::Get_LightDesc(_uint iIndex)
{
	if (nullptr == m_pLight_Manager)
		return D3DLIGHT9();

	return m_pLight_Manager->Get_LightDesc(iIndex);	
}

HRESULT CGameInstance::Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9& LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(pGraphic_Device, LightDesc);
}

HRESULT CGameInstance::Set_LightDesc(const D3DLIGHT9& LightDesc, _uint iIndex)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Set_LightDesc(LightDesc, iIndex);
}

#pragma endregion

void CGameInstance::Release_Engine()
{	
	/* ㅍ ㅡㄹ레이어를 만들었다. 오브젲거트 매니져에 보간하낟. */
	/* 레벨을 만들었다. 레벨매니져에보관하낟. */
	/* 원형객체를 만들었다. 원형객체 맨니져지;러ㅏㅣㅇ*/


	/* 싱글톤객체름ㄹ ㅏㄴ들었다. (GetInstance()) : 자ㅑ기자신이 보관하낟. */

	
	/*엔진내에서 정의된 모든 싱글톤 객체를 삭제한다. */
	if (0 != CGameInstance::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed to Release CGameInstance");

	if (0 != CLevel_Manager::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed to Release CLevel_Manager");

	if (0 != CObject_Manager::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed to Release CObject_Manager");
		
	if (0 != CComponent_Manager::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed to Release CComponent_Manager");

	if (0 != CPipeLine::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed to Release CPipeLine");

	if (0 != CTimer_Manager::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed to Release CTimer_Manager");

	if (0 != CLight_Manager::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed to Release CLight_Manager");

	if (0 != CGraphic_Device::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed to Release CGraphic_Device");

	if (0 != CInput_Device::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed to Release CInput_Device");



}

void CGameInstance::Free()
{
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pTimer_Manager);	
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pGraphic_Device);
}
