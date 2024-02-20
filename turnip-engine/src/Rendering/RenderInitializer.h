#pragma once

namespace tur
{
	class RenderDevice;

	class RenderInitializer
	{
	public:
		virtual ~RenderInitializer() = default;

	public:
		virtual void Initialize(NON_OWNING RenderDevice* device) = 0;
	};
}