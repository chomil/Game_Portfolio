#include "..\public\VIBuffer_Rect_Viewport.h"

CVIBuffer_Rect_Viewport::CVIBuffer_Rect_Viewport(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{

}

CVIBuffer_Rect_Viewport::CVIBuffer_Rect_Viewport(const CVIBuffer & rhs)
	: CVIBuffer(rhs)
{

}

HRESULT CVIBuffer_Rect_Viewport::NativeConstruct_Prototype(_float fLeftX, _float fTopY, _float fSizeX, _float fSizeY)
{

#pragma region VERTICES
	m_iNumVertices = 4;
	m_iStride = sizeof(VTXTEX_VP);
	m_dwVBUsage = 0;
	m_dwFVF = D3DFVF_XYZRHW | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
#pragma endregion


#pragma region INDICES
	m_iNumPrimitive = 2;
	m_iNumVerticesPerPrimitive = 3;
	m_dwIBUsage = 0;

	/* 2 or 4 */
	m_iOneIndexSize = 2;
#pragma endregion

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;	

	VTXTEX_VP*		pVertices = nullptr;

	/* 정점메모리공간에 접근할 수 있는 포인터를 얻어온다. */
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float4(fLeftX, fTopY, 0.f, 1.f);
	pVertices[0].vTexUV = _float2(0.f, 0.f);
	pVertices[1].vPosition = _float4(fLeftX + fSizeX, fTopY, 0.f, 1.f);
	pVertices[1].vTexUV = _float2(1.f, 0.f);
	pVertices[2].vPosition = _float4(fLeftX + fSizeX, fTopY + fSizeY, 0.f, 1.f);
	pVertices[2].vTexUV = _float2(1.f, 1.f);
	pVertices[3].vPosition = _float4(fLeftX, fTopY + fSizeY, 0.f, 1.f);
	pVertices[3].vTexUV = _float2(0.f, 1.f);		

	m_pVB->Unlock();


	_ushort*	pIndices = 0;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	((_ushort*)m_pIndices)[0] = pIndices[0] = 0;
	((_ushort*)m_pIndices)[1] = pIndices[1] = 1;
	((_ushort*)m_pIndices)[2] = pIndices[2] = 2;

	((_ushort*)m_pIndices)[3] = pIndices[3] = 0;
	((_ushort*)m_pIndices)[4] = pIndices[4] = 2;
	((_ushort*)m_pIndices)[5] = pIndices[5] = 3;

	m_pIB->Unlock();	

	return S_OK;
}

HRESULT CVIBuffer_Rect_Viewport::NativeConstruct(void * pArg)
{
	return S_OK;
}

CVIBuffer_Rect_Viewport * CVIBuffer_Rect_Viewport::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fLeftX, _float fTopY, _float fSizeX, _float fSizeY)
{
	CVIBuffer_Rect_Viewport*	pInstance = new CVIBuffer_Rect_Viewport(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(fLeftX, fTopY, fSizeX, fSizeY)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Rect_Viewport");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Rect_Viewport::Clone(void * pArg)
{
	CVIBuffer_Rect_Viewport*	pInstance = new CVIBuffer_Rect_Viewport(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Rect_Viewport");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Rect_Viewport::Free()
{
	__super::Free();


}
