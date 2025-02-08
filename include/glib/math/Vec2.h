#pragma once

#include "../DLLDefs.h"
#include <string>

#define GLIB_VEC2_ZERO glib::Vec2(0.0f, 0.0f)
#define GLIB_VEC2_ONE glib::Vec2(1.0, 1.0f)

namespace glib
{
	GLIB_API class Vec2
	{
	public:
		float x;
		float y;
	public:
		Vec2();
		Vec2(float x, float y);
		Vec2(float v);

		Vec2 operator+(const Vec2& other);
		Vec2 operator+(float v);
		Vec2 operator-(const Vec2& other);
		Vec2 operator-(float v);
		Vec2 operator*(const Vec2& other);
		Vec2 operator*(float v);
		Vec2 operator/(const Vec2& other);
		Vec2 operator/(float v);
		Vec2 operator^(const Vec2& other);
		Vec2 operator^(float v);

		void operator+=(const Vec2& other);
		void operator+=(float v);
		void operator-=(const Vec2& other);
		void operator-=(float v);
		void operator*=(const Vec2& other);
		void operator*=(float v);
		void operator/=(const Vec2& other);
		void operator/=(float v);
		void operator^=(const Vec2& other);
		void operator^=(float v);

		bool operator==(const Vec2& other);

		Vec2 Normalize();

		float Distance(const Vec2& other);

		Vec2 Absolute();

		std::string ToString();
	};
}