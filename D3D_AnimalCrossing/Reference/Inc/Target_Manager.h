#pragma once

#include "Base.h"

BEGIN(Engine)

class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
private:
	explicit CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	LPDIRECT3DTEXTURE9 Get_TargetTexture(const _tchar* pTargetTag);

public:
	HRESULT Add_RenderTarget(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pTargetTag, _uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR ClearColor);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);

	HRESULT Begin_MRT(const _tchar* pMRTTag);
	HRESULT End_MRT(const _tchar* pMRTTag);

#ifdef _DEBUG
public:
	HRESULT Ready_Debug_Buffer(const _tchar* pTargetTag, _float fLeftX, _float fTopY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug_Buffer(const _tchar* pMRTTag);
#endif // _DEBUG

private:
	map<const _tchar*, class CTarget*>			m_RenderTargets;
	typedef map<const _tchar*, CTarget*>		RENDERTARGETS;

private:
	map<const _tchar*, list<class CTarget*>>			m_MRTs;
	typedef map<const _tchar*, list<class CTarget*>>	MRTS;

private:
	class CTarget* Find_RenderTarget(const _tchar* pTargetTag) ;
	list<class CTarget*>* Find_MRT(const _tchar* pMRTTag);

public:
	virtual void Free() override;
};

END