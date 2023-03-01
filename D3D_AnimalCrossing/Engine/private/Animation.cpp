#include "..\public\Animation.h"

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXANIMATIONCONTROLLER pAnimationController)
	: m_pGraphic_Device(pGraphic_Device)
	, m_pAnimationController(pAnimationController)
{
	Safe_AddRef(m_pGraphic_Device);
}

CAnimation::CAnimation(const CAnimation & rhs)
	: m_pGraphic_Device(rhs.m_pGraphic_Device)	
{
	rhs.m_pAnimationController->CloneAnimationController(rhs.m_pAnimationController->GetMaxNumAnimationOutputs(),
		rhs.m_pAnimationController->GetMaxNumAnimationSets(),
		rhs.m_pAnimationController->GetMaxNumTracks(),
		rhs.m_pAnimationController->GetMaxNumEvents(),
		&m_pAnimationController);


	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CAnimation::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CAnimation::NativeConstruct()
{
	return S_OK;
}

HRESULT CAnimation::Set_AnimationIndex(_uint iAnimIndex, bool bLock /*= false*/, bool bForce/*=false*/)
{
	if (bForce == false)
	{
		if (m_iCurrentAnimationIndex == iAnimIndex)
		{
			return S_OK;
		}
		if (m_TimeAcc < 0.2)
		{
			return S_OK;
		}
	}

	//LPCSTR AnimName = "WalkF";
	//if (FAILED(m_pAnimationController->GetAnimationSetByName(AnimName,&pAS)))
	//		return E_FAIL;

	LPD3DXANIMATIONSET		pAS = nullptr;

	
	if (FAILED(m_pAnimationController->GetAnimationSet(m_iCurrentAnimationIndex, &pAS)))
		return E_FAIL;


	if (m_bAnimLock)
	{
		if (m_TimeAcc < pAS->GetPeriod())
		{
			if (bForce == false)
			{
				Safe_Release(pAS);
				return S_OK;
			}
		}
		else
		{
			m_bAnimLock = false;
		}
	}

	if (FAILED(m_pAnimationController->GetAnimationSet(iAnimIndex, &pAS)))
		return E_FAIL;


	m_iNewTrack = m_iCurrentTrack == 0 ? 1 : 0;


	/* 트랙위에 애니메이션 셋을 올려놓는다. */

	m_pAnimationController->SetTrackAnimationSet(m_iNewTrack, pAS);

	/* 애니메이션 하나를 재생하는 지속시간. */
	// pAS->GetPeriod();

	D3DXTRACK_DESC			TrackDesc;

	m_pAnimationController->GetTrackDesc(m_iCurrentTrack, &TrackDesc);

	m_pAnimationController->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAnimationController->UnkeyAllTrackEvents(m_iNewTrack);


	m_pAnimationController->KeyTrackEnable(m_iCurrentTrack, FALSE, m_TimeAcc + 0.2);
	m_pAnimationController->KeyTrackWeight(m_iCurrentTrack, 0.0f, m_TimeAcc, 0.2, D3DXTRANSITION_LINEAR);
	m_pAnimationController->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_TimeAcc, 0.2, D3DXTRANSITION_LINEAR);

	m_pAnimationController->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAnimationController->KeyTrackWeight(m_iNewTrack, 1.0f, m_TimeAcc, 0.2, D3DXTRANSITION_LINEAR);
	m_pAnimationController->KeyTrackSpeed(m_iNewTrack, 1.f, m_TimeAcc, 0.2, D3DXTRANSITION_LINEAR);
	

	m_pAnimationController->SetTrackPosition(m_iNewTrack, 0.0);
	m_pAnimationController->SetTrackSpeed(m_iNewTrack, 1.0f);

	m_pAnimationController->ResetTime();
	m_TimeAcc = 0.0;

	m_iCurrentTrack = m_iNewTrack;

	m_iCurrentAnimationIndex = iAnimIndex;

	m_bAnimLock = bLock;

	Safe_Release(pAS);
	return S_OK;
}

HRESULT CAnimation::Play_Animation(_double TimeDelta)
{
	if (nullptr == m_pAnimationController)
		return E_FAIL;


	m_TimeAcc += TimeDelta;
	m_pAnimationController->AdvanceTime(TimeDelta, nullptr);


	LPD3DXANIMATIONSET		pAS = nullptr;


	if (FAILED(m_pAnimationController->GetAnimationSet(m_iCurrentAnimationIndex, &pAS)))
		return E_FAIL;

	if (m_bAnimLock)
	{
		if (m_TimeAcc > pAS->GetPeriod() - 0.2)
		{
			m_bAnimLock = false;
		}
	}

	Safe_Release(pAS);

	return S_OK;
}


HRESULT CAnimation::Bind_Frames(D3DXFRAME_DERIVED* pRootFrame)
{
	if (m_pAnimationController == nullptr)
	{
		return E_FAIL;
	}
	if (FAILED(D3DXFrameRegisterNamedMatrices(pRootFrame, m_pAnimationController)))
		return E_FAIL;

	

	return S_OK;
}

_double CAnimation::Get_CurrentAnimTime()
{
	LPD3DXANIMATIONSET		pAS = nullptr;


	if (FAILED(m_pAnimationController->GetAnimationSet(m_iCurrentAnimationIndex, &pAS)))
		return E_FAIL;
	double CurrentAnimTime = pAS->GetPeriod();
	Safe_Release(pAS);

	return m_TimeAcc - (CurrentAnimTime * (int)(m_TimeAcc / CurrentAnimTime));
}

CAnimation * CAnimation::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LPD3DXANIMATIONCONTROLLER pAnimationController)
{
	CAnimation*	pInstance = new CAnimation(pGraphic_Device, pAnimationController);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CAnimation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAnimation * CAnimation::Clone()
{
	CAnimation*	pInstance = new CAnimation(*this);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CAnimation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimation::Free()
{
	Safe_Release(m_pAnimationController);
	Safe_Release(m_pGraphic_Device);
}
