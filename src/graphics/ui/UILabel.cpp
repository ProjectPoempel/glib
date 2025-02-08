#include "glib/graphics/ui/UILabel.h"

using namespace glib;

glib::UILabel::UILabel(const std::wstring& text, Font* font, const Color& color) : color(color), scale(1.0f), font(font)
{
	SetText(text);
}

glib::UILabel::~UILabel()
{
}

void glib::UILabel::Draw(UIDrawingInterface* di)
{
	if (bgSizeType == UISizeType::AUTO)
	{
		di->FillRect(padding.Pos(), GetSize(true) + padding.Size(), bgColor);
	}
	else
	{
		di->FillRect(padding.Pos(), bgSize + (padding.Size() * 2.0f), bgColor);
	}
	di->DrawText(Vec2(0.0f, size.y), text, scale, font, color);
}

void glib::UILabel::Update(float delta)
{
}

void glib::UILabel::SetText(const std::wstring& text)
{
	if (font == nullptr) return;

	this->text = text;
	size.x = font->CalculateWidth(text, scale);
	size.y = font->CalculateHeight(text, scale);
}

const std::wstring& glib::UILabel::GetText()
{
	return text;
}

Vec2 glib::UILabel::GetSize(bool ignoreMargin)
{
	if (ignoreMargin)
	{
		return size + padding.Size();
	}
	else
	{
		return size + padding.Size() + margin.Size();
	}
}

Vec2 glib::UILabel::GetParentSize(bool ignoreMargin)
{
	if (parent == nullptr)
	{
		return gWindow->GetInitialSize();
	}
	else
	{
		return parent->GetSize(ignoreMargin);
	}
}
