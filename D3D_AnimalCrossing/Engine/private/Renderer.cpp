#include "..\public\Renderer.h"
#include "Component.h"
#include "GameObject.h"

#include "Target_Manager.h"
#include "Light_Manager.h"
#include "VIBuffer_Rect_Viewport.h"
#include "Shader.h"
#include "GameInstance.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
	, m_pTarget_Manager(CTarget_Manager::Get_Instance())
{
	Safe_AddRef(m_pTarget_Manager);

}

HRESULT CRenderer::NativeConstruct_Prototype()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	D3DVIEWPORT9		Viewport;
	m_pGraphic_Device->GetViewport(&Viewport);

	/* For.Target_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, TEXT("Target_Diffuse"), Viewport.Width, Viewport.Height, D3DFMT_A8R8G8B8, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Normal */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, TEXT("Target_Normal"), Viewport.Width, Viewport.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;

	/* For.Target_Shade */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, TEXT("Target_Shade"), Viewport.Width, Viewport.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* For.Target_Specular */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, TEXT("Target_Specular"), Viewport.Width, Viewport.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;


	_uint		iShadowMapCX = 1280*2;
	_uint		iShadowMapCY = 720*2;

	// For.Target_ShadowDepth
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, L"Target_ShadowDepth", iShadowMapCX, iShadowMapCY, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateDepthStencilSurface(iShadowMapCX, iShadowMapCY, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pShadow_DS_Surface, nullptr)))
		return E_FAIL;

	/* For.Target_Depth*/
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pGraphic_Device, TEXT("Target_Depth"), Viewport.Width, Viewport.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.MRT_Deferred */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth"))))
		return E_FAIL;

	/* For.MRT_LightAcc */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;

	// For.MRT_ShadowDepth(Shadow)
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightDepth", L"Target_ShadowDepth")))
		return E_FAIL;

	/* 디퍼에의해 만들어진 타겟들을 조합하여. 백버퍼에 그려주기위한 버퍼. */
	m_pVIBuffer = CVIBuffer_Rect_Viewport::Create(m_pGraphic_Device, -0.5f, -0.5f, Viewport.Width, Viewport.Height);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"));
	if (nullptr == m_pShader)
		return E_FAIL;

#ifdef _DEBUG
		//if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Diffuse"), 0.f, 0.f, 200.f, 200.f)))
		//	return E_FAIL;
		//if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Normal"), 0.f, 200.f, 200.f, 200.f)))
		//	return E_FAIL;
		//if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Depth"), 0.f, 400.f, 200.f, 200.f)))
		//	return E_FAIL;
		//if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Shade"), 200.f, 0.f, 200.f, 200.f)))
		//	return E_FAIL;
		//if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_Specular"), 200.f, 200.f, 200.f, 200.f)))
		//	return E_FAIL;	
		//if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer(TEXT("Target_ShadowDepth"), 1080.f, 0.f, 200.0f, 200.f)))
		//	return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (nullptr == pGameObject || 
		RENDER_END <= eGroup)
		return E_FAIL;

	m_RenderObjects[eGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Add_RenderDebug(CComponent* pDebugObject)
{
	m_DebugObject.push_back(pDebugObject);

	Safe_AddRef(pDebugObject);

	return S_OK;
}

HRESULT CRenderer::Render_GameObject()
{
	Render_Priority();

	Render_ShadowDepth();

	/* Diffuse, Normal*/
	Render_NonAlpha();

	/* Shade */
	Render_LightAcc();

	/* 백버퍼에 찍는다. */
	Render_Blend();

	Render_Alpha();
	Render_UI();


#ifdef _DEBUG

	static bool bDebugTargets = true;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Key_PressOnce(DIK_F2))
	{
		bDebugTargets = !bDebugTargets;
	}
	if (bDebugTargets)
	{
		if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_Deferred"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_LightAcc"))))
			return E_FAIL;		
		if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_LightDepth"))))
			return E_FAIL;

		for (auto& pDebugObj : m_DebugObject)
		{
			pDebugObj->Render();
			Safe_Release(pDebugObj);
		}
		m_DebugObject.clear();
	}
	RELEASE_INSTANCE(CGameInstance);
#endif // _DEBUG

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{	
	for (auto& pGameObject : m_RenderObjects[RENDER_PRIORITY])
	{
		if (FAILED(pGameObject->Render()))
		{
			return E_FAIL;
		}
		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_PRIORITY].clear();
	
	return S_OK;
}

HRESULT CRenderer::Render_NonAlpha()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_Deferred"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderObjects[RENDER_NONALPHA])
	{
		if (FAILED(pGameObject->Render()))
		{
			return E_FAIL;
		}
		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_NONALPHA].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(TEXT("MRT_Deferred"))))
		return E_FAIL;

	return S_OK;
}

bool Compare(CGameObject* pSour, CGameObject* pDest)
{
	return pSour->Get_CamDistance() > pDest->Get_CamDistance();	
}

HRESULT CRenderer::Render_Alpha()
{
	m_RenderObjects[RENDER_ALPHA].sort(Compare);

	for (auto& pGameObject : m_RenderObjects[RENDER_ALPHA])
	{
		if (FAILED(pGameObject->Render()))
		{
			return E_FAIL;
		}
		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_ALPHA].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_UI])
	{
		if (FAILED(pGameObject->Render()))
		{
			return E_FAIL;
		}
		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_UI].clear();

	return S_OK;
}

HRESULT CRenderer::Render_ShadowDepth()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_LightDepth")))
		return E_FAIL;

	m_pGraphic_Device->GetDepthStencilSurface(&m_pOriginal_DS_Surface);
	m_pGraphic_Device->SetDepthStencilSurface(m_pShadow_DS_Surface);

	m_pGraphic_Device->Clear(0, 0, D3DCLEAR_ZBUFFER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 1.f, 0);

	for (auto& pGameObject : m_RenderObjects[RENDER_SHADOWDEPTH])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_ShadowDepth();
		Safe_Release(pGameObject);
	}

	m_RenderObjects[RENDER_SHADOWDEPTH].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_LightDepth")))
		return E_FAIL;

	m_pGraphic_Device->SetDepthStencilSurface(m_pOriginal_DS_Surface);
	Safe_Release(m_pOriginal_DS_Surface);

	return NOERROR;
}

/* Target_Shade에 화면에 그려져있는 픽셀들의 명암을 그린다.(빛 갯수만큼) */
HRESULT CRenderer::Render_LightAcc()
{
	/* Shade타겟을 장치에 바인드한다. */
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_LightAcc"))))
		return E_FAIL;

	CLight_Manager* pLight_Manager = GET_INSTANCE(CLight_Manager);

	pLight_Manager->Render();

	RELEASE_INSTANCE(CLight_Manager);

	if (FAILED(m_pTarget_Manager->End_MRT(TEXT("MRT_LightAcc"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	if (FAILED(m_pShader->SetUp_TextureConstantTable("g_DiffuseTexture", m_pTarget_Manager->Get_TargetTexture(TEXT("Target_Diffuse")))))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_TextureConstantTable("g_ShadeTexture", m_pTarget_Manager->Get_TargetTexture(TEXT("Target_Shade")))))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_TextureConstantTable("g_SpecularTexture", m_pTarget_Manager->Get_TargetTexture(TEXT("Target_Specular")))))
		return E_FAIL;	
	
	if (FAILED(m_pShader->SetUp_TextureConstantTable("g_DepthTexture", m_pTarget_Manager->Get_TargetTexture(TEXT("Target_Depth")))))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_TextureConstantTable("g_ShadowDepthTexture", m_pTarget_Manager->Get_TargetTexture(TEXT("Target_ShadowDepth")))))
		return E_FAIL;


	_matrix			matView, matProj;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	matView = pGameInstance->Get_Transform(D3DTS_VIEW);
	matProj = pGameInstance->Get_Transform(D3DTS_PROJECTION);

	m_pShader->SetUp_ConstantTable("g_ProjMatrixInv", D3DXMatrixInverse(&matProj, nullptr, &matProj), sizeof(_matrix));

	m_pShader->SetUp_ConstantTable("g_ViewMatrixInv", D3DXMatrixInverse(&matView, nullptr, &matView), sizeof(_matrix));





	_matrix			LightProjMatrix;

	D3DXMatrixPerspectiveFovLH(&LightProjMatrix, D3DXToRadian(15.f), _float(1280) / _float(720), 0.2f, 300.f);

	m_pShader->SetUp_ConstantTable("g_matLightProj", &LightProjMatrix, sizeof(_matrix));

//	_float3		vLightAt = _float3(65.f, 0.f, 65.f);
//_float3		vLightEye = _float3(60.f, 20.f, 60.f);
//_float3		vLightUp = _float3(0.f, 1.f, 0.f);
	_matrix			matLightView;
	D3DXMatrixLookAtLH(&matLightView, &m_vLightEye, &m_vLightAt, &m_vLightUp);

	m_pShader->SetUp_ConstantTable("g_matLightView", &matLightView, sizeof(_matrix));



	m_pShader->Begin_Shader(3);

	m_pVIBuffer->Render_VIBuffer();

	m_pShader->End_Shader();

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}
CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer*	pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CRenderer");
		Safe_Release(pInstance);
	}


	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();
	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pOriginal_DS_Surface);
	Safe_Release(m_pShadow_DS_Surface);
	Safe_Release(m_pTarget_Manager);
}
