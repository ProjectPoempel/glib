#pragma once

#include "../../DLLDefs.h"
#include "../pipeline/RenderPipeline.h"
#include "PostProcessingEffect.h"

#include <vector>
#include <string>

#define GLIB_GET_PPE(wnd, name) ((glib::PostProcessingEffect*)wnd->GetRenderPipeline()->GetElement(name))

namespace glib
{
	class PostProcessing
	{
	public:
		class Builder
		{
		private:
			RenderPipeline* m_Pipeline;
			bool m_Built;
			Window* m_Wnd;
			bool m_Use3D;
		public:
			Builder(Window* wnd, bool use3DRenderer);
			GLIB_API ~Builder();

			GLIB_API Builder& AddEffect(const std::string& name, PostProcessingEffect* effect);
			GLIB_API RenderPipeline* Build();
		};
	public:
		PostProcessing() = delete;
		~PostProcessing() = delete;

		GLIB_API static Builder NewBuilder(Window* wnd, bool use3DRenderer = false);
	};
}