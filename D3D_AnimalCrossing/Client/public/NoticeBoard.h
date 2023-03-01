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

class CNoticeBoard final : public CGameObject
{
private:
	explicit CNoticeBoard(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CNoticeBoard(const CNoticeBoard& rhs);
	virtual ~CNoticeBoard() = default;

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


	CCollider* m_pColliderCom = nullptr;
private:
	HRESULT Add_Components();	
	HRESULT SetUp_ConstantTable(); 
public:
	static CNoticeBoard* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END