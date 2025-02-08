#pragma once

#include "../DLLDefs.h"
#include "Shader.h"

namespace glib
{
	class Camera;
	class Window;

	struct DrawData
	{
		Camera* camera;
		Mat4* matrix;
		void* userData;
		Window* wnd;
	};

	/**
	* The base class of all drawable objects
	*/
	class Drawable
	{
	public:
		bool visible = true;
		int drawOrder = -1; // -1 = ignore draw order (always behind)
		bool flipX = false;
		bool flipY = false;
	public:
		GLIB_API virtual void Draw(const DrawData& data) = 0;
		GLIB_API virtual void Update(float delta) = 0;
	};
}