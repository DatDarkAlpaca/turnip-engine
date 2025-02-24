#pragma once

namespace tur
{
	struct Rect2D
	{
		float x = 0, y = 0, width, height;
	};

	struct Viewport
	{
		float x = 0;
		float y = 0;

		float width;
		float height;

		float minDepth = 0.f;
		float maxDepth = 1.f;
	};
}