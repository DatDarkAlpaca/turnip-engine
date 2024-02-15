#pragma once

namespace tur
{
	struct Context
	{
	public:
		virtual ~Context() = default;

	public:
		virtual void Begin() = 0;

		virtual void End() = 0;
	};
}