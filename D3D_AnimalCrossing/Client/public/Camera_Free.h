#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Free final : public CCamera
{
private:
	explicit CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Free(const CCamera_Free& rhs);
	virtual ~CCamera_Free() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT Add_Components();
public:
	HRESULT Set_CamPreset(int iCamPreset, bool bForce=false);
	bool  Get_IsCamMoving() { return m_bMoving; }

private:
	void Moving_Cam(_double TimeDelta);
	_float3 Get_PresetPos(int iCamPreset);
	float Get_PresetAngle(int iCamPreset);

private:
	int m_iCurrentCamPreset = 0;
	int m_iPrevCamPreset = 0;
	bool m_bMoving = false;
	double m_dMovingTime = 0;

	_float3 m_vDefaultPos = {};
public:
	static CCamera_Free* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END