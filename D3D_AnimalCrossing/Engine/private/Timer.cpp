#include "Timer.h"

CTimer::CTimer()
	:m_fDeltaTime(0.f)
	, m_tCpuTick({})
	, m_tBegin({})
	, m_tEnd({})
{
}

HRESULT CTimer::Ready_Time_Manager()
{
	QueryPerformanceFrequency(&m_tCpuTick);
	QueryPerformanceCounter(&m_tBegin);
	QueryPerformanceCounter(&m_tEnd);
	QueryPerformanceCounter(&m_tInvalidate);

	return S_OK;
}

void CTimer::Update_Time_Manager()
{
	QueryPerformanceCounter(&m_tEnd);

	if ((m_tEnd.QuadPart - m_tInvalidate.QuadPart) > m_tCpuTick.QuadPart)
	{
		m_tInvalidate = m_tEnd;
		QueryPerformanceFrequency(&m_tCpuTick);
	}


	m_fDeltaTime = float(m_tEnd.QuadPart - m_tBegin.QuadPart) / m_tCpuTick.QuadPart; 

	m_tBegin = m_tEnd; 
}

CTimer * CTimer::Create()
{
	CTimer*	pInstance = new CTimer();

	if (FAILED(pInstance->Ready_Time_Manager()))
	{
		MSGBOX("Failed to Creating CTimer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTimer::Free()
{
}
