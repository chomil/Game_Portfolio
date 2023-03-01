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

class CTextBalloon final : public CGameObject
{
private:
	explicit CTextBalloon(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTextBalloon(const CTextBalloon& rhs);
	virtual ~CTextBalloon() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;




public:
	void Set_ShowBalloon(bool bShow, int iTextIndex = -1, bool bName = false);
private:
	void Set_Text(int iTextIndex);
private:
	/* 직교투영행렬. */
	_matrix				m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

	bool				m_bShowBalloon = false;
	int					m_iTextIndex = -1;
	bool				m_bNameBalloon = false;
	double				m_dTimeShow = 1;

	TCHAR				m_szText[MAX_PATH];
private:
	CTexture* m_pTextureBalloonCom = nullptr;
	CTexture* m_pTextureNameCom = nullptr;
	CTexture* m_pTextureArrowCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;

	CShader* m_pShaderCom = nullptr;
private:
	HRESULT Add_Components();

public:
	static CTextBalloon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END