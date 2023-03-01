#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CMesh_Static;
END

BEGIN(Client)

class CPlayer_Hair final : public CGameObject
{
private:
	explicit CPlayer_Hair(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer_Hair(const CPlayer_Hair& rhs);
	virtual ~CPlayer_Hair() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;
private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CMesh_Static*		m_pMeshCom = nullptr;

private:
	_matrix					m_OriginMatrix;
	const _matrix*			m_pParentBoneMatrix;
	const _matrix*			m_pParentWorldMatrix;

private:
	HRESULT Add_Components();	
	HRESULT SetUp_ConstantTable(); 
public:
	static CPlayer_Hair* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END