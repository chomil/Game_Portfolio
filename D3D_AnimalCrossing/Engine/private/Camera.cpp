#include "..\public\Camera.h"
#include "PipeLine.h"
#include "Transform.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_pPipeLine(CPipeLine::Get_Instance())
{
	Safe_AddRef(m_pPipeLine);
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
	, m_pPipeLine(rhs.m_pPipeLine)
{
	Safe_AddRef(m_pPipeLine);
}

HRESULT CCamera::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CCamera::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	if (nullptr != pArg)
		memcpy(&m_CameraDesc, pArg, sizeof(CCamera::CAMERADESC));

	_float3		vLook = (m_CameraDesc.vAt - m_CameraDesc.vEye);
	D3DXVec3Normalize(&vLook, &vLook);

	_float3		vRight;
	D3DXVec3Cross(&vRight, &m_CameraDesc.vAxisY, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	_float3		vUp;
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	/* ViewMatrix : 카메라의 월드스페이스 변환 행렬의 역행렬이다. */
	m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransform->Set_State(CTransform::STATE_UP, vUp);
	m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);
	m_pTransform->Set_State(CTransform::STATE_POSITION, m_CameraDesc.vEye);

	Set_Transform();

	return S_OK;
}

_int CCamera::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	
	Set_Transform();

	return _int();
}

_int CCamera::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	return _int();
}

HRESULT CCamera::Render()
{
	return S_OK;
}


HRESULT CCamera::Set_Transform()
{
	_matrix		ProjMatrix;

	m_pPipeLine->Set_Transform(D3DTS_VIEW, m_pTransform->Get_WorldMatrixInverse());

	D3DXMatrixPerspectiveFovLH(&ProjMatrix, m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	m_pPipeLine->Set_Transform(D3DTS_PROJECTION, ProjMatrix);
	return S_OK;
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pPipeLine);

}
