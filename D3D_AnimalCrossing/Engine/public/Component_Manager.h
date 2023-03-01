#pragma once

/* 원형컴포넌트들을 모아서 관린하ㅏㄴㄷ.*/

#include "Shader.h"
#include "Texture.h"
#include "Picking.h"
#include "Renderer.h"
#include "Collider.h"
#include "Transform.h"
#include "Mesh_Static.h"
#include "Mesh_Dynamic.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Point.h"
#include "VIBuffer_Terrain.h"

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
private:
	explicit CComponent_Manager();
	virtual ~CComponent_Manager() = default;
public:
	HRESULT Reserve_Container(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);
	void Clear(_uint iLevelIndex);

private:
	map<const _tchar*, CComponent*>*		m_pPrototypes = nullptr;
	typedef map<const _tchar*, CComponent*>	PROTOTYPES;

private:
	_uint		m_iNumLevels = 0;

private:
	CComponent* Find_Component(_uint iLevelIndex, const _tchar* pPrototypeTag);
public:
	virtual void Free() override;
};

END