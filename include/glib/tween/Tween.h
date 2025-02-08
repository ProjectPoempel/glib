#pragma once

#include "../DLLDefs.h"
#include "Easing.h"
#include <cmath>
#include <iostream>
#include "glib/math/Vec2.h"
#include <functional>

namespace glib
{
	class BaseTween
	{
	protected:
		std::function<void()> m_OnComplete;
	public:
		virtual void Start() = 0;
		virtual void Update(float delta) = 0;
		virtual bool IsFinished() = 0;

		void SetOnComplete(const std::function<void()>& callback)
		{
			m_OnComplete = callback;
		}

		friend class TweenManager;
	};

	template<typename T>
	class Tween : public BaseTween
	{
	private:
		T m_From;
		T m_To;
		T* m_Ptr;
		float m_Duration;
		Easing::EasingFunc m_Easing;
		float m_Delay;
		float m_DelayElapsed = 0.0f;
	protected:
		bool m_Canceled = false;
	public:
		float m_Elapsed;
		float m_Progress;
		T m_Current;
	public:
		Tween(T to, T* ptr, float duration, Easing::EasingFunc easing, float delay) : m_To(to), m_Ptr(ptr), m_Duration(duration), m_Easing(easing), m_Delay(delay), m_Elapsed(0.0f), m_Progress(0.0f)
		{
			m_OnComplete = [&]{};
		}

		void Start() override
		{
			m_From = *m_Ptr;
			m_Current = m_From;
			m_Elapsed = 0.0f;
			m_Progress = 0.0f;
		}

		void Update(float delta) override
		{
			if (m_DelayElapsed < m_Delay)
			{
				m_DelayElapsed += delta;
				return;
			}

			m_Elapsed += delta;
			m_Progress = std::fminf(m_Elapsed / m_Duration, 1.0f);
			m_Current = m_From + (m_To - m_From) * m_Easing(m_Progress);
			*m_Ptr = m_Current;
		}

		bool IsFinished() override
		{
			return m_Progress >= 1.0f;
		}

		void Cancel()
		{
			m_Canceled = true;
		}

		static Tween<T>* Construct(T to, T* ptr, float duration, Easing::EasingFunc easing, float delay, bool immediateStart = false)
		{
			Tween<T>* tw = new Tween<T>(to, ptr, duration, easing, delay);
			if (immediateStart) tw->Start();
			return tw;
		}

		friend class TweenManager;
	};
}