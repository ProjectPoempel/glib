#include "glib/graphics/ui/UIDrawingInterface.h"
#include "glib/graphics/Shader.h"
#include "glib/utils/glibGlobals.h"
#include "glib/graphics/ui/UIElement.h"

#include <iostream>
#include <vector>

#define UV_DEFAULT { 0.0f, 0.0f, 1.0f, 1.0f }

using namespace glib;

namespace glib
{
	class UIDrawingInterfaceImpl
	{
	private:
		UIDrawingInterface* m_Parent;
		Backend* m_Backend;
		std::vector<Mat4> m_Stack;
	public:
		DrawData m_DrawData;
		UIDrawOffsets m_Offsets;
	public:
		UIDrawingInterfaceImpl(UIDrawingInterface* parent) : m_Parent(parent)
		{
			m_Backend = gInstance->GetBackend();
		}

		~UIDrawingInterfaceImpl()
		{
		}

		void PushStack(const Mat4& mat)
		{
			m_Stack.push_back(mat);
		}

		void PopStack(int at)
		{
			if (at == -1)
			{
				m_Stack.pop_back();
			}
			else
			{
				m_Stack.erase(m_Stack.begin() + at);
			}
		}

		void ClearStack()
		{
			m_Stack.clear();
		}

		Mat4 CompileStack()
		{
			Mat4 m;

			for (Mat4& mat : m_Stack)
			{
				m = m * mat;
			}
			
			return m;
		}

		void Begin(float scale)
		{
		}

		void End()
		{
		}

		void _DrawQuad(const Vec2& pos, const Vec2& size, const Vec2& scale, const Vec4& uv, const Color& color, float rotation)
		{
			Mat4 modelMat = CompileStack();

			Vec2 diff = Vec2((size.x * scale.x) - size.x, (size.y * scale.y) - size.y);

			modelMat.Translate(Vec2(pos.x - (diff.x / 2.0f), pos.y - (diff.y / 2.0f)));

			modelMat.Translate(Vec2(size.x / 2.0f * scale.x, size.y / 2.0f * scale.y));
			modelMat.Rotate(glib::Vec3(0.0f, 0.0f, rotation));
			modelMat.Translate(Vec2(-(size.x / 2.0f) * scale.x, -(size.y / 2.0f) * scale.y));

			modelMat.Scale(Vec2(size.x + diff.x, size.y + diff.y));

			m_Backend->GetSpriteShader()->Use();
			m_Backend->GetSpriteShader()->SetMat4("glib_model_matrix", modelMat);
			m_Backend->GetSpriteShader()->SetMat4("glib_view_matrix", *m_DrawData.matrix);
			m_Backend->GetSpriteShader()->SetColor("glib_color", color);
			m_Backend->GetSpriteShader()->SetVec2("glib_uv_coord", Vec2(uv.x, uv.y));
			m_Backend->GetSpriteShader()->SetVec2("glib_uv_size", Vec2(uv.z, uv.w));
			m_Backend->GetSpriteShader()->SetInt("glib_flip_x", false);
			m_Backend->GetSpriteShader()->SetInt("glib_flip_y", false);
			m_Backend->GetSpriteShader()->SetInt("glib_solid_color", true);

			m_Backend->DrawQuad();
		}

		void DrawImage(const Vec2& pos, const Vec2& size, const Vec2& scale, const Vec4& uv, Texture* image, const Color& color, float rotation)
		{
			image->Bind();
			m_Backend->GetSpriteShader()->Use();
			m_Backend->GetSpriteShader()->SetInt("solid_color", false);
			_DrawQuad(pos, size, scale, uv, color, rotation);
		}

		void DrawText(const Vec2& _pos, const std::wstring& text, float scale, Font* font, const Color& color)
		{
			/*
			float xOffset = 0.0f;

			for (wchar_t c : m_Text)
			{
				const Glyph& glyph = m_Parent->font->GetGlyph(c);

				Vec2 pos = m_Parent->pos;
				pos.x -= data.camera->pos.x * data.camera->zoom;
				pos.y -= data.camera->pos.y * data.camera->zoom;

				float xpos = pos.x + xOffset + glyph.bearing.x * m_Parent->scale;
				float ypos = pos.y - glyph.bearing.y * m_Parent->scale;

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

				xOffset += (glyph.advance >> 6) * m_Parent->scale;
			}

			backend->BindTexture(nullptr);
			*/

			float xOffset = 0.0f;

			for (wchar_t c : text)
			{
				const Glyph& glyph = font->GetGlyph(c);

				Vec2 pos = _pos;
				pos.x -= m_DrawData.camera->pos.x * m_DrawData.camera->zoom;
				pos.y -= m_DrawData.camera->pos.y * m_DrawData.camera->zoom;

				float xpos = pos.x + xOffset + glyph.bearing.x * scale;
				float ypos = pos.y - glyph.bearing.y * scale;

				float w = glyph.size.x * scale;
				float h = glyph.size.y * scale;

				Mat4 modelMat = CompileStack();

				modelMat.Translate(Vec2(xpos, ypos));

				//modelMat.Translate(Vec2(w / 2.0f, h / 2.0f));
				//modelMat.Rotate(Vec3(0.0f, 0.0f, 0.0f));
				//modelMat.Translate(Vec2(-(w / 2.0f), -(h / 2.0f)));

				modelMat.Scale(Vec2(w, h));

				m_Backend->GetTextShader()->Use();
				m_Backend->GetTextShader()->SetColor("glib_color", GLIB_COLOR_WHITE);
				m_Backend->GetTextShader()->SetMat4("glib_model_matrix", modelMat);
				m_Backend->GetTextShader()->SetMat4("glib_view_matrix", *m_DrawData.matrix);

				glyph.tex->Bind();

				m_Backend->DrawQuad();

				xOffset += (glyph.advance >> 6) * scale;
			}

			m_Backend->BindTexture(nullptr);
		}

		void FillRect(const Vec2& pos, const Vec2& size, const Color& color, float rotation)
		{
			_DrawQuad(pos, size, GLIB_VEC2_ONE, UV_DEFAULT, color, rotation);
		}
	};
}

glib::UIDrawingInterface::UIDrawingInterface()
{
	impl = new UIDrawingInterfaceImpl(this);
}

glib::UIDrawingInterface::~UIDrawingInterface()
{
	delete impl;
}

void glib::UIDrawingInterface::PushStack(const Mat4& mat)
{
	impl->PushStack(mat);
}

void glib::UIDrawingInterface::PopStack(int at)
{
	impl->PopStack(at);
}

void glib::UIDrawingInterface::ClearStack()
{
	impl->ClearStack();
}

void glib::UIDrawingInterface::PushAlignment(UIAlignable* element, UIAlignable* parent)
{
	Mat4 m;
	Vec2 size = element->GetSize();
	Vec2 parentSize = element->GetParentSize();

	switch (element->align)
	{
		case UIAlignment::LEFT:
			break;
		case UIAlignment::RIGHT:
			m.Translate(Vec2(
				parentSize.x - size.x,
				0.0f
			));
			break;
		case UIAlignment::TOP:
			break;
		case UIAlignment::BOTTOM:
			m.Translate(Vec2(
				0.0f,
				parentSize.y - size.y
			));
			break;
		case UIAlignment::START:
			break;
		case UIAlignment::END:
			m.Translate(Vec2(
				parentSize.x - size.x,
				parentSize.y - size.y
			));
			break;
		case UIAlignment::HORIZONTAL:
			m.Translate(Vec2(
				parentSize.x / 2.0f - size.x / 2.0f,
				0.0f
			));
			break;
		case UIAlignment::VERTICAL:
			m.Translate(Vec2(
				0.0f,
				parentSize.y / 2.0f - size.y / 2.0f
			));
			break;
		case UIAlignment::CENTER:
			m.Translate(Vec2(
				parentSize.x / 2.0f - size.x / 2.0f,
				parentSize.y / 2.0f - size.y / 2.0f
			));
			break;
	}

	PushStack(m);
}

void glib::UIDrawingInterface::BeginFrame(const DrawData& data, const UIDrawOffsets& offsets)
{
	impl->m_DrawData = data;
	impl->m_Offsets = offsets;
	impl->ClearStack();
}

void glib::UIDrawingInterface::Begin(float scale)
{
	impl->Begin(scale);
}

void glib::UIDrawingInterface::End()
{
	impl->End();
}

void glib::UIDrawingInterface::DrawImage(const Vec2& pos, const Vec2& size, Texture* image, const Color& color)
{
	impl->DrawImage(pos, size, glib::Vec2(1.0f, 1.0f), glib::Vec4(1.0f, 1.0f, 1.0f, 1.0f), image, color, 0.0f);
}

void glib::UIDrawingInterface::DrawImage(const Vec2& pos, const Vec2& size, const Vec2& scale, Texture* image, const Color& color)
{
	impl->DrawImage(pos, size, scale, glib::Vec4(1.0f, 1.0f, 1.0f, 1.0f), image, color, 0.0f);
}

void glib::UIDrawingInterface::DrawImage(const Vec2& pos, const Vec2& size, const Vec2& scale, const Vec4& uv, Texture* image, const Color& color)
{
	impl->DrawImage(pos, size, scale, uv, image, color, 0.0f);
}

void glib::UIDrawingInterface::DrawImage(const Vec2& pos, const Vec2& size, const Vec2& scale, const Vec4& uv, Texture* image, float rotation, const Color& color)
{
	impl->DrawImage(pos, size, scale, uv, image, color, rotation);
}

void glib::UIDrawingInterface::DrawText(const Vec2& pos, const std::wstring& text, float scale, Font* font, const Color& color)
{
	impl->DrawText(pos, text, scale, font, color);
}

void glib::UIDrawingInterface::FillRect(const Vec2& pos, const Vec2& size, const Color& color, float rotation)
{
	impl->FillRect(pos, size, color, rotation);
}
