#include "glib/graphics/pipeline/WindowRenderer.h"
#include "glib/window/Window.h"
#include "glib/Instance.h"

glib::WindowRenderer::WindowRenderer() : PipelineRenderer()
{
}

glib::WindowRenderer::~WindowRenderer()
{
}

static float Map(float inValue, float minInRange, float maxInRange, float minOutRange, float maxOutRange)
{
	float x = (inValue - minInRange) / (maxInRange - minInRange);
	return minOutRange + (maxOutRange - minOutRange) * x;
}

const glib::PipelineData glib::WindowRenderer::Downstream(const PipelineData data)
{
	Backend* backend = m_Wnd->GetInstance()->GetBackend();
	Camera* cam = (Camera*)data.ptr;
	if (cam == nullptr)
	{
		return data;
	}

	if (data.fb != nullptr)
	{
		backend->SetBlending(BlendMode::ADD, BlendFuncs::ONE_ONE_MINUS_SRC);

		backend->GetWindowShader()->Use();

		m_FB->Bind();
		if (cam->clipping)
		{
			backend->SetClip(cam->clip.XY(), { Map(cam->clip.z, 0.0f, m_Wnd->GetInitialSize().x, 0.0f, m_Size.x), Map(cam->clip.w, 0.0f, m_Wnd->GetInitialSize().y, 0.0f, m_Size.y) });
		}

		data.fb->PrepareDraw();
		backend->DrawScreenQuad();

		if (cam->clipping)
		{
			backend->NoClip();
		}

		m_FB->Unbind();
	}

	if (data.isFinal)
	{
		backend->SetBlending(BlendMode::ADD, BlendFuncs::SRC_DST);

		backend->BindFramebuffer(nullptr);
		backend->SetViewport(m_Pos, m_Size);

		m_FB->PrepareDraw();

		backend->DrawScreenQuad();

		m_FB->Bind();
		backend->ClearFramebuffer({ 0.0f, 0.0f, 0.0f, 0.0f });
		m_FB->Unbind();
	}

	return {};
}
