#pragma once

#include "PipelineRenderer.h"
#include "../Shader.h"
#include "../../math/Mat4.h"
#include "../../framebuffer/Framebuffer.h"

namespace glib
{
	class WRendererListener;

	class WindowRenderer : public PipelineRenderer
	{
	public:
		GLIB_API WindowRenderer();
		~WindowRenderer();

		const PipelineData Downstream(const PipelineData data) override;
	};
}