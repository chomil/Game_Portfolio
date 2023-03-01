#include "..\public\Target.h"
#include "VIBuffer_Rect_Viewport.h"
#include "Shader.h"

CTarget::CTarget(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CTarget::NativeConstruct(_uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR vClearColor)
{
	m_ClearColor = vClearColor;

	if (FAILED(D3DXCreateTexture(m_pGraphic_Device, iWidth, iHeight, 1, D3DUSAGE_RENDERTARGET, eFormat, D3DPOOL_DEFAULT, &m_pTexture)))
		return E_FAIL;

	if (FAILED(m_pTexture->GetSurfaceLevel(0, &m_pSurface)))
		return E_FAIL;	
	
	return S_OK;
}

HRESULT CTarget::Bind_RenderTarget(_uint iIndex)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->GetRenderTarget(iIndex, &m_pOldSurface);

	if (FAILED(m_pGraphic_Device->SetRenderTarget(iIndex, m_pSurface)))
		return E_FAIL;


	return S_OK;
}

HRESULT CTarget::Release_RenderTarget(_uint iIndex)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderTarget(iIndex, m_pOldSurface);

	Safe_Release(m_pOldSurface);

	return S_OK;
}

HRESULT CTarget::Clear()
{
	Bind_RenderTarget(0);

	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET, m_ClearColor, 1.f, 0);

	Release_RenderTarget(0);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CTarget::Ready_Debug_Buffer(_float fLeftX, _float fTopY, _float fSizeX, _float fSizeY)
{
	m_pVIBuffer = CVIBuffer_Rect_Viewport::Create(m_pGraphic_Device, fLeftX, fTopY, fSizeX, fSizeY);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"));
	if (nullptr == m_pShader)
		return E_FAIL;

	return S_OK;
}
HRESULT CTarget::Render_Debug_Buffer()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pShader->SetUp_TextureConstantTable("g_DebugTexture", m_pTexture);

	m_pShader->Begin_Shader(0);

	m_pVIBuffer->Render_VIBuffer();

	m_pShader->End_Shader();

	return S_OK;
}
#endif // _DEBUG

CTarget * CTarget::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR vClearColor)
{
	CTarget*	pInstance = new CTarget(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(iWidth, iHeight, eFormat, vClearColor)))
	{
		MSGBOX("Failed to Creating CTarget");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTarget::Free()
{
#ifdef _DEBUG
	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);
#endif // _DEBUG

	Safe_Release(m_pSurface);
	Safe_Release(m_pTexture);
	Safe_Release(m_pOldSurface);
	Safe_Release(m_pGraphic_Device);
}
