#pragma once

#include "../DLLDefs.h"
#include "Vec2.h"

namespace glib
{
	class Rect
	{
	public:
		float x;
		float y;
		float w;
		float h;
	public:
		GLIB_API Rect();
		GLIB_API Rect(float x, float y, float w, float h);
		GLIB_API Rect(const Vec2& pos, const Vec2& size);

		GLIB_API float Distance(const Rect& other);
	};
}