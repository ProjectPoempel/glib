#include "glib/math/Rect.h"

using namespace glib;

glib::Rect::Rect() : x(0.0f), y(0.0f), w(0.0f), h(0.0f)
{
}

glib::Rect::Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h)
{
}

glib::Rect::Rect(const Vec2& pos, const Vec2& size) : x(pos.x), y(pos.y), w(size.x), h(size.y)
{
}

float glib::Rect::Distance(const Rect& other)
{
	return 0.0f;
}
