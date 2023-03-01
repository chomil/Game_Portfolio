#include "..\public\HierarchyLoader.h"

CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}


HRESULT CHierarchyLoader::NativeConstruct(const _tchar * pMeshFilePath)
{
	m_pMeshFilePath = pMeshFilePath;

	return S_OK;
}

/* 메시를 구성하는 뼈대정보를 정리한다. */
STDMETHODIMP CHierarchyLoader::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	D3DXFRAME_DERIVED*			pFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pFrame, sizeof(D3DXFRAME_DERIVED));

	if (nullptr != Name)
	{
		Allocate_Name(Name, &pFrame->Name);
	}

	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	*ppNewFrame = pFrame;

	return S_OK;
}

STDMETHODIMP CHierarchyLoader::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA * pMeshData, CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED*	pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	if (nullptr != Name)
		Allocate_Name(Name, &pMeshContainer->Name);

	/* 현재 메시를 구성하고있는 정점의 멤버를 표현한 배열. */
	/* 배열하나가 정점 멤버하나. */
	/* 정점멤버가 세개라면 배열도 세개선인. */

	D3DVERTEXELEMENT9			Element[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, // 위치 
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },	// 노멀. 		
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },	// 텍스쳐uv				
		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		D3DDECL_END()
	};

	/* 복제된 메시를 생성한다. */
	if (FAILED(pMeshData->pMesh->CloneMesh(pMeshData->pMesh->GetOptions(), Element, m_pGraphic_Device, &pMeshContainer->MeshData.pMesh)))
		return E_FAIL;
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	pMeshContainer->NumMaterials = NumMaterials;

	pMeshContainer->ppMaterialTextures = new MESHMATERIALTEXTURE*[NumMaterials];	

	for (_uint i = 0; i < NumMaterials; ++i)	
		pMeshContainer->ppMaterialTextures[i] = new MESHMATERIALTEXTURE;	

	for (_uint i = 0; i < pMeshContainer->NumMaterials; ++i)
	{
		_tchar		szFullPath[MAX_PATH] = TEXT("");

		lstrcpy(szFullPath, m_pMeshFilePath);

		_tchar		szTextureFileName[MAX_PATH] = TEXT("");

		MultiByteToWideChar(CP_ACP, 0, pMaterials[i].pTextureFilename, strlen(pMaterials[i].pTextureFilename)
			, szTextureFileName, MAX_PATH);

		lstrcat(szFullPath, szTextureFileName);

		_uint		iLength = 0;	

		iLength = lstrlen(szFullPath);
		_uint	iIndex = 0;

		for (_uint i = iLength; i >= 0; --i)
		{
			if (szFullPath[i] == L'_')
			{				
				iIndex = i;
				break;
			}
		}

		/* For.Diffuse map */
		//szFullPath[iIndex+1] = L'A';
		//szFullPath[iIndex+2] = L'l';
		//szFullPath[iIndex+3] = L'b';
		D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshContainer->ppMaterialTextures[i]->pDiffuseMap);
		

		/* For.Normal map */
		szFullPath[iIndex + 1] = L'N';
		szFullPath[iIndex + 2] = L'r';
		szFullPath[iIndex + 3] = L'm';
		D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshContainer->ppMaterialTextures[i]->pNormalMap);
		

		/* For.Specular map */
		szFullPath[iIndex + 1] = L'M';
		szFullPath[iIndex + 2] = L'i';
		szFullPath[iIndex + 3] = L'x';
		D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pMeshContainer->ppMaterialTextures[i]->pSpecularMap);
	}

	pMeshContainer->pAdjacency = new _ulong[pMeshContainer->MeshData.pMesh->GetNumFaces() * 3];
 	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * pMeshContainer->MeshData.pMesh->GetNumFaces() * 3);

	if (FAILED(D3DXComputeTangent(pMeshContainer->MeshData.pMesh, 0, 0, 0, 1, pMeshContainer->pAdjacency)))
		return E_FAIL;

	/* 원래 메시. 위치 노말 유브이 탄젠트 바이노말 블렌드인데스, 블렌두에이트 */

	pMeshContainer->pSkinInfo = pSkinInfo;

	

	Safe_AddRef(pMeshContainer->pSkinInfo);

	pMeshContainer->pSkinInfo->SetDeclaration(Element);
	
	pMeshContainer->iNumBones = pMeshContainer->pSkinInfo->GetNumBones();

	pMeshContainer->pOffsetMatrices = new _matrix[pMeshContainer->iNumBones];

	/* 바뀌지않는 행렬인 offsetmatrix는 로드시에 미리 다 가져와놓느다. */
	for (_uint i = 0; i < pMeshContainer->iNumBones; ++i)
	{
		pMeshContainer->pOffsetMatrices[i] = *pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
	}

	/* 렌더링시마다 처리한다. OffsetMatrix * CombinedTransformationMatrix */
	pMeshContainer->pRenderingMatrices = new _matrix[pMeshContainer->iNumBones];

	/* 최초 한번. 현재 메시에 영향을 주고 있는 뼈들의 CombinedTransformationMatrix행렬의 주소를 보관시켜둘것이다. */
	//pMeshContainer->ppCombindTransformationMatrices = new _matrix*[pMeshContainer->iNumBones];

	pMeshContainer->MeshData.pMesh->CloneMesh(pMeshContainer->MeshData.pMesh->GetOptions(), Element, m_pGraphic_Device, &pMeshContainer->pOriginalMesh);

	*ppNewMeshContainer = pMeshContainer;

	return S_OK;
}

STDMETHODIMP CHierarchyLoader::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	if (false == ((D3DXFRAME_DERIVED*)pFrameToFree)->isCloned)
	{
		Safe_Delete_Array(pFrameToFree->Name);

		if (nullptr != pFrameToFree->pMeshContainer)
			DestroyMeshContainer(pFrameToFree->pMeshContainer);
	}

	if (nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	Safe_Delete(pFrameToFree);

	return S_OK;
}

STDMETHODIMP CHierarchyLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*	pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	for (_uint i = 0; i < pMeshContainer->NumMaterials; ++i)
	{
		Safe_Release(pMeshContainer->ppMaterialTextures[i]->pDiffuseMap);
		Safe_Release(pMeshContainer->ppMaterialTextures[i]->pSpecularMap);
		Safe_Release(pMeshContainer->ppMaterialTextures[i]->pNormalMap);

		Safe_Delete(pMeshContainer->ppMaterialTextures[i]);
	}
	Safe_Delete_Array(pMeshContainer->ppMaterialTextures);
	Safe_Release(pMeshContainer->pOriginalMesh);
	Safe_Delete_Array(pMeshContainer->pOffsetMatrices);
	Safe_Delete_Array(pMeshContainer->pRenderingMatrices);
	//Safe_Delete_Array(pMeshContainer->ppCombindTransformationMatrices);

	Safe_Delete_Array(pMeshContainer->Name);
	_ulong		dwRefCnt = 0;
	if (dwRefCnt = Safe_Release(pMeshContainer->MeshData.pMesh))
		int a = 10;
	Safe_Delete_Array(pMeshContainer->pAdjacency);
	Safe_Release(pMeshContainer->pSkinInfo);	

	Safe_Delete(pMeshContainer);

	return S_OK;
}

void CHierarchyLoader::Allocate_Name(const char * pSourName, char ** ppDestName)
{
	_uint	iLength = strlen(pSourName);

	*ppDestName = new char[iLength + 1];

	strcpy(*ppDestName, pSourName);
}

CHierarchyLoader * CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pMeshFilePath)
{
	CHierarchyLoader*	pInstance = new CHierarchyLoader(pGraphic_Device);
	
	if (FAILED(pInstance->NativeConstruct(pMeshFilePath)))
		return nullptr;

	return pInstance;
}

void CHierarchyLoader::Free()
{
	Safe_Release(m_pGraphic_Device);
}
