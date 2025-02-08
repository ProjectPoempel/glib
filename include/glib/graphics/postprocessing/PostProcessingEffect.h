#pragma once

#include "../pipeline/PipelineRenderer.h"

namespace glib
{
	class PostProcessingEffect : public PipelineRenderer
	{
	private:
		Shader* m_Shd;
	public:
		GLIB_API PostProcessingEffect(const std::string& fragmentCode);
		~PostProcessingEffect() override;

		const PipelineData Downstream(const PipelineData data) override;

		GLIB_API Shader* GetShader();
	};
}