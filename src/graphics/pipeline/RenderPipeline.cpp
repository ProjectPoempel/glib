#include "glib/graphics/pipeline/RenderPipeline.h"
#include "glib/window/Window.h"
#include "glib/glibError.h"

using namespace glib;

glib::RenderPipeline::RenderPipeline(Window* wnd) : m_Wnd(wnd)
{
}

glib::RenderPipeline::~RenderPipeline()
{
	for (auto& v : m_Elements)
	{
		delete v.second;
	}
}

void glib::RenderPipeline::Update(float delta)
{
}

void glib::RenderPipeline::Flush(const std::vector<Camera*>& cameras) const
{
	const Vec2& viewportPos = m_Wnd->GetViewportPos();
	const Vec2& viewportSize = m_Wnd->GetViewportSize();

	for (auto& v : m_Elements)
	{
		v.second->viewportPos = viewportPos;
		v.second->viewportSize = viewportSize;
	}

	int i = 0;
	for (Camera* camera : cameras)
	{
		if (!camera->canDraw)
		{
			i++;
			continue;
		}

		PipelineData data{};
		data.fb = nullptr;
		data.ptr = camera;
		data.wnd = m_Wnd;
		data.isFinal = i == cameras.size() - 1;
		data.camPositioned = false;

		for (auto& v : m_Elements)
		{
			if (v.second->IsExcludedFromCamera(camera)) continue;
			data = v.second->Downstream(data);
		}

		i++;
	}
}

void glib::RenderPipeline::AddElement(const std::string& name, PipelineElement* element)
{
	element->Construct(m_Wnd);
	m_Elements.push_back({ name, element });
}

void glib::RenderPipeline::AddElementBefore(const std::string& other, const std::string& name, PipelineElement* element)
{
	auto it = std::find_if(m_Elements.begin(), m_Elements.end(), [&](const std::pair<std::string, PipelineElement*>& p) {
		return p.first == other;
	});

	if (it == m_Elements.begin())
	{
		Error::Throw(GLIB_ERROR_INVALID_OPERATION, "Elements cannot be added before the first renderer!", "RenderPipeline", "AddElementBefore");
		return;
	}

	if (it != m_Elements.end()) {
		element->Construct(m_Wnd);
		m_Elements.insert(it, { name, element });
	}
}

void glib::RenderPipeline::AddElementAfter(const std::string& other, const std::string& name, PipelineElement* element)
{
	auto it = std::find_if(m_Elements.begin(), m_Elements.end(), [&](const std::pair<std::string, PipelineElement*>& p) {
		return p.first == other;
	}) + 1;

	if (it >= m_Elements.end())
	{
		Error::Throw(GLIB_ERROR_INVALID_OPERATION, "Elements cannot be after the last renderer!", "RenderPipeline", "AddElementAfter");
		return;
	}

	if (it != m_Elements.end()) {
		element->Construct(m_Wnd);
		m_Elements.insert(it, { name, element });
	}
}

void glib::RenderPipeline::RemoveElement(const std::string& name)
{
	int i = 0;
	int erase = -1;
	for (auto& v : m_Elements)
	{
		if (v.first == name)
		{
			erase = i;
			break;
		}
		i++;
	}

	if (erase == -1) return;
	m_Elements.erase(m_Elements.begin() + erase);
}

PipelineElement* glib::RenderPipeline::GetElement(const std::string& name) const
{
	for (auto& v : m_Elements)
	{
		if (v.first == name) return v.second;
	}
	return nullptr;
}
