#include "glib/graphics/postprocessing/PostProcessing.h"
#include "glib/window/Window.h"
#include "glib/graphics/pipeline/CameraRenderer.h"
#include "glib/graphics/pipeline/WindowRenderer.h"
#include <iostream>
#include "glib/graphics/pipeline3d/RenderPipeline3D.h"

using namespace glib;

PostProcessing::Builder glib::PostProcessing::NewBuilder(Window* wnd, bool use3DRenderer)
{
	return Builder(wnd, use3DRenderer);
}

glib::PostProcessing::Builder::Builder(Window* wnd, bool use3DRenderer) : m_Built(false), m_Wnd(wnd), m_Use3D(use3DRenderer)
{
	wnd->SetToCurrentContext();
	RenderPipeline* p;

	if (m_Use3D)
	{
		p = new RenderPipeline3D(wnd);
	}
	else
	{
		p = new RenderPipeline(wnd);
	}

	p->AddElement("cam", new CameraRenderer);
	m_Pipeline = p;
}

glib::PostProcessing::Builder::~Builder()
{
	if (!m_Built) delete m_Pipeline;
}

PostProcessing::Builder& glib::PostProcessing::Builder::AddEffect(const std::string& name, PostProcessingEffect* effect)
{
	m_Pipeline->AddElement(name, effect);
	return *this;
}

RenderPipeline* glib::PostProcessing::Builder::Build()
{
	m_Built = true;
	m_Pipeline->AddElement("wnd", new WindowRenderer);
	return m_Pipeline;
}
