#include "stdafx.h"
#include "..\public\MainApp.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "Level_Logo.h"
#include "BackGround.h"
#include "Transition.h"



CMainApp::CMainApp()		
{
}

HRESULT CMainApp::NativeConstruct()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Initialize_Engine(g_hInst, g_hWnd, LEVEL_END)))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Ready_Graphic_Device(g_hWnd, CGraphic_Device::MODE_WIN, g_nWinCX, g_nWinCY, &m_pGraphic_Device)))
		return E_FAIL;		

	if (FAILED(Add_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Add_Prototype_GameObject()))
		return E_FAIL;

	CSound_Manager::Get_Instance()->Initialize_Prev();
	CSound_Manager::Get_Instance()->PlayBGM(TEXT("BGM_Title.wav"));
	
	/* ���� �����Ѵ�. */
	if (FAILED(pGameInstance->SetUp_Level(CLevel_Logo::Create(m_pGraphic_Device))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

_int Client::CMainApp::Tick(double TimeDelta)
{
	static float fData = 0;
	

	/* ���� �����Ѵ�. . */
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* ī�޶�κ��� ����, �屸�ϳ�. */
	pGameInstance->Tick(TimeDelta);

	/*���콺 ��ǥ �̵�. */
	m_pPicking->Compute_MouseCursorPosInWorld(g_hWnd);

	/* �浹. */
	pGameInstance->LateTick(TimeDelta);
	
	Safe_Release(pGameInstance);
	return 0;
}

HRESULT Client::CMainApp::Render_MainApp()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* �������ϱ����� �غ�. */
	pGameInstance->Render_Begin();

	/* (����ۿ�) ��ü���� �׸���. */
	m_pRenderer->Render_GameObject();

		
	/* ���� �׸���. */
	if (FAILED(pGameInstance->Render_Level()))
		return E_FAIL;



	/* �������� ������.(���� �����ִ� ����) */

	/* (����۸�) ����Ʈ(����� or ������)�� �����ش�. */

	pGameInstance->Render_End();

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CMainApp::Add_Prototype_GameObject()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* For.LEVEL_STATIC */

	/* Prototype_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_BackGround"), CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;


	/* Prototype_Transition */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Transition"), CTransition::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_Camera_Free */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Camera_Free"), CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CMainApp::Add_Prototype_Component()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* For.LEVEL_STATIC */

	/* Prototype_Renderer */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Renderer"), m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return E_FAIL;
	Safe_AddRef(m_pRenderer);

	/* Prototype_Picking*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Picking"), m_pPicking = CPicking::Create(m_pGraphic_Device))))
		return E_FAIL;
	Safe_AddRef(m_pPicking);

	/* Prototype_Collider_AABB*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Collider_AABB"), CCollider::Create(m_pGraphic_Device, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* Prototype_Collider_OBB*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Collider_OBB"), CCollider::Create(m_pGraphic_Device, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* Prototype_Transform */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Transform"), CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;	

	/* Prototype_VIBuffer_Rect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;	

	/* Prototype_Shader_Rect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Shader_Rect"), CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Rect.hlsl")))))
		return E_FAIL;


	/* Prototype_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Texture_Default"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Main.png")))))
		return E_FAIL;	


	Safe_Release(pGameInstance);

	return S_OK;
}

CMainApp * Client::CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp;

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CMainApp");
		Safe_Release(pInstance);
	}


	return pInstance;
}

void Client::CMainApp::Free()
{	
	Safe_Release(m_pPicking);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pGraphic_Device);

	CGameInstance::Release_Engine();
}


