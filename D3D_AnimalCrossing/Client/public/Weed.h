#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CMesh_Static;
class CCollider;
END

BEGIN(Client)

class CWeed final : public CGameObject
{
private:
	explicit CWeed(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CWeed(const CWeed& rhs);
	virtual ~CWeed() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Picked(bool bPicked)
	{
		m_bPicked = bPicked;
	}

private:

	//잡았을때 왼손 행렬 담기위한
	_matrix					m_OriginMatrix;
	const _matrix* m_pParentBoneMatrix;
	const _matrix* m_pParentWorldMatrix;


	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CMesh_Static*		m_pMeshCom = nullptr;
	CMesh_Static*		m_pPickedMeshCom = nullptr;

	CCollider* m_pColliderCom = nullptr;
	
	bool				m_bPicked = false;
	_double				m_dPickedTime = 0;

private:
	HRESULT Add_Components();	
	HRESULT SetUp_ConstantTable(); 
public:
	static CWeed* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END