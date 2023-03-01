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

class CItem final : public CGameObject
{
public:
	struct ITEM_DESC
	{
		int m_iItemIndex = -1;
		int m_iInvenIndex = -1;
	};
private:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CItem(const CItem& rhs);
	virtual ~CItem() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	int Get_ItemIndex()
	{
		return m_ItemDesc.m_iItemIndex;
	}
	int Get_StackNum()
	{
		return m_iStackNum;
	}
	void Add_StackNum(int iNum)
	{
		m_iStackNum += iNum;
	}

private:
	CTexture* m_pTextureItem = nullptr;
	CRenderer*			m_pRendererCom = nullptr;	
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	CShader* m_pShaderCom = nullptr;
private:
	/* 직교투영행렬. */
	_matrix				m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

	ITEM_DESC m_ItemDesc;
	TCHAR m_szName[20] = {};
	int m_iPrice = 0;
	bool m_bIsShow = false;

	int m_iStackNum = 1;

private:
	HRESULT Add_Components();

public:
	static CItem* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END