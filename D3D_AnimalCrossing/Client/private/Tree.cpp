#include "stdafx.h"
#include "..\public\Tree.h"

#include "GameInstance.h"
#include "Player.h"
#include "Peach.h"

CTree::CTree(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTree::CTree(const CTree & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTree::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CTree::NativeConstruct(void * pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	__super::NativeConstruct(pArg);

	if (FAILED(Add_Components()))
		return E_FAIL;	

	_float3 vPos = *(_float3*)pArg;
	m_pTransformCom->Set_Scale(100, 100, 100);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);


	if (rand() % 2 == 0)
	{
		int iPeachNum = pGameInstance->Get_ObjectNumInLayer(LEVEL_GAMEPLAY0, TEXT("Layer_Peach"));
		vPos.z -= 2.2f;
		vPos.y += 5.f;
		vPos.x -= 2.2f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Peach"), TEXT("Layer_Peach"), &vPos)))
			return E_FAIL;
		m_pPeach[0] = (CPeach*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Peach"), iPeachNum);


		vPos.x += 4.4f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Peach"), TEXT("Layer_Peach"), &vPos)))
			return E_FAIL;
		m_pPeach[1] = (CPeach*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Peach"), iPeachNum+1);

		vPos.z += 1.5f;
		vPos.y += 2.5f;
		vPos.x -= 2.2f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Peach"), TEXT("Layer_Peach"), &vPos)))
			return E_FAIL;
		m_pPeach[2] = (CPeach*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Peach"), iPeachNum + 2);
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CTree::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (m_bShaking)
	{
		m_dShakeTime += TimeDelta;	


		
		if (m_dShakeTime - TimeDelta < 0.2 && m_dShakeTime > 0.2)
		{
			m_pTransformCom->SetUp_Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(0.5f));
		}		
		if (m_dShakeTime - TimeDelta < 0.25 && m_dShakeTime > 0.25)
		{
			m_pTransformCom->SetUp_Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(0.f));
		}
		if (m_dShakeTime - TimeDelta < 0.3 && m_dShakeTime > 0.3)
		{
			m_pTransformCom->SetUp_Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(-0.8f));
		}
		if (m_dShakeTime - TimeDelta < 0.35 && m_dShakeTime > 0.35)
		{
			m_pTransformCom->SetUp_Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(0.0f));
		}
		if (m_dShakeTime - TimeDelta < 0.4 && m_dShakeTime > 0.4)
		{
			m_pTransformCom->SetUp_Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(0.8f));
		}
		if (m_dShakeTime - TimeDelta < 0.45 && m_dShakeTime > 0.45)
		{
			m_pTransformCom->SetUp_Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(0.0f));
		}
		if (m_dShakeTime - TimeDelta < 0.5 && m_dShakeTime > 0.5)
		{
			m_pTransformCom->SetUp_Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(-0.5f));
		}
		if (m_dShakeTime > 0.3)
		{
			if (m_pPeach[0] != nullptr &&
				m_pPeach[1] != nullptr &&
				m_pPeach[2] != nullptr)
			{
				m_pPeach[0]->Falling(0, TimeDelta);
				m_pPeach[1]->Falling(1, TimeDelta);
				m_pPeach[2]->Falling(2, TimeDelta);

				CSound_Manager::Get_Instance()->PlaySound(TEXT("Obj_Fall_Normal.wav"), CSound_Manager::CHANNELID::EFFECT);
				m_pPeach[0] = nullptr;
				m_pPeach[1] = nullptr;
				m_pPeach[2] = nullptr;
			}
		}
		if (m_dShakeTime > 0.6)
		{
			m_pTransformCom->Remove_Rotation();
			m_bShaking = false;

		}
	}


	m_pColliderCom->Update_Collider();


	return _int();
}

_int CTree::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);


	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this)))
		return E_FAIL;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;

	return _int();
}

HRESULT CTree::Render()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//°ïÃæ Àâ¾ÒÀ»¶§ °¡¸®´Â °´Ã¼ ·»´õ ÆÐ½º
	CPlayer* pPlayer = (CPlayer*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), 0);

	if (pPlayer->Get_AnimState() == CPlayer::ANIM_STATE::GET_KEEP || pPlayer->Get_InvenOpen() == true)
	{
		CTransform* pPlayerTrans = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

		_float3 vPlayerPos = pPlayerTrans->Get_State(CTransform::STATE::STATE_POSITION);
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);

		if (abs(vPlayerPos.x - vPos.x)<6 && vPlayerPos.z > vPos.z)
		{
			RELEASE_INSTANCE(CGameInstance);
			return S_OK;
		}
	}
	//
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

HRESULT CTree::Render_ShadowDepth()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* vPlayerTrans = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).z > vPlayerTrans->Get_State(CTransform::STATE_POSITION).z+50)
	{
		RELEASE_INSTANCE(CGameInstance);
		return S_OK;
	}
	//_matrix		WorldMatrix = m_OriginMatrix * (*m_pParentBoneMatrix * *m_pParentWorldMatrix);
	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));


	/* ºû Á¤º¸ */
	D3DLIGHT9	LightDesc = pGameInstance->Get_LightDesc(0);
	_float3 vLightDir = LightDesc.Direction;
	D3DXVec3Normalize(&vLightDir, &vLightDir);


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


void CTree::Shaking()
{
	if (m_bShaking == false)
	{
		m_bShaking = true;
		m_dShakeTime = 0;
		CSound_Manager::Get_Instance()->PlaySound(TEXT("Tree_Shake_Oak.wav"), CSound_Manager::CHANNELID::EFFECT,0.4f);
	}
}

HRESULT CTree::Add_Components()
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
	if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Tree"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom)))
		return E_FAIL;


	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Mesh"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;	


	/* Com_Collider*/
	CCollider::COLLIDERDESC		ColliderDesc;

	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	ColliderDesc.vScale = _float3(3.f, 8.f, 3.f);
	ColliderDesc.vRadians = _float3(D3DXToRadian(0), 0.0f, 0.f);
	ColliderDesc.vInitPos = _float3(0.0f, 4.f, 0.f);

	/* Com_Collider */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_AABB"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CTree::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	/* ±âº» º¯È¯. */
	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &pGameInstance->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->SetUp_ConstantTable("g_ProjMatrix", &pGameInstance->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	/* ºû Á¤º¸ */
	D3DLIGHT9	LightDesc = pGameInstance->Get_LightDesc(0);

	m_pShaderCom->SetUp_ConstantTable("g_vLightDir", &_float4(LightDesc.Direction, 0.f), sizeof(_float4));
	m_pShaderCom->SetUp_ConstantTable("g_vLightDiffuse", &LightDesc.Diffuse, sizeof(_float4));
	m_pShaderCom->SetUp_ConstantTable("g_vLightAmbient", &LightDesc.Ambient, sizeof(_float4));
	m_pShaderCom->SetUp_ConstantTable("g_vLightSpecular", &LightDesc.Specular, sizeof(_float4));
	m_pShaderCom->SetUp_ConstantTable("g_vCamPosition", &_float4(pGameInstance->Get_CamPosition(), 1.f), sizeof(_float4));	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CTree * CTree::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTree*	pInstance = new CTree(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CTree");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTree::Clone(void * pArg)
{
	CTree*	pInstance = new CTree(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTree");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTree::Free()
{
	__super::Free();


	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pMeshCom);
}
