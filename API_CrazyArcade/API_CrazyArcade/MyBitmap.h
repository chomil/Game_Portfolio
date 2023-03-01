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
	HDC m_hMemDC; // �ٸ� ��ȭ���� �׸� �׷��� ��. 
	HBITMAP m_hBitmap; // ���������� �׸��� ���� ������ ������ ������. 
	HBITMAP m_hOldBitmap; // ���� ����� ���� ����� ��. 
};

