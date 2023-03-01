#include "pch.h"
#include "PathFind.h"

int iMoveX[4] = { 0,0,-40,40 };
int iMoveY[4] = { -40,40,0,0 };

int PosToIndex(int ix, int iy)
{
	if (ix < TILESTARTX - 20 || ix > TILESTARTX - 20 + 15 * TILECX || iy < TILESTARTY - 20 || iy > TILESTARTY - 20 + 13 * TILECY)
	{
		return -1;
	}
	return ((iy - TILESTARTY + 20) / TILECY) * TILEX + (ix - TILESTARTX + 20) / TILECX;
}

POINT AStarAlgorithm(POINT startPos, POINT endPos, int iRule)//iRule: 0-벽만 판단, 1-블럭모두 판단
{
	vector<CNode> nodePoints;
	list<CNode*> openList;
	list<CNode*> closeList;
	CNode* startNode = nullptr;
	CNode* endNode = nullptr;
	CNode* curNode = nullptr;

	auto pTileVector = CTile_Manager::Get_Instance()->Get_Tiles();
	for (int i = 0 ; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			CTile* pTile = static_cast<CTile*>((*pTileVector)[TILEX * i + j]);
			if (iRule == 0)
			{
				nodePoints.emplace_back(CNode(pTile->Is_Wall(), pTile->Get_Pos().x, pTile->Get_Pos().y));
			}
			else
			{
				nodePoints.emplace_back(CNode(!pTile->Is_Pass(), pTile->Get_Pos().x, pTile->Get_Pos().y));
			}
		}
	}
	startNode = &(nodePoints[PosToIndex(startPos.x, startPos.y)]);
	endNode = &(nodePoints[PosToIndex(endPos.x, endPos.y)]);

	if (startNode == endNode)
	{
		POINT targetPos;
		targetPos.x = endNode->x;
		targetPos.y = endNode->y;
		return targetPos;
	}

	startNode->G = 0;
	startNode->H = abs(endNode->x - startNode->x) + abs(endNode->y - startNode->y);
	
	openList.emplace_back(startNode);

	while (1)
	{
		int iMinF = 99999999;
		int iMinH = 99999999;
		for (CNode* pNode : openList)
		{
			if (pNode->F() <= iMinF)
			{
				if (pNode->F() == iMinF)
				{
					if (pNode->H < iMinH)
					{
						curNode = pNode;
						iMinF = pNode->F();
						iMinH = pNode->H;
					}
				}
				else
				{
					curNode = pNode;
					iMinF = pNode->F();
					iMinH = pNode->H;
				}
			}
		}
		for (auto iter = openList.begin(); iter != openList.end(); iter++)
		{
			if (*iter == curNode)
			{
				openList.erase(iter);
				break;
			}
		}
		closeList.emplace_back(curNode);

		if (curNode == endNode)
		{
			POINT targetPos;
			closeList.reverse();
			CNode* tmpNode = closeList.front();
			while (tmpNode->ParentNode != startNode)
			{
				tmpNode = tmpNode->ParentNode;
			}
			targetPos.x = tmpNode->x;
			targetPos.y = tmpNode->y;
			return targetPos;
		}

		for (int i = 0; i < 4; i++)
		{
			while (PosToIndex(curNode->x + iMoveX[i], curNode->y + iMoveY[i]) >= 0)
			{
				CNode* tmpNode = &(nodePoints[PosToIndex(curNode->x + iMoveX[i], curNode->y + iMoveY[i])]);
				bool bInClose = false;
				for (auto pNode : closeList)
				{
					if (pNode == tmpNode)
					{
						bInClose = true;
					}
				}
				if (bInClose == true)
				{
					break;
				}
				if (tmpNode->isWall == true)
				{
					break;
				}

				bool bInOpen = false;
				for (auto pNode : openList)
				{
					if (pNode == tmpNode)
					{
						bInOpen = true;
						if (curNode->G + 40 < pNode->G)
						{
							tmpNode->ParentNode = curNode;
							tmpNode->G = tmpNode->ParentNode->G + 40;
							tmpNode->H = abs(endNode->x - tmpNode->x) + abs(endNode->y - tmpNode->y);
						}
					}
				}
				if (bInOpen == false)
				{
					openList.emplace_back(tmpNode);
					tmpNode->ParentNode = curNode;
					tmpNode->G = tmpNode->ParentNode->G + 40;
					tmpNode->H = abs(endNode->x - tmpNode->x) + abs(endNode->y - tmpNode->y);
				}
				break;
			}
		}//상하좌우 탐색 끝

		if (openList.size() == 0)
		{
			POINT pt = { 0,0 };
			return pt;
		}
	}
}