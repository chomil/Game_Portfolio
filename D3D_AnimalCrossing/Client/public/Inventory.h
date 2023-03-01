#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CShader;
END

BEGIN(Client)

class CInventory final : public CGameObject
{
private:
	explicit CInventory(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CInventory(const CInventory& rhs);
	virtual ~CInventory() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture* m_pTextureInvenCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;	
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	CShader* m_pShaderCom = nullptr;

public:
	void Set_InvenOpen(bool bOpen)
	{
		m_bInvenOpen = bOpen;
		m_dTimeOpen = 0;
	}
	bool Get_InvenOpen()
	{
		return m_bInvenOpen && m_dTimeOpen>0.1;
	}

private:
	/* 직교투영행렬. */
	_matrix				m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

	bool				m_bInvenOpen = false;
	double				m_dTimeOpen = 1;
private:
	HRESULT Add_Components();

public:
	static CInventory* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END