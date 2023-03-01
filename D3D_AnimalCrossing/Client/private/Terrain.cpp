#include "stdafx.h"
#include "..\public\Terrain.h"
#include "GameInstance.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{

}

HRESULT CTerrain::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::NativeConstruct(void * pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;


	return S_OK;
}

_int CTerrain::Tick(_double TimeDelta)
{
	return _int();
}

_int CTerrain::Late_Tick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CTerrain::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	__super::Render();

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	static int wire = 0;
	if (pGameInstance->Key_PressOnce(DIK_F1))
	{
		wire ^= 1;
	}
	m_pShaderCom->Begin_Shader(wire);

	m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureCom_Grass, 0);
	m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture_Sand", m_pTextureCom_Sand, 0);
	m_pShaderCom->SetUp_TextureConstantTable("g_FilterTexture", m_pTextureCom_Filter, 0);
	int iTerrain = 0;
	m_pShaderCom->SetUp_ConstantTable("g_iTerrain",&iTerrain, sizeof(int));
	//m_pShaderCom->Commit();
	m_pVIBufferCom->Render_VIBuffer();

	m_pShaderCom->End_Shader();

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CTerrain::Add_Components()
{
	/* Com_Transform */
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* Com_Renderer*/
	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(CGameObject::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Terrain_Grass"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom_Grass)))
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Terrain_Sand"), TEXT("Com_Texture_Sand"), (CComponent**)&m_pTextureCom_Sand)))
		return E_FAIL;	
	if (FAILED(CGameObject::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Terrain_Filter"), TEXT("Com_Texture_Filter"), (CComponent**)&m_pTextureCom_Filter)))
		return E_FAIL;

	/* Com_VIBuffer*/
	if (FAILED(CGameObject::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_VIBuffer_Terrain"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Shader*/
	if (FAILED(CGameObject::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Terrain"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CTerrain::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	/* 기본 변환. */
	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjMatrix", &pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	/* 빛 정보 */
	D3DLIGHT9	LightDesc = pGameInstance->Get_LightDesc(0);

	m_pShaderCom->SetUp_ConstantTable("g_vLightDir", &_float4(LightDesc.Direction, 0.f), sizeof(_float4));
	m_pShaderCom->SetUp_ConstantTable("g_vLightDiffuse", &LightDesc.Diffuse, sizeof(_float4));
	m_pShaderCom->SetUp_ConstantTable("g_vLightAmbient", &LightDesc.Ambient, sizeof(_float4));
	m_pShaderCom->SetUp_ConstantTable("g_vLightSpecular", &LightDesc.Specular, sizeof(_float4));
	m_pShaderCom->SetUp_ConstantTable("g_vCamPosition", &_float4(pGameInstance->Get_CamPosition(), 1.f), sizeof(_float4));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain*	pInstance = new CTerrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTerrain::Clone(void * pArg)
{
	CTerrain*	pInstance = new CTerrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom_Grass);
	Safe_Release(m_pTextureCom_Sand);
	Safe_Release(m_pTextureCom_Filter);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
