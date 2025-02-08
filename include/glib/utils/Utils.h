#pragma once

#include "../DLLDefs.h"
#include <string>
#include "glib/math/Vec2.h"
#include "glib/window/Window.h"
#include "glib/graphics/Sprite.h"
#include "glib/math/Rect.h"

namespace glib
{
	class Utils
	{
	public:
		Utils() = delete;
		~Utils() = delete;
	public:
		GLIB_API static std::wstring ToWString(const std::string& str);
		GLIB_API static std::string ToString(const std::wstring& str);
		GLIB_API static Vec2 GetMousePosition(Window* wnd);
		GLIB_API static Vec2 GetMousePosition(Window* wnd, Camera* camera);
		GLIB_API static bool IsMouseTouchingSprite(Window* wnd, Sprite* sprite);
		GLIB_API static bool IsMouseTouchingSprite(Window* wnd, Camera* camera, Sprite* sprite);
		GLIB_API static bool IsMouseInArea(Window* wnd, const Rect& area);
		GLIB_API static bool IsMouseInArea(Window* wnd, Camera* camera, const Rect& area);
		GLIB_API static bool IsPointInArea(const Vec2& pos, const Rect& area);
	};
}