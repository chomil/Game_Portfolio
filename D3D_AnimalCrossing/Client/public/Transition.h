#pragma once

#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CShader;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CTransition final : public CGameObject
{
private:
	explicit CTransition(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTransition(const CTransition& rhs);
	virtual ~CTransition() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Transition(int iTransition, float fSpeedPerSec = 1000.f)
	{
		m_fSpeedPerSec = fSpeedPerSec;
		m_iTransition = iTransition;
	}

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;


private:
	float		m_fSpeedPerSec = 0;
	float		m_fRadius = 0;
	int			m_iTransition = 0; //0:stop 1:open -1:close


private:
	HRESULT Add_Components();

public:
	static CTransition* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END