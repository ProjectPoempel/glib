#pragma once

#include "../../DLLDefs.h"
#include "../Drawable.h"
#include "../../math/Mat4.h"
#include "../../tween/TweenManager.h"
#include "../../math/Vec2.h"

#include <vector>

namespace glib
{
	class CameraImpl;

	class Camera
	{
	public:
		Vec2 pos;
		float zoom;
		float rotation;
		TweenManager tw;
		bool canDraw;
		bool canUpdate;
		Vec4 clip;
		bool clipping;
	private:
		CameraImpl* impl;
	public:
		GLIB_API Camera(Vec2 initialSize);
		GLIB_API ~Camera();

		GLIB_API void Add(Drawable* drawable);
		GLIB_API void Add(Drawable& drawable);
		GLIB_API void Remove(Drawable* drawable);
		GLIB_API void Remove(Drawable& drawable);
		GLIB_API void RemoveAll();
		GLIB_API std::vector<Drawable*>& GetDrawables();
		GLIB_API Mat4 CalculateView();
		GLIB_API void Shake(float strength, float duration, float fadeDuration = 0.0f);
		GLIB_API void Organize();

		void Update(float delta);
	};
}