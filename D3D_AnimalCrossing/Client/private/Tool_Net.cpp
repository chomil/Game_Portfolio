#include "stdafx.h"
#include "..\public\Tool_Net.h"

#include "GameInstance.h"
#include "Insect.h"
#include "Player.h"
#include "TextBalloon.h"

CTool_Net::CTool_Net(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTool_Net::CTool_Net(const CTool_Net & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTool_Net::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CTool_Net::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	if (FAILED(Add_Components()))
		return E_FAIL;	
	m_eObjectType = OBJECT_TYPE::TOOL;
	m_iItemIndex = 13;


	//m_pMeshCom->Set_AnimationIndex(0);


	/* 초기상태행렬. */
	D3DXMatrixIdentity(&m_OriginMatrix);
	_matrix matScale, matRotate;
	D3DXMatrixScaling(&matScale, 2.6f, 2.6f, 2.6f);
	D3DXMatrixRotationYawPitchRoll(&matRotate, D3DXToRadian(-120), D3DXToRadian(95), D3DXToRadian(-120));

	m_OriginMatrix = m_OriginMatrix * matScale * matRotate;

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*	pPlayerTransformCom = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, L"Layer_Player", TEXT("Com_Transform"), 0);
	m_pParentWorldMatrix = pPlayerTransformCom->Get_WorldMatrix();

	CMesh_Dynamic*	pPlayerMeshCom = (CMesh_Dynamic*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, L"Layer_Player", TEXT("Com_Mesh"), 0);
	m_pParentBoneMatrix = pPlayerMeshCom->Get_BoneMatrixPointer("Hand_R");



	m_pPlayer = (CPlayer*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), 0);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CTool_Net::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);



	return _int();
}

_int CTool_Net::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	_matrix		WorldMatrix = m_OriginMatrix * (*m_pParentBoneMatrix * *m_pParentWorldMatrix);
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&WorldMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&WorldMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&WorldMatrix.m[2][0]);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, *(_float3*)&WorldMatrix.m[3][0]);



	m_pColliderCom->Update_Collider();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	int iInsectNum = pGameInstance->Get_ObjectNumInLayer(LEVEL_GAMEPLAY0, TEXT("Layer_Insect"));
	for (int i = 0; i < iInsectNum; i++)
	{

		CCollider* pTargetCol = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Insect"), TEXT("Com_Collider"), i);

		//잡았을때
		if (m_pColliderCom->Collision_OBB(pTargetCol) && m_pPlayer->Get_AnimState() == CPlayer::ANIM_STATE::NET_SWING)
		{

			CInsect* pInsect = (CInsect*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Insect"), i);
			pInsect->Set_State(CInsect::ANIM_STATE::GET);

			m_pPlayer->Get_Insect(pInsect);
			break;
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this)))
		return E_FAIL;
	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;

	return _int();
}

HRESULT CTool_Net::Render()
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

#ifdef _DEBUG
	m_pRendererCom->Add_RenderDebug(m_pColliderCom);
#endif // _DEBUG

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CTool_Net::Render_ShadowDepth()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix		WorldMatrix = m_OriginMatrix * (*m_pParentBoneMatrix * *m_pParentWorldMatrix);
	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", &WorldMatrix, sizeof(_matrix));


	/* 빛 정보 */
	D3DLIGHT9	LightDesc = pGameInstance->Get_LightDesc(0);
	_float3 vLightDir = LightDesc.Direction;
	D3DXVec3Normalize(&vLightDir, &vLightDir);

	CTransform* vPlayerTrans = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3		vLightAt = vPlayerTrans->Get_State(CTransform::STATE_POSITION);
	_float3		vLightEye = vLightAt - 250 * vLightDir;
	_float3		vLightUp = _float3(0.f, 1.f, 0.f);


	_matrix		LightViewMatrix;

	D3DXMatrixLookAtLH(&LightViewMatrix, &vLightEye, &vLightAt, &vLightUp);

	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &LightViewMatrix, sizeof(_matrix));

	_matrix		LightProjMatrix;

	D3DXMatrixPerspectiveFovLH(&LightProjMatrix, D3DXToRadian(15.f), _float(1280) / _float(720), 0.2f, 300.f);

	m_pShaderCom->SetUp_ConstantTable("g_ProjMatrix", &LightProjMatrix, sizeof(_matrix));

	m_pShaderCom->Begin_Shader(3);

	_uint iNumMaterials = m_pMeshCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pShaderCom->Commit();

		m_pMeshCom->Render(i);
	}

	m_pShaderCom->End_Shader();


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CTool_Net::Add_Components()
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
	if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Net"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom)))
		return E_FAIL;


	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Mesh"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;	


	/* Com_Collider*/
	CCollider::COLLIDERDESC		ColliderDesc;

	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	ColliderDesc.vScale = _float3(1.0f, 1.5f, 1.5f);
	ColliderDesc.vRadians = _float3(D3DXToRadian(0), 0.0f, 0.f);
	ColliderDesc.vInitPos = _float3(0.5f, 0.0f, 3.f);
	/* Com_Collider */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_OBB"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTool_Net::SetUp_ConstantTable()
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

CTool_Net * CTool_Net::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTool_Net*	pInstance = new CTool_Net(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CTool_Net");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTool_Net::Clone(void * pArg)
{
	CTool_Net*	pInstance = new CTool_Net(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTool_Net");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTool_Net::Free()
{
	__super::Free();


	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pMeshCom);
}
