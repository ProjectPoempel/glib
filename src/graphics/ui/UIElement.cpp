#include "glib/graphics/ui/UIElement.h"

using namespace glib;

void glib::UIElement::AddChild(UIElement* e)
{
	m_Children.push_back(e);
}

void glib::UIElement::RemoveChild(UIElement* e)
{
	std::vector<UIElement*>::iterator it = std::find(m_Children.begin(), m_Children.end(), e);
	if (it == m_Children.end()) return;
	m_Children.erase(it);
}

const std::vector<UIElement*>& glib::UIElement::GetChildren()
{
	return m_Children;
}
