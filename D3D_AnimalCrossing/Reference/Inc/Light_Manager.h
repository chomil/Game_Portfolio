#pragma once

/* 여러개의 빛들을 보관한다. */
#include "Base.h"


BEGIN(Engine)

class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager);
public:
	CLight_Manager();
	virtual ~CLight_Manager() = default;
public:
	D3DLIGHT9 Get_LightDesc(_uint iIndex = 0);
public:
	HRESULT Set_LightDesc(const D3DLIGHT9& LightDesc, _uint iIndex = 0);
public:
	HRESULT Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9& LightDesc);
	HRESULT Render();
private:
	list<class CLight*>				m_Lights;
	typedef list<class CLight*>		LIGHTS;

public:
	virtual void Free() override;
};

END