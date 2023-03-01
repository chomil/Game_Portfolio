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

class CPeach final : public CGameObject
{
private:
	explicit CPeach(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPeach(const CPeach& rhs);
	virtual ~CPeach() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	
public:
	void Falling(int iIndex, _double TimeDelta);
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

	CCollider* m_pColliderCom = nullptr;

	int m_iIndexInTree = -1;
	bool m_bFalling = false;
	_double m_dFallTime = 0;

	bool m_bPicked = false; 
	_double m_dPickedTime = 0;

private:
	HRESULT Add_Components();	
	HRESULT SetUp_ConstantTable(); 
public:
	static CPeach* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END