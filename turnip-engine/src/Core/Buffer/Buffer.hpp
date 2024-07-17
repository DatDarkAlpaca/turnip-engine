#pragma once

namespace tur
{
	class Buffer
	{
	public:
		Buffer() = default;

	public:
		void* data = nullptr;
		size_t size = 0;
	};
}