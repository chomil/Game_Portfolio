#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer final : public CBase
{	
private:
	CTimer();
	virtual ~CTimer() = default;
public:
	float Get_DeltaTime() { return m_fDeltaTime;  }
public:
	HRESULT Ready_Time_Manager(); 
	void Update_Time_Manager(); 

private:
	float			m_fDeltaTime; 
	LARGE_INTEGER	m_tCpuTick; 
	LARGE_INTEGER	m_tBegin; 
	LARGE_INTEGER	m_tEnd; 
	LARGE_INTEGER	m_tInvalidate;
public:
	static CTimer* Create();
	virtual void Free();
};

END