#pragma once

#include "../../DLLDefs.h"
#include "UIDrawingInterface.h"
#include "glib/utils/glibGlobals.h"
#include "glib/math/Vec2.h"
#include "UIAlignment.h"

#include <vector>

#define UI_SIZE_AUTO glib::Vec2(99999.0f, 99999.0f)

namespace glib
{
	struct UIOffsets
	{
		float left = 0.0f;
		float down = 0.0f;
		float up = 0.0f;
		float right = 0.0f;

		Vec2 Size()
		{
			return { right, down };
		}

		Vec2 Pos()
		{
			return { -left, -up };
		}
	};

	enum class UISizeType
	{
		VALUE,
		AUTO
	};

	class UIElement : public UIAlignable
	{
	protected:
		std::vector<UIElement*> m_Children;
	public:
		bool visible = true;
		Vec2 pos;
		UIOffsets margin;
		UIOffsets padding;
		UIElement* parent = nullptr;
	public:
		GLIB_API virtual void Draw(UIDrawingInterface* di) = 0;
		GLIB_API virtual void Update(float delta) = 0;

		GLIB_API void AddChild(UIElement* e);
		GLIB_API void RemoveChild(UIElement* e);
		GLIB_API const std::vector<UIElement*>& GetChildren();
		GLIB_API virtual Vec2 GetSize(bool ignoreMargin = false) = 0;
	};
}