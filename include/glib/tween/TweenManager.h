#pragma once

#include "../DLLDefs.h"
#include "Tween.h"
#include <initializer_list>
#include "Easing.h"
#include "../math/Vec2.h"
#include "../math/Vec3.h"
#include <vector>
#include "glib/utils/Color.h"

namespace glib
{
	template<typename T>
	struct TweenData
	{
		T to;
		T* ptr;
	};

	class TweenManager
	{
	private:
		std::vector<BaseTween*> m_Tweens;
		std::vector<BaseTween*> m_Queue;
		BaseTween* m_Current = nullptr;
	public:
		GLIB_API TweenManager();
		GLIB_API ~TweenManager();

		GLIB_API void Update(float delta);

		GLIB_API glib::Tween<float>* TweenFloat(TweenData<float> data, float duration, Easing::EasingFunc easing, float delay = 0.0f);
		GLIB_API glib::Tween<Vec2>* TweenVec2(TweenData<Vec2> data, float duration, Easing::EasingFunc easing, float delay = 0.0f);
		GLIB_API glib::Tween<Vec3>* TweenVec3(TweenData<Vec3> data, float duration, Easing::EasingFunc easing, float delay = 0.0f);
		GLIB_API glib::Tween<Color>* TweenColor(TweenData<Color> data, float duration, Easing::EasingFunc easing, float delay = 0.0f);

		GLIB_API glib::Tween<float>* QueueFloatTween(TweenData<float> data, float duration, Easing::EasingFunc easing, float delay = 0.0f);
		GLIB_API glib::Tween<Vec2>* QueueVec2Tween(TweenData<Vec2> data, float duration, Easing::EasingFunc easing, float delay = 0.0f);
		GLIB_API glib::Tween<Vec3>* QueueVec3Tween(TweenData<Vec3> data, float duration, Easing::EasingFunc easing, float delay = 0.0f);
		GLIB_API glib::Tween<Color>* QueueColorTween(TweenData<Color> data, float duration, Easing::EasingFunc easing, float delay = 0.0f);

		GLIB_API void CancelQueuedTweens();
		GLIB_API void CancelTweens();
	};
}