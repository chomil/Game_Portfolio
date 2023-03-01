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

class CTool_Net final : public CGameObject
{
private:
	explicit CTool_Net(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTool_Net(const CTool_Net& rhs);
	virtual ~CTool_Net() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;


	virtual HRESULT Render_ShadowDepth();
private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CMesh_Static*		m_pMeshCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

private:
	/* 플레이어 메시의 손 뼈행렬의 주소. */
	_matrix					m_OriginMatrix;
	const _matrix*			m_pParentBoneMatrix;
	const _matrix*			m_pParentWorldMatrix;

	class CPlayer*			m_pPlayer;

private:
	HRESULT Add_Components();	
	HRESULT SetUp_ConstantTable(); 
public:
	static CTool_Net* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END