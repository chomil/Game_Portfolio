#pragma once

#include "Base.h"

BEGIN(Engine)

/* ∑ª¥ı≈∏∞Ÿ */
class CTarget final : public CBase
{
private:
	explicit CTarget(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTarget() = default;
public:
	LPDIRECT3DTEXTURE9	Get_TargetTexture() {
		return m_pTexture;
	}
public:
	HRESULT NativeConstruct(_uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR vClearColor);
	HRESULT Bind_RenderTarget(_uint iIndex);
	HRESULT Release_RenderTarget(_uint iIndex);
	HRESULT Clear();

#ifdef _DEBUG
public:
	HRESULT Ready_Debug_Buffer(_float fLeftX, _float fTopY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug_Buffer();
#endif // _DEBUG
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	LPDIRECT3DTEXTURE9			m_pTexture = nullptr;
	LPDIRECT3DSURFACE9			m_pOldSurface = nullptr; /* ∑ª¥ı≈∏∞Ÿ */
	LPDIRECT3DSURFACE9			m_pSurface = nullptr; /* ∑ª¥ı≈∏∞Ÿ */
	D3DXCOLOR					m_ClearColor;

#ifdef _DEBUG
private:
	class CVIBuffer_Rect_Viewport*			m_pVIBuffer = nullptr;
	class CShader*							m_pShader = nullptr;
#endif // _DEBUG

public:
	static CTarget* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR vClearColor);
	virtual void Free() override;
};

END