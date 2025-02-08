#pragma once

#include "UIElement.h"

namespace glib
{
	class UILabel : public UIElement
	{
	private:
		std::wstring text;
		Vec2 size;
	public:
		float scale = 1.0f;
		Color color = GLIB_COLOR_WHITE;
		Font* font = nullptr;
		Vec2 bgSize;
		Color bgColor = GLIB_COLOR_WHITE;
		UISizeType bgSizeType = UISizeType::AUTO;
	public:
		GLIB_API UILabel(const std::wstring& text, Font* font, const Color& color = GLIB_COLOR_WHITE);
		GLIB_API ~UILabel();

		GLIB_API void Draw(UIDrawingInterface* di) override;
		GLIB_API void Update(float delta) override;

		GLIB_API void SetText(const std::wstring& text);
		GLIB_API const std::wstring& GetText();

		GLIB_API Vec2 GetSize(bool ignoreMargin = false) override;
		GLIB_API Vec2 GetParentSize(bool ignoreMargin = false) override;
	};
}