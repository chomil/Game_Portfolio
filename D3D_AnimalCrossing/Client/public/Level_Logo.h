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

	/* 현재 씬에서 보여줘야할 객체들의 렌더함수를 호출한다.  */
	virtual HRESULT Render() override;	

private: /* 객체들을 생성한다. */
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
public:
	static CLevel_Logo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END