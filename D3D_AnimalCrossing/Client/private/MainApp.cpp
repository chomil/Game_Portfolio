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
	
	/* 씬을 생성한다. */
	if (FAILED(pGameInstance->SetUp_Level(CLevel_Logo::Create(m_pGraphic_Device))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

_int Client::CMainApp::Tick(double TimeDelta)
{
	static float fData = 0;
	

	/* 씬을 갱신한다. . */
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* 카메라로부터 퉁영, 뷴구하낟. */
	pGameInstance->Tick(TimeDelta);

	/*마우스 좌표 이동. */
	m_pPicking->Compute_MouseCursorPosInWorld(g_hWnd);

	/* 충돌. */
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

	/* 렌더링하기위한 준비. */
	pGameInstance->Render_Begin();

	/* (백버퍼에) 객체들을 그린다. */
	m_pRenderer->Render_GameObject();

		
	/* 씬을 그린다. */
	if (FAILED(pGameInstance->Render_Level()))
		return E_FAIL;



	/* 렌더링의 마무리.(실제 보여주는 동작) */

	/* (백버퍼를) 뷰포트(모니터 or 윈도우)에 보여준다. */

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


