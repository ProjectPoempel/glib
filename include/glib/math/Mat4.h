#pragma once

#include "../DLLDefs.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

#include <array>

namespace glib
{
	class Mat4
	{
	private:
		std::array<float, 16> data;
	public:
		GLIB_API Mat4();
		GLIB_API Mat4(std::array<float, 16>& arr);
		GLIB_API ~Mat4();

		GLIB_API void Translate(const Vec2& vec);
		GLIB_API void Scale(const Vec2& vec);
		GLIB_API void Translate(const Vec3& vec);
		GLIB_API void Scale(const Vec3& vec);
		GLIB_API void Rotate(const Vec3& degrees = Vec3(0.0f, 0.0f, 0.0f)); // Z-Axis (last value) for 2D

		float& operator()(int col, int row);
		const float& operator()(int col, int row) const;

		GLIB_API Mat4 operator*(const Mat4& other);
		GLIB_API float* GetRaw();
	public:
		GLIB_API static Mat4 Ortho(float left, float right, float bottom, float top);
		GLIB_API static Mat4 Perspective(float fov, float aspect, float near, float far);
		GLIB_API static Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
	};
}