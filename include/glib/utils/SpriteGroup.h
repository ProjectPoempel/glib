#pragma once

#include "../DLLDefs.h"
#include "glib/graphics/Drawable.h"
#include "glib/graphics/Sprite.h"

namespace glib
{
	class SpriteGroup : public Drawable
	{
	private:
		std::vector<Sprite*> m_Sprites;
	public:
		GLIB_API SpriteGroup();
		GLIB_API ~SpriteGroup();

		GLIB_API void Add(Sprite* sprite);
		GLIB_API void Add(Sprite& sprite);
		GLIB_API void Remove(Sprite* sprite);
		GLIB_API void Remove(Sprite& sprite);
		GLIB_API void Center(Axis axis, const Vec2& containerSize);
		GLIB_API Vec2 GetSize();
		GLIB_API Vec2 GetPosition();
		GLIB_API void SetPosition(const Vec2& pos);

		GLIB_API const std::vector<Sprite*>& GetSprites();

		GLIB_API void Draw(const DrawData& data) override;
		GLIB_API void Update(float delta) override;
	};
}