#pragma once

namespace tur
{
	class IGraphicsAPI
	{
	public:
		virtual ~IGraphicsAPI() = default;

	public:
		virtual void SetViewport(float x, float y, float width, float height, float minDepth = 0.f, float maxDepth = 1.f) = 0;

		virtual void SwapBuffers() = 0;
	};
}