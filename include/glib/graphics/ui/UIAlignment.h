#pragma once

#include "../../DLLDefs.h"
#include "../../math/Vec2.h"

namespace glib
{
	enum class UIAlignment
	{
		LEFT,
		RIGHT,
		TOP,
		BOTTOM,
		START,
		END,
		HORIZONTAL,
		VERTICAL,
		CENTER
	};

	class UIAlignable
	{
	public:
		UIAlignment align = UIAlignment::LEFT;
	public:
		GLIB_API virtual Vec2 GetSize(bool ignoreMargin = false) = 0;
		GLIB_API virtual Vec2 GetParentSize(bool ignoreMargin = false) = 0;
	};
}