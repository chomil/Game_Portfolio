#include "..\public\Transform.h"
#include "VIBuffer_Terrain.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{

}

_float3 CTransform::Get_Scale() const
{

	return _float3(D3DXVec3Length(&Get_State(CTransform::STATE_RIGHT)), D3DXVec3Length(&Get_State(CTransform::STATE_UP)), D3DXVec3Length(&Get_State(CTransform::STATE_LOOK)));
}

void CTransform::Set_State(STATE eState, _float3 vState)
{
	memcpy(&m_WorldMatrix.m[eState][0], vState, sizeof(_float3));
}

void CTransform::Set_Scale(_float fScaleX, _float fScaleY, _float fScaleZ)
{
	_float3 vRight, vUp, vLook;

	vRight = Get_State(CTransform::STATE_RIGHT);
	vUp = Get_State(CTransform::STATE_UP);
	vLook = Get_State(CTransform::STATE_LOOK);
	
	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * fScaleX);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * fScaleY);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * fScaleZ);
}

HRESULT CTransform::NativeConstruct_Prototype()
{
	D3DXMatrixIdentity(&m_WorldMatrix);
	
	// *D3DXVec3Normalize(&_float3()) * Speed * TimeDelta;

	return S_OK;
}

HRESULT CTransform::NativeConstruct(void * pArg)
{
	if(nullptr != pArg)
		memcpy(&m_TransformState, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	D3DXVec3Normalize(&vLook, &vLook);

	_float3		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition += vLook * m_TransformState.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Back_Straight(_float fTimeDelta)
{
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	D3DXVec3Normalize(&vLook, &vLook);

	_float3		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition -= vLook * m_TransformState.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

}

void CTransform::Walk_Left(_float fTimeDelta)
{
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);

	D3DXVec3Normalize(&vRight, &vRight);

	_float3		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition -= vRight * m_TransformState.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Walk_Right(_float fTimeDelta)
{
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);

	D3DXVec3Normalize(&vRight, &vRight);

	_float3		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition += vRight * m_TransformState.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Rotation_Axis(_float3 vAxis, _float fTimeDelta)
{
	_float3			vRight, vUp, vLook;

	vRight = Get_State(CTransform::STATE_RIGHT);
	vUp = Get_State(CTransform::STATE_UP);
	vLook = Get_State(CTransform::STATE_LOOK);

	_matrix			RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_TransformState.fRotationPerSec * fTimeDelta);

	/* 위치벡터를 이동시킨다. */
	// 위치벡터와 이동상태를 가진 행렬이존재한다. 

	/* 방향벡터를 회전한다. */
	// vRight * RotationMatrix;

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix));
	Set_State(CTransform::STATE_UP, *D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix));
	Set_State(CTransform::STATE_LOOK, *D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix));	
}
void CTransform::Rotation_Axis(_float fRadian, _float3 vAxis)
{
	_float3			vRight, vUp, vLook;

	vRight = Get_State(CTransform::STATE_RIGHT);
	vUp = Get_State(CTransform::STATE_UP);
	vLook = Get_State(CTransform::STATE_LOOK);

	_matrix			RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	/* 위치벡터를 이동시킨다. */
	// 위치벡터와 이동상태를 가진 행렬이존재한다. 

	/* 방향벡터를 회전한다. */
	// vRight * RotationMatrix;

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix));
	Set_State(CTransform::STATE_UP, *D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix));
	Set_State(CTransform::STATE_LOOK, *D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix));
}

void CTransform::SetUp_Rotation(_float3 vAxis, _float fRadian)
{
	_float3			vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);
	_float3			vScale = Get_Scale();

	vRight *= vScale.x;
	vUp *= vScale.y;
	vLook *= vScale.z;

	_matrix			RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	/* 위치벡터를 이동시킨다. */
	// 위치벡터와 이동상태를 가진 행렬이존재한다. 

	/* 방향벡터를 회전한다. */
	// vRight * RotationMatrix;

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix));
	Set_State(CTransform::STATE_UP, *D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix));
	Set_State(CTransform::STATE_LOOK, *D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix));	
}

void CTransform::Move_Direction(_float3 vDir, _float fTimeDelta)
{
	D3DXVec3Normalize(&vDir,&vDir);

	_float3 vPos =  Get_State(CTransform::STATE_POSITION) + (vDir * m_TransformState.fSpeedPerSec * fTimeDelta);
	Set_State(CTransform::STATE::STATE_POSITION, vPos);
}

void CTransform::Chase_Target(const CTransform * pTargetTransform, _float fTimeDelta)
{
	/* 어떤 객체를 따라가고 싶다. */	
	_float3		vPosition = Get_State(STATE_POSITION);

	

	_float3		vDir = pTargetTransform->Get_State(CTransform::STATE_POSITION) - vPosition;

	_float		fDistance = D3DXVec3Length(&vDir);

	D3DXVec3Normalize(&vDir, &vDir);

	if(fDistance >= 0.5f)
		vPosition += vDir * m_TransformState.fSpeedPerSec * fTimeDelta;	

	/* 사각형을 구성하는 정점(4개)의 위치에 갱신된 위치정보를 전달해주낟. */
	Set_State(CTransform::STATE_POSITION, vPosition);

	LookAt_Target(pTargetTransform);
}

void CTransform::Chase_Target(_float3 vTargetPos, _float fTimeDelta)
{
	if (false == m_isMoving)
		return;

	/* 어떤 객체를 따라가고 싶다. */
	_float3		vPosition = Get_State(STATE_POSITION);

	_float3		vDir = vTargetPos - vPosition;

	_float		fDistance = D3DXVec3Length(&vDir);

	
	

	D3DXVec3Normalize(&vDir, &vDir);

	if (fDistance >= 0.5f)
		vPosition += vDir * m_TransformState.fSpeedPerSec * fTimeDelta;
	else
		m_isMoving = false;

	/* 사각형을 구성하는 정점(4개)의 위치에 갱신된 위치정보를 전달해주낟. */
	Set_State(CTransform::STATE_POSITION, vPosition);

	LookAt_Target(vTargetPos);
}

void CTransform::LookAt_Target(const CTransform * pTargetTransform)
{
	_float3	vTargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vPosition = Get_State(CTransform::STATE_POSITION);

	_float3	vLook = vTargetPos - vPosition;
	D3DXVec3Normalize(&vLook, &vLook);
	vLook = vLook * Get_Scale().z;

	_float3	vRight;
	D3DXVec3Cross(&vRight, &Get_State(CTransform::STATE_UP), &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vRight = vRight * Get_Scale().x;


	Set_State(CTransform::STATE_LOOK, vLook);
	Set_State(CTransform::STATE_RIGHT, vRight);

}

void CTransform::LookAt_Target(_float3 vTargetPos)
{
	_float3 vPosition = Get_State(CTransform::STATE_POSITION);

	_float3	vLook = vTargetPos - vPosition;
	D3DXVec3Normalize(&vLook, &vLook);
	vLook = vLook * Get_Scale().z;

	_float3	vRight;
	D3DXVec3Cross(&vRight, &Get_State(CTransform::STATE_UP), &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vRight = vRight * Get_Scale().x;


	Set_State(CTransform::STATE_LOOK, vLook);
	Set_State(CTransform::STATE_RIGHT, vRight);
}

void CTransform::LookAt_Target(_float3 vTargetDir, float fTimeDelta)
{
	_float3 vLook = Get_State(CTransform::STATE_LOOK);

	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vTargetDir, &vTargetDir);
	_float3 vCross;
	D3DXVec3Cross(&vCross, &vLook, &vTargetDir);
	float fCos = D3DXVec3Dot(&vLook, &vTargetDir);
	if (fCos > 0.995)
	{
		vLook = vTargetDir * Get_Scale().z;

		_float3	vRight;
		D3DXVec3Cross(&vRight, &Get_State(CTransform::STATE_UP), &vLook);
		D3DXVec3Normalize(&vRight, &vRight);
		vRight = vRight * Get_Scale().x;


		Set_State(CTransform::STATE_LOOK, vLook);
		Set_State(CTransform::STATE_RIGHT, vRight);
		return;
	}
	if (vCross.y > 0)
	{
		Rotation_Axis(_float3(0.f, 1.f, 0.f), fTimeDelta);
	}
	else
	{
		Rotation_Axis(_float3(0.f, 1.f, 0.f), -fTimeDelta);
	}
}

void CTransform::Remove_Rotation()
{
	Set_State(CTransform::STATE_RIGHT, _float3(1.f, 0.f, 0.f) * Get_Scale().x);
	Set_State(CTransform::STATE_UP, _float3(0.f, 1.f, 0.f) * Get_Scale().y);
	Set_State(CTransform::STATE_LOOK, _float3(0.f, 0.f, 1.f) * Get_Scale().z);

}
bool CTransform::Stand_OnTerrain(CVIBuffer_Terrain * pVIBuffer, const _matrix* pTerrainWorldMatrix, _double TimeDelta)
{
	/* 지형버퍼의 로컬로 이동하자. */
	_float3		vWorldPos = Get_State(CTransform::STATE_POSITION);
	_matrix		TerrainWorldMatrixInv = *D3DXMatrixInverse(&TerrainWorldMatrixInv, nullptr, pTerrainWorldMatrix);
	_float3		vLocalPos = *D3DXVec3TransformCoord(&vLocalPos, &vWorldPos, &TerrainWorldMatrixInv);

	D3DXPLANE		Plane = pVIBuffer->Get_Plane(vLocalPos);

	/* 지형 로컬에서 플레이어가 지형위에 있을때의 xyz를 구하자. */
	/* 평면과 점과의 거리. */

	float fDistY = D3DXPlaneDotCoord(&Plane, &vLocalPos);
	vLocalPos.y = vLocalPos.y - fDistY + 0.0F;

	if (fDistY > 0)
	{
		Move_Direction(_float3(0.f, -1.f, 0.f), TimeDelta);

		vWorldPos = Get_State(CTransform::STATE_POSITION);
		vLocalPos = *D3DXVec3TransformCoord(&vLocalPos, &vWorldPos, &TerrainWorldMatrixInv);

		Plane = pVIBuffer->Get_Plane(vLocalPos);

		/* 지형 로컬에서 플레이어가 지형위에 있을때의 xyz를 구하자. */
		/* 평면과 점과의 거리. */

		fDistY = D3DXPlaneDotCoord(&Plane, &vLocalPos);
		vLocalPos.y = vLocalPos.y - fDistY + 0.0F;
		if (fDistY <= 0)
		{
			D3DXVec3TransformCoord(&vWorldPos, &vLocalPos, pTerrainWorldMatrix);
			Set_State(CTransform::STATE_POSITION, vWorldPos);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		D3DXVec3TransformCoord(&vWorldPos, &vLocalPos, pTerrainWorldMatrix);
		Set_State(CTransform::STATE_POSITION, vWorldPos);

		return true;
	}
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform*	pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*	pInstance = new CTransform(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;	
}


void CTransform::Free()
{
	__super::Free();
}
