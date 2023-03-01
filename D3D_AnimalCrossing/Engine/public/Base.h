#pragma once

/* 모든 클래스의 부모 클래스다. */
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;
public:
	unsigned long AddRef();
	unsigned long Release();
private:
	/* 현재 객체의 참조 갯수를 저장 */
	unsigned long	m_dwRefCnt = 0;

public:
	virtual void Free() = 0;
};

END