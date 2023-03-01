#include "stdafx.h"
#include "Insect.h"

#include "GameInstance.h"
#include "Player.h"
#include "Quest.h"

CInsect::CInsect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CInsect::CInsect(const CInsect& rhs)
	: CGameObject(rhs)
{
}

HRESULT CInsect::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CInsect::NativeConstruct(void* pArg)
{
	__super::NativeConstruct(pArg);

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eObjectType = INSECT;

	m_tagInsectDesc = *(INSECT_DESC*)pArg;
	m_iItemIndex = m_tagInsectDesc.iInsectIndex;

	//인덱스에 따른 메쉬 초기화
	/* Com_Mesh */
	switch (m_tagInsectDesc.iInsectIndex)
	{
	case 0:
		if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_실잠자리"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom)))
			return E_FAIL;
		break;
	case 1:
		if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_왕잠자리"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom)))
			return E_FAIL;
		break;
	case 2:
		if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_배추흰나비"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom)))
			return E_FAIL;
		break;
	case 3:
		if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_호랑나비"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom)))
			return E_FAIL;
		break;
	case 4:
		if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_몰포나비"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom)))
			return E_FAIL;
		break;
	case 5:
		if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_꿀벌"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom)))
			return E_FAIL;
		break;
	case 6:
		if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_사마귀"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom)))
			return E_FAIL;
		break;
	case 7:
		if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_무당벌레"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom)))
			return E_FAIL;
		break;
	case 8:
		if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_인면노린재"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom)))
			return E_FAIL;
		break;
	case 9:
		if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Insect_아틀라스나방"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom)))
			return E_FAIL;
		break;
	default:
		break;
	}

	_float3 vPos = m_tagInsectDesc.vSpawnPos;
	m_pTransformCom->Set_Scale(0.3f, 0.3f, 0.3f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	if (m_tagInsectDesc.iInsectIndex == 9)
	{
		m_pTransformCom->SetUp_Rotation(_float3(1, 0, 0), D3DXToRadian(-90));
	}
	else
	{
		m_pTransformCom->SetUp_Rotation(_float3(0, 1, 0), D3DXToRadian(rand() % 360));
	}



	D3DXMatrixIdentity(&m_OriginMatrix);
	_matrix matScale, matRotate, matTrans;
	D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&matRotate, D3DXToRadian(0), D3DXToRadian(0), D3DXToRadian(-90));
	D3DXMatrixTranslation(&matTrans, 1.0, 0, 0);

	m_OriginMatrix = m_OriginMatrix * matScale * matRotate * matTrans;




	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pPlayerTransformCom = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, L"Layer_Player", TEXT("Com_Transform"), 0);
	m_pParentWorldMatrix = pPlayerTransformCom->Get_WorldMatrix();

	CMesh_Dynamic* pPlayerMeshCom = (CMesh_Dynamic*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, L"Layer_Player", TEXT("Com_Mesh"), 0);
	m_pParentBoneMatrix = pPlayerMeshCom->Get_BoneMatrixPointer("Hand_L");


	m_eState = IDLE;


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

_int CInsect::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_LifeTime += TimeDelta;
	if (m_eState == ANIM_STATE::GET)
	{
		m_GetTime += TimeDelta;
	}
	else if (m_eState == ANIM_STATE::FLY)
	{
		m_FlyTime += TimeDelta;
	}
	else if (m_eState == ANIM_STATE::IDLE)
	{
		m_IdleTime += TimeDelta;
	}


	CPlayer* pPlayer = (CPlayer*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), 0);
	CTransform* pPlayerTrans = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);
	_float3 vPlayerPos = pPlayerTrans->Get_State(CTransform::STATE::STATE_POSITION);
	_float3 vInsectPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
	_float3 vDir = vInsectPos - vPlayerPos;
	vDir.y = 0;
	float fDist = D3DXVec3Length(&vDir);
	//거리와 도망칠 방향

	switch (m_tagInsectDesc.iInsectIndex)
	{
	case 0: //실잠자리
	case 1: //왕잠자리
		//잠자리류
		if (m_eState == ANIM_STATE::GET)
		{
		}
		else if (m_eState == ANIM_STATE::FLY)
		{
			m_pTransformCom->LookAt_Target(m_vTargetDir, TimeDelta);
			m_pTransformCom->Go_Straight(TimeDelta);

			if (m_FlyTime > m_ChangeTime)
			{
				m_ChangeTime = (rand() % 11) / 10.f + 1.0;
				m_eState = ANIM_STATE::IDLE;
				m_IdleTime = 0;
			}
		}
		else if (m_eState == ANIM_STATE::IDLE)
		{

			if (m_IdleTime > m_ChangeTime)
			{
				m_ChangeTime = (rand() % 11) / 10.f + 1.0;
				if (fDist < 5.f)
				{
					m_vTargetDir = vDir;
				}
				else
				{
					m_vTargetDir = _float3{ (float)(rand() % 200 - 100),0,(float)(rand() % 200 - 100) };
				}
				m_eState = ANIM_STATE::FLY;
				m_FlyTime = 0;
			}
			if (fDist < 5.f)
			{
				m_pTransformCom->LookAt_Target(vDir, TimeDelta / 10.f);
			}
		}
		break;


	case 2: //배추흰나비
	case 3: //호랑나비
	case 4: //몰포나비
	case 5: //꿀벌
		//나비류

		if (m_eState == ANIM_STATE::GET)
		{
		}
		else if (m_eState == ANIM_STATE::FLY)
		{
			m_pTransformCom->LookAt_Target(m_vTargetDir, TimeDelta / 5.f);
			m_pTransformCom->Go_Straight(TimeDelta / 2.f);

			if (m_FlyTime > m_ChangeTime)
			{
				m_ChangeTime = 0;
				m_eState = ANIM_STATE::IDLE;
				m_IdleTime = 0;
			}
		}
		else if (m_eState == ANIM_STATE::IDLE)
		{
			if (m_IdleTime > m_ChangeTime)
			{
				m_ChangeTime = (rand() % 51) / 100.f + 0.5f;
				if (fDist < 5.f)
				{
					m_vTargetDir = vDir;
				}
				else
				{
					m_vTargetDir = _float3{ (float)(rand() % 200 - 100),0,(float)(rand() % 200 - 100) };
				}
				m_eState = ANIM_STATE::FLY;
				m_FlyTime = 0;
			}
			if (fDist < 5.f)
			{
				m_pTransformCom->LookAt_Target(vDir, TimeDelta / 10.f);
			}

		}
		break;


	case 6: //사마귀
	case 7: //무당벌레
	case 8: //인면노린재
		//꽃 스폰 류

		if (m_eState == ANIM_STATE::GET)
		{
		}
		else if (m_eState == ANIM_STATE::FLY)
		{
			m_pTransformCom->LookAt_Target(m_vTargetDir, TimeDelta);
			m_pTransformCom->Move_Direction(m_vTargetDir, TimeDelta);
			if (m_FlyTime > 2.f)
			{
				pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY0, TEXT("Layer_Insect"), this);
			}
		}
		else if (m_eState == ANIM_STATE::IDLE)
		{
			bool bFly = false;
			if (fDist < 5.f)
			{
				switch (pPlayer->Get_AnimState())
				{
				case CPlayer::ANIM_STATE::DASH:
				case CPlayer::ANIM_STATE::NET_DASH:
				case CPlayer::ANIM_STATE::NET_GROUNDSTOP:
				case CPlayer::ANIM_STATE::SHAKE:
				case CPlayer::ANIM_STATE::NET_SHAKE:
					bFly = true;
					break;
				default:
					break;
				}
				if (fDist < 2.f)
				{
					bFly = true;
				}
			}
			if (bFly == true)
			{
				if (m_tagInsectDesc.iInsectIndex==7) //무당벌레
				{
					CSound_Manager::Get_Instance()->PlaySound(TEXT("Insect_Tentou_Escape.wav"), CSound_Manager::CHANNELID::INSECT);
				}
				else
				{
					CSound_Manager::Get_Instance()->PlaySound(TEXT("Insect_BeetleS_Escape00.wav"), CSound_Manager::CHANNELID::INSECT);
				}
				D3DXVec3Normalize(&vDir, &vDir);
				m_vTargetDir = vDir;
				m_vTargetDir.y = 1;
				m_pTransformCom->LookAt_Target(vDir, TimeDelta);
				m_eState = ANIM_STATE::FLY;
				m_bRunAway = true;
				m_FlyTime = 0;
			}
		}
		break;

	case 9: //아틀라스나방
		//나무 스폰 류
		if (m_eState == ANIM_STATE::GET)
		{
		}
		else if (m_eState == ANIM_STATE::FLY)
		{
			_float3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
			m_pTransformCom->Rotation_Axis(vRight, TimeDelta / 5);
			m_pTransformCom->LookAt_Target(m_vTargetDir, TimeDelta);
			m_pTransformCom->Move_Direction(m_vTargetDir, TimeDelta);
			if (m_FlyTime > 2.f)
			{
				pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY0, TEXT("Layer_Insect"), this);
			}
		}
		else if (m_eState == ANIM_STATE::IDLE)
		{
			bool bFly = false;
			if (fDist < 5.f)
			{
				switch (pPlayer->Get_AnimState())
				{
				case CPlayer::ANIM_STATE::DASH:
				case CPlayer::ANIM_STATE::NET_DASH:
				case CPlayer::ANIM_STATE::NET_GROUNDSTOP:
				case CPlayer::ANIM_STATE::SHAKE:
				case CPlayer::ANIM_STATE::NET_SHAKE:
					bFly = true;
					break;
				default:
					break;
				}

				if (fDist < 2.f)
				{
					bFly = true;
				}
			}
			if (bFly == true)
			{
				CSound_Manager::Get_Instance()->PlaySound(TEXT("Insect_Yonagunisan_Escape00.wav"), CSound_Manager::CHANNELID::INSECT);

				D3DXVec3Normalize(&vDir, &vDir);
				m_vTargetDir = vDir;
				m_vTargetDir.y = 1;
				m_pTransformCom->LookAt_Target(-vDir, TimeDelta);
				m_eState = ANIM_STATE::FLY;
				m_bRunAway = true;
				m_FlyTime = 0;
			}
		}
		break;
	default:
		break;
	}




	//일정 시간이 지나거나 맵 밖으로 나간 곤충 삭제
	if ((m_LifeTime > 600.f && m_eState != ANIM_STATE::GET) ||
		(vInsectPos.x < 0 || vInsectPos.z < 0 || vInsectPos.x >256 || vInsectPos.z >256))
	{
		pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY0, TEXT("Layer_Insect"), this);
	}


	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CInsect::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);


	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this)))
		return E_FAIL;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_eState == ANIM_STATE::GET)
	{
		_matrix		WorldMatrix = m_OriginMatrix * (*m_pParentBoneMatrix * *m_pParentWorldMatrix);
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&WorldMatrix.m[0][0]);
		m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&WorldMatrix.m[1][0]);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&WorldMatrix.m[2][0]);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, *(_float3*)&WorldMatrix.m[3][0]);
	}

	m_pMeshCom->Set_AnimationIndex(m_eState);
	m_pMeshCom->Play_Animation(TimeDelta);

	m_pColliderCom->Update_Collider();

	RELEASE_INSTANCE(CGameInstance);
	return _int();
}

HRESULT CInsect::Render()
{
	if (m_eState == ANIM_STATE::GET && m_GetTime < 1.0f)
	{
		return S_OK;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	__super::Render();

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin_Shader(1);

	_uint iNumMeshContainers = m_pMeshCom->Get_NumMeshContainer();


	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		_uint iNumMaterials = m_pMeshCom->Get_NumMaterials(i);

		m_pMeshCom->Update_SkinnedMesh(i);

		for (_uint j = 0; j < iNumMaterials; ++j)
		{
			if (FAILED(m_pMeshCom->SetUp_TextureOnShader(m_pShaderCom, "g_DiffuseTexture", MESHMATERIALTEXTURE::TYPE_DIFFUSE, i, j)))
				return E_FAIL;

			m_pShaderCom->Commit();

			m_pMeshCom->Render(i, j);
		}
	}

	m_pShaderCom->End_Shader();


#ifdef _DEBUG
	m_pRendererCom->Add_RenderDebug(m_pColliderCom);
#endif // _DEBUG


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CInsect::Set_State(ANIM_STATE eState)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (m_eState != eState && eState == ANIM_STATE::GET)
	{
		if (m_eObjectType == OBJECT_TYPE::INSECT)
		{
			//잡았을때 퀘스트 갱신
			int iQuestIndex = m_iItemIndex + 0;
			CQuest* pQuest = (CQuest*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Quest"), 0);
			pQuest->Clear_Quest(iQuestIndex, 1);
			pQuest->Clear_Quest(10, 1);
		}
	}
	RELEASE_INSTANCE(CGameInstance);

	m_eState = eState;

}

HRESULT CInsect::Render_ShadowDepth()
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_eState == ANIM_STATE::GET)
	{
		_matrix		WorldMatrix = m_OriginMatrix * (*m_pParentBoneMatrix * *m_pParentWorldMatrix);
		m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", &WorldMatrix, sizeof(_matrix));
	}
	else
	{
		m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));

	}


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

	_uint iNumMeshContainers = m_pMeshCom->Get_NumMeshContainer();
	for (size_t i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pMeshCom->Update_SkinnedMesh(i)))
			break;

		_uint iNumMaterials = m_pMeshCom->Get_NumMaterials(i);
		for (size_t j = 0; j < iNumMaterials; ++j)
		{
			m_pShaderCom->Commit();

			m_pMeshCom->Render(i, j);
		}
	}

	m_pShaderCom->End_Shader();


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CInsect::Add_Components()
{
	/* Com_Renderer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 7.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(360.f);


	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Mesh"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_Collider*/
	CCollider::COLLIDERDESC		ColliderDesc;

	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRadians = _float3(D3DXToRadian(0), 0.0f, 0.f);
	ColliderDesc.vInitPos = _float3(0.0f, 0.0f, 0.f);

	/* Com_Collider */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_OBB"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInsect::SetUp_ConstantTable()
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

CInsect* CInsect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CInsect* pInstance = new CInsect(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CInsect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CInsect::Clone(void* pArg)
{
	CInsect* pInstance = new CInsect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CInsect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CInsect::Free()
{
	__super::Free();

	Safe_Release(m_pMeshCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
