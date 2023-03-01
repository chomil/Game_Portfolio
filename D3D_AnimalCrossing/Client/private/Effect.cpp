#include "stdafx.h"
#include "..\public\Effect.h"

#include "GameInstance.h"

CPointEffect::CPointEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPointEffect::CPointEffect(const CPointEffect& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPointEffect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPointEffect::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	_float3 pPos = *((_float3*)pArg);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pPos);
	m_fFrame = rand() % 4;
	m_fSize = (rand() % 3) / 10.f+0.8f;
	return S_OK;
}

_int CPointEffect::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	m_dTime += TimeDelta;
	m_fAlpha -= m_dTime / 10;
	m_fAlpha = (m_fAlpha > 0) ? m_fAlpha : 0;
	m_fSize += TimeDelta;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_fAlpha == 0)
	{
		pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY0,TEXT("Layer_Smoke"),this);
	}

	RELEASE_INSTANCE(CGameInstance);

	//m_fFrame += 4.f * TimeDelta;
	//if (m_fFrame >= 4.f)
	//	m_fFrame = 0.f;

	return _int();
}

_int CPointEffect::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);



	RELEASE_INSTANCE(CGameInstance);

	Compute_ViewZ(m_pTransformCom);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CPointEffect::Render()
{

	__super::Render();
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjMatrix", &pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));
	m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureCom, _uint(m_fFrame));
	m_pShaderCom->SetUp_ConstantTable("g_fAlpha", &m_fAlpha, sizeof(float));
	m_pShaderCom->SetUp_ConstantTable("g_fSize", &m_fSize, sizeof(float));
	m_pShaderCom->SetUp_ConstantTable("g_vCamPosition", &_float4(pGameInstance->Get_CamPosition(), 1.f), sizeof(_float4));

	D3DLIGHT9 LightDesc = pGameInstance->Get_LightDesc(0);
	m_pShaderCom->SetUp_ConstantTable("g_vLightDiffuse", &(LightDesc.Diffuse), sizeof(_float4));
	m_pShaderCom->Begin_Shader(0);

	m_pVIBufferCom->Render_VIBuffer();

	m_pShaderCom->End_Shader();


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CPointEffect::Add_Components()
{
	/* Com_Transform */
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* Com_Renderer*/
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(CGameObject::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Smoke"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* Com_VIBuffer*/
	if (FAILED(CGameObject::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_VIBuffer_Point"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(CGameObject::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Point"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;




	return S_OK;
}

CPointEffect* CPointEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPointEffect* pInstance = new CPointEffect(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CPointEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPointEffect::Clone(void* pArg)
{
	CPointEffect* pInstance = new CPointEffect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CPointEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPointEffect::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
