#include "glib/graphics/Texture.h"
#include "glib/utils/glibGlobals.h"

using namespace glib;

glib::Texture::Texture(void* backendData, int width, int height) : backendData(backendData), width(width), height(height)
{
}

glib::Texture::~Texture()
{
	gInstance->GetBackend()->DeleteTexture(this);
}

void glib::Texture::Bind()
{
	gInstance->GetBackend()->BindTexture(this);
}

void glib::Texture::Unbind()
{
	gInstance->GetBackend()->UnbindTexture(this);
}

Vec2 glib::Texture::GetSize()
{
	return Vec2(width, height);
}

void* glib::Texture::GetBackendData()
{
	return backendData;
}

ImageChannels glib::GetImageChannelsFromNum(uint8_t num)
{
	switch (num)
	{
	case 1: return ImageChannels::RED;
	case 2: return ImageChannels::RG;
	case 3: return ImageChannels::RGB;
	default: return ImageChannels::RGBA;
	}
}

uint8_t glib::GetNumFromImageChannels(ImageChannels channels)
{
	switch (channels)
	{
	case ImageChannels::RED: return 1;
	case ImageChannels::GREEN: return 1;
	case ImageChannels::BLUE: return 1;
	case ImageChannels::RG: return 2;
	case ImageChannels::RB: return 2;
	case ImageChannels::RGB: return 3;
	case ImageChannels::RGBA: return 4;
	default: return 1;
	}
}
