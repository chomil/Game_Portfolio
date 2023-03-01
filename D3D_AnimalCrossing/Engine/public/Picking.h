#pragma once

#include "Component.h"

BEGIN(Engine)

/* �嵐���� �ϳ��� ���콺�� �浹. */
class ENGINE_DLL CPicking final : public CComponent
{
private:
	explicit CPicking(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPicking() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;	
public:
	HRESULT Compute_MouseCursorPosInWorld(HWND hWnd);
	_float3* Compute_PickingPoint(class CVIBuffer* pVIBuffer, _matrix WorldMatrix);
private:
	_float3			m_vMouseRay;
	_float3			m_vMousePivot;

	_float3			m_vResultPos;


public:
	static CPicking* Create(LPDIRECT3DDEVICE9 pGraphic_Device);	
	virtual CComponent* Clone(void* pArg = nullptr)override;
	virtual void Free() override;
};

END