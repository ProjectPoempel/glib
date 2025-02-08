#pragma once

#include "../../DLLDefs.h"
#include "glib/math/Vec2.h"
#include "../Texture.h"
#include "glib/math/Vec4.h"
#include "../Font.h"
#include "glib/utils/Color.h"
#include "glib/math/Mat4.h"
#include "../camera/Camera.h"

#include <string>

namespace glib
{
	class UIDrawingInterfaceImpl;
	class UIAlignable;

	struct UIDrawOffsets
	{
		Vec2 pos;
	};

	class UIDrawingInterface
	{
	private:
		UIDrawingInterfaceImpl* impl;
	public:
		GLIB_API UIDrawingInterface();
		GLIB_API ~UIDrawingInterface();

		GLIB_API void PushStack(const Mat4& mat);
		GLIB_API void PopStack(int at = -1);
		GLIB_API void ClearStack();
		GLIB_API void PushAlignment(UIAlignable* element, UIAlignable* parent);
		
		void BeginFrame(const DrawData& data, const UIDrawOffsets& offsets);
		GLIB_API void Begin(float scale = 1.0f);
		GLIB_API void End();
		GLIB_API void DrawImage(const Vec2& pos, const Vec2& size, Texture* image, const Color& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		GLIB_API void DrawImage(const Vec2& pos, const Vec2& size, const Vec2& scale, Texture* image, const Color& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		GLIB_API void DrawImage(const Vec2& pos, const Vec2& size, const Vec2& scale, const Vec4& uv, Texture* image, const Color& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		GLIB_API void DrawImage(const Vec2& pos, const Vec2& size, const Vec2& scale, const Vec4& uv, Texture* image, float rotation, const Color& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		GLIB_API void DrawText(const Vec2& pos, const std::wstring& text, float scale, Font* font, const Color& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		GLIB_API void FillRect(const Vec2& pos, const Vec2& size, const Color& color, float rotation = 0.0f);
	};
}