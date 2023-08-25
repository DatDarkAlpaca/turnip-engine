#pragma once

namespace tur
{
	template<typename Derived>
	class BaseWindowLoader
	{
	public:
		bool Preload()
		{
			return static_cast<Derived*>(this)->Preload();
		}

		bool PostLoad()
		{
			return static_cast<Derived*>(this)->PostLoad();
		}
	};
}