#pragma once


template <typename T>
void Safe_Delete(T& pPointer)
{
	if (nullptr != pPointer)
	{
		delete pPointer;
		pPointer = nullptr;
	}
}

template <typename T>
void Safe_Delete_Array(T& pPointer)
{
	if (nullptr != pPointer)
	{
		delete [] pPointer;
		pPointer = nullptr;
	}
}

template <typename T>
unsigned long Safe_AddRef(T& pInstance)
{
	unsigned long		dwRefcnt = 0;

	if (nullptr != pInstance)	
		dwRefcnt = pInstance->AddRef();

	return dwRefcnt;
}



template <typename T>
unsigned long Safe_Release(T& pInstance)
{
	unsigned long		dwRefcnt = 0;

	if (nullptr != pInstance)
	{
		dwRefcnt = pInstance->Release();

		if(0 == dwRefcnt)
			pInstance = nullptr;
	}

	return dwRefcnt;
}


