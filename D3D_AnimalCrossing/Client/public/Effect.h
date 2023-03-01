#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Point;
END

BEGIN(Client)

class CPointEffect final : public CGameObject
{
private:
	explicit CPointEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPointEffect(const CPointEffect& rhs);
	virtual ~CPointEffect() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT Add_Components();
private:
	CShader* m_pShaderCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Point* m_pVIBufferCom = nullptr;

private:
	_float					m_fFrame = 0.f;
	_float					m_fAlpha = 0.75f;
	_double					m_dTime = 0;
	_float					m_fSize = 1.f;

public:
	static CPointEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
