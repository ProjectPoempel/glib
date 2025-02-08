#pragma once

#include "PipelineRenderer.h"
#include "../Sprite.h"
#include "../Text.h"
#include "../video/VideoPlayer.h"

namespace glib
{
	class CameraRenderer : public PipelineRenderer
	{
	public:
		GLIB_API CameraRenderer();
		~CameraRenderer() override;

		const PipelineData Downstream(const PipelineData data) override;
	};
}