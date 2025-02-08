#pragma once

#include "UIElement.h"
#include "../Texture.h"
#include "glib/utils/Color.h"
#include "../Font.h"

#include <string>
#include <functional>

namespace glib
{
	class UIButtonImpl;

	class UIButton : public UIElement
	{
	private:
		UIButtonImpl* impl;
	public:
		float textScale = 1.0f;
		Color textColor = GLIB_COLOR_WHITE;
		Color color = GLIB_COLOR_WHITE;
		Font* font = nullptr;
	public:
		GLIB_API UIButton(const Color& color, Font* font, const std::wstring& text = L"");
		GLIB_API UIButton(Texture* image, Font* font, const std::wstring& text = L"");
		GLIB_API UIButton(Texture* image, Texture* hoverImage, Texture* pressImage);
		GLIB_API ~UIButton();

		GLIB_API void Draw(UIDrawingInterface* di) override;
		GLIB_API void Update(float delta) override;

		GLIB_API bool IsHovering();
		GLIB_API bool IsPressed();

		GLIB_API void SetText(const std::wstring& text);
		GLIB_API const std::wstring& GetText();

		GLIB_API void SetOnClick(std::function<void(UIButton* btn)> callback);
		GLIB_API void SetOnEnter(std::function<void(UIButton* btn)> callback);
		GLIB_API void SetOnLeave(std::function<void(UIButton* btn)> callback);

		GLIB_API Vec2 GetSize(bool ignoreMargin = false) override;
	};
}