#pragma once

#include "Base.h"

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)


private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT SetUp_Level(class CLevel* pLevel);
	_int Tick(_double TimeDelta);
	HRESULT Render();

private:
	class CLevel*			m_pCurrentLevel = nullptr;

public:
	virtual void Free() override;
};

END