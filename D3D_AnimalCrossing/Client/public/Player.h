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

class CPlayer final : public CGameObject
{
public:
	enum ANIM_STATE { IDLE = 0, WALK, DASH, 
		NET_IDLE, NET_WALK, NET_DASH, NET_SWING ,NET_GROUNDSTOP, NET_GET,
		GET_KEEP, PUT_AWAY, NET_PUT_AWAY, PICK_UP, NET_PICK_UP, 
		SHAKE, NET_SHAKE, ANIM_END};

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	virtual HRESULT Render_ShadowDepth();
public:
	void Get_Insect(class CInsect* pInsect);
	void Get_Item(int iItemIndex);
	ANIM_STATE Get_AnimState()
	{
		return m_eState;
	}
	void Set_AnimState(ANIM_STATE eState)
	{
		m_eState = eState;
	}

	void Set_Stop(bool bStop)
	{
		m_bStop = bStop;
	}
	int Get_Money()
	{
		return m_iMoney;
	}
	bool Get_InvenOpen()
	{
		return m_bInvenOpen;
	}
private:

	void Collision_Layer(const _tchar* LayerTag);

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CMesh_Dynamic*		m_pMeshCom = nullptr;

	
	CCollider*			m_pColliderCom = nullptr;
	CCollider*			m_pPickUpColliderCom = nullptr;
	
	CGameObject*		m_pPickObjectPtr = nullptr;
	CGameObject*		m_pToolObjectPtr = nullptr;


private:
	_float3				m_vTargetPos;
	ANIM_STATE			m_eState = IDLE;
	bool				m_bInvenOpen = false;
	bool				m_bStop = false;

	class CItem* m_pItems[20] = {};
	int m_iMoney = 0;

private:
	HRESULT Add_Components();	
	HRESULT SetUp_ConstantTable(); 
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END