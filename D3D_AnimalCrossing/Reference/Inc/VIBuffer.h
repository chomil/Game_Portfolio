#pragma once

/* ���� ������ ������ ������ Ŭ�������� �θ� �� �� */
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
	/* �������� ������ �� �ִ� �޸�. */
	// IDirect3DVertexBuffer9*
	LPDIRECT3DVERTEXBUFFER9				m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9				m_pIB = nullptr;

	/* �������� ����. */
	_uint		m_iStride = 0;
	_uint		m_iNumVertices = 0;
	_ulong		m_dwVBUsage = 0;
	_ulong		m_dwFVF = 0;
	_float3*	m_pVerticesPos = nullptr;

	/* �ε������� ����.*/
	_uint		m_iNumPrimitive = 0; /* �׸����� �ϴ� ������ ����. */
	_uint		m_iNumVerticesPerPrimitive = 0; /* �����ϳ��� �׸��µ� �ʿ��� ������ ����. */
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