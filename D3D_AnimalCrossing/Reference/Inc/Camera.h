#pragma once

/* Ŭ���̾�Ʈ�� �ִ� İ�޶� Ŭ������ �θ��. */
/* ��� ī�޶��� �������� ��ɸ� �������ϰ���*/

#include "GameObject.h"

BEGIN(Engine)


class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct CameraDesc
	{
		_float3		vEye; /*ī�Ӹ޶� ��ġ. */
		_float3		vAt; /* ī�޶� ���� ��. */
		_float3		vAxisY = _float3(0.f, 1.f, 0.f);

		_float		fFovy;
		_float		fAspect;
		_float		fNear;
		_float		fFar;
	}CAMERADESC;
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

protected:
	CAMERADESC					m_CameraDesc;
	class CPipeLine*			m_pPipeLine = nullptr;
	class CTransform*			m_pTransform = nullptr;
private:
	/* ���� ī�޶��� ���¸� ��������Ͽ� ���� ��, ��������� ���������ο� �����Ѵ�. */
	HRESULT Set_Transform();
	
public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free();
};

END