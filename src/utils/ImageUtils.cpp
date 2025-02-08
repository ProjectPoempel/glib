#include "glib/utils/ImageUtils.h"

#include <cstring>

using namespace glib;

ImageData glib::ImageUtils::Cut(const ImageData& data, int _x, int _y, int width, int height)
{
	ImageData newData{};
	newData.width = width;
	newData.height = height;
	newData.channels = data.channels;

	if (_x + width >= data.width)
	{
		width = data.width - _x;
	}
	if (_y + height >= data.height)
	{
		height = data.height - _y;
	}

	unsigned char* arr = new unsigned char[width * height * GetNumFromImageChannels(data.channels)];

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int src_index = ((_y + y) * data.width + (_x + x)) * GetNumFromImageChannels(data.channels);
			int dest_index = (y * width + x) * GetNumFromImageChannels(data.channels);
			memcpy(&arr[dest_index], &data.data[src_index], GetNumFromImageChannels(data.channels));
		}
	}

	newData.data = arr;

	return newData;
}

void glib::ImageUtils::FreeData(const ImageData& data)
{
	delete[] data.data;
}
