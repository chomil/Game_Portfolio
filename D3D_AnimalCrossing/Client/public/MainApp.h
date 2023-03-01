#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CPicking;
class CCollider;
END

BEGIN(Client)

class CMainApp final : public CBase
{

public:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT NativeConstruct();
	_int Tick(double TimeDelta);
	HRESULT Render_MainApp();	

private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	/* 렌더러 */
	CRenderer*				m_pRenderer = nullptr;
	CPicking*				m_pPicking = nullptr;

private:
	/* 객체의 원형을 생성한다. */
	HRESULT Add_Prototype_GameObject();

	/* 컴포넌트의 원형을ㅇ 생성한다. */
	HRESULT Add_Prototype_Component();

public:
	static CMainApp* Create();	
	virtual void Free();
};

END



