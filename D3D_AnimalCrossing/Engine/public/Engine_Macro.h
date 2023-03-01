#pragma once

#define GET_INSTANCE(CLASSNAME)	[](){											\
	CLASSNAME*	pInstance = CLASSNAME::Get_Instance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->AddRef();}														\
	return pInstance;															\
	}();

#define RELEASE_INSTANCE(CLASSNAME)	[](){										\
	CLASSNAME*	pInstance = CLASSNAME::Get_Instance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->Release();}														\
	}();

#define MSGBOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("Error"), MB_OK)

#define BEGIN(NAME) namespace NAME { 
#define END }

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)

#else
#define ENGINE_DLL _declspec(dllimport)

#endif

#define DECLARE_SINGLETON(Class)			\
public:										\
static Class* Get_Instance()				\
{											\
	if (!m_pInstance)						\
		m_pInstance = new Class;			\
											\
	return m_pInstance;						\
}											\
static unsigned long Destroy_Instance()		\
{											\
	unsigned long	dwRefCnt = 0;			\
	dwRefCnt = Safe_Release(m_pInstance);	\
	return dwRefCnt;						\
}											\
private:									\
	static Class* m_pInstance;				

#define IMPLEMENT_SINGLETON(Class)			\
Class* Class::m_pInstance = nullptr; 
