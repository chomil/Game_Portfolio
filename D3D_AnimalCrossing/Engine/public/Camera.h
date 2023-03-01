#pragma once

/* 클라이언트에 있는 캅메라 클래스의 부모다. */
/* 모든 카메라의 공통적인 기능르 ㄹ보관하가게*/

#include "GameObject.h"

BEGIN(Engine)


class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct CameraDesc
	{
		_float3		vEye; /*카ㅣ메라 위치. */
		_float3		vAt; /* 카메라가 보는 점. */
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
	/* 현재 카메라의 상태를 기반으로하여 만든 뷰, 투영행렬을 파이프라인에 저장한다. */
	HRESULT Set_Transform();
	
public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free();
};

END