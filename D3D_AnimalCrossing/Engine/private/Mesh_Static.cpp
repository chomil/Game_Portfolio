#include "..\public\Mesh_Static.h"
#include "Shader.h"

CMesh_Static::CMesh_Static(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMesh(pGraphic_Device)
{
}

CMesh_Static::CMesh_Static(const CMesh_Static & rhs)
	: CMesh(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_dwNumMaterials(rhs.m_dwNumMaterials)
	, m_Materials(rhs.m_Materials)
{
	for (auto& pMeshMaterialTexture : m_Materials)
	{
		Safe_AddRef(pMeshMaterialTexture->pDiffuseMap);
		Safe_AddRef(pMeshMaterialTexture->pNormalMap);
		Safe_AddRef(pMeshMaterialTexture->pSpecularMap);
	}
		

	Safe_AddRef(m_pMesh);
}

HRESULT CMesh_Static::NativeConstruct_Prototype(const _tchar * pFilePath, const _tchar * pFileName)
{
	_tchar		szFullPath[MAX_PATH] = TEXT("");

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	/* Á¤Á¡¹öÆÛ¸¦ »ý¼º + ÀÎµ¦½º¹öÆÛµµ »ý¼º. */
	if (FAILED(D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphic_Device, &m_pAdjacency, &m_pMaterials, nullptr, &m_dwNumMaterials, &m_pMesh)))
		return E_FAIL;

	/* ¸Þ½ÃÀÇ µðÇ»Áî ¸Ê + ³ë¸Ö¸Ê + ½ºÆåÅ§·¯¸Ê À» ·ÎµåÇÏ³®. */
	for (_uint i = 0; i < m_dwNumMaterials; ++i)
	{
		D3DXMATERIAL*	pMaterial = (D3DXMATERIAL*)m_pMaterials->GetBufferPointer() + i;

		MESHMATERIALTEXTURE*	pMeshMaterialTexture = new MESHMATERIALTEXTURE;
		ZeroMemory(pMeshMaterialTexture, sizeof(MESHMATERIALTEXTURE));


		_tchar		szTextureFileName[MAX_PATH] = TEXT("");


		if (pMaterial->pTextureFilename != nullptr)
		{
			/* char -> tchar */
			MultiByteToWideChar(CP_ACP, 0, pMaterial->pTextureFilename, strlen(pMaterial->pTextureFilename), szTextureFileName, sizeof(_tchar) * MAX_PATH);
		}

		
		_tchar	szFileName[MAX_PATH] = TEXT("");

		/* µðÇ»Áî ¸Ê ·Îµå */		
		wsprintf(szFileName, szTextureFileName, L"D");

		lstrcpy(szFullPath, pFilePath);
		lstrcat(szFullPath, szFileName);
		D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshMaterialTexture->pDiffuseMap);
		//FAIL Àá±ñ ¾ø¾Ú

		/* ³ë¸Ö ¸Ê ·Îµå. */
		//wsprintf(szFileName, szTextureFileName, L"N");

		//lstrcpy(szFullPath, pFilePath);
		//lstrcat(szFullPath, szFileName);
		D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshMaterialTexture->pNormalMap);
			

		/* ½ºÆåÅ§·¯ ¸Ê ·Îµå. */
		//wsprintf(szFileName, szTextureFileName, L"S");

		//lstrcpy(szFullPath, pFilePath);
		//lstrcat(szFullPath, szFileName);
		D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshMaterialTexture->pSpecularMap);		

		m_Materials.push_back(pMeshMaterialTexture);
	}

	return S_OK;
}

HRESULT CMesh_Static::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CMesh_Static::SetUp_TextureOnShader(CShader* pShader, D3DXHANDLE hParameter, MESHMATERIALTEXTURE::TYPE eType, _uint iMaterialIndex)
{
	LPDIRECT3DTEXTURE9		pTexture = nullptr;

	switch (eType)
	{ 
	case MeshMaterialTexture::TYPE_DIFFUSE:
		pTexture = m_Materials[iMaterialIndex]->pDiffuseMap;
		break;
	case MeshMaterialTexture::TYPE_NORMAL:
		pTexture = m_Materials[iMaterialIndex]->pNormalMap;
		break;
	case MeshMaterialTexture::TYPE_SPECULAR:
		pTexture = m_Materials[iMaterialIndex]->pSpecularMap;
		break;	
	}

	if (FAILED(pShader->SetUp_TextureConstantTable(hParameter, pTexture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMesh_Static::Render(_uint iMaterialIndex)
{	
	m_pMesh->DrawSubset(iMaterialIndex);

	return S_OK;
}

CMesh_Static * CMesh_Static::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath, const _tchar * pFileName)
{
	CMesh_Static*	pInstance = new CMesh_Static(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(pFilePath, pFileName)))
	{
		MSGBOX("Failed to Creating CMesh_Static");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CMesh_Static::Clone(void * pArg)
{
	CMesh_Static*	pInstance = new CMesh_Static(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CMesh_Static");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMesh_Static::Free()
{
	__super::Free();

	for (auto& pMeshMaterialTexture : m_Materials)
	{
		Safe_Release(pMeshMaterialTexture->pDiffuseMap);
		Safe_Release(pMeshMaterialTexture->pNormalMap);
		Safe_Release(pMeshMaterialTexture->pSpecularMap);

		if(false == m_isClone)
			Safe_Delete(pMeshMaterialTexture);
	}
	m_Materials.clear();


	Safe_Release(m_pMesh);
}
