#pragma once

#include "../DLLDefs.h"

#define GLIB_COLOR_WHITE { 1.0f, 1.0f, 1.0f, 1.0f }
#define GLIB_COLOR_BLACK { 0.0f, 0.0f, 0.0f, 1.0f }
#define GLIB_COLOR_TRANSPARENT { 0.0f, 0.0f, 0.0f, 0.0f }

namespace glib
{
	struct GLIB_API Color
	{
		float r;
		float g;
		float b;
		float a;

		Color operator+(const Color& other)
		{
			return { r + other.r, g + other.g, b + other.b, a + other.a };
		}
		Color operator-(const Color& other)
		{
			return { r - other.r, g - other.g, b - other.b, a - other.a };
		}
		Color operator*(const Color& other)
		{
			return { r * other.r, g * other.g, b * other.b, a * other.a };
		}
		Color operator*(float value)
		{
			return { r * value, g * value, b * value, a * value };
		}
	};
}