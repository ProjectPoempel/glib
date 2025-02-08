#pragma once

#include "../../DLLDefs.h"
#include "../Drawable.h"
#include "UIElement.h"
#include "UIDrawingInterface.h"
#include "glib/math/Mat4.h"

#include <vector>

namespace glib
{
	class UIFrame : public Drawable, UIAlignable
	{
	private:
		std::vector<UIElement*> m_Children;
		UIDrawingInterface* m_DI;
	public:
		Vec2 pos;
	public:
		GLIB_API UIFrame();
		GLIB_API ~UIFrame();

		GLIB_API void Draw(const DrawData& data) override;
		GLIB_API void Update(float delta) override;

		GLIB_API void AddChild(UIElement* e);
		GLIB_API void RemoveChild(UIElement* e);
		GLIB_API const std::vector<UIElement*>& GetChildren();
		GLIB_API Vec2 GetSize(bool ignoreMargin = false) override;
		GLIB_API Vec2 GetParentSize(bool ignoreMargin = false) override;
	};
}