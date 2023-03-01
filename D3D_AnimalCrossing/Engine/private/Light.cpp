#include "..\public\Light.h"
#include "Shader.h"
#include "PipeLine.h"
#include "Target_Manager.h"
#include "VIBuffer_Rect_Viewport.h"


CLight::CLight(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CLight::Render()
{
	CTarget_Manager* pTargetManager = GET_INSTANCE(CTarget_Manager);
	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	_uint			iPassIndex = 0;

	if (D3DLIGHT_DIRECTIONAL == m_LightDesc.Type)
	{
		iPassIndex = 1;
		m_pShader->SetUp_ConstantTable("g_vLightDir", &_float4(m_LightDesc.Direction, 0.f), sizeof(_float4));
	}
	else if (D3DLIGHT_POINT == m_LightDesc.Type)
	{
		iPassIndex = 2;
		m_pShader->SetUp_ConstantTable("g_vLightPos", &_float4(m_LightDesc.Position, 1.f), sizeof(_float4));
		m_pShader->SetUp_ConstantTable("g_fRange", &m_LightDesc.Range, sizeof(_float));
	}

	/* 빛정보를 던진다. */
	m_pShader->SetUp_ConstantTable("g_vLightDiffuse", &m_LightDesc.Diffuse, sizeof(_float4));
	m_pShader->SetUp_ConstantTable("g_vLightAmbient", &m_LightDesc.Ambient, sizeof(_float4));
	m_pShader->SetUp_ConstantTable("g_vLightSpecular", &m_LightDesc.Specular, sizeof(_float4));
	m_pShader->SetUp_ConstantTable("g_vCamPosition", &_float4(pPipeLine->Get_CamPosition(), 1.f), sizeof(_float4));


	_matrix		ViewMatrix = pPipeLine->Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	m_pShader->SetUp_ConstantTable("g_ViewMatrixInv", &ViewMatrix, sizeof(_matrix));

	_matrix		ProjMatrix = pPipeLine->Get_Transform(D3DTS_PROJECTION);
	D3DXMatrixInverse(&ProjMatrix, nullptr, &ProjMatrix);
	m_pShader->SetUp_ConstantTable("g_ProjMatrixInv", &ProjMatrix, sizeof(_matrix));

	/* 노멀 렌더타겟텍스쳘르 던진다. */
	if (FAILED(m_pShader->SetUp_TextureConstantTable("g_NormalTexture", pTargetManager->Get_TargetTexture(TEXT("Target_Normal")))))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_TextureConstantTable("g_DepthTexture", pTargetManager->Get_TargetTexture(TEXT("Target_Depth")))))
		return E_FAIL;

	m_pShader->Begin_Shader(iPassIndex);

	/* Shade에 그리기위한 버퍼다. */
	m_pVIBuffer->Render_VIBuffer();

	m_pShader->End_Shader();

	RELEASE_INSTANCE(CPipeLine);

	RELEASE_INSTANCE(CTarget_Manager);

	return S_OK;
}

HRESULT CLight::NativeConstruct(const D3DLIGHT9 & LightDesc)
{
	m_LightDesc = LightDesc;

	D3DVIEWPORT9		Viewport;
	m_pGraphic_Device->GetViewport(&Viewport);

	m_pVIBuffer = CVIBuffer_Rect_Viewport::Create(m_pGraphic_Device, -0.5f, -0.5f, Viewport.Width, Viewport.Height);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"));
	if (nullptr == m_pShader)
		return E_FAIL;

	return S_OK;
}

CLight * CLight::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9 & LightDesc)
{
	CLight*	pInstance = new CLight(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(LightDesc)))
	{
		MSGBOX("Failed to Creating CLight");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLight::Free()
{
	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);

	Safe_Release(m_pGraphic_Device);
}
