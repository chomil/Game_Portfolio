#include "..\public\Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)	
	: CComponent(pGraphic_Device)
{
	
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_Textures(rhs.m_Textures)
{
	for (auto& pTexture : m_Textures)
		Safe_AddRef(pTexture);
}

HRESULT CTexture::NativeConstruct_Prototype(TYPE eType, const _tchar * pTextureFilePath, _uint iNumTextures)
{
	m_Textures.reserve(iNumTextures);

	_tchar		szTexturePath[MAX_PATH] = TEXT("");

	for (_uint i = 0; i < iNumTextures; ++i)
	{
		IDirect3DBaseTexture9*	pTexture = nullptr;

		wsprintf(szTexturePath, pTextureFilePath, i);

		if (eType == TYPE_GENERAL)
		{
			/*
			if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szTexturePath, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;
				*/

			if (FAILED(D3DXCreateTextureFromFileEx(m_pGraphic_Device, szTexturePath, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_FROM_FILE, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, 0, 0, (LPDIRECT3DTEXTURE9*)&pTexture)))
			{
				return E_FAIL;
			}
		}
		else
		{
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphic_Device, szTexturePath, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
		}
		

		/*D3DXCreateCubeTextureFromFile();*/

		m_Textures.push_back(pTexture);
	}
	return S_OK;
}

HRESULT CTexture::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CTexture::SetUp_OnGraphic_Device(_uint iStage, _uint iTextureIndex)
{
	if (nullptr == m_pGraphic_Device || 
		iTextureIndex >= m_Textures.size())
		return E_FAIL;

	return m_pGraphic_Device->SetTexture(iStage, m_Textures[iTextureIndex]);	
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType, const _tchar * pTextureFilePath, _uint iNumTextures)
{
	CTexture*	pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType, pTextureFilePath, iNumTextures)))
	{
		MSGBOX("Failed to Creating CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTexture::Clone(void * pArg)
{
	CTexture*	pInstance = new CTexture(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pTexture : m_Textures)
		Safe_Release(pTexture);
	m_Textures.clear();
}
