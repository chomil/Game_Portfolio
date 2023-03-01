#include "..\public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{

}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_fInterval(rhs.m_fInterval)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
	
{

}

D3DXPLANE CVIBuffer_Terrain::Get_Plane(_float3 & vPosition)
{
	_uint	iIndex = _int(vPosition.z / m_fInterval) * m_iNumVerticesX + _int(vPosition.x / m_fInterval);

	_uint	iIndices[4] = {
		iIndex + m_iNumVerticesX,
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1,
		iIndex		
	};

	_float		fRatioX = (vPosition.x - m_pVerticesPos[iIndices[0]].x) / m_fInterval;
	_float		fRatioZ = (m_pVerticesPos[iIndices[0]].z - vPosition.z) / m_fInterval;

	D3DXPLANE			Plane;

	/* 우상단 */
	if (fRatioX > fRatioZ)
	{		
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[1]], &m_pVerticesPos[iIndices[2]]);		
	}

	/* 좌하단 */
	else
	{
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[2]], &m_pVerticesPos[iIndices[3]]);
	}

	return Plane;
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ, _float fInterval)
{

#pragma region VERTICES
	m_iNumVertices = iNumVerticesX * iNumVerticesZ;
	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;
	m_fInterval = fInterval;
	m_iStride = sizeof(VTXNORTEX);
	m_dwVBUsage = 0;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
#pragma endregion

#pragma region INDICES
	m_iNumPrimitive = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;
	m_iNumVerticesPerPrimitive = 3;
	m_dwIBUsage = 0;

	/* 2 or 4 */
	m_iOneIndexSize = 4;
#pragma endregion

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;	

	m_pVerticesPos = new _float3[m_iNumVertices];
	ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

	VTXNORTEX*		pVertices = nullptr;

	/* 정점메모리공간에 접근할 수 있는 포인터를 얻어온다. */
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (_uint i = 0; i < iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < iNumVerticesX; ++j)
		{
			_uint iIndex = i * iNumVerticesX + j;

			pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3(j * fInterval, 0.0f, i * fInterval);
			pVertices[iIndex].vNormal = _float3(0.0f, 1.f, 0.f);
			pVertices[iIndex].vTexUV = _float2(_float(j) / (iNumVerticesX - 1) * 20.f, _float(i) / (iNumVerticesZ - 1) * 20.f);
		}
	}

	m_pVB->Unlock();


	_uint*	pIndices = 0;

	_uint		iVertexIndex = 0;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (_uint i = 0; i < iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * iNumVerticesX + j;

			// 우상단 삼각형. 
			pIndices[iVertexIndex++] = iIndex + iNumVerticesX;
			pIndices[iVertexIndex++] = iIndex + iNumVerticesX + 1;
			pIndices[iVertexIndex++] = iIndex + 1;


			// 좌하단 삼각형. 
			pIndices[iVertexIndex++] = iIndex + iNumVerticesX;
			pIndices[iVertexIndex++] = iIndex + 1;
			pIndices[iVertexIndex++] = iIndex;
		}
	}

	m_pIB->Unlock();

	

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(const _tchar * pHeightMapPath, _float fInterval)
{
	_uint			iNumVerticesX = 0;
	_uint			iNumVerticesZ = 0;

	HANDLE		hFile = CreateFile(pHeightMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;


	_ulong		dwByte = 0;

	BITMAPFILEHEADER		fh;
	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
		
	BITMAPINFOHEADER		ih;
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	iNumVerticesX = ih.biWidth;
	iNumVerticesZ = ih.biHeight;

	_ulong*			pPixel = new _ulong[iNumVerticesX * iNumVerticesZ];

	ReadFile(hFile, pPixel, sizeof(_ulong) * iNumVerticesX * iNumVerticesZ, &dwByte, nullptr);

	CloseHandle(hFile);
	
#pragma region VERTICES
	m_iNumVertices = iNumVerticesX * iNumVerticesZ;
	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;
	m_fInterval = fInterval;
	m_iStride = sizeof(VTXNORTEX);
	m_dwVBUsage = 0;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
#pragma endregion

#pragma region INDICES
	m_iNumPrimitive = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;
	m_iNumVerticesPerPrimitive = 3;
	m_dwIBUsage = 0;

	/* 2 or 4 */
	m_iOneIndexSize = 4;
#pragma endregion

	/* 정점버퍼와 인덱스버퍼의 공간을 할당한다. */
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	VTXNORTEX*		pVertices = nullptr;

	/* 정점메모리공간에 접근할 수 있는 포인터를 얻어온다. */
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	srand(0);
	for (_uint i = 0; i < iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < iNumVerticesX; ++j)
		{
			_uint iIndex = i * iNumVerticesX + j;
			float fHeight = (pPixel[iIndex] & 0x000000ff) / 100.f;
			if (fHeight > 2.5f)
			{
				fHeight = 2.5f;
			}
			//fHeight += float(rand() % 100) / 1000.f;

			m_pVerticesPos[iIndex] = pVertices[iIndex].vPosition = _float3(j * fInterval, fHeight, i * fInterval);
			pVertices[iIndex].vNormal = _float3(0.0f, 0.f, 0.f);
			D3DXVec3Normalize(&pVertices[iIndex].vNormal, &pVertices[iIndex].vNormal);
			pVertices[iIndex].vTexUV = _float2(_float(j) / (iNumVerticesX - 1), _float(i) / (iNumVerticesZ - 1));
		}
	}

	_uint*	pIndices = 0;

	_uint		iVertexIndex = 0;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (_uint i = 0; i < iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * iNumVerticesX + j;

			// 우상단 삼각형. 
			pIndices[iVertexIndex++] = ((_uint*)m_pIndices)[iVertexIndex] = iIndex + iNumVerticesX;
			pIndices[iVertexIndex++] = ((_uint*)m_pIndices)[iVertexIndex] = iIndex + iNumVerticesX + 1;
			pIndices[iVertexIndex++] = ((_uint*)m_pIndices)[iVertexIndex] = iIndex + 1;

			_float3	vNormal, vSour, vDest;
			
			vSour = pVertices[iIndex + 1].vPosition - pVertices[iIndex + iNumVerticesX + 1].vPosition;
			vDest = pVertices[iIndex + iNumVerticesX + 1].vPosition - pVertices[iIndex + iNumVerticesX].vPosition;

			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			D3DXVec3Normalize(&vNormal, &vNormal);

			pVertices[iIndex + iNumVerticesX].vNormal += vNormal;
			pVertices[iIndex + iNumVerticesX + 1].vNormal += vNormal;
			pVertices[iIndex + 1].vNormal += vNormal;


			// 좌하단 삼각형. 
			pIndices[iVertexIndex++] = ((_uint*)m_pIndices)[iVertexIndex] = iIndex + iNumVerticesX;
			pIndices[iVertexIndex++] = ((_uint*)m_pIndices)[iVertexIndex] = iIndex + 1;
			pIndices[iVertexIndex++] = ((_uint*)m_pIndices)[iVertexIndex] = iIndex;

			vSour = pVertices[iIndex].vPosition - pVertices[iIndex + 1].vPosition;
			vDest = pVertices[iIndex + 1].vPosition - pVertices[iIndex + iNumVerticesX].vPosition;

			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			D3DXVec3Normalize(&vNormal, &vNormal);

			pVertices[iIndex + iNumVerticesX].vNormal += vNormal;
			pVertices[iIndex + 1].vNormal += vNormal;
			pVertices[iIndex].vNormal += vNormal;

		}
	}

	for (_uint i = 0; i < iNumVerticesX * iNumVerticesZ; ++i)
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);


	m_pVB->Unlock();
	m_pIB->Unlock();

	Safe_Delete_Array(pPixel);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct(void * pArg)
{
	return S_OK;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ, _float fInterval)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(iNumVerticesX, iNumVerticesZ, fInterval)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pHeightMapPath, _float fInterval)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(pHeightMapPath, fInterval)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();

	


}
