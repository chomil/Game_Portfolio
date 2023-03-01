#pragma once

#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Timer_Manager.h"
#include "PipeLine.h"
#include "Input_Device.h"
#include "Light_Manager.h"


BEGIN(Engine)


class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(HINSTANCE hInst, HWND hWnd, _uint iNumLevel);
	_int Tick(_double TimeDelta);
	_int LateTick(_double TimeDelta);
	void Clear(_uint iLevelIndex);


#pragma region GRAPHIC_DEVICE
public:
	HRESULT Ready_Graphic_Device(HWND hWnd, CGraphic_Device::WINMODE eMode, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppGraphic_Device = nullptr);
	void Render_Begin();
	void Render_End(HWND hWND = nullptr);


	LPD3DXFONT Get_Font();

	HRESULT Set_Font(D3DXFONT_DESCW * pFont_Desc);
#pragma endregion


#pragma region LEVEL_MANAGER
public:
	HRESULT SetUp_Level(class CLevel* pLevel);
	HRESULT Render_Level();
#pragma endregion

#pragma region OBJECT_MANAGER
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg = nullptr);
	CComponent* Get_ComponentPtr(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex = 0);
	CGameObject* Get_ObjectPtr(_uint iLevelIndex, const _tchar * pLayerTag, _uint iIndex = 0);

	HRESULT Delete_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, CGameObject * pObject);
	int Get_ObjectNumInLayer(_uint iLevelIndex, const _tchar * pLayerTag);
#pragma endregion

#pragma region COMPONENT_MANAGER
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);
#pragma endregion


#pragma region TIMER_MANAGER
	HRESULT Add_Timers(const _tchar* pTimerTag);
	_float Compute_TimeDelta(const _tchar* pTimerTag);
#pragma endregion

#pragma region PIPELINE
	_matrix Get_Transform(D3DTRANSFORMSTATETYPE eStateType);
	_float3 Get_CamPosition();
#pragma endregion

#pragma region INPUT_DEVICE
	_byte Get_DIKeyState(_byte byKey);
	_byte Get_DIMouseKeyState(CInput_Device::DIM eMouseKey);
	_long Get_DIMouseMoveState(CInput_Device::DIMM eMouseMove);
	bool Key_PressOnce(_byte byKey);
	bool Key_Pressing(_byte byKey);
	bool Key_ReleaseOnce(_byte byKey);
	bool Key_Releasing(_byte byKey);
#pragma endregion

#pragma region LIGHT_MANAGER
	D3DLIGHT9 Get_LightDesc(_uint iIndex = 0);
	HRESULT Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9& LightDesc);

	HRESULT Set_LightDesc(const D3DLIGHT9& LightDesc, _uint iIndex = 0);
#pragma endregion
public:
	static void Release_Engine();

private:
	CGraphic_Device*		m_pGraphic_Device = nullptr;
	CLevel_Manager*			m_pLevel_Manager = nullptr;
	CObject_Manager*		m_pObject_Manager = nullptr;
	CComponent_Manager*		m_pComponent_Manager = nullptr;
	CTimer_Manager*			m_pTimer_Manager = nullptr;
	CPipeLine*				m_pPipeLine = nullptr;
	CInput_Device*			m_pInput_Device = nullptr;
	CLight_Manager*			m_pLight_Manager = nullptr;

public:
	virtual void Free() override;
};

END