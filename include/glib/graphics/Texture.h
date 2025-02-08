#pragma once

#include "../DLLDefs.h"
#include "glib/math/Vec2.h"

#include <cstdint>

namespace glib
{
	enum class GLIB_API ImageChannels : uint8_t
	{
		RED,
		GREEN,
		BLUE,
		RGB,
		RGBA,
		RG,
		RB,
		FR
	};

	GLIB_API ImageChannels GetImageChannelsFromNum(uint8_t num);
	GLIB_API uint8_t GetNumFromImageChannels(ImageChannels channels);

	struct GLIB_API ImageData {
		ImageChannels channels = ImageChannels::RGBA;
		int width = 0;
		int height = 0;
		unsigned char* data = nullptr;
		bool pixelart = false;
	};

	class Texture
	{
	private:
		void* backendData;
	public:
		int width;
		int height;
	public:
		GLIB_API Texture(void* backendData, int width, int height);
		GLIB_API ~Texture();

		GLIB_API void Bind();
		GLIB_API void Unbind();
		GLIB_API Vec2 GetSize();

		GLIB_API void* GetBackendData();
	};
}