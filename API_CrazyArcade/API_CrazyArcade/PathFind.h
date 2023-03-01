#pragma once

class CNode
{
public:
	CNode(bool _isWall, int _x, int _y) { isWall = _isWall; x = _x; y = _y; }

public:
	int F() { return G + H; }

	// G : �������κ��� �̵��ߴ� �Ÿ�, H : |����|+|����| ��ֹ� �����Ͽ� ��ǥ������ �Ÿ�, F : G + H
public:
	int x = 0, y = 0, G = 0, H = 0;
	bool isWall = 0;
	CNode* ParentNode = nullptr;
};
int PosToIndex(int ix, int iy);
POINT AStarAlgorithm(POINT startPos, POINT endPos, int iRule=0);