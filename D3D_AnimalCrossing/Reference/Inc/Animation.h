#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	explicit CAnimation(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXANIMATIONCONTROLLER pAnimationController);
	explicit CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;
public:
	HRESULT NativeConstruct_Prototype();
	HRESULT NativeConstruct();
	HRESULT Set_AnimationIndex(_uint iAnimIndex, bool bLock = false, bool bForce = false);
	HRESULT Play_Animation(_double TimeDelta);
	HRESULT Bind_Frames(D3DXFRAME_DERIVED* pRootFrame);
	bool	Is_AnimLock() { return m_bAnimLock; }
	_double Get_CurrentAnimTime();
	

private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	LPD3DXANIMATIONCONTROLLER	m_pAnimationController = nullptr;
	_uint						m_iCurrentAnimationIndex = 0;

	_uint						m_iCurrentTrack = 0;
	_uint						m_iNewTrack = 1;
	_double						m_TimeAcc = 0.0;
	bool						m_bAnimLock = false;
public:
	static CAnimation* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXANIMATIONCONTROLLER pAnimationController);
	virtual CAnimation* Clone();
	virtual void Free() override;
};

END