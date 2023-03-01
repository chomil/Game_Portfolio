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
	/* ������ */
	CRenderer*				m_pRenderer = nullptr;
	CPicking*				m_pPicking = nullptr;

private:
	/* ��ü�� ������ �����Ѵ�. */
	HRESULT Add_Prototype_GameObject();

	/* ������Ʈ�� �������� �����Ѵ�. */
	HRESULT Add_Prototype_Component();

public:
	static CMainApp* Create();	
	virtual void Free();
};

END



