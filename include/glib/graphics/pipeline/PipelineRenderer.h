#pragma once

#include "PipelineElement.h"
#include "../Shader.h"
#include "../../math/Mat4.h"
#include "../../framebuffer/Framebuffer.h"
#include "../Sprite.h"
#include "../Text.h"
#include "../../DLLDefs.h"

namespace glib
{
	class RendererListener;

	class PipelineRenderer : public PipelineElement
	{
	protected:
		Framebuffer* m_FB;
		Vec2 m_Size;
		Vec2 m_Pos;
		RendererListener* m_L;
		Window* m_Wnd;
	public:
		GLIB_API PipelineRenderer();
		GLIB_API virtual ~PipelineRenderer();

		GLIB_API virtual void Construct(Window* wnd) override;
		GLIB_API virtual const PipelineData Downstream(const PipelineData data) = 0;
		GLIB_API virtual void ConstructFBO(Vec2 pos, Vec2 size);
	};
}