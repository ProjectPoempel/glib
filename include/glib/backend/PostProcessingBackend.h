#pragma once

#include "Backend.h"

namespace glib
{
	struct BloomPassData
	{
		Framebuffer* fb1;
		Framebuffer* fb2;
		Framebuffer* fb3;
		Framebuffer* fb4;
		float threshold;
		Vec2 resolution;
	};

	class PostProcessingBackend
	{
	public:
		GLIB_API virtual void PerformBloomPass(int pass, const BloomPassData& data) = 0;
	};
}