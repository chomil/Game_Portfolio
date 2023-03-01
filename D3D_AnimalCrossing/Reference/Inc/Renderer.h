#pragma once

/* 현재 화면안에 그려져야할 객체들을모아서 그리는순서에따라 분리보관한다. */
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY,RENDER_SHADOWDEPTH, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;	


public:
	HRESULT Add_RenderGroup(RENDERGROUP eGroup, class CGameObject* pGameObject);
	HRESULT Add_RenderDebug(class CComponent* pDebugObject);
	HRESULT Render_GameObject();

	void Set_LightAtEyeUp(_float3 vLightAt, _float3 vLightEye, _float3 vLightUp)
	{
		m_vLightAt = vLightAt;
		m_vLightEye = vLightEye;
		m_vLightUp = vLightUp;
	}

private:
	list<class CGameObject*>			m_RenderObjects[RENDER_END];
	list<class CComponent*>				m_DebugObject;

	_float3	m_vLightAt = {};
	_float3	m_vLightEye = {};
	_float3	m_vLightUp = {};

private:
	class CTarget_Manager* m_pTarget_Manager = nullptr;
	class CShader* m_pShader = nullptr;
	class CVIBuffer_Rect_Viewport* m_pVIBuffer = nullptr;

private:
	LPDIRECT3DSURFACE9			m_pOriginal_DS_Surface = nullptr;
	LPDIRECT3DSURFACE9			m_pShadow_DS_Surface = nullptr;
private:
	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_UI();

private:
	HRESULT Render_ShadowDepth();

private:
	HRESULT Render_LightAcc();
	HRESULT Render_Blend();
public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

	
};

END