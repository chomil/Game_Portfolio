#include "..\public\Picking.h"
#include "PipeLine.h"
#include "VIBuffer.h"

CPicking::CPicking(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

HRESULT CPicking::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPicking::Compute_MouseCursorPosInWorld(HWND hWnd)
{
	_float4		vMousePos;

	POINT		ptMouse = { 0, 0 };

	GetCursorPos(&ptMouse);

	/* 뷰포트 스페이스 상의 마우스 위치. */
	ScreenToClient(hWnd, &ptMouse);

	/* 투영스페이스까지 변환. */
	D3DVIEWPORT9			ViewPort;
	m_pGraphic_Device->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;
	vMousePos.w = 1.f;

	/* 나누어ㅏㅆ던 W(VIEW Z) 값을 다시 곱해준다. */

	/* 뷰스페이스까지 변환. */
	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	_matrix			ProjMatrix;
	ProjMatrix = pPipeLine->Get_Transform(D3DTS_PROJECTION);
	D3DXVec4Transform(&vMousePos, &vMousePos, D3DXMatrixInverse(&ProjMatrix, nullptr, &ProjMatrix));
	

	m_vMouseRay = _float3(vMousePos.x - 0.f, vMousePos.y - 0.f, vMousePos.z - 0.f);
	m_vMousePivot = _float3(0.f, 0.f, 0.f);

	/* 월드스페이스까지 변환. */
	_matrix			ViewMatrix;
	ViewMatrix = pPipeLine->Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	D3DXVec3TransformCoord(&m_vMousePivot, &m_vMousePivot, &ViewMatrix);
	D3DXVec3TransformNormal(&m_vMouseRay, &m_vMouseRay, &ViewMatrix);

	RELEASE_INSTANCE(CPipeLine);

	return S_OK;
}

_float3* CPicking::Compute_PickingPoint(CVIBuffer* pVIBuffer, _matrix WorldMatrix)
{
	_float3		vRay, vPivot;

	_matrix		WorldMatrixInverse = *D3DXMatrixInverse(&WorldMatrixInverse, nullptr, &WorldMatrix);

	/* 지형 로컬스페이스에 있는 레이와 피벗을 구한다. */
	D3DXVec3TransformCoord(&vPivot, &m_vMousePivot, &WorldMatrixInverse);
	D3DXVec3TransformNormal(&vRay, &m_vMouseRay, &WorldMatrixInverse);

	_float3		vPoint[3];

	for (_uint i = 0; i < pVIBuffer->Get_NumPolygons(); ++i)
	{
		pVIBuffer->Get_PolygonVertexPositions(i, vPoint);

		_float		fU, fV, fDist;
		if (true == D3DXIntersectTri(&vPoint[0], &vPoint[1], &vPoint[2], &vPivot, &vRay, &fU, &fV, &fDist))
		{
			m_vResultPos = vPivot + *D3DXVec3Normalize(&vRay, &vRay) * fDist;

			return &m_vResultPos;
		}		
	}

	return nullptr;	
}

CPicking * CPicking::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPicking*	pInstance = new CPicking(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CPicking");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CPicking::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CPicking::Free()
{
	__super::Free();


}
