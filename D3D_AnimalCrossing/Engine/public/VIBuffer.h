#pragma once

/* 각종 형태의 정점을 정의한 클래스들의 부모가 될 것 */
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;
public:
	_uint Get_NumPolygons() const {
		return m_iNumPrimitive;
	}

	void Get_PolygonVertexPositions(_uint iPolygonIndex, _float3* pPoints) const;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render_VIBuffer();




protected:
	/* 정점들을 보관할 수 있는 메모리. */
	// IDirect3DVertexBuffer9*
	LPDIRECT3DVERTEXBUFFER9				m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9				m_pIB = nullptr;

	/* 정점버퍼 관련. */
	_uint		m_iStride = 0;
	_uint		m_iNumVertices = 0;
	_ulong		m_dwVBUsage = 0;
	_ulong		m_dwFVF = 0;
	_float3*	m_pVerticesPos = nullptr;

	/* 인덱스버퍼 관련.*/
	_uint		m_iNumPrimitive = 0; /* 그릴려고 하는 도형의 갯수. */
	_uint		m_iNumVerticesPerPrimitive = 0; /* 도형하나를 그리는데 필요한 정점의 갯수. */
	_uint		m_iOneIndexSize = 0;
	_ulong		m_dwIBUsage = 0;
	void*		m_pIndices = nullptr;

protected:
	D3DPRIMITIVETYPE	m_ePrimitiveType = D3DPRIMITIVETYPE(0);


	


public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END