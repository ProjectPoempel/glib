#include "glib/utils/SpriteGroup.h"

#include <algorithm>

using namespace glib;

glib::SpriteGroup::SpriteGroup()
{
}

glib::SpriteGroup::~SpriteGroup()
{
}

void glib::SpriteGroup::Add(Sprite* sprite)
{
	m_Sprites.push_back(sprite);
}

void glib::SpriteGroup::Add(Sprite& sprite)
{
	m_Sprites.push_back(&sprite);
}

void glib::SpriteGroup::Remove(Sprite* sprite)
{
	std::vector<Sprite*>::iterator it = std::find(m_Sprites.begin(), m_Sprites.end(), sprite);
	if (it == m_Sprites.end()) return;
	m_Sprites.erase(it);
}

void glib::SpriteGroup::Remove(Sprite& sprite)
{
	Remove(&sprite);
}

void glib::SpriteGroup::Center(Axis axis, const Vec2& containerSize)
{
	Vec2 pos = GetPosition();
	Vec2 size = GetSize();

	switch (axis)
	{
	case Axis::X:
	{
		SetPosition({ containerSize.x / 2.0f - size.x / 2.0f, pos.y });
		break;
	}
	case Axis::Y:
	{
		SetPosition({ pos.x, containerSize.y / 2.0f - size.y / 2.0f });
		break;
	}
	case Axis::XY:
	{
		SetPosition({ containerSize.x / 2.0f - size.x / 2.0f, containerSize.y / 2.0f - size.y / 2.0f });
		break;
	}
	}
}

Vec2 glib::SpriteGroup::GetSize()
{
	Vec2 size;
	Vec2 closestToZeroPos = GetPosition();

	std::vector<Sprite*> sorted = m_Sprites;
	std::sort(sorted.begin(), sorted.end(), [](Sprite* s1, Sprite* s2) { return s1->pos.x < s2->pos.x && s1->pos.y < s2->pos.y; });

	size_t i = 0;
	for (Sprite* s : sorted)
	{
		Vec2 dstToNext;

		if (s != sorted.back())
		{
			Sprite* next = sorted.at(i + 1);
			dstToNext = next->pos - (s->pos + s->size * s->scale);
		}

		size += s->size * s->scale;
		size += dstToNext.Absolute();
		i++;
	}

	return size;
}

Vec2 glib::SpriteGroup::GetPosition()
{
	float closestX = 10000000000.0f;
	float closestY = 10000000000.0f;
	float closestXPos = 0.0f;
	float closestYPos = 0.0f;

	for (Sprite* s : m_Sprites)
	{
		if (s->pos.x < closestX)
		{
			closestX = s->pos.x;
			closestXPos = closestX;
		}
		if (s->pos.y < closestY)
		{
			closestY = s->pos.y;
			closestYPos = closestY;
		}
	}

	return { closestXPos, closestYPos };
}

void glib::SpriteGroup::SetPosition(const Vec2& pos)
{
	Vec2 groupPos = GetPosition();

	for (Sprite* s : m_Sprites)
	{
		Vec2 offset = s->pos - groupPos;
		s->pos = pos;
		s->pos += offset;
	}
}

const std::vector<Sprite*>& glib::SpriteGroup::GetSprites()
{
	return m_Sprites;
}

void glib::SpriteGroup::Draw(const DrawData& data)
{
	for (Sprite* s : m_Sprites) s->Draw(data);
}

void glib::SpriteGroup::Update(float delta)
{
	for (Sprite* s : m_Sprites) s->Update(delta);
}
