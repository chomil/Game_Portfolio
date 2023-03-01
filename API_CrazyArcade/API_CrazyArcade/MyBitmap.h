#pragma once
class CMyBitmap
{
public:
	CMyBitmap();
	~CMyBitmap();
public:
	HDC Get_MemDC() { return m_hMemDC; }
	// wchar_t*
	void Insert_Bitmap(const TCHAR* pFilePath);
	void Release_Bitmap();
private:
	HDC m_hMemDC; // 다른 도화지에 그림 그려둘 것. 
	HBITMAP m_hBitmap; // 실질적으로 그림에 대한 정보를 가지고 있을놈. 
	HBITMAP m_hOldBitmap; // 추후 지우기 위해 사용할 놈. 
};

