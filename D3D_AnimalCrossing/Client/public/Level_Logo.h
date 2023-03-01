#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Logo final : public CLevel
{
private:
	CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Logo() = default;

public:
	virtual HRESULT NativeConstruct() override;
	virtual _int Tick(_double TimeDelta) override;

	/* ���� ������ ��������� ��ü���� �����Լ��� ȣ���Ѵ�.  */
	virtual HRESULT Render() override;	

private: /* ��ü���� �����Ѵ�. */
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
public:
	static CLevel_Logo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END