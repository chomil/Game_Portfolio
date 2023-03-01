#include "stdafx.h"
#include "..\public\BackGround.h"
#include "GameInstance.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CBackGround::CBackGround(const CBackGround & rhs)
	: CGameObject(rhs)	
{
}



HRESULT CBackGround::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	
	

	return S_OK;
}

HRESULT CBackGround::NativeConstruct(void* pArg)
{
	__super::NativeConstruct(pArg);



	if (FAILED(Add_Components()))
		return E_FAIL;


	return S_OK;
}

_int CBackGround::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	
	return _int();
}

_int CBackGround::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);	

	/*if()
		렌더러에 추가할께*/
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

	return _int();
}

HRESULT CBackGround::Render()
{
	__super::Render();	

	_matrix		IdentityMatrix, ScaleMatrix;

	D3DXMatrixScaling(&ScaleMatrix, 2.f, 2.f, 1.f);
	D3DXMatrixIdentity(&IdentityMatrix);

	/* 쉐이더 전역변수로 값을 전달한다. */

	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", &ScaleMatrix, sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &IdentityMatrix, sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjMatrix", &IdentityMatrix, sizeof(_matrix));
	m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureCom, 0);
	

	m_pShaderCom->Begin_Shader(0);

	m_pVIBufferCom->Render_VIBuffer();		

	m_pShaderCom->End_Shader();

	return S_OK;
}

HRESULT CBackGround::Add_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Transform*/
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Texture_Default"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;	

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_Rect"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CBackGround * CBackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackGround*	pInstance = new CBackGround(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CBackGround");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBackGround::Clone(void* pArg)
{
	CBackGround*	pInstance = new CBackGround(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CBackGround");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
}
