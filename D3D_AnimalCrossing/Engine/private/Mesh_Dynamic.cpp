#include "..\public\Mesh_Dynamic.h"
#include "HierarchyLoader.h"
#include "Shader.h"

#include "Animation.h"

CMesh_Dynamic::CMesh_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMesh(pGraphic_Device)
{

}

CMesh_Dynamic::CMesh_Dynamic(const CMesh_Dynamic& rhs)
	: CMesh(rhs)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_MeshContainers(rhs.m_MeshContainers)
	, m_pAnimation(rhs.m_pAnimation->Clone())
	, m_pLoader(rhs.m_pLoader)
{
	/* 정점정보는 원본을 그대로 유지. */
	/* 뼈의 정보만 복제. 애니메이션컨트롤러 복제.  */
	Safe_AddRef(m_pLoader);

	_uint iNumMeshContainer = m_MeshContainers.size();

	for (_uint i = 0; i < iNumMeshContainer; ++i)
	{
		Safe_AddRef(m_MeshContainers[i]->MeshData.pMesh);
		Safe_AddRef(m_MeshContainers[i]->pOriginalMesh);
		for (_uint j = 0; j < m_MeshContainers[i]->NumMaterials; ++j)
		{
			Safe_AddRef(m_MeshContainers[i]->ppMaterialTextures[j]->pDiffuseMap);
			Safe_AddRef(m_MeshContainers[i]->ppMaterialTextures[j]->pSpecularMap);
			Safe_AddRef(m_MeshContainers[i]->ppMaterialTextures[j]->pNormalMap);
		}
	}
}

HRESULT CMesh_Dynamic::NativeConstruct_Prototype(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar		szFullPath[MAX_PATH] = TEXT("");

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphic_Device, pFilePath);

	LPD3DXANIMATIONCONTROLLER	pAnimationController = nullptr;

	/* 메시에 포함되어있는 프레임(뼈 : 계층구조)의 정보를 함께 불러들인다. */
	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphic_Device,
		m_pLoader, nullptr, &m_pRootFrame, &pAnimationController)))
		return E_FAIL;

	m_pAnimation = CAnimation::Create(m_pGraphic_Device, pAnimationController);
	if (nullptr == m_pAnimation)
		return E_FAIL;

	D3DXMatrixRotationY(&m_PivotMatrix, D3DXToRadian(180.0f));

	Update_CombinedTransformationMatrices(m_pRootFrame, m_PivotMatrix);

	SetUp_CombinedTransformationMatricesPointer(m_pRootFrame);

	return S_OK;
}

HRESULT CMesh_Dynamic::NativeConstruct(void* pArg)
{
	/* 1. 애니메이션이 재생될경우 Transformation행렬이 갱신. PlayAnimation함수를 봐. */
	/* 2. UpdasteCombinedTransform함술르 호출해서 컴바인드 행렬을 갱신. */
	/* 3. 복제된 객체수만큼의 뼈의 정보들이필요햇다. */



	/* 컴바인드행렬의 주소를 담기위한 배열을 메시컨테이너 갯수만큼 확보한다. */
	m_CombinedTransfromationMatrixPointers.resize(m_MeshContainers.size());

	/* 뼈를 복제하낟. */
	D3DXFRAME_DERIVED* pOriginalFrame = (D3DXFRAME_DERIVED*)m_pRootFrame;

	/* m_pRootFrame에 원본뼏르을을 복샇ㅇ령 담는다.  */
	if (FAILED(Clone_Frame(pOriginalFrame)))
		return E_FAIL;

	/* 렌더링할때 UpdateSkinnedMesh함수를 호출한다. */
	/* UpdateSkinnedMesh : 내가 그릴려고하는 메시(정점으로이루어져있다.)에 뼈들의 컴바인드 행렬의 상태를 곱하여. 정점들을 모두 뼈의 상태로 갱사ㅣㄴ하다. */
	/* 뼈의 컴바인드를 찾아서 가져오는 형태가 아니었다고. 로드시에 현재 메시에 영향을 미치는 뼏르의 컴바인드행렬의 주소를 담아놓는 배열을 미리 ㅁ나들고. */
	/* UpdateSkinnedMesh에서 저장해놓은 주소를 따라가서 행렬르 ㄹ가져오는형태 .*/
	/* 그 뼈들이 메시_다이나믹마다 각각존재했다. */
	/* 각각보관하기위한 작업이 필요했다. */
	/* 그래서 이변수를 메시_다이나믹에다가 각각 선언해놓은거야. */
	/* 공간할당한거고. */
	for (_uint i = 0; i < m_MeshContainers.size(); ++i)
	{
		m_CombinedTransfromationMatrixPointers[i] = new _matrix * [m_MeshContainers[i]->iNumBones];
	}

	_uint		iIndex = 0;

	/* 이게 그거야. */
	/* 현재 메시에 영햐을 미치는 뼈들의 컴바인드행렬의 주소를 찾아서 위에서 확보해놓은 배열에 주소를 담아놓는다. */
	SetUp_CloneCombinedTransformationMatricesPointer(m_pRootFrame, iIndex);

	/* */
	if (m_pAnimation != nullptr)
	{
		m_pAnimation->Bind_Frames((D3DXFRAME_DERIVED*)m_pRootFrame);
	}

	return S_OK;
}

/* 애니메이션이 재생된다면 매프레임마다 호출되는 함수. */
/* 현재 애니메이션 재생값에따라 TransformationMatrix를 갱신하기때문 */
void CMesh_Dynamic::Update_CombinedTransformationMatrices(LPD3DXFRAME pFrame, _matrix ParentCombinedTransformationMatrix)
{
	D3DXFRAME_DERIVED* pFrame_Derived = (D3DXFRAME_DERIVED*)pFrame;

	pFrame_Derived->CombinedTransformationMatrix = pFrame_Derived->TransformationMatrix * ParentCombinedTransformationMatrix;

	if (nullptr != pFrame_Derived->pFrameFirstChild)
		Update_CombinedTransformationMatrices(pFrame_Derived->pFrameFirstChild, pFrame_Derived->CombinedTransformationMatrix);

	if (nullptr != pFrame_Derived->pFrameSibling)
		Update_CombinedTransformationMatrices(pFrame_Derived->pFrameSibling, ParentCombinedTransformationMatrix);

}

void CMesh_Dynamic::SetUp_CombinedTransformationMatricesPointer(LPD3DXFRAME pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED* pMeshContainer_Derived = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		m_MeshContainers.push_back(pMeshContainer_Derived);

		for (_uint i = 0; i < pMeshContainer_Derived->iNumBones; ++i)
		{
			/* 현재 메시에 영향을 미치는 뼈들 중, i번째 뼈의 이름을 리턴해주낟. */
			const char* pBoneName = pMeshContainer_Derived->pSkinInfo->GetBoneName(i);

			D3DXFRAME_DERIVED* pFindFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);
			//pMeshContainer_Derived->ppCombindTransformationMatrices[i] = &pFindFrame->CombinedTransformationMatrix;
		}
	}

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_CombinedTransformationMatricesPointer(pFrame->pFrameFirstChild);

	if (nullptr != pFrame->pFrameSibling)
		SetUp_CombinedTransformationMatricesPointer(pFrame->pFrameSibling);
}

void CMesh_Dynamic::SetUp_CloneCombinedTransformationMatricesPointer(LPD3DXFRAME pFrame, _uint& iIndex)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED* pMeshContainer_Derived = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_uint i = 0; i < pMeshContainer_Derived->iNumBones; ++i)
		{
			/* 현재 메시에 영향을 미치는 뼈들 중, i번째 뼈의 이름을 리턴해주낟. */
			const char* pBoneName = pMeshContainer_Derived->pSkinInfo->GetBoneName(i);

			D3DXFRAME_DERIVED* pFindFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);

			m_CombinedTransfromationMatrixPointers[iIndex][i] = &pFindFrame->CombinedTransformationMatrix;
			// pMeshContainer_Derived->ppCombindTransformationMatrices[i] = &pFindFrame->CombinedTransformationMatrix;
		}

		++iIndex;
	}



	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_CloneCombinedTransformationMatricesPointer(pFrame->pFrameFirstChild, iIndex);

	if (nullptr != pFrame->pFrameSibling)
		SetUp_CloneCombinedTransformationMatricesPointer(pFrame->pFrameSibling, iIndex);
}


HRESULT CMesh_Dynamic::SetUp_TextureOnShader(CShader* pShader, D3DXHANDLE hParameter, MESHMATERIALTEXTURE::TYPE eType, _uint iMeshContainerIndex, _uint iMaterialIndex)
{
	LPDIRECT3DTEXTURE9		pTexture = nullptr;

	switch (eType)
	{
	case MeshMaterialTexture::TYPE_DIFFUSE:
		pTexture = m_MeshContainers[iMeshContainerIndex]->ppMaterialTextures[iMaterialIndex]->pDiffuseMap;
		break;
	case MeshMaterialTexture::TYPE_NORMAL:
		pTexture = m_MeshContainers[iMeshContainerIndex]->ppMaterialTextures[iMaterialIndex]->pNormalMap;
		break;
	case MeshMaterialTexture::TYPE_SPECULAR:
		pTexture = m_MeshContainers[iMeshContainerIndex]->ppMaterialTextures[iMaterialIndex]->pSpecularMap;
		break;
	}

	if (FAILED(pShader->SetUp_TextureConstantTable(hParameter, pTexture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMesh_Dynamic::Update_SkinnedMesh(_uint iMeshContainerIndex)
{
	for (_uint i = 0; i < m_MeshContainers[iMeshContainerIndex]->iNumBones; ++i)
	{
		m_MeshContainers[iMeshContainerIndex]->pRenderingMatrices[i] =
			m_MeshContainers[iMeshContainerIndex]->pOffsetMatrices[i]
			* *m_CombinedTransfromationMatrixPointers[iMeshContainerIndex][i];
	}

	void* pSour = nullptr, * pDest = nullptr;

	m_MeshContainers[iMeshContainerIndex]->pOriginalMesh->LockVertexBuffer(0, &pSour);
	m_MeshContainers[iMeshContainerIndex]->MeshData.pMesh->LockVertexBuffer(0, &pDest);

	m_MeshContainers[iMeshContainerIndex]->pSkinInfo->UpdateSkinnedMesh(
		m_MeshContainers[iMeshContainerIndex]->pRenderingMatrices, nullptr, pSour, pDest);

	m_MeshContainers[iMeshContainerIndex]->pOriginalMesh->UnlockVertexBuffer();
	m_MeshContainers[iMeshContainerIndex]->MeshData.pMesh->UnlockVertexBuffer();

	return S_OK;
}

HRESULT CMesh_Dynamic::Set_AnimationIndex(_uint iAnimIndex, bool bLock, bool bForce)
{
	if (nullptr == m_pAnimation)
		return E_FAIL;

	return m_pAnimation->Set_AnimationIndex(iAnimIndex, bLock, bForce);
}

HRESULT CMesh_Dynamic::Play_Animation(_double TimeDelta)
{
	if (nullptr == m_pAnimation)
		return E_FAIL;

	/* 뼈ㅛ들의 트랜스포메이션 =행렬을 갱신. */
	if (FAILED(m_pAnimation->Play_Animation(TimeDelta)))
		return E_FAIL;

	/* 트랜스포메이션 행렬을 누적해서 컴바인드 행렬 갱신. */
	Update_CombinedTransformationMatrices(m_pRootFrame, m_PivotMatrix);

	return S_OK;

}

bool CMesh_Dynamic::Is_AnimLock()
{
	return	m_pAnimation->Is_AnimLock();
}

_double CMesh_Dynamic::Get_CurrentAnimTime()
{
	return m_pAnimation->Get_CurrentAnimTime();
}



HRESULT CMesh_Dynamic::Render(_uint iMeshContainerIndex, _uint iMaterialIndex)
{
	_uint		iNumMeshContainer = m_MeshContainers.size();

	m_MeshContainers[iMeshContainerIndex]->MeshData.pMesh->DrawSubset(iMaterialIndex);

	return S_OK;
}



HRESULT CMesh_Dynamic::Clone_Frame(D3DXFRAME_DERIVED* pOriginalFrame)
{

	m_pRootFrame = new D3DXFRAME_DERIVED;
	memcpy(m_pRootFrame, pOriginalFrame, sizeof(D3DXFRAME_DERIVED));

	((D3DXFRAME_DERIVED*)m_pRootFrame)->isCloned = true;

	if (nullptr != pOriginalFrame->pFrameFirstChild)
	{
		m_pRootFrame->pFrameFirstChild = new D3DXFRAME_DERIVED;
		Clone_Frame((D3DXFRAME_DERIVED*)pOriginalFrame->pFrameFirstChild, (D3DXFRAME_DERIVED*)m_pRootFrame->pFrameFirstChild);
	}

	if (nullptr != pOriginalFrame->pFrameSibling)
	{
		m_pRootFrame->pFrameSibling = new D3DXFRAME_DERIVED;
		Clone_Frame((D3DXFRAME_DERIVED*)pOriginalFrame->pFrameSibling, (D3DXFRAME_DERIVED*)m_pRootFrame->pFrameSibling);
	}

	return S_OK;
}

void CMesh_Dynamic::Clone_Frame(D3DXFRAME_DERIVED* pOriginalFrame, D3DXFRAME_DERIVED* pOut)
{
	memcpy(pOut, pOriginalFrame, sizeof(D3DXFRAME_DERIVED));

	pOut->isCloned = true;

	if (nullptr != pOriginalFrame->pFrameFirstChild)
	{
		pOut->pFrameFirstChild = new D3DXFRAME_DERIVED;
		Clone_Frame((D3DXFRAME_DERIVED*)pOriginalFrame->pFrameFirstChild, (D3DXFRAME_DERIVED*)pOut->pFrameFirstChild);
	}

	if (nullptr != pOriginalFrame->pFrameSibling)
	{
		pOut->pFrameSibling = new D3DXFRAME_DERIVED;
		Clone_Frame((D3DXFRAME_DERIVED*)pOriginalFrame->pFrameSibling, (D3DXFRAME_DERIVED*)pOut->pFrameSibling);
	}


}




CMesh_Dynamic* CMesh_Dynamic::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName)
{
	CMesh_Dynamic* pInstance = new CMesh_Dynamic(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(pFilePath, pFileName)))
	{
		MSGBOX("Failed to Creating CMesh_Dynamic");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CMesh_Dynamic::Clone(void* pArg)
{
	CMesh_Dynamic* pInstance = new CMesh_Dynamic(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CMesh_Dynamic");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMesh_Dynamic::Free()
{
	__super::Free();

	_uint iNumMeshContainer = m_MeshContainers.size();

	for (_uint i = 0; i < iNumMeshContainer; ++i)
	{
		Safe_Release(m_MeshContainers[i]->MeshData.pMesh);
		Safe_Release(m_MeshContainers[i]->pOriginalMesh);
		for (_uint j = 0; j < m_MeshContainers[i]->NumMaterials; ++j)
		{
			Safe_Release(m_MeshContainers[i]->ppMaterialTextures[j]->pDiffuseMap);
			Safe_Release(m_MeshContainers[i]->ppMaterialTextures[j]->pSpecularMap);
			Safe_Release(m_MeshContainers[i]->ppMaterialTextures[j]->pNormalMap);
		}
	}

	m_MeshContainers.clear();

	m_pLoader->DestroyFrame(m_pRootFrame);

	for (auto& ppCombinedMatrixPointers : m_CombinedTransfromationMatrixPointers)
		Safe_Delete_Array(ppCombinedMatrixPointers);
	m_CombinedTransfromationMatrixPointers.clear();

	Safe_Release(m_pLoader);

	Safe_Release(m_pAnimation);






}
