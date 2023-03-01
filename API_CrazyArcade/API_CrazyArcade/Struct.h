#pragma once


typedef struct tagInfo
{
	float fX;
	float fY;
	int iCX;
	int iCY;
}INFO;


typedef struct tagFrame
{
	int iFrameStart;
	int iFrameEnd;
	int iFrameState;
	DWORD dwFrameSpeed;
	DWORD dwFrameTime;

	int iFrameX;
	int iFrameY;
}FRAME;