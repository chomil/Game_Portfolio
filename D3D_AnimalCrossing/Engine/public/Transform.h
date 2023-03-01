#pragma once

/* ��ü�� ��������󿡼��� ���¸� ǥ���Ѵ�. */
/* 1�� : ����Ʈ, 2�� : ��, 3�� : ��, 4�� : ������ */
/* ������������� ��ȯ, ���¸� ǥ�� */
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };
public:
	typedef struct tagTransformDesc
	{
		_float	fSpeedPerSec;
		_float	fRotationPerSec;
	}TRANSFORMDESC;
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	const _matrix* Get_WorldMatrix() const {
		return &m_WorldMatrix;
	}
	_matrix Get_WorldMatrixInverse() const {
		_matrix WorldMatrixInverse;
		return *D3DXMatrixInverse(&WorldMatrixInverse, nullptr, &m_WorldMatrix);
	}

	_float3 Get_State(STATE eState) const {
		return *(_float3*)&m_WorldMatrix.m[eState][0];
	}

	_float3 Get_Scale() const;
public:
	void Set_State(STATE eState, _float3 vState);
	void Set_Scale(_float fScaleX, _float fScaleY, _float fScaleZ);	
	void Set_WorldMatrix(const _matrix& WorldMatrix) {
		m_WorldMatrix = WorldMatrix;
	}
	void Set_MovingState(bool isMoving) {
		m_isMoving = isMoving;
	}

public:
	virtual HRESULT NativeConstruct_Prototype()override;
	virtual HRESULT NativeConstruct(void* pArg)override;
public:	
	void Go_Straight(_float fTimeDelta);
	void Back_Straight(_float fTimeDelta);
	void Walk_Left(_float fTimeDelta);
	void Walk_Right(_float fTimeDelta);
	void Rotation_Axis(_float3 vAxis, _float fTimeDelta); /* ���� */
	void Rotation_Axis(_float fRadian, _float3 vAxis); /* ���� */
	void SetUp_Rotation(_float3 vAxis, _float fRadian); /* ���� */
	void Move_Direction(_float3 vDir, _float fTimeDelta);
	void Chase_Target(const CTransform* pTargetTransform, _float fTimeDelta);
	void Chase_Target(_float3	vTargetPos, _float fTimeDelta);
	void LookAt_Target(const CTransform* pTargetTransform);
	void LookAt_Target(_float3	vTargetPos);
	void LookAt_Target(_float3 vTargetDir, float fTimeDelta);
	bool Stand_OnTerrain(class CVIBuffer_Terrain* pVIBuffer, const _matrix* pTerrainWorldMatrix, _double TimeDelta);
	void Remove_Rotation();
private:
	/* ��ü�� ���¸� ǥ���Ѵ�. */
	_matrix			m_WorldMatrix;	
	TRANSFORMDESC	m_TransformState;

private:
	bool			m_isMoving = false;

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr)override;
	virtual void Free() override;
};

END