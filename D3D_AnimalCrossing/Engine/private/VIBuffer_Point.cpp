#include "..\public\VIBuffer_Point.h"

CVIBuffer_Point::CVIBuffer_Point(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{

}

CVIBuffer_Point::CVIBuffer_Point(const CVIBuffer & rhs)
	: CVIBuffer(rhs)
{

}

HRESULT CVIBuffer_Point::NativeConstruct_Prototype()
{

#pragma region VERTICES
	m_iNumVertices = 1;
	m_iStride = sizeof(VTXPOINT);
	m_dwVBUsage = 0;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_POINTLIST;
#pragma endregion

	m_pVerticesPos = new _float3[m_iNumVertices];

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iStride * m_iNumVertices, m_dwVBUsage, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTXPOINT*		pVertices = nullptr;

	/* 정점메모리공간에 접근할 수 있는 포인터를 얻어온다. */
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = m_pVerticesPos[0] = _float3(0.f, 0.f, 0.f);
	pVertices[0].fSize = 1.f;
	pVertices[0].vTexUV = _float2(0.f, 0.f);

	m_pVB->Unlock();	

	return S_OK;
}

HRESULT CVIBuffer_Point::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Point::Render_VIBuffer()
{
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);
	//m_pGraphic_Device->SetStreamSource(1, m_pVBInstance, 0, sizeof(VTXMATRIX));

	m_pGraphic_Device->SetFVF(m_dwFVF);

	m_pGraphic_Device->DrawPrimitive(m_ePrimitiveType, 0, 1);

	return S_OK;
}

CVIBuffer_Point * CVIBuffer_Point::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Point*	pInstance = new CVIBuffer_Point(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CVIBuffer_Point");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Point::Clone(void * pArg)
{
	CVIBuffer_Point*	pInstance = new CVIBuffer_Point(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Point");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Point::Free()
{
	__super::Free();


}
