#include "glib/graphics/postprocessing/PostProcessingEffect.h"
#include "glib/utils/glibGlobals.h"

using namespace glib;

glib::PostProcessingEffect::PostProcessingEffect(const std::string& fragmentCode)
{
	m_Shd = gInstance->GetBackend()->CreateWindowShader(gInstance->GetBackend()->GetFragmentShaderHeader() + fragmentCode);
}

glib::PostProcessingEffect::~PostProcessingEffect()
{
	delete m_Shd;
}

const PipelineData glib::PostProcessingEffect::Downstream(const PipelineData data)
{
	if (m_Shd == nullptr)
	{
		return data;
	}

	Camera* cam = (Camera*)data.ptr;
	if (cam == nullptr)
	{
		return data;
	}

	Backend* backend = m_Wnd->GetInstance()->GetBackend();
	backend->NoBlending();

	m_Shd->Use();
	m_Shd->SetVec2("glib_resolution", m_Size);

	m_FB->Bind();
	backend->ClearFramebuffer(GLIB_COLOR_TRANSPARENT);

	data.fb->PrepareDraw();
	backend->DrawScreenQuad();

	m_FB->Unbind();

	PipelineData newData = data;
	newData.fb = m_FB;
	return newData;
}

Shader* glib::PostProcessingEffect::GetShader()
{
	return m_Shd;
}
