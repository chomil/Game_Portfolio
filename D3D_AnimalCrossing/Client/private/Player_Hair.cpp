#include "stdafx.h"
#include "..\public\Player_Hair.h"

#include "GameInstance.h"

CPlayer_Hair::CPlayer_Hair(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPlayer_Hair::CPlayer_Hair(const CPlayer_Hair & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer_Hair::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CPlayer_Hair::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	if (FAILED(Add_Components()))
		return E_FAIL;	


	/* 초기상태행렬. */
	D3DXMatrixIdentity(&m_OriginMatrix);
	_matrix matScale, matRotate;
	D3DXMatrixScaling(&matScale, 2.6f, 2.6f, 2.6f);
	D3DXMatrixRotationYawPitchRoll(&matRotate, D3DXToRadian(-90), D3DXToRadian(0), D3DXToRadian(180));

	m_OriginMatrix = m_OriginMatrix * matScale *matRotate;

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*	pPlayerTransformCom = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, L"Layer_Player", TEXT("Com_Transform"), 0);
	m_pParentWorldMatrix = pPlayerTransformCom->Get_WorldMatrix();

	CMesh_Dynamic*	pPlayerMeshCom = (CMesh_Dynamic*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, L"Layer_Player", TEXT("Com_Mesh"), 0);
	m_pParentBoneMatrix = pPlayerMeshCom->Get_BoneMatrixPointer("Head");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CPlayer_Hair::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);



	return _int();
}

_int CPlayer_Hair::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	_matrix		WorldMatrix = m_OriginMatrix * (*m_pParentBoneMatrix * *m_pParentWorldMatrix);
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&WorldMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&WorldMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&WorldMatrix.m[2][0]);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, *(_float3*)&WorldMatrix.m[3][0]);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;

	return _int();
}

HRESULT CPlayer_Hair::Render()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	__super::Render();
		
	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin_Shader(0);

	_uint iNumMaterials = m_pMeshCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pMeshCom->SetUp_TextureOnShader(m_pShaderCom, "g_DiffuseTexture", MESHMATERIALTEXTURE::TYPE_DIFFUSE, i)))
			return E_FAIL;

		m_pShaderCom->Commit();

		m_pMeshCom->Render(i);
	}

	m_pShaderCom->End_Shader();

	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer_Hair::Add_Components()
{
	/* Com_Renderer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);


	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_Mesh */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Player_Hair"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Player"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;	

	return S_OK;
}

HRESULT CPlayer_Hair::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


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

	m_pShaderCom->SetUp_ConstantTable("g_vCustomColor", &_float4(35 / 255.f, 33 / 255.f, 33 / 255.f, 1), sizeof(_float4));


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CPlayer_Hair * CPlayer_Hair::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer_Hair*	pInstance = new CPlayer_Hair(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CPlayer_Hair");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayer_Hair::Clone(void * pArg)
{
	CPlayer_Hair*	pInstance = new CPlayer_Hair(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CPlayer_Hair");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Hair::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pMeshCom);
}
