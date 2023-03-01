#include "..\public\VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{

}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer & rhs)
	: CVIBuffer(rhs)
{

}

HRESULT CVIBuffer_Rect::NativeConstruct_Prototype()
{

#pragma region VERTICES
	m_iNumVertices = 4;
	m_iStride = sizeof(VTXTEX);
	m_dwVBUsage = 0;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
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

	VTXTEX*		pVertices = nullptr;

	/* 정점메모리공간에 접근할 수 있는 포인터를 얻어온다. */
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);


	pVertices[0].vPosition = m_pVerticesPos[0] = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexUV = _float2(0.f, 0.f);
	pVertices[1].vPosition = m_pVerticesPos[1] = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexUV = _float2(1.f, 0.f);
	pVertices[2].vPosition = m_pVerticesPos[2] = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexUV = _float2(1.f, 1.f);
	pVertices[3].vPosition = m_pVerticesPos[3] = _float3(-0.5f, -0.5f, 0.f);
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

HRESULT CVIBuffer_Rect::NativeConstruct(void * pArg)
{
	return S_OK;
}

CVIBuffer_Rect * CVIBuffer_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Rect*	pInstance = new CVIBuffer_Rect(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Rect::Clone(void * pArg)
{
	CVIBuffer_Rect*	pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();


}
