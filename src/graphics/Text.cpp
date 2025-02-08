#include "glib/graphics/Text.h"
#include "glib/graphics/camera/Camera.h"
#include "glib/utils/glibGlobals.h"

#include <vector>

namespace glib
{
	class TextImpl
	{
	private:
		Text* m_Parent;
		std::wstring m_Text;
		std::vector<Glyph> m_Glyphs;
		bool m_AutoCenterToggle;
		Axis m_AutoCenterAxis;
		Vec2 m_AutoCenterContainerSize;
	public:
		TextImpl(Text* txt) : m_Parent(txt), m_AutoCenterToggle(false)
		{
		}

		~TextImpl()
		{
		}

		void SetText(const std::wstring& text)
		{
			m_Text = text;
			m_Parent->size = glib::Vec2(m_Parent->font->CalculateWidth(text, m_Parent->scale), m_Parent->font->CalculateHeight(text, m_Parent->scale));
			if (m_AutoCenterToggle)
			{
				m_Parent->Center(m_AutoCenterAxis, m_AutoCenterContainerSize);
			}
		}

		const std::wstring& GetText()
		{
			return m_Text;
		}

		void SetAutoCenter(bool toggle, Axis axis, const Vec2& containerSize)
		{
			m_AutoCenterToggle = toggle;
			m_AutoCenterAxis = axis;
			m_AutoCenterContainerSize = containerSize;
		}

		void Update(float delta)
		{
		}

		void Draw(const DrawData& data)
		{
			Backend* backend = gInstance->GetBackend();

			float offsetX = 0.0f;
			float offsetY = 0.0f;

			for (wchar_t c : m_Text)
			{
				if (c == '\n')
				{
					offsetY += m_Parent->size.y;
					offsetX = 0.0f;
					continue;
				}

				const Glyph& glyph = m_Parent->font->GetGlyph(c);

				Vec2 pos = m_Parent->pos;
				pos.x -= data.camera->pos.x * data.camera->zoom;
				pos.y -= data.camera->pos.y * data.camera->zoom;

				float xpos = pos.x + offsetX + glyph.bearing.x * m_Parent->scale;
				float ypos = pos.y - glyph.bearing.y * m_Parent->scale;
				ypos += offsetY;

				float w = glyph.size.x * m_Parent->scale;
				float h = glyph.size.y * m_Parent->scale;

				Mat4 model;

				model.Translate(Vec2(xpos, ypos));
				model.Translate(m_Parent->offset);

				model.Translate(Vec2(w / 2.0f, h / 2.0f));
				model.Rotate(Vec3(0.0f, 0.0f, m_Parent->rotation));
				model.Translate(Vec2(-(w / 2.0f), -(h / 2.0f)));

				model.Scale(Vec2(w, h));

				backend->GetTextShader()->Use();
				backend->GetTextShader()->SetColor("glib_color", m_Parent->color);
				backend->GetTextShader()->SetMat4("glib_model_matrix", model);
				backend->GetTextShader()->SetMat4("glib_view_matrix", *data.matrix);

				glyph.tex->Bind();

				backend->DrawQuad();

				offsetX += (glyph.advance >> 6) * m_Parent->scale;
			}

			backend->BindTexture(nullptr);
		}
	};
}

using namespace glib;

glib::Text::Text() : scale(1.0f), font(nullptr), color({ 1.0f, 1.0f, 1.0f, 1.0f }), xFlip(false), yFlip(false), rotation(0.0f)
{
	visible = true;
	impl = new TextImpl(this);
}

glib::Text::~Text()
{
	delete impl;
}

void glib::Text::Center(const Axis& axis, const Vec2& containerSize)
{
	switch (axis)
	{
	case Axis::X:
	{
		pos.x = containerSize.x / 2.0f - font->CalculateWidth(GetText(), scale) / 2.0f;
		break;
	}
	case Axis::Y:
	{
		pos.y = containerSize.y / 2.0f - font->CalculateHeight(GetText(), scale) / 2.0f;
		break;
	}
	case Axis::XY:
	{
		pos.x = containerSize.x / 2.0f - font->CalculateWidth(GetText(), scale) / 2.0f;
		pos.y = containerSize.y / 2.0f - font->CalculateHeight(GetText(), scale) / 2.0f;
		break;
	}
	}
}

void glib::Text::Draw(const DrawData& data)
{
	impl->Draw(data);
}

void glib::Text::Update(float delta)
{
	impl->Update(delta);
	tw.Update(delta);
}

void glib::Text::SetText(const std::wstring& text)
{
	impl->SetText(text);
}

const std::wstring& glib::Text::GetText()
{
	return impl->GetText();
}

void glib::Text::SetAutoCenter(bool toggle, Axis axis, const Vec2& containerSize)
{
	impl->SetAutoCenter(toggle, axis, containerSize);
}
