#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CPicking;
class CRenderer;
class CTransform;
class CMesh_Dynamic;
class CCollider;
END

BEGIN(Client)

class CInsect final : public CGameObject
{
public:
	struct INSECT_DESC
	{
		_float3 vSpawnPos = {};
		int iInsectIndex = 0;
	};
public:
	enum ANIM_STATE { IDLE = 0, FLY, GET, ANIM_END };
private:
	explicit CInsect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CInsect(const CInsect& rhs);
	virtual ~CInsect() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_State(ANIM_STATE eState);
private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CMesh_Dynamic*		m_pMeshCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

	virtual HRESULT Render_ShadowDepth();
private:
	//잡았을때 왼손 행렬 담기위한
	_matrix					m_OriginMatrix;
	const _matrix*			m_pParentBoneMatrix;
	const _matrix*			m_pParentWorldMatrix;


	INSECT_DESC			m_tagInsectDesc;
	ANIM_STATE			m_eState = IDLE;

	double				m_GetTime = 0;
	double				m_FlyTime = 0;
	double				m_IdleTime = 0;
	double				m_ChangeTime = 0;
	double				m_LifeTime = 0;

	_float3				m_vTargetDir = { 0,0,0 };
	bool				m_bRunAway = false;
private:
	HRESULT Add_Components();
	HRESULT SetUp_ConstantTable(); 
public:
	static CInsect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END