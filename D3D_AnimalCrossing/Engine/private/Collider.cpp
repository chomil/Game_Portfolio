#include "..\public\Collider.h"
#include "Shader.h"
#include "PipeLine.h"
#include "Transform.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_eType(rhs.m_eType)
	, m_pTransform((CTransform*)rhs.m_pTransform->Clone())
	, m_vMin(rhs.m_vMin)
	, m_vMax(rhs.m_vMax)
	#ifdef _DEBUG
	, m_pShader(rhs.m_pShader)
#endif
{
	Safe_AddRef(m_pMesh);

#ifdef _DEBUG
	Safe_AddRef(m_pShader);
#endif

	memcpy(m_vPoints, rhs.m_vPoints, sizeof(_float3) * 8);
		
}

HRESULT CCollider::NativeConstruct_Prototype(COLLIDER  eType)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;	

	m_pTransform = CTransform::Create(m_pGraphic_Device);
	if (nullptr == m_pTransform)
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(Compile_ShaderDebug()))
		return E_FAIL;
#endif

	m_eType = eType;

	HRESULT			hr = 0;

	switch (m_eType)
	{
	case CCollider::TYPE_AABB: 
		hr = D3DXCreateBox(m_pGraphic_Device, 1.f, 1.f, 1.f, &m_pMesh, nullptr);	

		m_vMin = _float3(-0.5f, -0.5f, -0.5f);
		m_vMax = _float3(0.5f, 0.5f, 0.5f);
		break;

	case CCollider::TYPE_OBB:
		hr = D3DXCreateBox(m_pGraphic_Device, 1.f, 1.f, 1.f, &m_pMesh, nullptr);

		m_vPoints[0] = _float3(-0.5f, 0.5f, -0.5f);
		m_vPoints[1] = _float3(0.5f, 0.5f, -0.5f);
		m_vPoints[2] = _float3(0.5f, -0.5f, -0.5f);
		m_vPoints[3] = _float3(-0.5f, -0.5f, -0.5f);

		m_vPoints[4] = _float3(-0.5f, 0.5f, 0.5f);
		m_vPoints[5] = _float3(0.5f, 0.5f, 0.5f);
		m_vPoints[6] = _float3(0.5f, -0.5f, 0.5f);
		m_vPoints[7] = _float3(-0.5f, -0.5f, 0.5f);
		
		break;
	
	case CCollider::TYPE_SPHERE:
		hr = D3DXCreateSphere(m_pGraphic_Device, 0.5f, 10, 10, &m_pMesh, nullptr);
		break;
	}

	return S_OK;
}


HRESULT CCollider::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
	
	memcpy(&m_ColliderDesc, pArg, sizeof(COLLIDERDESC));

	/* 원점이 콜라이더의 바닥에 올수 있도록 */
	_float3		vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

	/* 현재 콜라이더의 상태를 표현하ㅡㄴㄴ 컴포넌트. */
	m_pTransform->Set_State(CTransform::STATE_POSITION, m_ColliderDesc.vInitPos);
	m_pTransform->Set_Scale(m_ColliderDesc.vScale.x, m_ColliderDesc.vScale.y, m_ColliderDesc.vScale.z);
	m_pTransform->Rotation_Axis(m_ColliderDesc.vRadians.x, _float3(1.f, 0.f, 0.f));
	m_pTransform->Rotation_Axis(m_ColliderDesc.vRadians.y, _float3(0.f, 1.f, 0.f));
	m_pTransform->Rotation_Axis(m_ColliderDesc.vRadians.z, _float3(0.f, 0.f, 1.f));

	/* 복제할 당시에 콜라이더의 초기상태를  m_OriginalMatrix 에 보관한다. */
	/*m_OriginalMatrix 앞으로 절대 바뀌지 않는다. */
	m_OriginalMatrix = *m_pTransform->Get_WorldMatrix();	

	//D3DXVec3TransformCoord(&m_vMin, &m_vMin, &m_OriginalMatrix);
	//D3DXVec3TransformCoord(&m_vMax, &m_vMax, &m_OriginalMatrix);

	return S_OK;
}

void CCollider::Update_Collider()
{	
	m_ParentMatrix = *m_ColliderDesc.pParentMatrix;

	/* 우리가 지정해놓은 사이즈있는 그대로 출력되게 만들어주기위해서. */
	/* m_ParentMatrix 에는 플레이어 메시를 그리기위한 스케일정보가 담길수도 있기때문에 .*/
	D3DXVec3Normalize((_float3*)&m_ParentMatrix.m[0][0], (_float3*)&m_ParentMatrix.m[0][0]);
	D3DXVec3Normalize((_float3*)&m_ParentMatrix.m[1][0], (_float3*)&m_ParentMatrix.m[1][0]);
	D3DXVec3Normalize((_float3*)&m_ParentMatrix.m[2][0], (_float3*)&m_ParentMatrix.m[2][0]);

	/* m_OriginalMatriㅌ에는 콜라이더의 초기상태정보가 담겨있다. */
	/* m_ParentMatrix에는 이 콜라이더를 사용하고있는 개체의 행렬이담겨있다.  */
	m_pTransform->Set_WorldMatrix(m_OriginalMatrix * m_ParentMatrix);	

	if (m_eType == CCollider::TYPE_AABB)
		m_pTransform->Remove_Rotation();
}

bool CCollider::Collision_AABB(CCollider * pTargetCollider, _float3* pOverlap)
{
	_float3		vSourMin, vSourMax;
	_float3		vDestMin, vDestMax;

	/* 민과 맥스를 현재 콜라이더의 상태에 맞게끔 다시 구한다. */
	// m_pTransform->Set_WorldMatrix(m_OriginalMatrix * m_ParentMatrix);
	_matrix		TransformMatrix = *m_pTransform->Get_WorldMatrix();

	/* m_vMin 원본상태의 콜라이더(사이즈1, 위치는 중앙이 000 )*/
	/* TrasnformMatrix = Original * Parent */
	D3DXVec3TransformCoord(&vSourMin, &m_vMin, &TransformMatrix);
	D3DXVec3TransformCoord(&vSourMax, &m_vMax, &TransformMatrix);

	D3DXVec3TransformCoord(&vDestMin, &pTargetCollider->m_vMin, pTargetCollider->m_pTransform->Get_WorldMatrix());
	D3DXVec3TransformCoord(&vDestMax, &pTargetCollider->m_vMax, pTargetCollider->m_pTransform->Get_WorldMatrix());

	if (max(vSourMin.x, vDestMin.x) - min(vSourMax.x, vDestMax.x) >= 0 ||
		max(vSourMin.y, vDestMin.y) - min(vSourMax.y, vDestMax.y) >= 0 ||
		max(vSourMin.z, vDestMin.z) - min(vSourMax.z, vDestMax.z) >= 0 )
	{
		m_isCollision = false;
		return false;
	}

	if (pOverlap != nullptr)
	{
		pOverlap->x = max(vSourMin.x, vDestMin.x) - min(vSourMax.x, vDestMax.x);
		if (vSourMin.x > vDestMin.x)
		{
			pOverlap->x *= -1;
		}

		pOverlap->y = max(vSourMin.y, vDestMin.y) - min(vSourMax.y, vDestMax.y);
		if (vSourMin.y > vDestMin.y)
		{
			pOverlap->y *= -1;
		}

		pOverlap->z = max(vSourMin.z, vDestMin.z) - min(vSourMax.z, vDestMax.z);
		if (vSourMin.z > vDestMin.z)
		{
			pOverlap->z *= -1;
		}
	}

	m_isCollision = true;

	return true;
}

bool CCollider::Collision_OBB(CCollider * pTargetCollider)
{
	if (pTargetCollider == nullptr)
	{
		return false;
	}
	_float3		vSourPoint[8], vDestPoint[8];

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&vSourPoint[i], &m_vPoints[i], m_pTransform->Get_WorldMatrix());
		D3DXVec3TransformCoord(&vDestPoint[i], &pTargetCollider->m_vPoints[i], pTargetCollider->m_pTransform->Get_WorldMatrix());
	}

	OBBDESC			OBBDesc[2];

	OBBDesc[0] = Compute_OBBDesc(vSourPoint);
	OBBDesc[1] = Compute_OBBDesc(vDestPoint);

	_float			fDistance[3];

	for (_uint i = 0; i < 2; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(D3DXVec3Dot(&OBBDesc[0].vCenterAxis[0], &OBBDesc[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBBDesc[0].vCenterAxis[1], &OBBDesc[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBBDesc[0].vCenterAxis[2], &OBBDesc[i].vAlignAxis[j]));

			fDistance[1] = fabs(D3DXVec3Dot(&OBBDesc[1].vCenterAxis[0], &OBBDesc[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBBDesc[1].vCenterAxis[1], &OBBDesc[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBBDesc[1].vCenterAxis[2], &OBBDesc[i].vAlignAxis[j]));


			fDistance[2] = fabs(D3DXVec3Dot(&(OBBDesc[1].vCenter - OBBDesc[0].vCenter), &OBBDesc[i].vAlignAxis[j]));

			if (fDistance[2] > fDistance[0] + fDistance[1])
			{
				m_isCollision = false;
				return false;
			}

		}
	}
	
	m_isCollision = true;
	
	return true;
}

bool CCollider::Collision_SPHERE(CCollider * pTargetCollider)
{
	return false;
}

#ifdef _DEBUG

HRESULT CCollider::Render()
{
	if (nullptr == m_pMesh ||
		nullptr == m_pGraphic_Device)
		return E_FAIL;

	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	m_pShader->SetUp_ConstantTable("g_WorldMatrix", m_pTransform->Get_WorldMatrix(), sizeof(_matrix));
	m_pShader->SetUp_ConstantTable("g_ViewMatrix", &pPipeLine->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShader->SetUp_ConstantTable("g_ProjMatrix", &pPipeLine->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));
	m_pShader->SetUp_Bool("g_isCollision", (BOOL)m_isCollision);

	RELEASE_INSTANCE(CPipeLine);


	m_pShader->Begin_Shader(0);

	m_pMesh->DrawSubset(0);

	m_pShader->End_Shader();

	return S_OK;
}
#endif // _DEBUG


CCollider::OBBDESC CCollider::Compute_OBBDesc(_float3 * pPoints)
{
	OBBDESC			OBBDesc;
	ZeroMemory(&OBBDesc, sizeof(OBBDESC));

	OBBDesc.vCenter = (pPoints[5] + pPoints[3]) * 0.5f;
	OBBDesc.vAlignAxis[0] = pPoints[5] - pPoints[4];
	OBBDesc.vAlignAxis[1] = pPoints[5] - pPoints[6];
	OBBDesc.vAlignAxis[2] = pPoints[5] - pPoints[1];

	for (_uint i = 0; i < 3; ++i)
		D3DXVec3Normalize(&OBBDesc.vAlignAxis[i], &OBBDesc.vAlignAxis[i]);

	OBBDesc.vCenterAxis[0] = (pPoints[5] + pPoints[2]) * 0.5f - OBBDesc.vCenter;
	OBBDesc.vCenterAxis[1] = (pPoints[5] + pPoints[0]) * 0.5f - OBBDesc.vCenter;
	OBBDesc.vCenterAxis[2] = (pPoints[5] + pPoints[7]) * 0.5f - OBBDesc.vCenter;

	return OBBDesc;
}

#ifdef _DEBUG
HRESULT CCollider::Compile_ShaderDebug()
{
	m_pShader = CShader::Create(m_pGraphic_Device, TEXT("../bin/ShaderFiles/Shader_Collider.hlsl"));
	if (nullptr == m_pShader)
		return E_FAIL;

	return S_OK;
}
#endif

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device, COLLIDER eType)
{
	CCollider*	pInstance = new CCollider(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType)))
	{
		MSGBOX("Failed to Creating CCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CCollider::Clone(void * pArg)
{
	CCollider*	pInstance = new CCollider(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pMesh);

#ifdef _DEBUG
	Safe_Release(m_pShader);
#endif // _DEBUG
	
}
