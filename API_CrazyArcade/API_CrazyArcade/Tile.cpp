#include "pch.h"
#include "Tile.h"
#include "GameObject_Manager.h"
#include "Player.h"
#include "Scene_Room.h"
#include "Block.h"
#include "Monster.h"
#include "ComPlayer.h"


CTile::CTile()
	:m_iDrawID(0)
	, m_iOption(0)
{
}


CTile::~CTile()
{
	Release_GameObject();
}

bool CTile::Is_Pass()
{
	bool bBlock = true;
	for (CGameObject* pObj : m_pObjectOnTile)
	{
		if ((pObj->Get_ObjectType() == OBJECT::BLOCK && static_cast<CBlock*>(pObj)->Get_BlockType() != BLOCK_TYPE::BUSH)||
			(pObj->Get_ObjectType()==OBJECT::BUBBLE))
		{
			bBlock = false;
		}
	}
	return bBlock;
}

bool CTile::Is_Wall()
{
	for (CGameObject* pObj : m_pObjectOnTile)
	{
		if ((pObj->Get_ObjectType() == OBJECT::BLOCK && static_cast<CBlock*>(pObj)->Get_BlockType() == BLOCK_TYPE::BLOCK))
		{
			return true;
		}
	}
	return false;
}

int CTile::Ready_GameObject()
{
	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;
	m_eRenderGroup = OBJECT::BACKGROUND;
	m_iStageNum = CScene_Room::Get_StageNum();
	return 0;
}

int CTile::Update_GameObject()
{
	CScene_Manager::ID eSceneID = CScene_Manager::Get_Instance()->Get_SceneID();
	if (eSceneID < CScene_Manager::ID::SCENE_STAGE || eSceneID > CScene_Manager::ID::SCENE_BOSS)
	{
		return 0;
	}

	m_pObjectOnTile.clear();
	for (int i = OBJECT::ID::BLOCK; i <= OBJECT::ID::MONSTER; i++)
	{
		auto pListObject = CGameObject_Manager::Get_Instance()->Get_ListObject((OBJECT::ID)i);
		for (auto& pObject : *pListObject)
		{
			const INFO* infoObejct = pObject->Get_Info();
			//오브젝트를 순회하면서 현재 타일 좌표내에 존재하는지 확인
			if (CTile_Manager::Get_Instance()->Pick_Tile(infoObejct->fX, infoObejct->fY) == this)
			{
				if (pObject->Get_ObjectType() == OBJECT::PLAYER && static_cast<CPlayer*>(pObject)->Get_IsShow()==false)
				{
					continue;
				}
				if (pObject->Get_ObjectType() == OBJECT::COMPLAYER && static_cast<CComPlayer*>(pObject)->Get_IsShow() == false)
				{
					continue;
				}
				//현재 타일에 존재하는 오브젝트들을 리스트에 추가
				m_pObjectOnTile.emplace_back(pObject);
			}
			else
			{
				if (pObject->Get_ObjectType() == OBJECT::MONSTER)
				{
					if (static_cast<CMonster*>(pObject)->Get_MonsterType() == 2)
					{
						for (int i = -40; i <= 40; i += 40)
						{
							for (int j = -40; j <= 40; j += 40)
							{
								if (CTile_Manager::Get_Instance()->Pick_Tile(infoObejct->fX+i, infoObejct->fY+j) == this)
								{
									m_pObjectOnTile.emplace_back(pObject);
								}

							}
						}
					}
				}
			}
		}
	}

	return 0;
}

void CTile::Late_Update_GameObject()
{

}

void CTile::Render_GameObject(HDC hDC)
{
	CGameObject::Update_Rect_GameObject();
	HDC hMemDC;
	if (m_iStageNum == 0 || m_iStageNum == 2)
	{
		hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"Tile_town");
		if (nullptr == hMemDC)
			return;
	}
	else
	{
		hMemDC = CBitmap_Manager::Get_Instance()->FindImage(L"Tile_pirate");
		if (nullptr == hMemDC)
			return;
	}
	
	BitBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.iCX, m_tInfo.iCY, hMemDC, m_tInfo.iCX * m_iDrawID, m_tInfo.iCY * m_iOption, SRCCOPY);


}

void CTile::Release_GameObject()
{
}

CGameObject* CTile::Create()
{
	CGameObject* pInstance = new CTile;
	if (0 > pInstance->Ready_GameObject())
		Safe_Delete(pInstance);
	return pInstance;
}
