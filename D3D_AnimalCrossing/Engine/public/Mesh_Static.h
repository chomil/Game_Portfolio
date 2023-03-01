#pragma once

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CMesh_Static final : public CMesh
{
private:
	explicit CMesh_Static(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMesh_Static(const CMesh_Static& rhs);
	virtual ~CMesh_Static() = default;
public:
	_ulong Get_NumMaterials() const {
		return m_dwNumMaterials;
	}
	
public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar * pFilePath, const _tchar * pFileName);
	virtual HRESULT NativeConstruct(void* pArg) override;
public:
	HRESULT SetUp_TextureOnShader(class CShader* pShader, D3DXHANDLE hParameter, MESHMATERIALTEXTURE::TYPE eType, _uint iMaterialIndex);
	HRESULT Render(_uint iMaterialIndex);
	

private:
	LPD3DXBUFFER			m_pAdjacency = nullptr;

	
	_ulong					m_dwNumMaterials = 0;
	LPD3DXMESH				m_pMesh = nullptr;

	/* D3DXMATERIAL D3DXMATERIAL D3DXMATERIAL */
	LPD3DXBUFFER					m_pMaterials = nullptr;
	vector<MESHMATERIALTEXTURE*>	m_Materials;
	

public:
	static CMesh_Static* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END