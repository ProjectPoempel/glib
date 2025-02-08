#pragma once

#include "../pipeline/PipelineRenderer.h"

#define GLIB_PE_CAMERA_RENDERER_3D 0x100

namespace glib
{
	class Camera3DRenderer : public PipelineRenderer
	{
	private:
		float m_FOV;
	public:
		GLIB_API Camera3DRenderer();
		~Camera3DRenderer() override;

		void SetFOV(float degrees);

		void Construct(Window* wnd) override;
		const PipelineData Downstream(const PipelineData data) override;
		void ConstructFBO(Vec2 pos, Vec2 size) override;
	};
}