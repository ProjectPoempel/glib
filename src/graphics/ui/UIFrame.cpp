#include "glib/graphics/ui/UIFrame.h"

#include <iostream>

using namespace glib;

glib::UIFrame::UIFrame()
{
	m_DI = new UIDrawingInterface;
}

glib::UIFrame::~UIFrame()
{
	delete m_DI;
}

void glib::UIFrame::Draw(const DrawData& data)
{
	UIDrawOffsets offsets{};
	offsets.pos = data.camera->pos + pos;

	m_DI->BeginFrame(data, offsets);

	Mat4 m;
	m.Translate(offsets.pos);
	m_DI->PushStack(m);

	for (UIElement* e : m_Children)
	{
		m_DI->PushAlignment(e, this);
		e->Draw(m_DI);
	}
}

void glib::UIFrame::Update(float delta)
{
	for (UIElement* e : m_Children)
	{
		e->Update(delta);
	}
}

void glib::UIFrame::AddChild(UIElement* e)
{
	m_Children.push_back(e);
}

void glib::UIFrame::RemoveChild(UIElement* e)
{
	std::vector<UIElement*>::iterator it = std::find(m_Children.begin(), m_Children.end(), e);
	if (it == m_Children.end()) return;
	m_Children.erase(it);
}

const std::vector<UIElement*>& glib::UIFrame::GetChildren()
{
	return m_Children;
}

Vec2 glib::UIFrame::GetSize(bool ignoreMargin)
{
	return gWindow->GetInitialSize();
}

Vec2 glib::UIFrame::GetParentSize(bool ignoreMargin)
{
	return gWindow->GetInitialSize();
}
