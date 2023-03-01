#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Viewport final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Rect_Viewport(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Rect_Viewport(const CVIBuffer& rhs);
	virtual ~CVIBuffer_Rect_Viewport() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(_float fLeftX, _float fTopY, _float fSizeX, _float fSizeY);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	static CVIBuffer_Rect_Viewport* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fLeftX, _float fTopY, _float fSizeX, _float fSizeY);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END