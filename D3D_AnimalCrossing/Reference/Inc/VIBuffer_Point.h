#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Point final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Point(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Point(const CVIBuffer& rhs);
	virtual ~CVIBuffer_Point() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render_VIBuffer() override;

public:
	static CVIBuffer_Point* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END