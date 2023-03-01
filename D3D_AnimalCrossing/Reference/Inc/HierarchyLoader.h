#pragma once

#include "Base.h"

BEGIN(Engine)

class CHierarchyLoader final : public ID3DXAllocateHierarchy, public CBase
{
public:
	explicit CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CHierarchyLoader() = default;
public:
	HRESULT NativeConstruct(const _tchar* pMeshFilePath);
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,
		LPD3DXFRAME *ppNewFrame) override;
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer) override;
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override;
public:
	void Allocate_Name(const char* pSourName, char** ppDestName);

private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	const _tchar*					m_pMeshFilePath = nullptr;
public:
	static CHierarchyLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pMeshFilePath);
	virtual void Free() override;
};

END