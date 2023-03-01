#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CShader(const CShader& rhs);
	virtual ~CShader() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pShaderFilePath) ;
	virtual HRESULT NativeConstruct(void* pArg);
public:
	HRESULT SetUp_ConstantTable(D3DXHANDLE hHandle, const void* pData, _uint iLengthByte);
	HRESULT SetUp_Bool(D3DXHANDLE hHandle, BOOL isBool);
	HRESULT SetUp_TextureConstantTable(D3DXHANDLE hHandle, class CTexture* pTextureCom, _uint iTextureIndex = 0);
	HRESULT SetUp_TextureConstantTable(D3DXHANDLE hHandle, LPDIRECT3DBASETEXTURE9 pTexture);

	HRESULT Begin_Shader(_uint iPassIndex);
	HRESULT End_Shader();
	void Commit();

	LPD3DXEFFECT Get_EffectHandle() {
		return m_pEffect;
	}
protected:
	LPD3DXEFFECT			m_pEffect = nullptr;
public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END