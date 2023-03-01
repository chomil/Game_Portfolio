#pragma once

class CNode
{
public:
	CNode(bool _isWall, int _x, int _y) { isWall = _isWall; x = _x; y = _y; }

public:
	int F() { return G + H; }

	// G : 시작으로부터 이동했던 거리, H : |가로|+|세로| 장애물 무시하여 목표까지의 거리, F : G + H
public:
	int x = 0, y = 0, G = 0, H = 0;
	bool isWall = 0;
	CNode* ParentNode = nullptr;
};
int PosToIndex(int ix, int iy);
POINT AStarAlgorithm(POINT startPos, POINT endPos, int iRule=0);