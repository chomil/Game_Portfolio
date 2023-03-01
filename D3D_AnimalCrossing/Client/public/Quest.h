#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "StorableVar.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CShader;
END

BEGIN(Client)

class CQuest final : public CGameObject
{
private:
	explicit CQuest(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CQuest(const CQuest& rhs);
	virtual ~CQuest() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_QuestOpen(bool bOpen);
	bool Get_QuestOpen()
	{
		return m_bQuestOpen;
	}
	void Clear_Quest(int iQuestIndex, int iAmount);

private:
	void Set_Quest();

private:
	CTexture*			m_pTextureQuestCom = nullptr;
	CTexture*			m_pTextureQuestBarCom = nullptr;
	CTexture*			m_pTextureQuestClearCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	CShader*			m_pShaderCom = nullptr;

private:
	/* 직교투영행렬. */
	_matrix				m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

	bool				m_bQuestOpen = false;
	double				m_dTimeOpen = 1;

	int					m_iQuestIndex[4] = {};
	_tchar				m_szQuestName[4][50] = {};
	int					m_iQuestPoint[4] = {};
	int					m_iQuestProgress[4][2] = {};
	int					m_iQuestSeed = 0;

	StorableInt					m_iStaticPoint;
	StorableInt					m_iStaticQuestSeed;
	StorableInt					m_iStaticProgress[4];
private:
	HRESULT Add_Components();

public:
	static CQuest* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END