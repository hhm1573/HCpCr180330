#pragma once

namespace Engine
{
	class CFinder_Tag
	{
	public:
		CFinder_Tag(const TCHAR* pTag) : m_pTag(pTag) {}
		~CFinder_Tag() = default;
	public:
		template <typename T>
		bool operator ()  (T& Pair)
		{
			return !lstrcmp(Pair.first, m_pTag);
		}
	private:
		const TCHAR* m_pTag;


	};

}