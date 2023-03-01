#include "pch.h"
#include "Tile_Manager.h"
#include "GameObject_Manager.h"
#include "Tile.h"
#include "Block.h"
#include "Scene_Room.h"
CTile_Manager* CTile_Manager::m_pInstance = nullptr;
CTile_Manager::CTile_Manager()
{
}


CTile_Manager::~CTile_Manager()
{
	Release_Tile_Manager();
}

void CTile_Manager::Load_Data_Tile_Manager(const TCHAR* pMapPath)
{
	HANDLE hFile = CreateFile(pMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwbyte = 0;

	if (!m_vecTile.empty())
	{
		for (auto& pTile : m_vecTile)
			Safe_Delete(pTile);
		m_vecTile.clear();
		m_vecTile.shrink_to_fit();
	}
	m_vecTile.reserve(TILEX* TILEY);

	while (true)
	{
		int iDrawID = 0;
		int iOption = 0;
		INFO tInfo{};

		ReadFile(hFile, &iDrawID, sizeof(char), &dwbyte, nullptr);
		if (iDrawID == 10|| iDrawID == 13)
		{
			continue;
		}
		iDrawID -= '0';
		if (0 == dwbyte)
			break;

		CGameObject* pTile = CTile::Create();
		
		dynamic_cast<CTile*>(pTile)->Set_DrawID(iDrawID);
		dynamic_cast<CTile*>(pTile)->Set_Option(iOption);

		tInfo.iCX = TILECX;
		tInfo.iCY = TILECY;
		tInfo.fX = (m_vecTile.size() % TILEX) * TILECX + TILESTARTX ;
		tInfo.fY = (m_vecTile.size() / TILEX) * TILECY + TILESTARTY ;
		pTile->Set_Info(tInfo);
		m_vecTile.emplace_back(pTile);
	}
	CloseHandle(hFile);

}

void CTile_Manager::Load_Data_Block_Manager(const TCHAR* pMapPath)
{
	HANDLE hFile = CreateFile(pMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwbyte = 0;

	int iBlockIndex = 0;
	int iStageNum = CScene_Room::Get_StageNum();

	while (true)
	{
		int iDrawID = 0;
		int iOption = 0;
		INFO tInfo{};

		ReadFile(hFile, &iDrawID, sizeof(char), &dwbyte, nullptr);
		if (iDrawID == 10 || iDrawID == 13)
		{
			continue;
		}
		iDrawID -= '1';
		if (0 == dwbyte)
			break;

		if (iDrawID >= 0)
		{
			BLOCK_TYPE::TYPE eType = BLOCK_TYPE::EMPTY;
			if (iStageNum == 0 || iStageNum == 2)
			{
				switch (iDrawID)
				{
				case 0:
				case 1:
					eType = BLOCK_TYPE::NORMAL;
					break;
				case 2:
					eType = BLOCK_TYPE::BOX;
					break;
				case 3:
					eType = BLOCK_TYPE::BUSH;
					break;
				default:
					eType = BLOCK_TYPE::BLOCK;
					break;
				}
			}
			else
			{
				switch (iDrawID)
				{
				case 0:
					eType = BLOCK_TYPE::NORMAL;
					break;
				case 1:
					eType = BLOCK_TYPE::BOX;
					break;
				default:
					eType = BLOCK_TYPE::BLOCK;
					break;
				}
			}


			CGameObject* pBlock = CBlock::Create(iDrawID, eType);
			CGameObject_Manager::Get_Instance()->Add_GameObject(OBJECT::BLOCK, pBlock);

			tInfo.iCX = TILECX;
			tInfo.iCY = TILECY;
			tInfo.fX = (iBlockIndex % TILEX) * TILECX + TILESTARTX;
			tInfo.fY = (iBlockIndex / TILEX) * TILECY + TILESTARTY;
			pBlock->Set_Info(tInfo);
		}
		iBlockIndex++;
	}
	CloseHandle(hFile);
}

CGameObject* CTile_Manager::Pick_Tile(int iX, int iY)
{
	int iIndexTileX = (iX - (TILESTARTX - TILECX / 2)) / TILECX;
	int iIndexTileY = (iY - (TILESTARTY - TILECY / 2)) / TILECY;
	if (iX < (TILESTARTX - TILECX / 2) || iIndexTileX >= TILEX || iY < (TILESTARTY - TILECY / 2) || iIndexTileY >= TILEY)
	{
		return nullptr;
	}
	return m_vecTile[iIndexTileY * TILEX + iIndexTileX];
}

int CTile_Manager::Update_Tile_Manager()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			int iIndex = j + (i * TILEX);

			if (0 > iIndex || m_vecTile.size() <= iIndex)
				continue;
			m_vecTile[iIndex]->Update_GameObject();
		}
	}
	return 0;
}

void CTile_Manager::Render_Tile_Manager(HDC hDC)
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			int iIndex = j + (i * TILEX);

			if (0 > iIndex || m_vecTile.size() <= iIndex)
				continue;
			m_vecTile[iIndex]->Render_GameObject(hDC);
		}
	}

}

void CTile_Manager::Release_Tile_Manager()
{
	for (auto& pObject : m_vecTile)
		Safe_Delete(pObject);

	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}
