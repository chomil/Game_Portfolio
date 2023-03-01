#include "stdafx.h"
#include "..\public\Sky.h"

#include "GameInstance.h"

CSky::CSky(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CSky::CSky(const CSky & rhs)
	: CGameObject(rhs)
{

}

HRESULT CSky::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();


	return S_OK;
}

HRESULT CSky::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	if (FAILED(Add_Components()))
		return E_FAIL;

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, -1.f));


	return S_OK;
}

_int CSky::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	m_pTransformCom->Rotation_Axis(_float3(0.f, 1.f, 0.f), TimeDelta*0.01);

	return _int();
}

_int CSky::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	/* 카메라 월드행렬의 역행렬. */
	_matrix		ViewMatrix = pGameInstance->Get_Transform(D3DTS_VIEW);

	/* 카메라 월드행렬의 역행렬의 역행렬을 구하면 = 카메라 월드 행렬.  */
	_matrix		CamWorldMatrix = *D3DXMatrixInverse(&CamWorldMatrix, nullptr, &ViewMatrix);
	_float3		CamPos = *(_float3*)&CamWorldMatrix.m[3][0];
	CamPos.y -= 0.2f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, CamPos);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}
HRESULT CSky::Render()
{

	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	__super::Render();

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin_Shader(0);

	m_pShaderCom->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTextureCom, 2);

	m_pVIBufferCom->Render_VIBuffer();

	m_pShaderCom->End_Shader();

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CSky::Add_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Transform*/
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 2.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Texture_Sky"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Sky"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CSky::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	/* 기본 변환. */
	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));

	_matrix projMat;
	D3DXMatrixPerspectiveFovLH(&projMat, D3DXToRadian(80), (float)g_nWinCX / (float)g_nWinCY, 0.03f, 400.f);
	//m_pShaderCom->SetUp_ConstantTable("g_ProjMatrix", &pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjMatrix", &projMat, sizeof(_matrix));

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

CSky * CSky::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSky*	pInstance = new CSky(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CSky");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSky::Clone(void * pArg)
{
	CSky*	pInstance = new CSky(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CSky");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSky::Free()
{
	CGameObject::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);

}
