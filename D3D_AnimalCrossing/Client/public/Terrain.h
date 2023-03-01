#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CTexture;
class CVIBuffer_Terrain;
class CMesh_Static;
END

BEGIN(Client)

class CTerrain final : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;
private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTexture*			m_pTextureCom_Grass = nullptr;
	CTexture*			m_pTextureCom_Sand = nullptr;
	CTexture*			m_pTextureCom_Filter = nullptr;
	
	CVIBuffer_Terrain*	m_pVIBufferCom = nullptr;
	CShader*			m_pShaderCom = nullptr;

	//LPDIRECT3DTEXTURE9	m_pTexture = nullptr;
private:
	HRESULT Add_Components();
	HRESULT SetUp_ConstantTable();
public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END