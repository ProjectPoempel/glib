#include "glib/graphics/pipeline/CameraRenderer.h"
#include <vector>
#include "glib/graphics/camera/Camera.h"
#include "glib/window/Window.h"
#include "glib/graphics/Sprite.h"
#include "glib/graphics/ui/UIFrame.h"
#include "glib/utils/SpriteGroup.h"

#include <iostream>
#include <memory>

using namespace glib;

glib::CameraRenderer::CameraRenderer() : glib::PipelineRenderer()
{
}

glib::CameraRenderer::~CameraRenderer()
{
}

const PipelineData glib::CameraRenderer::Downstream(const PipelineData data)
{
	Backend* backend = m_Wnd->GetInstance()->GetBackend();
	
	backend->SetBlending(BlendMode::ADD, BlendFuncs::SRC_ONE_MINUS_SRC);

	Camera* cam = (Camera*) data.ptr;
	if (cam == nullptr)
	{
		PipelineData newData = data;
		newData.fb = m_FB;
		return newData;
	}

	m_FB->Bind();
	backend->SetViewport(GLIB_VEC2_ZERO, m_Size);
	backend->ClearFramebuffer({ 0.0f, 0.0f, 0.0f, 0.0f });

	Mat4 m = cam->CalculateView();

	DrawData dd{};
	dd.camera = cam;
	dd.matrix = &m;
	dd.userData = nullptr;
	dd.wnd = m_Wnd;

	for (Drawable* d : cam->GetDrawables())
	{
		if (!d->visible) continue;

		Text* t = dynamic_cast<Text*>(d);
		UIFrame* uiFrame = dynamic_cast<UIFrame*>(d);

		d->Draw(dd);
	}

	m_FB->Unbind();

	PipelineData newData = data;
	newData.fb = m_FB;
	return newData;
}
