#pragma once

#include "Component.h"

/* 캐릭터에게 씌워져있는 충돌체. */
/* 충돌체크에 기능ㄲㅏ지 부여한다. */

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum COLLIDER { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };

public:
	typedef struct tagColliderDesc
	{
		const _matrix*		pParentMatrix;
		_float3			vScale = _float3(1, 1.f, 1.f);
		_float3			vRadians = _float3(0.f, 0.f, 0.f);
		_float3			vInitPos = _float3(0.f, 0.f, 0.f);
	}COLLIDERDESC;

	typedef struct tagOBBDesc
	{
		_float3			vCenter;
		_float3			vAlignAxis[3];
		_float3			vCenterAxis[3];		
	}OBBDESC;
private:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;
public:
	HRESULT NativeConstruct_Prototype(COLLIDER eType);
	HRESULT NativeConstruct(void* pArg);	
	void Update_Collider();

public:
	bool Collision_AABB(CCollider* pTargetCollider, _float3* pOverlap = nullptr);
	bool Collision_OBB(CCollider* pTargetCollider);
	bool Collision_SPHERE(CCollider* pTargetCollider);

#ifdef _DEBUG
public:
	virtual HRESULT Render() override;
#endif

private:
	COLLIDER			m_eType = TYPE_END;
	LPD3DXMESH			m_pMesh = nullptr;
	bool				m_isCollision = false;
	COLLIDERDESC		m_ColliderDesc;	
	_matrix				m_OriginalMatrix;
	_matrix				m_ParentMatrix;
	class CTransform*	m_pTransform = nullptr;

	_float3				m_vMin, m_vMax;
	_float3				m_vPoints[8];


#ifdef _DEBUG
private:
	class CShader*		m_pShader = nullptr;
#endif

private:
	OBBDESC Compute_OBBDesc(_float3* pPoints);

#ifdef _DEBUG
private:
	HRESULT Compile_ShaderDebug();
#endif


public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device, COLLIDER eType);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END