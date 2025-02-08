#pragma once

#include "../DLLDefs.h"
#include "Drawable.h"
#include "../math/Vec2.h"
#include "Texture.h"
#include "../tween/TweenManager.h"
#include "../math/Axis.h"
#include "../utils/Color.h"
#include "../animation/Animation.h"
#include "../physics/PhysicsObject.h"
#include "../reflect/FieldAccessor.h"

#include <string>
#include <map>

namespace glib
{
	class SpriteImpl;

	class Sprite : public FieldAccessor, public Drawable, public PhysicsObject
	{
	public:
		Vec2 size;
		Vec2 scale;
		Vec2 offset;
		Vec2 scrollFactor;
		Texture* tex;
		Color color;
		Vec2 uvPos;
		Vec2 uvSize;
		TweenManager tw;
		Vec2 pivotPoint;
		bool overlay;
		Shader* spriteShader;

		bool _drawDebugBox;
	private:
		SpriteImpl* impl;
	public:
		GLIB_API Sprite();
		GLIB_API Sprite(const Vec2& pos, Texture* tex);
		GLIB_API Sprite(const Vec2& pos, Texture* tex, const Vec2& scrollFactor);
		GLIB_API Sprite(const Vec2& pos);
		GLIB_API Sprite(const Vec2& pos, const Vec2& scrollFactor);
		GLIB_API ~Sprite();

		/**
		* Centers this sprite in the given container around the given axis.
		* 
		* @param axis[in] - The axis to center on
		* @param containerSize[in] - The container to center the sprite in (usually window->GetInitialSize())
		*/
		GLIB_API void Center(const Axis& axis, const Vec2& containerSize);

		/**
		* Sets the texture of this sprite to the given one and the size of this sprite to the of the given texture.
		* 
		* @param tex[in] - The new texture
		*/
		GLIB_API void SetTexture(Texture* tex);

		GLIB_API void Draw(const DrawData& data) override;
		GLIB_API void Update(float delta) override;

		GLIB_API void Shake(float strength, float duration, float fadeDuration = 0.0f);

		GLIB_API void EnableAutoAnimationCenter(glib::Axis axis, const Vec2& containerSize);
		GLIB_API void DisableAutoAnimationCenter();

		// See AnimationManager

		GLIB_API void AddAnimation(const std::string& name, Animation* animation);
		GLIB_API void PlayAnimation(const std::string& animationName);
		GLIB_API void AddAllAnimations(const std::map<std::string, Animation*> animations);
		GLIB_API Animation* GetAnimation(const std::string& name);
		GLIB_API void SetDefaultAnimation(Animation* animation);
		GLIB_API void SetDefaultAnimation(const std::string& name);
		GLIB_API const Animation* GetCurrentAnimation() const;
		GLIB_API const std::string& GetCurrentAnimationName() const;
	};
}