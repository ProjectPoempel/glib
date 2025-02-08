#include "glib/graphics/ui/UIButton.h"
#include "glib/utils/Utils.h"
#include "glib/math/Rect.h"

using namespace glib;

namespace glib
{
	class UIButtonImpl : public EventSubscriber
	{
	private:
		UIButton* m_Parent;
		Texture* m_Image;
		Texture* m_HoverImage;
		Texture* m_PressImage;
		std::wstring m_Text;
		bool m_UseHPImages = false;
		bool m_UseImages = false;
		bool m_Hovering = false;
		bool m_Pressed = false;
		Vec2 m_TextSize;
		std::function<void(UIButton* btn)> m_OnClickCallback;
		std::function<void(UIButton* btn)> m_OnEnterCallback;
		std::function<void(UIButton* btn)> m_OnLeaveCallback;
	public:
		UIButtonImpl(UIButton* parent, const std::wstring& text) : m_Parent(parent)
		{
			gWindow->GetEventManager().Subscribe(GLIB_EVENT_MOUSE_PRESS | GLIB_EVENT_MOUSE_RELEASE | GLIB_EVENT_MOUSE_MOVE, this);
			SetText(text);
			m_UseImages = false;
		}

		UIButtonImpl(UIButton* parent, Texture* image, Texture* hoverImage, Texture* pressImage, const std::wstring& text) : UIButtonImpl(parent, text)
		{
			m_UseHPImages = hoverImage != nullptr && pressImage != nullptr;
			m_HoverImage = hoverImage;
			m_PressImage = pressImage;
			m_UseImages = true;
		}

		~UIButtonImpl()
		{
			gWindow->GetEventManager().Unsubscribe(this);
		}

		void OnMouseButtonPress(const MouseEvent& event) override
		{
			if (!m_Hovering) return;
			m_Pressed = true;
			if (m_OnClickCallback)
			{
				m_OnClickCallback(m_Parent);
			}
		}

		void OnMouseButtonRelease(const MouseEvent& event) override
		{
			m_Pressed = false;
		}

		void OnMouseMove(const MouseMoveEvent& event) override
		{
			bool touching = Utils::IsPointInArea(Utils::GetMousePosition(gWindow), Rect(m_Parent->pos, GetSize(false)));

			if (touching && !m_Hovering)
			{
				m_Hovering = true;
				if (m_OnEnterCallback)
				{
					m_OnEnterCallback(m_Parent);
				}
			}
			else if (!touching && m_Hovering)
			{
				m_Hovering = false;
				if (m_OnLeaveCallback)
				{
					m_OnLeaveCallback(m_Parent);
				}
			}
		}

		void Draw(UIDrawingInterface* di)
		{
			if (!m_UseHPImages && m_Parent->font == nullptr) return;

			di->Begin(1.0f);
			
			if (m_UseImages)
			{
				if (m_UseHPImages)
				{
					if (m_Pressed)
					{
						di->DrawImage(m_Parent->pos, m_PressImage->GetSize(), m_PressImage, m_Parent->color);
					}
					else if (m_Hovering)
					{
						di->DrawImage(m_Parent->pos, m_HoverImage->GetSize(), m_HoverImage, m_Parent->color);
					}
					else
					{
						di->DrawImage(m_Parent->pos, m_Image->GetSize(), m_Image, m_Parent->color);
					}
				}
				else
				{
					di->DrawImage(m_Parent->pos, m_Image->GetSize(), m_Image, m_Parent->color);
				}
			}
			else
			{
				di->FillRect({ 100, 100 }, { 100, 100 }, GLIB_COLOR_WHITE);
			}

			di->DrawText(m_Parent->pos + (GetSize(true) / 2.0f - m_TextSize / 2.0f), m_Text, m_Parent->textScale, m_Parent->font, m_Parent->textColor);

			di->End();
		}

		void Update(float delta)
		{

		}

		void SetText(const std::wstring& text)
		{
			m_Text = text;
			m_TextSize.x = m_Parent->font->CalculateWidth(m_Text, m_Parent->textScale);
			m_TextSize.y = m_Parent->font->CalculateHeight(m_Text, m_Parent->textScale);
		}

		const std::wstring& GetText()
		{
			return m_Text;
		}

		void SetOnClick(std::function<void(UIButton* btn)> callback)
		{
			m_OnClickCallback = callback;
		}

		void SetOnEnter(std::function<void(UIButton* btn)> callback)
		{
			m_OnEnterCallback = callback;
		}

		void SetOnLeave(std::function<void(UIButton* btn)> callback)
		{
			m_OnLeaveCallback = callback;
		}

		Vec2 GetSize(bool ignoreMargin)
		{
			Vec2 size;

			if (m_UseImages)
			{
				if (m_UseHPImages)
				{
					if (m_Pressed)
					{
						size = m_PressImage->GetSize();
					}
					else if (m_Hovering)
					{
						size = m_HoverImage->GetSize();
					}
					else
					{
						size = m_Image->GetSize();
					}
				}
				else
				{
					size = m_Image->GetSize();
				}
			}
			else
			{
				size = m_TextSize;
			}

			if (ignoreMargin)
			{
				return size + m_Parent->padding.Size();
			}
			else
			{
				return size + m_Parent->margin.Size() + m_Parent->padding.Size();
			}
		}
	};
}

glib::UIButton::UIButton(const Color& color, Font* font, const std::wstring& text) : font(font)
{
	impl = new UIButtonImpl(this, text);
	this->color = color;
}

glib::UIButton::UIButton(Texture* image, Font* font, const std::wstring& text) : font(font)
{
	impl = new UIButtonImpl(this, image, nullptr, nullptr, text);
}

glib::UIButton::UIButton(Texture* image, Texture* hoverImage, Texture* pressImage)
{
	impl = new UIButtonImpl(this, image, hoverImage, pressImage, L"");
}

glib::UIButton::~UIButton()
{
	delete impl;
}

void glib::UIButton::Draw(UIDrawingInterface* di)
{
	impl->Draw(di);
}

void glib::UIButton::Update(float delta)
{
	impl->Update(delta);
}

bool glib::UIButton::IsHovering()
{
	return false;
}

bool glib::UIButton::IsPressed()
{
	return false;
}

void glib::UIButton::SetText(const std::wstring& text)
{
	impl->SetText(text);
}

const std::wstring& glib::UIButton::GetText()
{
	return impl->GetText();
}

void glib::UIButton::SetOnClick(std::function<void(UIButton* btn)> callback)
{
	impl->SetOnClick(callback);
}

void glib::UIButton::SetOnEnter(std::function<void(UIButton* btn)> callback)
{
	impl->SetOnEnter(callback);
}

void glib::UIButton::SetOnLeave(std::function<void(UIButton* btn)> callback)
{
	impl->SetOnLeave(callback);
}

Vec2 glib::UIButton::GetSize(bool ignoreMargin)
{
	return impl->GetSize(ignoreMargin);
}
