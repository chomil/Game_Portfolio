#include "stdafx.h"
#include "..\public\Player.h"

#include "GameInstance.h"
#include "Inventory.h"
#include "Camera_Free.h"
#include "Item.h"
#include "TextBalloon.h"
#include "Quest.h"
#include "Tree.h"
#include "Weed.h"
#include "Peach.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void* pArg)
{
	__super::NativeConstruct(pArg);

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(0.3f, 0.3f, 0.3f);
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, _float3(120.f, 0.f, 80.f));
	m_pTransformCom->SetUp_Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
	m_eState = IDLE;


	ZeroMemory(m_pItems, sizeof(CItem*) * 20);
	return S_OK;
}

_int CPlayer::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (m_bStop)
	{
		return S_OK;
	}


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	CCamera_Free* pCam = (CCamera_Free*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Camera"), 0);

	//애니메이션 끝나면 다음으로 넘어갈 애니메이션들
	if (m_eState == NET_SWING && !(m_pMeshCom->Is_AnimLock()))
	{ 
		m_eState = NET_GROUNDSTOP;
		m_pMeshCom->Set_AnimationIndex(m_eState,true);
		//CSound_Manager::Get_Instance()->PlaySound(TEXT("Pl_NetHitGround_Grass_00.wav"), CSound_Manager::CHANNELID::TOOL);

		RELEASE_INSTANCE(CGameInstance);
		return S_OK;
	}

	if (m_eState == PICK_UP)
	{
		if (!(m_pMeshCom->Is_AnimLock()))
		{
			m_eState = PUT_AWAY;
			m_pMeshCom->Set_AnimationIndex(m_eState, true, true);

			RELEASE_INSTANCE(CGameInstance);
			return S_OK;
		}
	}
	if ( m_eState == NET_PICK_UP)
	{
		if (!(m_pMeshCom->Is_AnimLock()))
		{
			m_eState = NET_PUT_AWAY;
			m_pMeshCom->Set_AnimationIndex(m_eState, true, true);

			RELEASE_INSTANCE(CGameInstance);
			return S_OK;
		}
	}

	if (m_eState == PUT_AWAY || m_eState == NET_PUT_AWAY)
	{
		if (!(m_pMeshCom->Is_AnimLock()))
		{
			if (m_pPickObjectPtr != nullptr)
			{

				if (m_pPickObjectPtr->Get_ObjectType() == OBJECT_TYPE::INSECT)
				{
					pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY0, TEXT("Layer_Insect"), m_pPickObjectPtr);


					for (int i = 0; i < 20; i++)
					{
						if (m_pItems[i] == nullptr)
						{
							//아이템 얻을때
							CItem::ITEM_DESC item_desc;
							item_desc.m_iItemIndex = m_pPickObjectPtr->Get_ItemIndex();
							item_desc.m_iInvenIndex = i;
							pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Item"), TEXT("Layer_Item"), &item_desc);

							int iItemNum = pGameInstance->Get_ObjectNumInLayer(LEVEL_GAMEPLAY0, TEXT("Layer_Item"));
							m_pItems[i] = (CItem*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Item"), iItemNum - 1);
							
							break;
						}
					}
				}
				else if (m_pPickObjectPtr->Get_ObjectType() == OBJECT_TYPE::PEACH)
				{
					pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY0, TEXT("Layer_Peach"), m_pPickObjectPtr);


					for (int i = 0; i < 20; i++)
					{
						if (m_pItems[i] != nullptr)
						{
							if (m_pItems[i]->Get_ItemIndex() == m_pPickObjectPtr->Get_ItemIndex())
							{
								if (m_pItems[i]->Get_StackNum() < 10)
								{
									m_pItems[i]->Add_StackNum(1);
									m_pPickObjectPtr = nullptr;
									break;
								}
							}
						}
						if (m_pItems[i] == nullptr)
						{
							//아이템 얻을때
							CItem::ITEM_DESC item_desc;
							item_desc.m_iItemIndex = m_pPickObjectPtr->Get_ItemIndex();
							item_desc.m_iInvenIndex = i;
							pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Item"), TEXT("Layer_Item"), &item_desc);

							int iItemNum = pGameInstance->Get_ObjectNumInLayer(LEVEL_GAMEPLAY0, TEXT("Layer_Item"));
							m_pItems[i] = (CItem*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Item"), iItemNum - 1);
							
							break;
						}
					}
				}
				else if (m_pPickObjectPtr->Get_ObjectType() == OBJECT_TYPE::WEED)
				{
					pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY0, TEXT("Layer_Weed"), m_pPickObjectPtr);


					for (int i = 0; i < 20; i++)
					{
						if (m_pItems[i] != nullptr)
						{
							if (m_pItems[i]->Get_ItemIndex() == m_pPickObjectPtr->Get_ItemIndex())
							{
								if (m_pItems[i]->Get_StackNum() < 99)
								{
									m_pItems[i]->Add_StackNum(1);
									m_pPickObjectPtr = nullptr;
									break;
								}
							}
						}
						if (m_pItems[i] == nullptr)
						{
							//아이템 얻을때
							CItem::ITEM_DESC item_desc;
							item_desc.m_iItemIndex = m_pPickObjectPtr->Get_ItemIndex();
							item_desc.m_iInvenIndex = i;
							pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Item"), TEXT("Layer_Item"), &item_desc);

							int iItemNum = pGameInstance->Get_ObjectNumInLayer(LEVEL_GAMEPLAY0, TEXT("Layer_Item"));
							m_pItems[i] = (CItem*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Item"), iItemNum - 1);
							
							break;
						}
					}
				}
				m_pPickObjectPtr = nullptr;
			}
		}
	}


	if (m_eState == GET_KEEP)
	{
		m_eState = GET_KEEP;
		m_pMeshCom->Set_AnimationIndex(m_eState, true);

		m_pTransformCom->LookAt_Target(_float3(0.f, 0.f, -1.f), TimeDelta * 2);

		if (m_bStop == false)
		{
			HRESULT resultCam = pCam->Set_CamPreset(0);
			if (resultCam == S_OK)
			{
				m_eState = NET_PUT_AWAY;
				m_pMeshCom->Set_AnimationIndex(m_eState, true, true);

				CSound_Manager::Get_Instance()->PlaySound(TEXT("Pl_ItemPutIn_00.wav"), CSound_Manager::CHANNELID::PLAYER_EFFECT);

			}
		}
		RELEASE_INSTANCE(CGameInstance);
		return S_OK;
	}


	if (m_eState == NET_GET)
	{
		if (!(m_pMeshCom->Is_AnimLock()))
		{
			m_eState = GET_KEEP;
			m_pMeshCom->Set_AnimationIndex(m_eState, true);

			int iTextIndex = 0;
			if (m_pPickObjectPtr != nullptr)
			{
				
				if (m_pPickObjectPtr->Get_ObjectType() == OBJECT_TYPE::INSECT)
				{
					//잡았을때 텍스트 띄우기
					iTextIndex = m_pPickObjectPtr->Get_ItemIndex();
					CTextBalloon* pTextBallon = (CTextBalloon*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_TextBalloon"), 0);
					pTextBallon->Set_ShowBalloon(true, iTextIndex, false);


					CSound_Manager::Get_Instance()->PlaySound(TEXT("BGM_Demo_GetFanfare.wav"),CSound_Manager::CHANNELID::EVENT_BGM);
				}
			}
		}

		m_pTransformCom->LookAt_Target(_float3(0.f, 0.f, -1.f), TimeDelta );
		
		HRESULT resultCam = pCam->Set_CamPreset(2,true);

		RELEASE_INSTANCE(CGameInstance);
		return S_OK;
	}


	//인벤토리
	if (m_eState == ANIM_STATE::IDLE || m_eState == ANIM_STATE::NET_IDLE)
	{
		CInventory* pInven = (CInventory*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Inven"), 0);

		if (m_bInvenOpen)
		{
			if (pGameInstance->Key_PressOnce(DIK_I))
			{
				HRESULT resultCam = pCam->Set_CamPreset(0);
				if (resultCam == S_OK)
				{
					m_bInvenOpen = false;
					pInven->Set_InvenOpen(false);
				}
			}
			RELEASE_INSTANCE(CGameInstance);
			return S_OK;
		}
		else
		{
			if (pGameInstance->Key_PressOnce(DIK_I))
			{
				HRESULT resultCam = pCam->Set_CamPreset(1);
				if (resultCam == S_OK)
				{
					m_bInvenOpen = true;
					pInven->Set_InvenOpen(true);

					m_pMeshCom->Set_AnimationIndex(m_eState, false, true);


					RELEASE_INSTANCE(CGameInstance);
					return S_OK;
				}
			}
		}
	}

	//이동 및 IDLE
	if(!(m_pMeshCom->Is_AnimLock()))
	{
		_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK);
		_float3 fDir = {};
		D3DXVec3Normalize(&vLook, &vLook);

		//스페이스바
		if (pGameInstance->Key_PressOnce(DIK_SPACE))
		{
			int iNum = pGameInstance->Get_ObjectNumInLayer(LEVEL_GAMEPLAY0, TEXT("Layer_Tree"));
			for (int i = 0; i < iNum; i++)
			{
				CCollider* pTargetCol = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Tree"), TEXT("Com_Collider"), i);

				//잡았을때
				if (m_pPickUpColliderCom->Collision_AABB(pTargetCol))
				{
					m_eState = NET_SHAKE;
					m_pMeshCom->Set_AnimationIndex(m_eState, true, true);

					CTree* pTree = (CTree*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Tree"), i);
					pTree->Shaking();
					RELEASE_INSTANCE(CGameInstance);


					return S_OK;
				}
			}
			//게시판 콜라이더
			CCollider* pCol = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Building"), TEXT("Com_Collider"), 0);
			if (m_pPickUpColliderCom->Collision_AABB(pCol))
			{
				CQuest* pQuest = (CQuest*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Quest"));
				pQuest->Set_QuestOpen(true);
				m_bStop = true;
				m_eState = IDLE;
			}
			else
			{
				m_eState = NET_SWING;
				m_pMeshCom->Set_AnimationIndex(m_eState, true, true);


				CSound_Manager::Get_Instance()->PlaySound(TEXT("Pl_Net_Swing00.wav"), CSound_Manager::CHANNELID::TOOL);
			}



		}
		else if (pGameInstance->Key_PressOnce(DIK_E)) //줍기
		{
			int iNumWeed = pGameInstance->Get_ObjectNumInLayer(LEVEL_GAMEPLAY0, TEXT("Layer_Weed"));
			for (int i = 0; i < iNumWeed; i++)
			{
				CCollider* pTargetCol = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Weed"), TEXT("Com_Collider"), i);

				//잡았을때
				if (m_pPickUpColliderCom->Collision_AABB(pTargetCol))
				{
					m_eState = PICK_UP;

					if (m_pPickObjectPtr == nullptr)
					{
						CSound_Manager::Get_Instance()->PlaySound(TEXT("Pl_Pickup_02.wav"), CSound_Manager::CHANNELID::PLAYER_EFFECT);
						CSound_Manager::Get_Instance()->PlaySound(TEXT("Pl_ItemPutIn_00.wav"), CSound_Manager::CHANNELID::PLAYER_EFFECT);

						m_pPickObjectPtr = pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Weed"), i);
						((CWeed*)m_pPickObjectPtr)->Set_Picked(true);

						CQuest* pQuest = (CQuest *)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Quest"), 0);
						pQuest->Clear_Quest(11, 1);
					}
					break;
				}
			}

			int iNumPeach = pGameInstance->Get_ObjectNumInLayer(LEVEL_GAMEPLAY0, TEXT("Layer_Peach"));
			for (int i = 0; i < iNumPeach; i++)
			{
				CCollider* pTargetCol = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Peach"), TEXT("Com_Collider"), i);

				//잡았을때
				if (m_pPickUpColliderCom->Collision_AABB(pTargetCol))
				{
					m_eState = PICK_UP;

					if (m_pPickObjectPtr == nullptr)
					{
						CSound_Manager::Get_Instance()->PlaySound(TEXT("Pl_Pickup_02.wav"), CSound_Manager::CHANNELID::PLAYER_EFFECT);
						CSound_Manager::Get_Instance()->PlaySound(TEXT("Pl_ItemPutIn_00.wav"), CSound_Manager::CHANNELID::PLAYER_EFFECT);

						m_pPickObjectPtr = pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Peach"), i);
						((CPeach*)m_pPickObjectPtr)->Set_Picked(true);

						CQuest* pQuest = (CQuest*)pGameInstance->Get_ObjectPtr(LEVEL_GAMEPLAY0, TEXT("Layer_Quest"), 0);
						pQuest->Clear_Quest(12, 1);
					}
					break;
				}
			}


		}
		else if (pGameInstance->Key_Pressing(DIK_W)) 
		{
			fDir = _float3(0.f, 0.f, 1.f);
			if (pGameInstance->Key_Pressing(DIK_A))
			{
				fDir.x = -1.f;
			}
			else if (pGameInstance->Key_Pressing(DIK_D))
			{
				fDir.x = 1;
			}
			
			if (pGameInstance->Key_Pressing(DIK_LSHIFT))
			{
				m_eState = DASH;
			}
			else
			{
				m_eState = WALK;
			}
		}
		else if (pGameInstance->Key_Pressing(DIK_S))
		{
			fDir = _float3(0.f, 0.f, -1.f);
			if (pGameInstance->Key_Pressing(DIK_A))
			{
				fDir.x = -1.f;
			}
			else if (pGameInstance->Key_Pressing(DIK_D))
			{
				fDir.x = 1;
			}

			if (pGameInstance->Key_Pressing(DIK_LSHIFT))
			{
				m_eState = DASH;
			}
			else
			{
				m_eState = WALK;
			}
		}
		else if (pGameInstance->Key_Pressing(DIK_A))
		{
			fDir = _float3(-1.f, 0.f, 0.f);

			if (pGameInstance->Key_Pressing(DIK_LSHIFT))
			{
				m_eState = DASH;
			}
			else
			{
				m_eState = WALK;
			}
		}
		else if (pGameInstance->Key_Pressing(DIK_D))
		{
			fDir = _float3(1.f, 0.f, 0.f);

			if (pGameInstance->Key_Pressing(DIK_LSHIFT))
			{
				m_eState = DASH;
			}
			else
			{
				m_eState = WALK;
			}
		}
		else
		{
			m_eState = IDLE;
		}


		if (m_eState == DASH || m_eState == WALK)
		{
			m_pTransformCom->LookAt_Target(fDir, TimeDelta);

			if (D3DXVec3Dot(&fDir, &vLook) > 0.5)
			{
				if (m_eState == DASH)
				{
					m_pTransformCom->Go_Straight(TimeDelta * 1.7);

				}
				else if (m_eState == WALK)
				{
					m_pTransformCom->Go_Straight(TimeDelta);		
				}
			}

			//충돌
			m_pColliderCom->Update_Collider();
			Collision_Layer(TEXT("Layer_Tree"));
			Collision_Layer(TEXT("Layer_Building"));
		}
	}


	//지형 타기
	CVIBuffer_Terrain* pVIBuffer = (CVIBuffer_Terrain*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, L"Layer_BackGround", L"Com_VIBuffer");
	CTransform* pTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, L"Layer_BackGround", L"Com_Transform");

	m_pTransformCom->Stand_OnTerrain(pVIBuffer, pTransform->Get_WorldMatrix(),TimeDelta);
	

	if (m_eState == DASH) //발자국 먼지 이펙트 생성
	{
		double dDashTime = m_pMeshCom->Get_CurrentAnimTime();
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
		_float3 vRight = m_pTransformCom->Get_State(CTransform::STATE::STATE_RIGHT);
		_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK);
		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Normalize(&vLook, &vLook);
		vPos -= vLook * 0.6f; 
		vPos.y += 0.5f;
		if (dDashTime - TimeDelta < 0.13 && dDashTime>0.13)
		{
			vPos += vRight * 0.16f;
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Effect"), TEXT("Layer_Smoke"), &vPos); 

			int iRand = rand() % 3;
			if (iRand == 0)
			{
				CSound_Manager::Get_Instance()->PlaySound(TEXT("Pl_Footstep_Socks_Grass_Tmp_00_L.wav"), CSound_Manager::CHANNELID::PLAYER_EFFECT, 0.4f);
			}
			else if (iRand == 1)
			{
				CSound_Manager::Get_Instance()->PlaySound(TEXT("Pl_Footstep_Socks_Grass_Tmp_01_L.wav"), CSound_Manager::CHANNELID::PLAYER_EFFECT, 0.4f);
			}
			else
			{
				CSound_Manager::Get_Instance()->PlaySound(TEXT("Pl_Footstep_Socks_Grass_Tmp_02_L.wav"), CSound_Manager::CHANNELID::PLAYER_EFFECT, 0.4f);
			}
		}
		if (dDashTime - TimeDelta < 0.4 && dDashTime> 0.4)
		{
			vPos -= vRight * 0.16f;
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY0, TEXT("Prototype_Effect"), TEXT("Layer_Smoke"), &vPos);
			int iRand = rand() % 3;
			if (iRand == 0)
			{
				CSound_Manager::Get_Instance()->PlaySound(TEXT("Pl_Footstep_Socks_Grass_Tmp_00_L.wav"), CSound_Manager::CHANNELID::PLAYER_EFFECT, 0.4f);
			}
			else if (iRand == 1)
			{
				CSound_Manager::Get_Instance()->PlaySound(TEXT("Pl_Footstep_Socks_Grass_Tmp_01_L.wav"), CSound_Manager::CHANNELID::PLAYER_EFFECT, 0.4f);
			}
			else
			{
				CSound_Manager::Get_Instance()->PlaySound(TEXT("Pl_Footstep_Socks_Grass_Tmp_02_L.wav"), CSound_Manager::CHANNELID::PLAYER_EFFECT, 0.4f);
			}
		}
	}

	if (m_eState == IDLE || m_eState == WALK || m_eState == DASH)
	{
		m_eState = (ANIM_STATE)((int)m_eState + 3);
		m_pMeshCom->Set_AnimationIndex(m_eState);
	}
	if (m_eState == PUT_AWAY || m_eState == PICK_UP)
	{
		m_eState = (ANIM_STATE)((int)m_eState + 1);
		m_pMeshCom->Set_AnimationIndex(m_eState,true,true);
	}
	if (m_eState == ANIM_STATE::NET_SWING)
	{
		m_pMeshCom->Set_AnimationIndex(m_eState, true);
	}


	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CPlayer::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this)))
		return E_FAIL;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;

	m_pMeshCom->Play_Animation(TimeDelta);


	m_pColliderCom->Update_Collider();
	m_pPickUpColliderCom->Update_Collider();


	return _int();
}

HRESULT CPlayer::Render()
{
	__super::Render();
	
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin_Shader(0);

	_uint iNumMeshContainers = m_pMeshCom->Get_NumMeshContainer();

	static int mesh_in = 0;

	if (GetKeyState('2') & 0x8000)
	{
		mesh_in++;
	}	if (GetKeyState('1') & 0x8000)
	{
		mesh_in--;
	}

	for (_uint i = 0; i < iNumMeshContainers; ++i) //1:눈,2:입,8:신발, 9:상의, 10:하의
	{
		_uint iNumMaterials = m_pMeshCom->Get_NumMaterials(i);

		m_pMeshCom->Update_SkinnedMesh(i);

		if (i >= 8)
		{
			m_pShaderCom->SetUp_ConstantTable("g_vCustomColor", &_float4(1.f, 1.f, 1.f, 1), sizeof(_float4));
		}
		else
		{
			m_pShaderCom->SetUp_ConstantTable("g_vCustomColor", &_float4(190 / 255.f, 140 / 255.f, 100 / 255.f, 1), sizeof(_float4));
		}

		if (i == 1 || i == 2)
		{
			int i = 0;
			m_pShaderCom->SetUp_ConstantTable("g_iSkin", &i, sizeof(int));
		}
		else if (i < 8)
		{
			int i = 1;
			m_pShaderCom->SetUp_ConstantTable("g_iSkin", &i, sizeof(int));
		}
		else 
		{
			int i = 2;
			m_pShaderCom->SetUp_ConstantTable("g_iSkin", &i, sizeof(int));
		}

		for (_uint j = 0; j < iNumMaterials; ++j)
		{
			if (FAILED(m_pMeshCom->SetUp_TextureOnShader(m_pShaderCom, "g_DiffuseTexture", MESHMATERIALTEXTURE::TYPE_DIFFUSE, i, j)))
				return E_FAIL;
			m_pMeshCom->SetUp_TextureOnShader(m_pShaderCom, "g_NormalTexture", MESHMATERIALTEXTURE::TYPE_NORMAL, i, j);
			m_pMeshCom->SetUp_TextureOnShader(m_pShaderCom, "g_MixTexture", MESHMATERIALTEXTURE::TYPE_SPECULAR, i, j);

			m_pShaderCom->Commit();

			m_pMeshCom->Render(i, j);
		}
	}

	m_pShaderCom->End_Shader();

#ifdef _DEBUG
	m_pRendererCom->Add_RenderDebug(m_pColliderCom);
	m_pRendererCom->Add_RenderDebug(m_pPickUpColliderCom);
#endif // _DEBUG


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


HRESULT CPlayer::Render_ShadowDepth()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pShaderCom->SetUp_ConstantTable("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	

	/* 빛 정보 */
	D3DLIGHT9	LightDesc = pGameInstance->Get_LightDesc(0);
	_float3 vLightDir = LightDesc.Direction;
	D3DXVec3Normalize(&vLightDir, &vLightDir);
	_float3		vLightAt = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3		vLightEye = vLightAt - 250 * vLightDir;
	_float3		vLightUp = _float3(0.f, 1.f, 0.f);

	m_pRendererCom->Set_LightAtEyeUp(vLightAt, vLightEye, vLightUp);

	_matrix		LightViewMatrix;

	D3DXMatrixLookAtLH(&LightViewMatrix, &vLightEye, &vLightAt, &vLightUp);
	m_pShaderCom->SetUp_ConstantTable("g_ViewMatrix", &LightViewMatrix, sizeof(_matrix));

	_matrix		LightProjMatrix;

	D3DXMatrixPerspectiveFovLH(&LightProjMatrix, D3DXToRadian(15.f), _float(1280) / _float(720), 0.2f, 300.f);

	m_pShaderCom->SetUp_ConstantTable("g_ProjMatrix", &LightProjMatrix, sizeof(_matrix));

	m_pShaderCom->Begin_Shader(2);

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

void CPlayer::Get_Insect(CInsect* pInsect)
{
	if (m_pPickObjectPtr == nullptr)
	{
		Set_AnimState(CPlayer::ANIM_STATE::NET_GET);
		m_pMeshCom->Set_AnimationIndex(CPlayer::ANIM_STATE::NET_GET, true, true);
		m_pPickObjectPtr = (CGameObject*)pInsect;

		CSound_Manager::Get_Instance()->PlaySound(TEXT("Pl_Net_GetCmn.wav"), CSound_Manager::CHANNELID::EFFECT);
		CSound_Manager::Get_Instance()->Set_Volume(CSound_Manager::CHANNELID::BGM, 0.01f);


	}
}

void CPlayer::Collision_Layer(const _tchar* LayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	int iNum = pGameInstance->Get_ObjectNumInLayer(LEVEL_GAMEPLAY0, LayerTag);
	
	for (int i = 0; i < iNum; i++)
	{
		CCollider* pCol = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY0, LayerTag, TEXT("Com_Collider"), i);

		bool bCollision = false;
		_float3 vOverlap = { 0.f,0.f,0.f };
		if (m_pColliderCom->Collision_AABB(pCol, &vOverlap))
		{
			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
			if (fabsf(vOverlap.x) < fabsf(vOverlap.z))
			{
				vPos.x += vOverlap.x;
			}
			else
			{
				vPos.z += vOverlap.z;
			}
			m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vPos);
			bCollision = true;
			break;
		}
	}
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CPlayer::Add_Components()
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

	/* Com_Mesh */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Mesh_Player"), TEXT("Com_Mesh"), (CComponent**)&m_pMeshCom)))
		return E_FAIL;


	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY0, TEXT("Prototype_Shader_Player"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;


	/* Com_Collider*/
	CCollider::COLLIDERDESC		ColliderDesc;

	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	ColliderDesc.vScale = _float3(2.f, 4.5f, 2.f);
	ColliderDesc.vRadians = _float3(D3DXToRadian(0), 0.0f, 0.f);
	ColliderDesc.vInitPos = _float3(0.f, 2.25f, 0.f);
	/* Com_Collider */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_AABB"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;


	ColliderDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix();
	ColliderDesc.vScale = _float3(1.0f, 1.0f, 1.0f);
	ColliderDesc.vRadians = _float3(D3DXToRadian(0), 0.0f, 0.f);
	ColliderDesc.vInitPos = _float3(0.0f, 0.5f, 1.0f);

	/* Com_Collider */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Collider_AABB"), TEXT("Com_Collider_PickUp"), (CComponent**)&m_pPickUpColliderCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::SetUp_ConstantTable()
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

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer* pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pMeshCom);
}
