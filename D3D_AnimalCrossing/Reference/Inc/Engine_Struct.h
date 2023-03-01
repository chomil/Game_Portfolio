#pragma once

namespace Engine
{
	typedef struct MeshMaterialTexture
	{
		enum TYPE { TYPE_DIFFUSE, TYPE_NORMAL, TYPE_SPECULAR, TYPE_END };
		LPDIRECT3DTEXTURE9		pDiffuseMap = nullptr;
		LPDIRECT3DTEXTURE9		pNormalMap = nullptr;
		LPDIRECT3DTEXTURE9		pSpecularMap = nullptr;
	}MESHMATERIALTEXTURE;


	typedef struct D3DXMeshContainerDerived : public D3DXMESHCONTAINER
	{
		MESHMATERIALTEXTURE**	ppMaterialTextures = nullptr;

		LPD3DXMESH				pOriginalMesh = nullptr;

		/* 현재 메시컨테이너에 영향을 미치는 뼈의 갯수*/
		unsigned int			iNumBones = 0;
		/* 뼈의 스페이스에서 로컬 스페이스로 이동시키기위한 행렬. */
		D3DXMATRIX*				pOffsetMatrices = nullptr;
		/* 화면에 그리기위한 행렬(OffsetMatrix * CombinedTransformationMatrix */
		D3DXMATRIX*				pRenderingMatrices = nullptr;
		/* 뼈의 TransformationMAtrix를 누적시킨 행렬이다. */
		//D3DXMATRIX**			ppCombindTransformationMatrices = nullptr;		

	}D3DXMESHCONTAINER_DERIVED;

	/* 메시를 구성하느 ㄴ뼈하나의 정보. */
	typedef struct D3DXFrame_Derived : public D3DXFRAME
	{
		bool			isCloned = false;
		/* TransformationMatrix * 부모 뼈대의 CombinedTransformationMatrix*/
		D3DXMATRIX		CombinedTransformationMatrix;
	}D3DXFRAME_DERIVED;

	typedef struct tagVertexMeshTexture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexUV;
		/*unsigned int	BoneIndex[4]; 0		1		3	8
		D3DXVECTOR4		BlendWeight;  0.2   0.3   0.4  0.1*/
	}VTXMESHTEX; 

	
	typedef struct tagVertexTexture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexUV;
	}VTXTEX;

	typedef struct tagVertexPointTexture
	{
		D3DXVECTOR3		vPosition;
		float			fSize;
		D3DXVECTOR2		vTexUV;
	}VTXPOINT;
	typedef struct tagVertexTexture_Viewport
	{
		D3DXVECTOR4		vPosition;
		D3DXVECTOR2		vTexUV;
	}VTXTEX_VP;

	typedef struct tagVertexCubeTexture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vTexUV;		
	}VTXCUBETEX;

	typedef struct tagVertexNormalTexture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR2		vTexUV;		
	}VTXNORTEX;

	
	
}


