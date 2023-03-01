#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;
public:
	D3DXPLANE Get_Plane(_float3& vPosition);

public:
	virtual HRESULT NativeConstruct_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ, _float fInterval);
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pHeightMapPath, _float fInterval);
	virtual HRESULT NativeConstruct(void* pArg) override;

private:
	_float		m_fInterval = 0.f;
	_uint		m_iNumVerticesX = 0;
	_uint		m_iNumVerticesZ = 0;



public:
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ, _float fInterval = 1.f);
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pHeightMapPath, _float fInterval = 1.f);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END