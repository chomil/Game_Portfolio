#pragma once

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CMesh_Dynamic final : public CMesh
{
private:
	explicit CMesh_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMesh_Dynamic(const CMesh_Dynamic& rhs);
	virtual ~CMesh_Dynamic() = default;
public:
	_uint Get_NumMeshContainer() const {
		return m_MeshContainers.size();
	}

	_uint Get_NumMaterials(_uint iMeshContainerIndex) const {
		return m_MeshContainers[iMeshContainerIndex]->NumMaterials;
	}

	const _matrix* Get_BoneMatrixPointer(const char* pBoneName) {
		return &((D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName))->CombinedTransformationMatrix;
	}

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar * pFilePath, const _tchar * pFileName);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	/* 뼈들의 TransformationMatrix를 누적시켜서 CombinedTransformationMatrix를 만든다. */
	void Update_CombinedTransformationMatrices(LPD3DXFRAME pFrame, _matrix ParentCombinedTransformationMatrix);

	/* 메시컨테이너들이 가지공 ㅣㅆ는 CombinedTransformatrixPointer 이차원배열에 값을 채원두ㅏㄴㄷ. */
	void SetUp_CombinedTransformationMatricesPointer(LPD3DXFRAME pFrame);
	void SetUp_CloneCombinedTransformationMatricesPointer(LPD3DXFRAME pFrame, _uint & iIndex);

	HRESULT SetUp_TextureOnShader(class CShader* pShader, D3DXHANDLE hParameter, MESHMATERIALTEXTURE::TYPE eType, _uint iMeshContainerIndex, _uint iMaterialIndex);

	HRESULT Update_SkinnedMesh(_uint iMeshContainerIndex);

	HRESULT Set_AnimationIndex(_uint iAnimIndex, bool bLock = false, bool bForce = false);
	HRESULT Play_Animation(_double TimeDelta);

	bool	Is_AnimLock();	
	_double Get_CurrentAnimTime();

	HRESULT Render(_uint iMeshContainerIndex, _uint iMaterialIndex);

private:
	LPD3DXFRAME				m_pRootFrame = nullptr;
	_matrix					m_PivotMatrix;
	class CAnimation*		m_pAnimation = nullptr;
	class CHierarchyLoader* m_pLoader = nullptr;

	vector<_matrix**>		m_CombinedTransfromationMatrixPointers;



private: /* 현재 모델을 구성하는 메시컨테이너들을 모아두낟. */
	vector<D3DXMESHCONTAINER_DERIVED*>		m_MeshContainers;

private:
	HRESULT Clone_Frame(D3DXFRAME_DERIVED * pOriginalFrame);

	void Clone_Frame(D3DXFRAME_DERIVED * pOriginalFrame, D3DXFRAME_DERIVED * pOut);


public:
	static CMesh_Dynamic* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath, const _tchar * pFileName);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END