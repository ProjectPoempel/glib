#include "glib/graphics/Font.h"
#include "glib/apkg/apkg.h"
#include "glib/utils/ImageUtils.h"
#include "glib/utils/glibGlobals.h"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <unordered_map>
#include <iostream>

namespace glib
{
	class FontImpl
	{
	private:
		std::unordered_map<wchar_t, Glyph> m_Glyphs;
        int m_Size;
	public:
		FontImpl(const std::string& path, wchar_t* alphabet, size_t alphabetLen, int size, bool pixelart) : m_Size(size)
		{
            FT_Library ft;
            if (FT_Init_FreeType(&ft))
            {
                std::cout << "Failed to init FreeType!" << std::endl;
                return;
            }

            FT_Face face;
            if (FT_New_Face(ft, path.c_str(), 0, &face))
            {
                std::cout << "Failed to load file!" << std::endl;
                return;
            }

            FT_Select_Charmap(face, FT_ENCODING_UNICODE);
            FT_Set_Pixel_Sizes(face, 0, size);

            for (int i = 0; i < alphabetLen; i++)
            {
                FT_UInt glyph_index = FT_Get_Char_Index(face, alphabet[i]);

                FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);

                ImageData data{};
                data.channels = ImageChannels::FR;
                data.width = face->glyph->bitmap.width;
                data.height = face->glyph->bitmap.rows;
                data.data = face->glyph->bitmap.buffer;
                data.pixelart = pixelart;

                Texture* tex = gInstance->GetBackend()->CreateTexture(data);

                Glyph glyph = {
                    tex,
                    Vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    Vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x
                };

                m_Glyphs.insert({ alphabet[i], glyph });
            }

            FT_Done_Face(face);
            FT_Done_FreeType(ft);
		}

        FontImpl(const std::string& packagePath, const std::string& path, wchar_t* alphabet, size_t alphabetLen, int size, bool pixelart) : m_Size(size)
        {
            FT_Library ft;
            if (FT_Init_FreeType(&ft))
            {
                std::cout << "Failed to init FreeType!" << std::endl;
                return;
            }

            apkg::FileData fd = apkg::UnpackOnce(packagePath, path);

            FT_Face face;
            if (FT_New_Memory_Face(ft, (const FT_Byte*)fd.buf, fd.bufLen, 0, &face))
            {
                std::cout << "Failed to load file!" << std::endl;
                return;
            }

            FT_Select_Charmap(face, FT_ENCODING_UNICODE);
            FT_Set_Pixel_Sizes(face, 0, size);
            
            for (int i = 0; i < alphabetLen; i++)
            {
                FT_UInt glyph_index = FT_Get_Char_Index(face, alphabet[i]);

                FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);

                ImageData data{};
                data.channels = ImageChannels::FR;
                data.width = face->glyph->bitmap.width;
                data.height = face->glyph->bitmap.rows;
                data.data = face->glyph->bitmap.buffer;
                data.pixelart = pixelart;

                Texture* tex = gInstance->GetBackend()->CreateTexture(data);

                Glyph glyph = {
                    tex,
                    Vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    Vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x
                };

                m_Glyphs.insert({ alphabet[i], glyph });
            }

            FT_Done_Face(face);
            FT_Done_FreeType(ft);
            delete[] fd.buf;
        }

		~FontImpl()
		{
            for (const auto& v : m_Glyphs)
            {
                delete v.second.tex;
            }
		}

        const Glyph& GetGlyph(wchar_t c)
        {
            try
            {
                return m_Glyphs.at(c);
            }
            catch (std::exception e)
            {
                return m_Glyphs.at(0);
            }
        }

        int GetSize()
        {
            return m_Size;
        }
	};
}

using namespace glib;

glib::Font::Font(const std::string& path, wchar_t* alphabet, size_t alphabetLen, int size, bool pixelart)
{
	impl = new FontImpl(path, alphabet, alphabetLen, size, pixelart);
}

glib::Font::Font(const std::string& packagePath, const std::string& path, wchar_t* alphabet, size_t alphabetLen, int size, bool pixelart)
{
    impl = new FontImpl(packagePath, path, alphabet, alphabetLen, size, pixelart);
}

glib::Font::~Font()
{
	delete impl;
}

const Glyph& glib::Font::GetGlyph(wchar_t c)
{
    return impl->GetGlyph(c);
}

float glib::Font::CalculateWidth(const std::wstring& text, float scale)
{
    float width = 0.0f;

    for (wchar_t c : text)
    {
        const Glyph& glyph = GetGlyph(c);
        width += (glyph.advance >> 6) * scale;
    }

    return width;
}

float glib::Font::CalculateHeight(const std::wstring& text, float scale)
{
    float height = 0.0f;

    for (wchar_t c : text)
    {
        const Glyph& glyph = GetGlyph(c);
        if (glyph.size.y * scale > height)
        {
            height = glyph.size.y * scale;
        }
    }

    return height;
}

int glib::Font::GetSize()
{
    return impl->GetSize();
}
