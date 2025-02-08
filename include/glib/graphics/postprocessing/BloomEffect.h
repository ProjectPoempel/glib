#pragma once

#include "PostProcessingEffect.h"

namespace glib
{
	class BloomEffect : public PostProcessingEffect
	{
	private:
		Framebuffer* m_LFB = nullptr; // Brightness FB
		Framebuffer* m_MFB = nullptr; // Intermediate FB
	public:
		float threshold = 0.35f;
	public:
		GLIB_API BloomEffect();
		~BloomEffect() override;

		const PipelineData Downstream(const PipelineData data) override;
		void ConstructFBO(Vec2 pos, Vec2 size) override;
	};
}