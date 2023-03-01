#include "stdafx.h"
#include "Transition.h"
#include "GameInstance.h"

CTransition::CTransition(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTransition::CTransition(const CTransition & rhs)
	: CGameObject(rhs)	
{
}


HRESULT CTransition::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();
	return S_OK;
}

HRESULT CTransition::NativeConstruct(void* pArg)
{
	__super::NativeConstruct(pArg);



	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

_int CTransition::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);


	if (m_iTransition == 1)
	{
		m_fRadius += m_fSpeedPerSec * TimeDelta;
	}
	else if (m_iTransition == -1)
	{
		m_fRadius -= m_fSpeedPerSec * TimeDelta;
	}

	if (m_fRadius < 0.f)
	{
		m_fRadius = 0.f;
		m_iTransition = 0;
	}
	if (m_fRadius > 1000.f)
	{
		m_fRadius = 1000.f;
		m_iTransition = 0;
	}

	return _int();
}

_int CTransition::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);	

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CTransition::Render()
{
	__super::Render();	

	_matrix		IdentityMatrix, ScaleMatrix;

	D3DXMatrixScaling(&ScaleMatrix, 2.f, 2.f, 1.f);
	D3DXMatrixIdentity(&IdentityMatrix);

	/* 쉐이더 전역변수로 값을 전달한다. */

	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", &ScaleMatrix, sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &IdentityMatrix, sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjMatrix", &IdentityMatrix, sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_fRadius", &m_fRadius, sizeof(float));
	
	


	m_pShaderCom->Begin_Shader(1);

	m_pVIBufferCom->Render_VIBuffer();		

	m_pShaderCom->End_Shader();

	return S_OK;
}

HRESULT CTransition::Add_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_Rect"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CTransition * CTransition::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransition*	pInstance = new CTransition(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CTransition");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTransition::Clone(void* pArg)
{
	CTransition*	pInstance = new CTransition(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTransition");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTransition::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
