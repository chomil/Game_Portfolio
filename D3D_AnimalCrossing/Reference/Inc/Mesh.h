#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CMesh abstract : public CComponent
{
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

};

END