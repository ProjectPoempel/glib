#pragma once

#include "../DLLDefs.h"
#include "Vec2.h"

namespace glib
{
	class GLIB_API Vec4
	{
	public:
		float x;
		float y;
		float z;
		float w;
	public:
		Vec4();
		Vec4(float x, float y, float z, float w);

		Vec4 operator+(const Vec4& other);
		Vec4 operator+(float v);
		Vec4 operator-(const Vec4& other);
		Vec4 operator-(float v);
		Vec4 operator*(const Vec4& other);
		Vec4 operator*(float v);
		Vec4 operator/(const Vec4& other);
		Vec4 operator/(float v);
		Vec4 operator^(const Vec4& other);
		Vec4 operator^(float v);

		void operator+=(const Vec4& other);
		void operator+=(float v);
		void operator-=(const Vec4& other);
		void operator-=(float v);
		void operator*=(const Vec4& other);
		void operator*=(float v);
		void operator/=(const Vec4& other);
		void operator/=(float v);
		void operator^=(const Vec4& other);
		void operator^=(float v);

		Vec4 operator-();

		Vec4 Normalize();

		float Distance(const Vec4& other);

		Vec2 XY();
		Vec2 ZW();
	};
}