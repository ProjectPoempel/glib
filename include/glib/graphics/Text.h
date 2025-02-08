#pragma once

#include "../DLLDefs.h"
#include "Drawable.h"
#include "../math/Vec2.h"
#include "../utils/Color.h"
#include "../math/Axis.h"
#include "../tween/TweenManager.h"
#include "Font.h"

namespace glib
{
	class TextImpl;

	class Text : public Drawable
	{
	public:
		Vec2 pos;
		Vec2 size;
		float scale;
		Vec2 offset;
		float rotation;
		Color color;
		Font* font;
		bool xFlip;
		bool yFlip;
		TweenManager tw;
	private:
		TextImpl* impl;
	public:
		GLIB_API Text();
		GLIB_API ~Text();

		/**
		* Centers this text in the given container around the given axis.
		*
		* @param axis[in] - The axis to center on
		* @param containerSize[in] - The container to center the text in (usually window->GetInitialSize())
		*/
		GLIB_API void Center(const Axis& axis, const Vec2& containerSize);

		/**
		* Enables or disables Auto centering for this text. Auto centering means, that the text will always re-center when the text changes.
		* 
		* @param toggle[in] - Wether to enable or disable auto-centering
		* @param axis[in] - The axis to center on
		* @param containerSize[in] - The container to center the text in (usually window->GetInitialSize())
		*/
		GLIB_API void SetAutoCenter(bool toggle, Axis axis = Axis::XY, const Vec2& containerSize = Vec2(0.0f, 0.0f));

		GLIB_API void Draw(const DrawData& data) override;
		GLIB_API virtual void Update(float delta) override;

		/**
		* Sets the text of this text object.
		* 
		* @param text[in] - The new text
		*/
		GLIB_API void SetText(const std::wstring& text);

		/**
		* @returns the current text
		*/
		GLIB_API const std::wstring& GetText();
	};
}