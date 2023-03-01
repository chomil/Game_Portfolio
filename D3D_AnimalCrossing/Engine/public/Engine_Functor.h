#pragma once

namespace Engine
{
	class CTagFinder
	{
	public:
		CTagFinder(const _tchar* pTargetTag) : m_pTargetTag(pTargetTag) {}
		~CTagFinder() = default;

	public:
		template <typename T>
		bool operator () (/*map<const _tchar*, class CGameObject*>::value_type*/T Pair)
		{
			if (0 == lstrcmp(Pair.first, m_pTargetTag))
				return true;

			return false;
		}

	private:
		const _tchar*		m_pTargetTag = nullptr;
	};
}

//void Test()
//{
//	Engine::CTagFinder			Test;	
//	
//	if (Test(1))
//	{
//
//	}
//}

