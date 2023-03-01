#include "Graphic_Device.h"

IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
	:m_pSDK(nullptr)
	,m_pDevice(nullptr)
{

}
HRESULT CGraphic_Device::Set_Font(D3DXFONT_DESCW* pFont_Desc)
{
	Safe_Release(m_pFont);
	return D3DXCreateFontIndirectW(m_pDevice, pFont_Desc, &m_pFont);
}

HRESULT CGraphic_Device::Ready_Graphic_Device(HWND hWnd, WINMODE eMode, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppGraphic_Device)
{

	D3DCAPS9 d3dcaps; 
	ZeroMemory(&d3dcaps, sizeof(D3DCAPS9));
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION); 
	
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps)))
		goto ERR; 

	DWORD vp = 0; 
	if (d3dcaps.DevCaps  & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.BackBufferWidth = iWinCX;
	d3dpp.BackBufferHeight = iWinCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality =0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = eMode;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_INTERVAL_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp,&d3dpp, &m_pDevice)))
		goto ERR; 
	
	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
		goto ERR; 

	D3DXFONT_DESCW tFontDesc; 
	ZeroMemory(&tFontDesc, sizeof(D3DXFONT_DESCW)); 
	tFontDesc.Height = 15;
	tFontDesc.Width = 16;
	tFontDesc.Weight = FW_HEAVY;
	tFontDesc.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontDesc.FaceName , L"@넥슨Lv1고딕 Bold");
	
	if (FAILED(D3DXCreateFontIndirectW(m_pDevice, &tFontDesc, &m_pFont)))
		goto ERR; 

	if (FAILED(D3DXCreateLine(m_pDevice, &m_pLine)))
		goto ERR; 

	if (nullptr != ppGraphic_Device)
	{
		*ppGraphic_Device = m_pDevice;
		Safe_AddRef(m_pDevice);
	}

	return S_OK;
ERR:	
	MSGBOX("Creating Failed Graphic_Device ");
	return E_FAIL; 
}

void CGraphic_Device::Render_Begin()
{	

	/* API : 백버퍼에 계속 중첩하여 그리낟. */
	/* DirectX : 백버퍼에 그려져있던 픽셀정보를 모두 초기화한다.  */
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(255, 255, 255, 255), 1.f, 0);
	m_pDevice->BeginScene();
	
}

void CGraphic_Device::Render_End(HWND hWND /*= nullptr*/)
{
	
	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, hWND, nullptr);
}


void CGraphic_Device::Free()
{
	if (0 != Safe_Release(m_pLine))
		MSGBOX("Failed to Release ID3DXLine");


	if (0 != Safe_Release(m_pFont))
		MSGBOX("Failed to Release ID3DXFont");


	if (0 != Safe_Release(m_pSprite))
		MSGBOX("Failed to Release ID3DXSprite");


	if (0 != Safe_Release(m_pDevice))
		MSGBOX("Failed to Release ID3DXDevice9");


	if (0 != Safe_Release(m_pSDK))
		MSGBOX("Failed to Release ID3DX9");	
}


