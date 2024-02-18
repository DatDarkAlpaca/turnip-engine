#pragma once

namespace tur
{
	struct RenderCommands
	{
	public:
		virtual ~RenderCommands() = default;

	public:
		virtual void Begin() = 0;

		virtual void End() = 0;
	};
}