#include "stdafx.h"
#include "Weed.h"

#include "GameInstance.h"

CWeed::CWeed(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CWeed::CWeed(const CWeed & rhs)
	: CGameObject(rhs)
{
}

HRESULT CWeed::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CWeed::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	if (FAILED(Add_Components()))
		return E_FAIL;	

	m_eObjectType = OBJECT_TYPE::WEED;
	m_iItemIndex = 10;

	_float3 vPos = *(_float3*)pArg;
	m_pTransformCom->Set_Scale(0.5f, 0.5f, 0.5f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);




	D3DXMatrixIdentity(&m_OriginMatrix);
	_matrix matScale, matRotate, matTrans;
	D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&matRotate, D3DXToRadian(0), D3DXToRadian(0), D3DXToRadian(0));
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

	m_OriginMatrix = m_OriginMatrix * matScale * matRotate * matTrans;


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pPlayerTransformCom = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, L"Layer_Player", TEXT("Com_Transform"), 0);
	m_pParentWorldMatrix = pPlayerTransformCom->Get_WorldMatrix();

	CMesh_Dynamic* pPlayerMeshCom = (CMesh_Dynamic*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, L"Layer_Player", TEXT("Com_Mesh"), 0);
	m_pParentBoneMatrix = pPlayerMeshCom->Get_BoneMatrixPointer("Hand_L");



	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_int CWeed::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	m_pColliderCom->Update_Collider();


	if (m_bPicked == true)
	{
		m_dPickedTime += TimeDelta;

		_matrix matScale;
		float fScale = (1.0 - m_dPickedTime) > 0.5 ? (1.0 - m_dPickedTime) : 0.5;
		D3DXMatrixScaling(&matScale, fScale, fScale, fScale);
		_matrix		WorldMatrix = (m_OriginMatrix * matScale) * (*m_pParentBoneMatrix * *m_pParentWorldMatrix);

		_float3 vRight, vUp, vLook, vPos;
		vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
		vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


		_float3 vLerpRight, vLerpUp, vLerpLook, vLerpPos;
		D3DXVec3Lerp(&vLerpRight, &vRight, (_float3*)&WorldMatrix.m[0][0], 0.4f);
		D3DXVec3Lerp(&vLerpUp, &vUp, (_float3*)&WorldMatrix.m[1][0], 0.4f);
		D3DXVec3Lerp(&vLerpLook, &vLook, (_float3*)&WorldMatrix.m[2][0], 0.4f);
		D3DXVec3Lerp(&vLerpPos, &vPos, (_float3*)&WorldMatrix.m[3][0], 0.4f);


		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vLerpRight);
		m_pTransformCom->Set_State(CTransform::STATE_UP, vLerpUp);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLerpLook);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vLerpPos);
	}

	return _int();
}

_int CWeed::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;

	return _int();
}


HRESULT CWeed::Render()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	__super::Render();
		
	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin_Shader(1);

	_uint iNumMaterials = m_pMeshCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pMeshCom->SetUp_TextureOnShader(m_pShaderCom, "g_DiffuseTexture", MESHMATERIALTEXTURE::TYPE_DIFFUSE, i)))
			return E_FAIL;

		m_pShaderCom->Commit();

		m_pMeshCom->Render(i);
	}

	m_pShaderCom->End_Shader();

#ifdef _DEBUG
	m_pRendererCom->Add_RenderDebug(m_pColliderCom);
#endif // _DEBUG


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


HRESULT CWeed::Add_Components()
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
	if (rand() % 2 == 0)
	{
		if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Weed0"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Weed1"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom)))
			return E_FAIL;
	}


	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Mesh"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;	


	/* Com_Collider*/
	CCollider::COLLIDERDESC		ColliderDesc;

	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	ColliderDesc.vScale = _float3(1.0f, 1.0f, 1.0f);
	ColliderDesc.vRadians = _float3(D3DXToRadian(0), 0.0f, 0.f);
	ColliderDesc.vInitPos = _float3(0.0f, 0.0f, 0.f);
	/* Com_Collider */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_AABB"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeed::SetUp_ConstantTable()
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

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CWeed * CWeed::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWeed*	pInstance = new CWeed(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CWeed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CWeed::Clone(void * pArg)
{
	CWeed*	pInstance = new CWeed(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CWeed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWeed::Free()
{
	__super::Free();


	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pMeshCom);
}
